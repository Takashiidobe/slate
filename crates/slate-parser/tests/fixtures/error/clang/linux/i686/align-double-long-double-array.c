long double values[2];

// SLATE-FILECHECK-ARGS -malign-double
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × semantic analysis failed
// PARSE: Error:
// PARSE:   × array element size is not a multiple of its alignment
// PARSE:    ╭─[tests/fixtures/error/clang/linux/i686/align-double-long-double-array.c:1:1]
// PARSE:  1 │ long double values[2];
// PARSE:    · ──────────────────────
// PARSE:  2 │
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
