// SLATE-FILECHECK-ERROR PARSE

void f(int x) {
  asm("" : : "@cce"(x));
}

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid input constraint '@cce' in asm
// PARSE: ╰─▶ invalid input constraint '@cce' in asm
// PARSE: ╭─[tests/fixtures/error/clang/linux/x86_64/asm-flag-output-input.c:3:14]
// PARSE: 2 │ void f(int x) {
// PARSE: 3 │   asm("" : : "@cce"(x));
// PARSE: ·              ──────
// PARSE: 4 │ }
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
