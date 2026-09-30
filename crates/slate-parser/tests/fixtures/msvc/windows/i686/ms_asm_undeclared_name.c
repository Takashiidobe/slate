// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-IR-ERROR DEFAULT

int f(void) {
  __asm mov eax, missing
  return 0;
}

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × unresolved label name `missing`
// DEFAULT: ╭─[tests/fixtures/msvc/windows/i686/ms_asm_undeclared_name.c:3:18]
// DEFAULT: 2 │ int f(void) {
// DEFAULT: 3 │   __asm mov eax, missing
// DEFAULT: ·                  ───────
// DEFAULT: 4 │   return 0;
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
