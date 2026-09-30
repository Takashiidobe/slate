int *p;
float *f;

int assign(void) {
  int i = 0;
  i = p;
  i += p;
  f = p;
  return i;
}

int x = "abc";

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error: -Wint-conversion
// DEFAULT: × incompatible pointer to integer conversion
// DEFAULT: ╭─[tests/fixtures/clang/linux/x86_64/default_error_conversions_rejected_by_parse.c:6:7]
// DEFAULT: 5 │   int i = 0;
// DEFAULT: 6 │   i = p;
// DEFAULT: ·       ─
// DEFAULT: 7 │   i += p;
// DEFAULT: ╰────
// DEFAULT: Error: -Wint-conversion
// DEFAULT: × incompatible pointer to integer conversion
// DEFAULT: ╭─[tests/fixtures/clang/linux/x86_64/default_error_conversions_rejected_by_parse.c:7:3]
// DEFAULT: 6 │   i = p;
// DEFAULT: 7 │   i += p;
// DEFAULT: ·   ──────
// DEFAULT: 8 │   f = p;
// DEFAULT: ╰────
// DEFAULT: Error: -Wincompatible-pointer-types
// DEFAULT: × incompatible pointer types
// DEFAULT: ╭─[tests/fixtures/clang/linux/x86_64/default_error_conversions_rejected_by_parse.c:8:7]
// DEFAULT: 7 │   i += p;
// DEFAULT: 8 │   f = p;
// DEFAULT: ·       ─
// DEFAULT: 9 │   return i;
// DEFAULT: ╰────
// DEFAULT: Error: -Wint-conversion
// DEFAULT: × incompatible pointer to integer conversion
// DEFAULT: ╭─[tests/fixtures/clang/linux/x86_64/default_error_conversions_rejected_by_parse.c:12:9]
// DEFAULT: 11 │
// DEFAULT: 12 │ int x = "abc";
// DEFAULT: ·         ─────
// DEFAULT: 13 │
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
