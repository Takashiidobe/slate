// SLATE-FILECHECK-ERROR PARSE

void f(int x) {
  asm("" : "+@cceq"(x));
}

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid output constraint '+@cceq' in asm
// PARSE:   ╰─▶ invalid output constraint '+@cceq' in asm
// PARSE:    ╭─[tests/fixtures/error/gcc/linux/armv7-unknown-linux-gnueabihf/asm-flag-output-readwrite.c:3:12]
// PARSE:  2 │ void f(int x) {
// PARSE:  3 │   asm("" : "+@cceq"(x));
// PARSE:    ·            ────────
// PARSE:  4 │ }
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
