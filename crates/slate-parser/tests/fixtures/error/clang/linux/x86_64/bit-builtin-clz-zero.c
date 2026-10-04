// SLATE-FILECHECK-ERROR PARSE

enum { CLZ_ZERO = __builtin_clz(0U) };

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × semantic analysis failed
// PARSE: Error:
// PARSE: × builtin call is not a constant
// PARSE: ╭─[tests/fixtures/error/clang/linux/x86_64/bit-builtin-clz-zero.c:2:1]
// PARSE: 1 │
// PARSE: 2 │ enum { CLZ_ZERO = __builtin_clz(0U) };
// PARSE: · ──────────────────────────────────────
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
