// SLATE-FILECHECK-ERROR PARSE

void f(void) {
  asm goto("");
}

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × expected `:`
// PARSE: ╰─▶ expected `:`
// PARSE: ╭─[tests/fixtures/error/gcc/linux/x86_64/asm-goto-no-labels.c:3:14]
// PARSE: 2 │ void f(void) {
// PARSE: 3 │   asm goto("");
// PARSE: ·              ─
// PARSE: 4 │ }
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
