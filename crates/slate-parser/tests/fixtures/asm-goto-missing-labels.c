void f(int x) {
  asm goto("" : : "r"(x));
}

// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × expected `:`
// PARSE: ╰─▶ expected `:`
// PARSE: ╭─[tests/fixtures/asm-goto-missing-labels.c:2:25]
// PARSE: 1 │ void f(int x) {
// PARSE: 2 │   asm goto("" : : "r"(x));
// PARSE: ·                         ─
// PARSE: 3 │ }
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
