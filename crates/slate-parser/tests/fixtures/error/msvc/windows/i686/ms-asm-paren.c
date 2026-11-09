// SLATE-FILECHECK-ERROR PARSE

void f(void) {
  __asm("nop");
}

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × expected instruction mnemonic in `__asm`
// PARSE:   ╰─▶ expected instruction mnemonic in `__asm`
// PARSE:    ╭─[tests/fixtures/error/msvc/windows/i686/ms-asm-paren.c:3:8]
// PARSE:  2 │ void f(void) {
// PARSE:  3 │   __asm("nop");
// PARSE:    ·        ─
// PARSE:  4 │ }
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
