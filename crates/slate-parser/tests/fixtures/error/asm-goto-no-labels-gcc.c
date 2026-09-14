// SLATE-FILECHECK-FLAVOR gcc
// SLATE-FILECHECK-ERROR PARSE

void f(void) {
  asm goto("");
}

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × expected `:`
// PARSE: ╰─▶ expected `:`
// PARSE: ╭─[tests/fixtures/error/asm-goto-no-labels-gcc.c:3:14]
// PARSE: 2 │ void f(void) {
// PARSE: 3 │   asm goto("");
// PARSE: ·              ─
// PARSE: 4 │ }
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
