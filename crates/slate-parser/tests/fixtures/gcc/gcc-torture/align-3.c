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
// DEFAULT: Error:   × unsupported in numeric IR lowering: incomplete field type
// SLATE-FILECHECK-END DEFAULT
