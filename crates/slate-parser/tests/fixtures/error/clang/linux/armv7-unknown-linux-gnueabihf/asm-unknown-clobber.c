// SLATE-FILECHECK-ERROR PARSE

void f(void) {
  asm volatile("" ::: "d32");
}

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × unknown register name 'd32' in asm
// PARSE:   ╰─▶ unknown register name 'd32' in asm
// PARSE:    ╭─[tests/fixtures/error/clang/linux/armv7-unknown-linux-gnueabihf/asm-unknown-clobber.c:3:23]
// PARSE:  2 │ void f(void) {
// PARSE:  3 │   asm volatile("" ::: "d32");
// PARSE:    ·                       ─────
// PARSE:  4 │ }
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
