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
// DEFAULT: Error:   × invalid in this context: no such struct or union member in `__asm`
// SLATE-FILECHECK-END DEFAULT
