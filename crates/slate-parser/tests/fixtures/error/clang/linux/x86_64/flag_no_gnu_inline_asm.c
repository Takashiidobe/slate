// SLATE-FILECHECK-DEFINES ERROR
// SLATE-FILECHECK-ERROR ERROR
// SLATE-FILECHECK-ARGS -fno-gnu-inline-asm

int f(void) {
  __asm__ volatile("nop");
  return 0;
}

// SLATE-FILECHECK-BEGIN ERROR
// ERROR: Error:   × GNU-style inline assembly is disabled
// ERROR:   ╰─▶ GNU-style inline assembly is disabled
// ERROR:    ╭─[tests/fixtures/error/clang/linux/x86_64/flag_no_gnu_inline_asm.c:3:3]
// ERROR:  2 │ int f(void) {
// ERROR:  3 │   __asm__ volatile("nop");
// ERROR:    ·   ───────
// ERROR:  4 │   return 0;
// ERROR:    ╰────
// SLATE-FILECHECK-END ERROR
