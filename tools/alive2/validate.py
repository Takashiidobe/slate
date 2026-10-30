import argparse
import copy
import fnmatch
import hashlib
import json
import os
import re
import shlex
import shutil
import signal
import subprocess
import sys
import tempfile
import tomllib
from collections import Counter
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
PASSES = "always-inline,sroa,mem2reg"
SYMBOL = r'@(?:"([^"\\]*(?:\\.[^"\\]*)*)"|([-a-zA-Z$._0-9]+))'


class HarnessError(Exception):
    def __init__(self, status, message):
        self.status = status
        super().__init__(message)


class Runner:
    def __init__(self, output, timeout):
        self.output = output
        self.timeout = timeout
        self.commands = []
        self.env = os.environ.copy()
        self.env.pop("CARGO_ENCODED_RUSTFLAGS", None)
        self.env["RUSTFLAGS"] = "-Awarnings"

    def run(self, stage, command, input_path=None):
        command = [str(arg) for arg in command]
        self.commands.append({"stage": stage, "argv": command})
        stdout = self.output / f"{stage}.stdout"
        stderr = self.output / f"{stage}.stderr"
        with (
            stdout.open("w") as out,
            stderr.open("w") as err,
            input_path.open() if input_path else open(os.devnull) as stdin,
        ):
            try:
                process = subprocess.Popen(
                    command,
                    cwd=ROOT,
                    env=self.env,
                    stdout=out,
                    stderr=err,
                    stdin=stdin,
                    start_new_session=True,
                )
            except OSError as error:
                raise HarnessError("tool_error", f"{stage}: {error}") from error
            try:
                code = process.wait(timeout=self.timeout)
            except subprocess.TimeoutExpired as error:
                os.killpg(process.pid, signal.SIGKILL)
                process.wait()
                raise HarnessError(
                    "timeout", f"{stage} exceeded {self.timeout}s"
                ) from error
        text = stdout.read_text(errors="replace")
        errors = stderr.read_text(errors="replace")
        self.commands[-1]["returncode"] = code
        return code, text, errors

    def require(self, stage, command):
        code, text, errors = self.run(stage, command)
        if code:
            raise HarnessError(
                "tool_error", f"{stage} exited {code}; see {stage}.stdout/.stderr"
            )
        return text + errors


def tool(value):
    path = shutil.which(value)
    if path is None:
        raise HarnessError("tool_error", f"tool not found: {value}")
    return os.path.abspath(path)


def definitions(path):
    text = path.read_text()
    result = {}
    for match in re.finditer(r"^define\b[^\n]*?" + SYMBOL + r"[^\n]*\{\n", text, re.M):
        end = text.find("\n}", match.end())
        if end < 0:
            raise HarnessError("tool_error", f"unterminated LLVM function in {path}")
        result[match[1] or match[2]] = text[match.end() : end]
    return result


def calls(body):
    calls = []
    for line in body.splitlines():
        match = re.search(r"\b(call|invoke|callbr)\b", line)
        if match is None:
            continue
        callee = re.search(SYMBOL + r"\(", line[match.end() :])
        name = (callee[1] or callee[2]) if callee else "indirect/asm"
        if not name.startswith("llvm."):
            calls.append(name)
    return sorted(set(calls))


def rename_symbol(text, old, new):
    token = re.compile(SYMBOL + r'|"(?:[^"\\]|\\.)*"|;[^\n]*')
    return token.sub(
        lambda match: (
            f"@{new}"
            if match[0].startswith("@") and (match[1] or match[2]) == old
            else match[0]
        ),
        text,
    )


def rust_symbols(runner, executable):
    text = (runner.output / "rust.raw.ll").read_text()
    names = list(
        dict.fromkeys(match[1] or match[2] for match in re.finditer(SYMBOL, text))
    )
    path = runner.output / "symbols.txt"
    path.write_text("\n".join(names) + "\n")
    code, output, _ = runner.run(
        "demangle", [executable, "--no-strip-underscore"], path
    )
    if code or len(output.splitlines()) != len(names):
        raise HarnessError("tool_error", "could not demangle Rust symbols")
    mapped = {}
    for symbol, name in zip(names, output.splitlines()):
        if name.startswith(("translated::", "alive_input::")):
            leaf = re.sub(r"::h[0-9a-f]+$", "", name).rsplit("::", 1)[-1]
            mapped.setdefault(leaf, []).append(symbol)
    return mapped


def classify(code, stdout, stderr):
    counts = {}
    for key, label in (
        ("correct", "correct transformations"),
        ("incorrect", "incorrect transformations"),
        ("unproved", "failed-to-prove transformations"),
        ("errors", "Alive2 errors"),
    ):
        match = re.search(r"^\s*(\d+) " + label + r"\s*$", stdout, re.M)
        if match:
            counts[key] = int(match[1])
    text = stdout + stderr
    if len(counts) == 4:
        if counts["incorrect"]:
            return "incorrect", counts
        if code == 0 and counts == {
            "correct": 1,
            "incorrect": 0,
            "unproved": 0,
            "errors": 0,
        }:
            return "validated", counts
        if "timeout" in text.lower():
            return "timeout", counts
        if "unsupported" in text.lower() or "not supported" in text.lower():
            return "unsupported", counts
        if counts["errors"] or code:
            return "tool_error", counts
        return "unknown", counts
    if "unsupported" in text.lower() or "not supported" in text.lower():
        return "unsupported", counts
    if "timeout" in text.lower():
        return "timeout", counts
    return "tool_error", counts


def fixture_flags(source):
    flags = []
    for line in source.read_text(errors="replace").splitlines():
        if re.search(r"\bdg-(?:additional-)?options\b", line) is None:
            continue
        if re.search(r"\{\s*target\b", line):
            raise HarnessError(
                "unsupported", "target-conditional dg-options need selection"
            )
        match = re.search(r'\bdg-(?:additional-)?options\s+"((?:\\.|[^"\\])*)"', line)
        if match is None:
            raise HarnessError("unsupported", "cannot parse fixture dg-options")
        flags.extend(shlex.split(match[1]))
    return flags


def prepare(args, runner, report):
    tools = {
        name: tool(getattr(args, name))
        for name in ("clang", "rustc", "opt", "alive_tv", "cxxfilt")
    }
    if args.rust is None:
        tools.update({name: tool(getattr(args, name)) for name in ("slate", "cargo")})
    report["tools"] = tools
    runner.env["RUSTC"] = tools["rustc"]
    versions = {}
    for name, executable in tools.items():
        versions[name] = runner.require(
            f"version-{name}", [executable, "-vV" if name == "rustc" else "--version"]
        )
    report["versions"] = versions
    majors = {}
    for name in ("clang", "rustc", "opt", "alive_tv"):
        pattern = r"(?:clang version|LLVM version:?)[ \t]+(\d+)"
        match = re.search(pattern, versions[name], re.I)
        if match is None:
            raise HarnessError("tool_error", f"cannot determine {name}'s LLVM version")
        majors[name] = int(match[1])
    if len(set(majors.values())) != 1:
        raise HarnessError("tool_error", f"LLVM major versions differ: {majors}")
    host = re.search(r"^host: (.+)$", versions["rustc"], re.M)
    if host is None:
        raise HarnessError("tool_error", "cannot determine rustc host target")
    target = args.target or host[1]
    report["target"] = target
    output = runner.output
    c_flags = [
        f"--target={target}",
        "-std=c23",
        *fixture_flags(args.source),
        *args.c_arg,
    ]
    report["c_flags"] = c_flags
    runner.require(
        "compile-c",
        [
            tools["clang"],
            *c_flags,
            "-O0",
            "-g0",
            "-S",
            "-emit-llvm",
            "-Xclang",
            "-disable-O0-optnone",
            "-femit-all-decls",
            args.source,
            "-o",
            output / "c.raw.ll",
        ],
    )
    rust_flags = [
        "-C",
        "opt-level=0",
        "-C",
        "no-prepopulate-passes",
        "-C",
        "codegen-units=1",
        "-C",
        "panic=abort",
        "-C",
        "overflow-checks=off",
        "-C",
        "debug-assertions=off",
        "-C",
        "debuginfo=0",
        "-Z",
        "mir-opt-level=0",
        f"--emit=llvm-ir={output / 'rust.raw.ll'}",
    ]
    if args.rust:
        shutil.copyfile(args.rust, output / "input.rs")
        runner.require(
            "compile-rust",
            [
                tools["rustc"],
                "--crate-name",
                "alive_input",
                "--crate-type=lib",
                "--edition=2024",
                "--target",
                target,
                output / "input.rs",
                *rust_flags,
            ],
        )
    else:
        database = output / "compile_commands.json"
        database.write_text(
            json.dumps(
                [
                    {
                        "directory": str(ROOT),
                        "file": str(args.source),
                        "arguments": [tools["clang"], *c_flags, "-c", str(args.source)],
                    }
                ],
                indent=2,
            )
            + "\n"
        )
        project = output / "translated"
        runner.require(
            "translate",
            [
                tools["slate"],
                "translate-project",
                "--compile-commands",
                database,
                args.source.parent,
                project,
            ],
        )
        manifest = tomllib.loads((project / "Cargo.toml").read_text())
        crate_selector = (
            ["--lib"] if "lib" in manifest else ["--bin", manifest["package"]["name"]]
        )
        runner.require(
            "compile-rust",
            [
                tools["cargo"],
                "rustc",
                "--manifest-path",
                project / "Cargo.toml",
                *crate_selector,
                "--target",
                target,
                "--target-dir",
                args.cargo_target_dir.resolve(),
                "--",
                *rust_flags,
            ],
        )
    for language in ("c", "rust"):
        runner.require(
            f"normalize-{language}",
            [
                tools["opt"],
                "-S",
                f"-passes={PASSES}",
                output / f"{language}.raw.ll",
                "-o",
                output / f"{language}.ll",
            ],
        )
    return tools


def validate(args, runner, tools, report):
    functions = {
        language: definitions(runner.output / f"{language}.ll")
        for language in ("c", "rust")
    }
    original = definitions(runner.output / "c.raw.ll")
    selected = args.function or list(original)
    report["functions"] = selected
    symbols = rust_symbols(runner, tools["cxxfilt"])
    source_text = (runner.output / "c.raw.ll").read_text()
    source_symbols = {
        match[1] or match[2] for match in re.finditer(SYMBOL, source_text)
    }
    target_text = (runner.output / "rust.ll").read_text()
    mappings = {}
    if "main" in original and len(symbols.get("__slate_main", [])) == 1:
        if "@__slate_runtime_main" in target_text:
            raise HarnessError("tool_error", "runtime main symbol collision")
        target_text = rename_symbol(target_text, "main", "__slate_runtime_main")
        mappings[symbols["__slate_main"][0]] = "main"
    target_symbols = {
        match[1] or match[2] for match in re.finditer(SYMBOL, target_text)
    }
    for name in source_symbols:
        candidates = symbols.get(name, [])
        if len(candidates) == 1 and name not in target_symbols:
            mappings[candidates[0]] = name
    for old, new in mappings.items():
        target_text = rename_symbol(target_text, old, new)
    (runner.output / "rust.paired.ll").write_text(target_text)
    report["symbol_map"] = mappings
    functions["rust"] = definitions(runner.output / "rust.paired.ll")
    for index, name in enumerate(selected):
        result = {"function": name}
        report["results"].append(result)
        candidates = symbols.get("__slate_main" if name == "main" else name, [])
        target_name = name if name in functions["rust"] else None
        if target_name is None and len(candidates) == 1:
            target_name = candidates[0]
        if target_name is None and args.rust and name in functions["rust"]:
            target_name = name
        if name not in functions["c"] or target_name not in functions["rust"]:
            result.update(
                status="unsupported",
                reason="no unique corresponding function in normalized IR",
            )
            continue
        result["rust_function"] = target_name
        call_sets = {
            "c": calls(functions["c"][name]),
            "rust": calls(functions["rust"][target_name]),
        }
        result["calls"] = call_sets
        if call_sets["c"] != call_sets["rust"]:
            result.update(
                status="unsupported",
                reason="different uninlined calls require relational modeling",
                calls=call_sets,
            )
            continue
        pair = runner.output / f"pair-{index}"
        pair.mkdir()
        check_name = f"__slate_alive_check_{index}"
        for language, symbol in (("c", name), ("rust", target_name)):
            path = "rust.paired.ll" if language == "rust" else "c.ll"
            text = (runner.output / path).read_text()
            if f"@{check_name}" in text:
                raise HarnessError("tool_error", "validation symbol collision")
            (pair / f"{language}.ll").write_text(
                rename_symbol(text, symbol, check_name)
            )
        try:
            code, stdout, stderr = runner.run(
                f"alive-{index}",
                [
                    tools["alive_tv"],
                    f"--func={check_name}",
                    f"--src-unroll={args.unroll}",
                    f"--tgt-unroll={args.unroll}",
                    f"--smt-to={args.smt_timeout}",
                    pair / "c.ll",
                    pair / "rust.ll",
                ],
            )
            result["status"], result["counts"] = classify(code, stdout, stderr)
        except HarnessError as error:
            result.update(status=error.status, reason=str(error))


def main():
    parser = argparse.ArgumentParser(
        description="Opt-in, bounded C-to-Rust translation validation"
    )
    parser.add_argument(
        "sources", type=Path, nargs="+", help="C files or recursive fixture directories"
    )
    parser.add_argument(
        "--function",
        "-f",
        action="append",
        help="function to validate; repeatable; default: all C definitions",
    )
    parser.add_argument(
        "--rust",
        type=Path,
        help="compare supplied Rust instead of translating with Slate",
    )
    parser.add_argument(
        "--output",
        type=Path,
        help="new artifact directory; default: unique directory under target/alive2",
    )
    parser.add_argument("--target", help="target triple; default: rustc host")
    parser.add_argument(
        "--c-arg",
        action="append",
        default=[],
        help="shared Clang/Slate argument, e.g. --c-arg=-fwrapv",
    )
    parser.add_argument(
        "--timeout", type=float, default=60, help="wall timeout per command in seconds"
    )
    parser.add_argument(
        "--smt-timeout",
        type=int,
        default=10000,
        help="timeout per SMT query in milliseconds",
    )
    parser.add_argument("--unroll", type=int, default=4)
    parser.add_argument(
        "--cargo-target-dir", type=Path, default=ROOT / "target/alive2-cargo"
    )
    for name, default in (
        ("clang", "clang"),
        ("rustc", "rustc"),
        ("opt", "opt"),
        ("cxxfilt", "llvm-cxxfilt"),
        ("cargo", "cargo"),
        ("alive-tv", "alive-tv"),
        ("slate", str(ROOT / "target/test-cache/release/slate")),
    ):
        env = "SLATE_" + name.upper().replace("-", "_")
        parser.add_argument(f"--{name}", default=os.environ.get(env, default))
    parser.add_argument(
        "--glob", action="append", help="relative path glob; repeatable"
    )
    parser.add_argument("--limit", type=int, help="maximum number of fixtures")
    args = parser.parse_args()
    if Path.cwd() != ROOT:
        parser.error("run from the workspace root")
    if args.timeout <= 0 or args.smt_timeout <= 0 or args.unroll < 0:
        parser.error("timeouts must be positive and unroll must be nonnegative")
    if args.function and any(
        not re.fullmatch(r"[a-zA-Z_][a-zA-Z_0-9]*", name) for name in args.function
    ):
        parser.error("function names must be C identifiers")
    args.function = list(dict.fromkeys(args.function or []))
    sources = {}
    for source in args.sources:
        source = source.resolve()
        if source.is_dir():
            sources.update(
                (path, path.relative_to(source).as_posix())
                for path in sorted(source.rglob("*.c"))
            )
        elif source.is_file() and source.suffix == ".c":
            sources[source] = source.name
        else:
            parser.error(f"not a C file or directory: {source}")
    sources = {
        path: label
        for path, label in sources.items()
        if not args.glob
        or any(fnmatch.fnmatch(label, pattern) for pattern in args.glob)
    }
    if args.limit is not None:
        if args.limit < 1:
            parser.error("--limit must be positive")
        sources = dict(list(sources.items())[: args.limit])
    if not sources:
        parser.error("no C fixtures selected")
    if args.rust:
        args.rust = args.rust.resolve()
        if not args.rust.is_file() or len(sources) != 1:
            parser.error("--rust requires an existing file and exactly one C fixture")
    try:
        if args.output:
            output = args.output.resolve()
            output.mkdir(parents=True, exist_ok=False)
        else:
            parent = ROOT / "target/alive2"
            parent.mkdir(parents=True, exist_ok=True)
            output = Path(tempfile.mkdtemp(prefix="run-", dir=parent))
    except OSError as error:
        parser.error(str(error))
    reports: list[dict] = []
    batch: dict = {"fixtures": reports, "status": "running"}
    for source, label in sources.items():
        case_args = copy.copy(args)
        case_args.source = source
        key = source.stem + "-" + hashlib.sha256(str(source).encode()).hexdigest()[:10]
        case_output = output / key
        case_output.mkdir()
        report = run_case(case_args, case_output)
        reports.append(
            {
                "fixture": label,
                "source": str(source),
                "status": report["status"],
                "report": str(case_output / "report.json"),
                "results": report["results"],
                **({"reason": report["reason"]} if "reason" in report else {}),
            }
        )
        print(
            f"{report['status']}: {label}"
            + (f" ({report['reason']})" if "reason" in report else ""),
            flush=True,
        )
        for result in report["results"]:
            print(
                f"  {result['status']}: {result['function']}"
                + (f" ({result['reason']})" if "reason" in result else ""),
                flush=True,
            )
        batch["fixture_counts"] = dict(Counter(item["status"] for item in reports))
        batch["function_counts"] = dict(
            Counter(result["status"] for item in reports for result in item["results"])
        )
        (output / "report.json").write_text(json.dumps(batch, indent=2) + "\n")
    batch["status"] = overall(item["status"] for item in reports)
    (output / "report.json").write_text(json.dumps(batch, indent=2) + "\n")
    print(f"fixtures: {batch['fixture_counts']}")
    print(f"functions: {batch['function_counts']}")
    print(
        f"artifacts: {output.relative_to(ROOT) if output.is_relative_to(ROOT) else output}"
    )
    return (
        0
        if batch["status"] == "validated"
        else 1
        if batch["status"] == "incorrect"
        else 2
    )


def overall(statuses):
    statuses = list(statuses)
    return next(
        (
            status
            for status in (
                "incorrect",
                "tool_error",
                "timeout",
                "unsupported",
                "unknown",
            )
            if status in statuses
        ),
        "validated" if statuses else "unknown",
    )


def run_case(args, output):
    runner = Runner(output, args.timeout)
    report = {
        "source": str(args.source),
        "rust_override": str(args.rust) if args.rust else None,
        "functions": args.function,
        "normalization": PASSES,
        "unroll": args.unroll,
        "smt_timeout_ms": args.smt_timeout,
        "command_timeout_seconds": args.timeout,
        "results": [],
        "commands": runner.commands,
        "rustflags": runner.env["RUSTFLAGS"],
    }
    try:
        report["source_sha256"] = hashlib.sha256(args.source.read_bytes()).hexdigest()
        shutil.copyfile(args.source, output / "input.c")
        parts = args.source.parts
        if "msvc" in parts or "gcc" in parts:
            raise HarnessError(
                "unsupported",
                "Clang LLVM oracle does not model this fixture's compiler flavor",
            )
        if args.target is None:
            for arch, triple in (
                ("aarch64", "aarch64-unknown-linux-gnu"),
                ("i686", "i686-unknown-linux-gnu"),
                ("arm", "armv7-unknown-linux-gnueabihf"),
                ("x86_64", "x86_64-unknown-linux-gnu"),
            ):
                if arch in parts:
                    args.target = triple
                    break
        tools = prepare(args, runner, report)
        validate(args, runner, tools, report)
    except HarnessError as error:
        report.update(status=error.status, reason=str(error))
    except (OSError, ValueError) as error:
        report.update(status="tool_error", reason=str(error))
    if "status" not in report:
        report["status"] = overall(result["status"] for result in report["results"])
    (output / "report.json").write_text(json.dumps(report, indent=2) + "\n")
    return report


if __name__ == "__main__":
    sys.exit(main())
