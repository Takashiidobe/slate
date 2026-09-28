// SLATE-FILECHECK-FLAVOR msvc
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-IR-ERROR DEFAULT

int f(void) {
  __asm mov eax, offset 5
  return 0;
}

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × invalid in this context: `OFFSET` needs a C name or label in `__asm`
// DEFAULT: ╭─[tests/fixtures/sema/i686-pc-windows-msvc/ms_asm_offset_of_constant.c:2:1]
// DEFAULT: 1 │
// DEFAULT: 2 │ ╭─▶ int f(void) {
// DEFAULT: 3 │ │     __asm mov eax, offset 5
// DEFAULT: 4 │ │     return 0;
// DEFAULT: 5 │ ╰─▶ }
// DEFAULT: 6 │
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
