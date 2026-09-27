void f(int x) {
  asm("" : "r"(x));
}

// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid output constraint 'r' in asm
// PARSE: ╰─▶ invalid output constraint 'r' in asm
// PARSE: ╭─[tests/fixtures/error/asm-output-constraint-without-write.c:2:12]
// PARSE: 1 │ void f(int x) {
// PARSE: 2 │   asm("" : "r"(x));
// PARSE: ·            ───
// PARSE: 3 │ }
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
