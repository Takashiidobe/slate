// SLATE-FILECHECK-DEFINES ERROR
// SLATE-FILECHECK-ERROR ERROR
// SLATE-FILECHECK-ARGS --dump-ir

void invalid(int i) {
  __asm__("" : "=r"(__imag__ i));
}

// SLATE-FILECHECK-BEGIN ERROR
// ERROR: Error:   × semantic analysis failed
// ERROR: Error:
// ERROR:   × lvalue required in 'asm' statement
// ERROR:    ╭─[tests/fixtures/error/gcc/linux/x86_64/asm_imag_scalar_output.c:3:3]
// ERROR:  2 │ void invalid(int i) {
// ERROR:  3 │   __asm__("" : "=r"(__imag__ i));
// ERROR:    ·   ───────────────────────────────
// ERROR:  4 │ }
// ERROR:    ╰────
// SLATE-FILECHECK-END ERROR
