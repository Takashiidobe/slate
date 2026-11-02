// SLATE-FILECHECK-DEFINES ERROR
// SLATE-FILECHECK-ERROR ERROR

void bad_state(void) __arm_inout("zz");

// SLATE-FILECHECK-BEGIN ERROR
// ERROR: Error:   × semantic analysis failed
// ERROR: Error:
// ERROR:   × invalid attribute
// ERROR:    ╭─[tests/fixtures/error/clang/linux/aarch64/sme_keyword_attribute_bad_state.c:2:6]
// ERROR:  1 │
// ERROR:  2 │ void bad_state(void) __arm_inout("zz");
// ERROR:    ·      ─────────────────────────────────
// ERROR:  3 │
// ERROR:    ╰────
// ERROR: Error:
// ERROR:   × invalid arguments for attribute `__arm_inout`
// ERROR:    ╭─[tests/fixtures/error/clang/linux/aarch64/sme_keyword_attribute_bad_state.c:2:22]
// ERROR:  1 │
// ERROR:  2 │ void bad_state(void) __arm_inout("zz");
// ERROR:    ·                      ─────────────────
// ERROR:  3 │
// ERROR:    ╰────
// SLATE-FILECHECK-END ERROR
