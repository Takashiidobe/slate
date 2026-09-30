// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-IR-ERROR DEFAULT

void g(void) {
  __asm {
  target:
    nop
  }
}

int f(void) {
  __asm jmp target
  return 0;
}

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × unresolved label name `target`
// DEFAULT: ╭─[tests/fixtures/msvc/windows/i686/ms_asm_label_in_other_function.c:10:13]
// DEFAULT: 9 │ int f(void) {
// DEFAULT: 10 │   __asm jmp target
// DEFAULT: ·             ──────
// DEFAULT: 11 │   return 0;
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
