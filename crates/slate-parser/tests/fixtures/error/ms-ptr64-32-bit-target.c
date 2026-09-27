int * __ptr64 wide;

// SLATE-FILECHECK-FLAVOR msvc
// SLATE-FILECHECK-ARGS -target=i686-unknown-linux-gnu
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × `__ptr64` on a 32-bit target is not supported
// PARSE: ╰─▶ `__ptr64` on a 32-bit target is not supported
// PARSE: ╭─[tests/fixtures/error/ms-ptr64-32-bit-target.c:1:7]
// PARSE: 1 │ int * __ptr64 wide;
// PARSE: ·       ───────
// PARSE: 2 │
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
