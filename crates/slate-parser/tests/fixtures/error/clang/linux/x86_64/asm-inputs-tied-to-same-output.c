void f(int x, int y, int z) {
  asm("" : "=r"(x) : "0"(y), "0"(z));
}

// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × more than one input constraint matches the same output '0'
// PARSE: ╰─▶ more than one input constraint matches the same output '0'
// PARSE: ╭─[tests/fixtures/error/clang/linux/x86_64/asm-inputs-tied-to-same-output.c:2:30]
// PARSE: 1 │ void f(int x, int y, int z) {
// PARSE: 2 │   asm("" : "=r"(x) : "0"(y), "0"(z));
// PARSE: ·                              ───
// PARSE: 3 │ }
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
