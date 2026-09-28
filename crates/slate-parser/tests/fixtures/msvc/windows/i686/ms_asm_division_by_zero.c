// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-IR-ERROR DEFAULT

int f(void) {
  __asm mov eax, 5 / 0
  return 0;
}

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × `__asm` division by zero
// DEFAULT: ╭─[tests/fixtures/msvc/windows/i686/ms_asm_division_by_zero.c:3:3]
// DEFAULT: 2 │ int f(void) {
// DEFAULT: 3 │   __asm mov eax, 5 / 0
// DEFAULT: ·   ────────────────────
// DEFAULT: 4 │   return 0;
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
