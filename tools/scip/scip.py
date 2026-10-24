#!/usr/bin/env python3
import argparse
import contextlib
import fcntl
import importlib
import io
import json
import os
import pathlib
import re
import sqlite3
import subprocess
import sys
import time

ROOT = pathlib.Path(__file__).resolve().parents[2]
TOOL_DIR = pathlib.Path(__file__).resolve().parent
OUT = ROOT / "target" / "scip"
DB = OUT / "index.db"
LOCK = OUT / "build.lock"
RERUN = OUT / "rebuild-requested"
LOG = OUT / "build.log"

FUNCTION_KINDS = {"Function", "Method", "StaticMethod", "AbstractMethod", "TraitMethod"}

SCHEMA = """
CREATE TABLE meta(key TEXT PRIMARY KEY, value TEXT);
CREATE TABLE files(id INTEGER PRIMARY KEY, path TEXT UNIQUE);
CREATE TABLE symbols(
    id INTEGER PRIMARY KEY, symbol TEXT UNIQUE, name TEXT, kind TEXT,
    crate TEXT, signature TEXT, docs TEXT);
CREATE TABLE occurrences(
    symbol_id INTEGER, file_id INTEGER, line INTEGER, col INTEGER,
    is_definition INTEGER, enclosing_id INTEGER);
"""

INDEXES = """
CREATE INDEX symbols_name ON symbols(name);
CREATE INDEX occurrences_symbol ON occurrences(symbol_id);
CREATE INDEX occurrences_file ON occurrences(file_id, line);
"""


def git(*args):
    return subprocess.run(["git", *args], cwd=ROOT, capture_output=True, text=True).stdout.strip()


def load_scip_pb2():
    generated = OUT / "scip_pb2.py"
    proto = TOOL_DIR / "scip.proto"
    if not generated.exists() or generated.stat().st_mtime < proto.stat().st_mtime:
        subprocess.run(
            ["protoc", f"-I{TOOL_DIR}", f"--python_out={OUT}", str(proto)], check=True
        )
    sys.path.insert(0, str(OUT))
    return importlib.import_module("scip_pb2")


def crate_of(symbol):
    parts = symbol.split(" ", 4)
    return parts[2] if len(parts) == 5 else ""


def name_of(symbol):
    descriptor = symbol.split(" ", 4)[-1].rstrip(".#/:!")
    descriptor = re.sub(r"\([^)]*\)$", "", descriptor)
    return re.split(r"[/#.:\[\]()]", descriptor)[-1]


def signature_and_docs(info):
    signature = info.signature_documentation.text
    docs = list(info.documentation)
    if not signature and docs and docs[0].startswith("```"):
        signature = docs.pop(0).strip("`\n").removeprefix("rust\n")
    return signature, "\n\n".join(docs)


def innermost_function(definitions, line):
    best = None
    for start, end, symbol_id in definitions:
        if start <= line <= end and (best is None or start >= best[0]):
            best = (start, end, symbol_id)
    return best[2] if best else None


def load(scip_path, db_path, commit):
    pb2 = load_scip_pb2()
    index = pb2.Index()
    index.ParseFromString(scip_path.read_bytes())
    kind_name = pb2.SymbolInformation.Kind.Name
    db_path.unlink(missing_ok=True)
    con = sqlite3.connect(db_path)
    con.executescript(SCHEMA)
    symbol_ids = {}
    info_rows = {}

    def symbol_id(symbol):
        if symbol not in symbol_ids:
            symbol_ids[symbol] = len(symbol_ids) + 1
        return symbol_ids[symbol]

    for document in index.documents:
        for info in document.symbols:
            if info.symbol.startswith("local "):
                continue
            signature, docs = signature_and_docs(info)
            info_rows[info.symbol] = (info.display_name, kind_name(info.kind), signature, docs)
    for info in index.external_symbols:
        signature, docs = signature_and_docs(info)
        info_rows.setdefault(
            info.symbol, (info.display_name, kind_name(info.kind), signature, docs)
        )

    occurrence_rows = []
    for file_id, document in enumerate(index.documents, 1):
        con.execute("INSERT INTO files VALUES (?, ?)", (file_id, document.relative_path))
        functions = []
        for occurrence in document.occurrences:
            if occurrence.symbol.startswith("local ") or not occurrence.symbol:
                continue
            is_definition = occurrence.symbol_roles & pb2.Definition
            kind = info_rows.get(occurrence.symbol, ("", "", "", ""))[1]
            if is_definition and kind in FUNCTION_KINDS and occurrence.enclosing_range:
                enclosing = occurrence.enclosing_range
                end_line = enclosing[2] if len(enclosing) == 4 else enclosing[0]
                functions.append((enclosing[0], end_line, symbol_id(occurrence.symbol)))
        for occurrence in document.occurrences:
            if occurrence.symbol.startswith("local ") or not occurrence.symbol:
                continue
            line, col = occurrence.range[0], occurrence.range[1]
            is_definition = 1 if occurrence.symbol_roles & pb2.Definition else 0
            sid = symbol_id(occurrence.symbol)
            enclosing = innermost_function(functions, line)
            occurrence_rows.append(
                (sid, file_id, line + 1, col + 1, is_definition, None if enclosing == sid else enclosing)
            )

    con.executemany(
        "INSERT INTO symbols VALUES (?, ?, ?, ?, ?, ?, ?)",
        (
            (
                sid,
                symbol,
                info_rows.get(symbol, ("",))[0] or name_of(symbol),
                info_rows.get(symbol, ("", ""))[1],
                crate_of(symbol),
                info_rows.get(symbol, ("", "", ""))[2],
                info_rows.get(symbol, ("", "", "", ""))[3],
            )
            for symbol, sid in symbol_ids.items()
        ),
    )
    con.executemany("INSERT INTO occurrences VALUES (?, ?, ?, ?, ?, ?)", occurrence_rows)
    con.executescript(INDEXES)
    con.executemany(
        "INSERT INTO meta VALUES (?, ?)",
        [("commit", commit), ("built_at", str(int(time.time())))],
    )
    con.commit()
    con.close()


def build_once():
    commit = git("rev-parse", "HEAD")
    scip_tmp = OUT / "index.scip.tmp"
    db_tmp = OUT / "index.db.tmp"
    started = time.time()
    subprocess.run(
        ["rust-analyzer", "scip", str(ROOT), "--output", str(scip_tmp)], cwd=ROOT, check=True
    )
    load(scip_tmp, db_tmp, commit)
    os.replace(db_tmp, DB)
    scip_tmp.unlink(missing_ok=True)
    print(f"indexed {commit[:12]} in {time.time() - started:.0f}s", flush=True)


def build(_args):
    OUT.mkdir(parents=True, exist_ok=True)
    with open(LOCK, "w") as lock:
        try:
            fcntl.flock(lock, fcntl.LOCK_EX | fcntl.LOCK_NB)
        except BlockingIOError:
            RERUN.touch()
            print("a build is already running; it will rebuild when done")
            return
        while True:
            RERUN.unlink(missing_ok=True)
            build_once()
            if not RERUN.exists():
                return


def build_background(_args):
    OUT.mkdir(parents=True, exist_ok=True)
    with open(LOG, "a") as log:
        subprocess.Popen(
            [sys.executable, __file__, "build"],
            cwd=ROOT,
            stdin=subprocess.DEVNULL,
            stdout=log,
            stderr=log,
            start_new_session=True,
        )


def building():
    if not LOCK.exists():
        return False
    with open(LOCK) as lock:
        try:
            fcntl.flock(lock, fcntl.LOCK_SH | fcntl.LOCK_NB)
        except BlockingIOError:
            return True
    return False


def connect():
    if not DB.exists():
        sys.exit("no index yet: run `python3 tools/scip/scip.py build`")
    return sqlite3.connect(DB)


def status(_args):
    if not DB.exists():
        print("no index" + (" (building)" if building() else ""))
        return
    con = connect()
    meta = dict(con.execute("SELECT key, value FROM meta"))
    commit = meta["commit"]
    age = int(time.time()) - int(meta["built_at"])
    head = git("rev-parse", "HEAD")
    state = "current" if commit == head else f"stale (HEAD is {head[:12]})"
    print(f"index at {commit[:12]}, built {age // 60} min ago, {state}"
          + ("; rebuild running" if building() else ""))
    changed = sorted(set(git("diff", "--name-only", commit, "--", "*.rs").split()))
    if changed:
        print(f"{len(changed)} .rs files differ from the index (lines there may be off):")
        for path in changed[:20]:
            print(f"  {path}")


def session_context(_args):
    if not DB.exists() and not building():
        build_background(_args)
    report = io.StringIO()
    with contextlib.redirect_stdout(report):
        status(_args)
    context = (
        "Rust navigation: query the SCIP index first with `python3 tools/scip/scip.py "
        "def|refs|callers|hover|outline|search <name>` (see Navigating code in CLAUDE.md). "
        "Use the LSP for files you edit this session or files listed below as changed.\n"
        f"SCIP index: {report.getvalue().strip()}"
    )
    print(json.dumps({"hookSpecificOutput": {
        "hookEventName": "SessionStart", "additionalContext": context}}))


def matching_symbols(con, query, crate):
    parts = query.split("::")
    rows = con.execute(
        "SELECT id, symbol, name, kind, crate, signature, docs FROM symbols WHERE name = ?",
        (parts[-1],),
    ).fetchall()
    qualifiers = parts[:-1]

    def qualified(symbol):
        words = re.findall(r"[A-Za-z_][A-Za-z0-9_]*", symbol.split(" ", 4)[-1])[:-1]
        position = 0
        for qualifier in qualifiers:
            try:
                position = words.index(qualifier, position) + 1
            except ValueError:
                return False
        return True

    return [
        row for row in rows
        if qualified(row[1]) and (crate is None or row[4] == crate)
    ]


def definition_site(con, symbol_id):
    return con.execute(
        "SELECT f.path, o.line FROM occurrences o JOIN files f ON f.id = o.file_id "
        "WHERE o.symbol_id = ? AND o.is_definition = 1 ORDER BY f.path, o.line LIMIT 1",
        (symbol_id,),
    ).fetchone()


def symbols_or_exit(con, args):
    rows = matching_symbols(con, args.name, args.crate)
    if not rows:
        sys.exit(f"no symbol named {args.name}")
    return rows


def describe(con, row):
    site = definition_site(con, row[0])
    where = f"{site[0]}:{site[1]}" if site else "(external)"
    return f"{where}  {row[3] or '?'}  {one_line(row[5]) or row[2]}"


def one_line(signature):
    return re.sub(r"\(\s+|\s+\)", lambda m: m[0].strip(), re.sub(r"\s*\n\s*", " ", signature))


def cmd_def(args):
    con = connect()
    rows = symbols_or_exit(con, args)
    defined = [row for row in rows if definition_site(con, row[0])]
    for row in defined or rows:
        print(describe(con, row))


def cmd_hover(args):
    con = connect()
    for row in symbols_or_exit(con, args):
        print(describe(con, row))
        if row[6]:
            print(row[6])
        print()


def enclosing_name(con, enclosing_id):
    if enclosing_id is None:
        return ""
    row = con.execute("SELECT name FROM symbols WHERE id = ?", (enclosing_id,)).fetchone()
    return f"  in {row[0]}" if row else ""


def cmd_refs(args):
    con = connect()
    for row in symbols_or_exit(con, args):
        print(describe(con, row))
        for path, line, col, enclosing in con.execute(
            "SELECT f.path, o.line, o.col, o.enclosing_id FROM occurrences o "
            "JOIN files f ON f.id = o.file_id WHERE o.symbol_id = ? AND o.is_definition = 0 "
            "ORDER BY f.path, o.line",
            (row[0],),
        ):
            print(f"  {path}:{line}:{col}{enclosing_name(con, enclosing)}")


def cmd_callers(args):
    con = connect()
    for row in symbols_or_exit(con, args):
        print(describe(con, row))
        for enclosing, in con.execute(
            "SELECT DISTINCT o.enclosing_id FROM occurrences o WHERE o.symbol_id = ? "
            "AND o.is_definition = 0 AND o.enclosing_id IS NOT NULL",
            (row[0],),
        ):
            caller = con.execute(
                "SELECT id, symbol, name, kind, crate, signature, docs FROM symbols WHERE id = ?",
                (enclosing,),
            ).fetchone()
            print(f"  {describe(con, caller)}")


def cmd_outline(args):
    con = connect()
    path = os.path.relpath(pathlib.Path(args.file).resolve(), ROOT)
    rows = con.execute(
        "SELECT o.line, s.kind, s.name, s.signature FROM occurrences o "
        "JOIN symbols s ON s.id = o.symbol_id JOIN files f ON f.id = o.file_id "
        "WHERE f.path = ? AND o.is_definition = 1 ORDER BY o.line",
        (path,),
    ).fetchall()
    if not rows:
        sys.exit(f"no definitions indexed for {path}")
    for line, kind, name, signature in rows:
        print(f"{line:>6}  {kind or '?':<14} {one_line(signature) or name}")


def cmd_search(args):
    con = connect()
    query = "SELECT id, symbol, name, kind, crate, signature, docs FROM symbols WHERE name LIKE ?"
    params = [f"%{args.pattern}%"]
    if args.crate:
        query += " AND crate = ?"
        params.append(args.crate)
    for row in con.execute(query + " ORDER BY name LIMIT 200", params):
        if definition_site(con, row[0]):
            print(describe(con, row))


def main():
    parser = argparse.ArgumentParser(description="Query the SCIP index of the workspace.")
    sub = parser.add_subparsers(dest="command", required=True)
    sub.add_parser("build", help="regenerate the index now").set_defaults(func=build)
    sub.add_parser("build-background", help="regenerate detached, logging to target/scip/build.log").set_defaults(func=build_background)
    sub.add_parser("status", help="index commit, age, and files changed since").set_defaults(func=status)
    sub.add_parser("session-context", help="SessionStart hook output").set_defaults(func=session_context)
    for name, func, text in [
        ("def", cmd_def, "definition site and signature"),
        ("hover", cmd_hover, "signature and docs"),
        ("refs", cmd_refs, "references with their enclosing function"),
        ("callers", cmd_callers, "functions that reference the symbol"),
    ]:
        command = sub.add_parser(name, help=text)
        command.add_argument("name", help="name, optionally qualified: Preprocessor::reject_poisoned")
        command.add_argument("--crate")
        command.set_defaults(func=func)
    outline = sub.add_parser("outline", help="definitions in a file")
    outline.add_argument("file")
    outline.set_defaults(func=cmd_outline)
    search = sub.add_parser("search", help="defined symbols whose name contains a substring")
    search.add_argument("pattern")
    search.add_argument("--crate")
    search.set_defaults(func=cmd_search)
    args = parser.parse_args()
    args.func(args)


if __name__ == "__main__":
    main()
