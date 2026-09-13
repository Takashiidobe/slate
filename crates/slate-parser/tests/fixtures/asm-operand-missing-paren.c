void f(int x) {
  asm("" : : "r" x);
}

// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × expected `(` after asm operand
// PARSE: ╰─▶ expected `(` after asm operand
// PARSE: ╭─[tests/fixtures/asm-operand-missing-paren.c:2:18]
// PARSE: 1 │ void f(int x) {
// PARSE: 2 │   asm("" : : "r" x);
// PARSE: ·                  ─
// PARSE: 3 │ }
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
