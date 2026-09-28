// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-IR-ERROR DEFAULT

int f(void) {
  __asm mov eax, TYPE 5
  return 0;
}

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × invalid in this context: `TYPE`, `LENGTH` and `SIZE` need a C object or
// DEFAULT: ╭─[tests/fixtures/msvc/windows/i686/ms_asm_type_of_constant.c:3:3]
// DEFAULT: 2 │ int f(void) {
// DEFAULT: 3 │   __asm mov eax, TYPE 5
// DEFAULT: ·   ─────────────────────
// DEFAULT: 4 │   return 0;
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
