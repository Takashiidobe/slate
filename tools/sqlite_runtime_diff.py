#!/usr/bin/env python3
import argparse
import ctypes
import pathlib
import subprocess
import tempfile
import threading
import time


SQL = """
PRAGMA foreign_keys=ON;
CREATE TABLE parent(id INTEGER PRIMARY KEY);
INSERT INTO parent VALUES(1);
CREATE TABLE data(id INTEGER PRIMARY KEY, parent INTEGER REFERENCES parent, name TEXT, score REAL, payload BLOB);
BEGIN;
WITH RECURSIVE n(i) AS (VALUES(1) UNION ALL SELECT i+1 FROM n WHERE i<120)
INSERT INTO data SELECT i,1,printf('row-%03d',i),i*0.5-7,CAST(printf('%04x',i) AS BLOB) FROM n;
SAVEPOINT keep;
DELETE FROM data WHERE id<10;
ROLLBACK TO keep;
RELEASE keep;
COMMIT;
SELECT count(*),sum(id),round(avg(score),3),min(name),max(name) FROM data;
SELECT id,hex(payload),score FROM data WHERE id IN (1,60,120) ORDER BY id;
SELECT id,row_number() OVER (PARTITION BY id%3 ORDER BY id),sum(id) OVER (ORDER BY id ROWS 2 PRECEDING) FROM data WHERE id<=9;
SELECT length('é猫'),substr('aé猫z',2,2),hex(char(0,65)),quote(x'00ff10');
SELECT json_extract('{"a":[1,2,3],"b":{"x":"é"}}','$.b.x'),json_array_length('[1,2,3]'),json_valid('{bad}');
SELECT json_patch('{"a":1,"b":2}','{"b":null,"c":3}');
SELECT group_concat(value,'|') FROM json_each('[1,2,3]');
SELECT round(sqrt(81),3),round(pow(2,10),3);
CREATE INDEX by_name ON data(name);
SELECT count(*) FROM data WHERE name>='row-100';
DELETE FROM data WHERE id%7=0;
VACUUM;
PRAGMA integrity_check;
"""


def shell(binary, database, sql):
    result = subprocess.run(
        [str(binary), "-batch", "-bail", str(database)],
        input=sql.encode(), capture_output=True, timeout=30,
    )
    return result.returncode, result.stdout, result.stderr


def compare(label, native, translated):
    if native != translated:
        raise AssertionError(f"{label}: native={native!r}, generated={translated!r}")
    print(f"PASS {label}")


def compare_shell(label, native, translated, error=False):
    if bool(native[0]) != error:
        raise AssertionError(f"{label}: unexpected native result {native!r}")
    compare(label, native, translated)


class Library:
    callback_type = ctypes.CFUNCTYPE(
        ctypes.c_int, ctypes.c_void_p, ctypes.c_int,
        ctypes.POINTER(ctypes.c_char_p), ctypes.POINTER(ctypes.c_char_p),
    )

    def __init__(self, path):
        self.lib = ctypes.CDLL(str(path))
        self.lib.sqlite3_open.argtypes = [ctypes.c_char_p, ctypes.POINTER(ctypes.c_void_p)]
        self.lib.sqlite3_open.restype = ctypes.c_int
        self.lib.sqlite3_close.argtypes = [ctypes.c_void_p]
        self.lib.sqlite3_close.restype = ctypes.c_int
        self.lib.sqlite3_exec.argtypes = [
            ctypes.c_void_p, ctypes.c_char_p, self.callback_type,
            ctypes.c_void_p, ctypes.POINTER(ctypes.c_void_p),
        ]
        self.lib.sqlite3_exec.restype = ctypes.c_int
        self.lib.sqlite3_free.argtypes = [ctypes.c_void_p]
        self.lib.sqlite3_free.restype = None
        self.lib.sqlite3_interrupt.argtypes = [ctypes.c_void_p]
        self.lib.sqlite3_interrupt.restype = None

    def open(self, path=":memory:"):
        connection = ctypes.c_void_p()
        code = self.lib.sqlite3_open(str(path).encode(), ctypes.byref(connection))
        if code:
            raise RuntimeError(f"sqlite3_open returned {code}")
        return connection

    def execute(self, connection, sql):
        rows = []

        @self.callback_type
        def callback(context, count, values, names):
            rows.append(tuple(values[i] for i in range(count)))
            return 0

        error = ctypes.c_void_p()
        code = self.lib.sqlite3_exec(connection, sql.encode(), callback, None, ctypes.byref(error))
        message = ctypes.string_at(error) if error else None
        self.lib.sqlite3_free(error)
        return code, rows, message

    def memory(self):
        connection = self.open()
        try:
            result = self.execute(connection, SQL)
            if result[0] or result[1][0] != (b"120", b"7260", b"23.25", b"row-001", b"row-120"):
                raise AssertionError(result)
            return result
        finally:
            if self.lib.sqlite3_close(connection):
                raise RuntimeError("sqlite3_close failed")

    def concurrent(self, path):
        connection = self.open(path)
        setup = self.execute(connection, "PRAGMA journal_mode=WAL; CREATE TABLE counter(n); INSERT INTO counter VALUES(0);")
        if setup[0]:
            raise RuntimeError(setup)
        errors = []

        def worker():
            other = self.open(path)
            try:
                self.execute(other, "PRAGMA busy_timeout=5000;")
                for _ in range(40):
                    result = self.execute(other, "BEGIN IMMEDIATE; UPDATE counter SET n=n+1; COMMIT;")
                    if result[0]:
                        errors.append(result)
                        return
            finally:
                self.lib.sqlite3_close(other)

        threads = [threading.Thread(target=worker, daemon=True) for _ in range(2)]
        for thread in threads:
            thread.start()
        for thread in threads:
            thread.join(15)
        if any(thread.is_alive() for thread in threads):
            raise RuntimeError("WAL workers timed out")
        try:
            result = self.execute(connection, "SELECT n FROM counter; PRAGMA integrity_check;")
            if errors or result != (0, [(b"80",), (b"ok",)], None):
                raise AssertionError((errors, result))
            return errors, result
        finally:
            self.lib.sqlite3_close(connection)

    def interrupt(self):
        connection = self.open()
        result = []
        thread = threading.Thread(
            target=lambda: result.append(self.execute(connection, "WITH RECURSIVE n(i) AS (VALUES(1) UNION ALL SELECT i+1 FROM n WHERE i<1000000000) SELECT sum(i) FROM n;")),
            daemon=True,
        )
        thread.start()
        deadline = time.monotonic() + 5
        while thread.is_alive() and time.monotonic() < deadline:
            self.lib.sqlite3_interrupt(connection)
            thread.join(0.01)
        if thread.is_alive():
            raise RuntimeError("sqlite3_interrupt timed out")
        self.lib.sqlite3_close(connection)
        if result[0][0] != 9:
            raise AssertionError(f"expected SQLITE_INTERRUPT: {result!r}")
        return result


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("native_shell", type=pathlib.Path)
    parser.add_argument("generated_shell", type=pathlib.Path)
    parser.add_argument("--native-library", type=pathlib.Path)
    parser.add_argument("--generated-library", type=pathlib.Path)
    args = parser.parse_args()
    with tempfile.TemporaryDirectory(prefix="slate-sqlite-runtime-") as directory:
        root = pathlib.Path(directory)
        native_memory = shell(args.native_shell, ":memory:", SQL)
        if native_memory[0] or not native_memory[1].startswith(b"120|7260|23.25|row-001|row-120\n"):
            raise AssertionError(native_memory)
        compare_shell("SQL memory", native_memory, shell(args.generated_shell, ":memory:", SQL))
        for label, sql in [
            ("foreign key error", "PRAGMA foreign_keys=ON; CREATE TABLE p(id PRIMARY KEY); CREATE TABLE c(id REFERENCES p); INSERT INTO c VALUES(1);"),
            ("unique error", "CREATE TABLE t(x UNIQUE); INSERT INTO t VALUES(1),(1);"),
            ("SQL error", "SELECT missing FROM nowhere;"),
        ]:
            compare_shell(label, shell(args.native_shell, ":memory:", sql), shell(args.generated_shell, ":memory:", sql), error=True)
        paths = [root / "native.db", root / "generated.db"]
        setup = "PRAGMA journal_mode=WAL; CREATE TABLE durable(x); BEGIN; INSERT INTO durable VALUES(3),(5),(7); COMMIT; PRAGMA wal_checkpoint(TRUNCATE);"
        compare_shell("WAL commit", shell(args.native_shell, paths[0], setup), shell(args.generated_shell, paths[1], setup))
        reopen = "SELECT count(*),sum(x) FROM durable; PRAGMA integrity_check; .dump"
        reopen = reopen.replace("; .dump", ";\n.dump\n")
        compare_shell("reopen and dump", shell(args.native_shell, paths[0], reopen), shell(args.generated_shell, paths[1], reopen))
        csv = root / "input.csv"
        csv.write_text("1,alpha\n2,beta\n", encoding="utf-8")
        script = f'CREATE TABLE imported(id INTEGER,name TEXT);\n.mode csv\n.import "{csv}" imported\n.mode list\nSELECT * FROM imported ORDER BY id;\n'
        compare_shell("CSV import", shell(args.native_shell, ":memory:", script), shell(args.generated_shell, ":memory:", script))
        if bool(args.native_library) != bool(args.generated_library):
            parser.error("provide both library paths")
        if args.native_library:
            native, generated = Library(args.native_library), Library(args.generated_library)
            compare("library C API", native.memory(), generated.memory())
            compare("library WAL concurrency", native.concurrent(root / "native-concurrent.db"), generated.concurrent(root / "generated-concurrent.db"))
            compare("library interrupt", native.interrupt(), generated.interrupt())


if __name__ == "__main__":
    main()
