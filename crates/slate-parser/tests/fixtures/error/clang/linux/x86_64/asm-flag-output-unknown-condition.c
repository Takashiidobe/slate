// SLATE-FILECHECK-ERROR PARSE

void f(int x) {
  asm("" : "=@ccq"(x));
}

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid output constraint '=@ccq' in asm
// PARSE: ╰─▶ invalid output constraint '=@ccq' in asm
// PARSE: ╭─[tests/fixtures/error/clang/linux/x86_64/asm-flag-output-unknown-condition.c:3:12]
// PARSE: 2 │ void f(int x) {
// PARSE: 3 │   asm("" : "=@ccq"(x));
// PARSE: ·            ───────
// PARSE: 4 │ }
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
