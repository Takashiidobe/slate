void f(int x, int y, int z) {
  asm("" : "=r,r"(x), "=r,r"(y) : "0,1"(z));
}

// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid input constraint '0,1' in asm
// PARSE: ╰─▶ invalid input constraint '0,1' in asm
// PARSE: ╭─[tests/fixtures/error/asm-input-matches-two-outputs.c:2:35]
// PARSE: 1 │ void f(int x, int y, int z) {
// PARSE: 2 │   asm("" : "=r,r"(x), "=r,r"(y) : "0,1"(z));
// PARSE: ·                                   ─────
// PARSE: 3 │ }
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
