// SLATE-FILECHECK-ERROR PARSE

int f(int a) {
  __asm__("addl $1, %0" : "+r"(a));
  return a;
}

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × expected `)`, found `:`
// PARSE:   ╰─▶ expected `)`, found `:`
// PARSE:    ╭─[tests/fixtures/error/msvc/windows/x86_64/gnu-asm-operands.c:3:3]
// PARSE:  2 │ int f(int a) {
// PARSE:  3 │   __asm__("addl $1, %0" : "+r"(a));
// PARSE:    ·   ───────
// PARSE:  4 │   return a;
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
