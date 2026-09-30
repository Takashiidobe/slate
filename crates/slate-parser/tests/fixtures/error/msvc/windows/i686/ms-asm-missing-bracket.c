// SLATE-FILECHECK-ERROR PARSE

void f(void) {
  __asm mov eax, [ecx
}

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × expected `]` in `__asm` operand
// PARSE: ╰─▶ expected `]` in `__asm` operand
// PARSE: ╭─[tests/fixtures/error/msvc/windows/i686/ms-asm-missing-bracket.c:3:19]
// PARSE: 2 │ void f(void) {
// PARSE: 3 │   __asm mov eax, [ecx
// PARSE: ·                   ───
// PARSE: 4 │ }
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
