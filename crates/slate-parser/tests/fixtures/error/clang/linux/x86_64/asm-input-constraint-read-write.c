void f(int x, int y) {
  asm("" : "=r"(x) : "+r"(y));
}

// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid input constraint '+r' in asm
// PARSE: ╰─▶ invalid input constraint '+r' in asm
// PARSE: ╭─[tests/fixtures/error/clang/linux/x86_64/asm-input-constraint-read-write.c:2:22]
// PARSE: 1 │ void f(int x, int y) {
// PARSE: 2 │   asm("" : "=r"(x) : "+r"(y));
// PARSE: ·                      ────
// PARSE: 3 │ }
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
