// SLATE-FILECHECK-FLAVOR msvc
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
// DEFAULT: Error:   × unresolved label name `target`
// SLATE-FILECHECK-END DEFAULT
