// SLATE-FILECHECK-FLAVOR msvc
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-IR-ERROR DEFAULT

struct s { int x; };

int f(void) {
  struct s value;
  __asm mov eax, value.missing
  return 0;
}

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × invalid in this context: no such struct or union member in `__asm`
// DEFAULT: ╭─[tests/fixtures/sema/i686-pc-windows-msvc/ms_asm_unknown_member.c:6:3]
// DEFAULT: 5 │   struct s value;
// DEFAULT: 6 │   __asm mov eax, value.missing
// DEFAULT: ·   ────────────────────────────
// DEFAULT: 7 │   return 0;
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
