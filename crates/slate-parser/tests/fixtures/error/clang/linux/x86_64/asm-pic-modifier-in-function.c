void f(int x) {
  asm("" : : "-r"(x));
}

// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid input constraint '-r' in asm
// PARSE: ╰─▶ invalid input constraint '-r' in asm
// PARSE: ╭─[tests/fixtures/error/clang/linux/x86_64/asm-pic-modifier-in-function.c:2:14]
// PARSE: 1 │ void f(int x) {
// PARSE: 2 │   asm("" : : "-r"(x));
// PARSE: ·              ────
// PARSE: 3 │ }
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
