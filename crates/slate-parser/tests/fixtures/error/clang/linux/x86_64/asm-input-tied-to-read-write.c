void f(int x, int y) {
  asm("" : "+r"(x) : "0"(y));
}

// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid input constraint '0' in asm
// PARSE: ╰─▶ invalid input constraint '0' in asm
// PARSE: ╭─[tests/fixtures/error/clang/linux/x86_64/asm-input-tied-to-read-write.c:2:22]
// PARSE: 1 │ void f(int x, int y) {
// PARSE: 2 │   asm("" : "+r"(x) : "0"(y));
// PARSE: ·                      ───
// PARSE: 3 │ }
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
