// SLATE-FILECHECK-ERROR PARSE

enum too_wide { TOO_WIDE_A = 300 } __attribute__((mode(QI)));

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × semantic analysis failed
// PARSE: Error:
// PARSE: × specified mode too small for enumerated values
// PARSE: ╭─[tests/fixtures/error/gcc/linux/x86_64/enum-mode-too-small.c:2:1]
// PARSE: 1 │
// PARSE: 2 │ enum too_wide { TOO_WIDE_A = 300 } __attribute__((mode(QI)));
// PARSE: · ─────────────────────────────────────────────────────────────
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
