// SLATE-FILECHECK-ERROR PARSE

void f(int x) {
  asm("" : "=@cce,r"(x));
}

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid output constraint '=@cce,r' in asm
// PARSE: ╰─▶ invalid output constraint '=@cce,r' in asm
// PARSE: ╭─[tests/fixtures/error/clang/linux/x86_64/asm-flag-output-alternatives.c:3:12]
// PARSE: 2 │ void f(int x) {
// PARSE: 3 │   asm("" : "=@cce,r"(x));
// PARSE: ·            ─────────
// PARSE: 4 │ }
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
