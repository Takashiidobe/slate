// SLATE-FILECHECK-ERROR PARSE

void f(void) {
  __asm mov eax, 1 1
}

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × unexpected token in `__asm` operand
// PARSE: ╰─▶ unexpected token in `__asm` operand
// PARSE: ╭─[tests/fixtures/error/msvc/windows/i686/ms-asm-unexpected-operand-token.c:3:20]
// PARSE: 2 │ void f(void) {
// PARSE: 3 │   __asm mov eax, 1 1
// PARSE: ·                    ─
// PARSE: 4 │ }
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
