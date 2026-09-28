#pragma float_control(precise, ON)
int x;

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × pragma float_control is malformed; use 'float_control({push|pop})' or
// DEFAULT: ╭─[tests/fixtures/clang/linux/x86_64/fp_pragma_float_control_malformed.c:1:1]
// DEFAULT: 1 │ #pragma float_control(precise, ON)
// DEFAULT: · ──────────────────────────────────
// DEFAULT: 2 │ int x;
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
