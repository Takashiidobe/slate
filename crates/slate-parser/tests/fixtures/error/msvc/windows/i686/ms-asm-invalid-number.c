// SLATE-FILECHECK-ERROR PARSE

void f(void) {
  __asm mov eax, 12z
}

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid number in `__asm` operand
// PARSE: ╰─▶ invalid number in `__asm` operand
// PARSE: ╭─[tests/fixtures/error/msvc/windows/i686/ms-asm-invalid-number.c:3:18]
// PARSE: 2 │ void f(void) {
// PARSE: 3 │   __asm mov eax, 12z
// PARSE: ·                  ───
// PARSE: 4 │ }
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
