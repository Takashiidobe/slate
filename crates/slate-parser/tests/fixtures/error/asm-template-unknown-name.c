void f(int x) {
  asm("%[missing]" : : [x] "r"(x));
}

// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × unknown symbolic operand name in inline asm string: `missing`
// PARSE: ╰─▶ unknown symbolic operand name in inline asm string: `missing`
// PARSE: ╭─[tests/fixtures/error/asm-template-unknown-name.c:2:7]
// PARSE: 1 │ void f(int x) {
// PARSE: 2 │   asm("%[missing]" : : [x] "r"(x));
// PARSE: ·       ────────────
// PARSE: 3 │ }
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
