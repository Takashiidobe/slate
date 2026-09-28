int        a = 0, c = 0;
static int d[][8] = {};

int main() {
  int e;
  for (int b = 0; b < 4; b++) {
    __builtin_printf("%d\n", b, e);
    while (a && c++)
      e = d[300000000000000000][0];
  }

  return 0;
}


// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × empty initializer for array of unknown length
// DEFAULT: ╭─[tests/fixtures/suites/gcc-torture/gcc/linux/x86_64/pr79286.c:2:1]
// DEFAULT: 1 │ int        a = 0, c = 0;
// DEFAULT: 2 │ static int d[][8] = {};
// DEFAULT: · ───────────────────────
// DEFAULT: 3 │
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
