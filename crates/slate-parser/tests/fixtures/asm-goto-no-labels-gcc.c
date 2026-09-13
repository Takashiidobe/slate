void f(void) {
  asm goto("");
}

// SLATE-FILECHECK-FLAVOR gcc
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × expected `:`
// PARSE: ╰─▶ expected `:`
// PARSE: ╭─[tests/fixtures/asm-goto-no-labels-gcc.c:2:14]
// PARSE: 1 │ void f(void) {
// PARSE: 2 │   asm goto("");
// PARSE: ·              ─
// PARSE: 3 │ }
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
