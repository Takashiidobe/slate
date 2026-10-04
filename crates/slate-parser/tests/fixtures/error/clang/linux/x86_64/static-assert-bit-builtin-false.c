// SLATE-FILECHECK-ERROR PARSE

_Static_assert(__builtin_popcount(7U) == 2, "popcount");

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × semantic analysis failed
// PARSE: Error:
// PARSE: × static assertion failed: popcount
// PARSE: ╭─[tests/fixtures/error/clang/linux/x86_64/static-assert-bit-builtin-false.c:2:16]
// PARSE: 1 │
// PARSE: 2 │ _Static_assert(__builtin_popcount(7U) == 2, "popcount");
// PARSE: ·                ───────────────────────────
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
