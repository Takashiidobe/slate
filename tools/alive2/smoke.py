import json
import subprocess
import sys
import tempfile
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
HERE = Path(__file__).resolve().parent


def main():
    if Path.cwd() != ROOT:
        raise SystemExit("run from the workspace root")
    parent = ROOT / "target/alive2"
    parent.mkdir(parents=True, exist_ok=True)
    output = Path(tempfile.mkdtemp(prefix="smoke-", dir=parent))
    invalid = output / "invalid.rs"
    invalid.write_text("this is not Rust\n")
    slow = output / "slow-alive"
    slow.write_text(f"#!{sys.executable}\nimport time\ntime.sleep(30)\n")
    slow.chmod(0o755)
    cases = (
        ("directory", [HERE / "fixtures"], 0, "validated"),
        (
            "counterexample",
            [
                HERE / "fixtures/scalars.c",
                "-f",
                "add",
                "--rust",
                HERE / "fixtures/wrong.rs",
            ],
            1,
            "incorrect",
        ),
        (
            "missing",
            [
                HERE / "fixtures/scalars.c",
                "-f",
                "absent",
                "--rust",
                HERE / "fixtures/wrong.rs",
            ],
            2,
            "unsupported",
        ),
        (
            "existing",
            [ROOT / "crates/slate/tests/fixtures", "--glob=conditionals.c"],
            2,
            "unsupported",
        ),
        (
            "compile-error",
            [HERE / "fixtures/scalars.c", "-f", "add", "--rust", invalid],
            2,
            "tool_error",
        ),
        (
            "timeout",
            [HERE / "fixtures/scalars.c", "--alive-tv", slow, "--timeout=1"],
            2,
            "timeout",
        ),
    )
    for name, arguments, expected_code, expected_status in cases:
        directory = output / name
        result = subprocess.run(
            [
                sys.executable,
                str(HERE / "validate.py"),
                *map(str, arguments),
                "--output",
                str(directory),
            ],
            cwd=ROOT,
        )
        report = json.loads((directory / "report.json").read_text())
        if result.returncode != expected_code or report["status"] != expected_status:
            raise RuntimeError(f"{name}: unexpected result; see {directory}")
        if name == "directory" and report["function_counts"] != {"validated": 6}:
            raise RuntimeError("directory run did not validate all six functions")
        if name == "existing" and report["function_counts"].get("validated", 0) < 2:
            raise RuntimeError("existing fixture helpers were not validated")
    print(f"smoke checks passed: {output.relative_to(ROOT)}")


if __name__ == "__main__":
    main()
