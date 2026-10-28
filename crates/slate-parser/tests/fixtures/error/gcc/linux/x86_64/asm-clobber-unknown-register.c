void f(void) {
  asm volatile("" ::: "bogus");
}

// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × unknown register name 'bogus' in asm
// PARSE:   ╰─▶ unknown register name 'bogus' in asm
// PARSE:    ╭─[tests/fixtures/error/gcc/linux/x86_64/asm-clobber-unknown-register.c:2:23]
// PARSE:  1 │ void f(void) {
// PARSE:  2 │   asm volatile("" ::: "bogus");
// PARSE:    ·                       ───────
// PARSE:  3 │ }
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
