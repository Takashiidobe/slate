void f(void) {
  register int x asm("bogus");
}

// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × unknown register name 'bogus' in asm
// PARSE:   ╰─▶ unknown register name 'bogus' in asm
// PARSE:    ╭─[tests/fixtures/error/clang/linux/x86_64/asm-label-unknown-register.c:2:22]
// PARSE:  1 │ void f(void) {
// PARSE:  2 │   register int x asm("bogus");
// PARSE:    ·                      ───────
// PARSE:  3 │ }
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
