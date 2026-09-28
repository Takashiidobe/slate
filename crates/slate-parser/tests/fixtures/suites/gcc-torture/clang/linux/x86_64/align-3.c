/* { dg-skip-if "small alignment" { pdp11-*-* } } */

void abort(void);

void func(void) __attribute__((aligned(256)));

void func(void) {}

int main() {
  if (__alignof__(func) != 256)
    abort();
  return 0;
}


// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × incomplete field type
// DEFAULT: ╭─[tests/fixtures/suites/gcc-torture/clang/linux/x86_64/align-3.c:10:7]
// DEFAULT: 9 │ int main() {
// DEFAULT: 10 │   if (__alignof__(func) != 256)
// DEFAULT: ·       ─────────────────
// DEFAULT: 11 │     abort();
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
