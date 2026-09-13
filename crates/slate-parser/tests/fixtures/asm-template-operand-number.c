void f(int x) {
  asm("%3" : : "r"(x));
}

// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid operand number in inline asm string
// PARSE: ╰─▶ invalid operand number in inline asm string
// PARSE: ╭─[tests/fixtures/asm-template-operand-number.c:2:7]
// PARSE: 1 │ void f(int x) {
// PARSE: 2 │   asm("%3" : : "r"(x));
// PARSE: ·       ────
// PARSE: 3 │ }
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
