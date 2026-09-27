// SLATE-FILECHECK-FLAVOR msvc
// SLATE-FILECHECK-ARGS -target=i686-pc-windows-msvc
// SLATE-FILECHECK-ERROR PARSE

void f(void) {
  __asm mov eax, 1 1
}

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × unexpected token in `__asm` operand
// PARSE: ╰─▶ unexpected token in `__asm` operand
// PARSE: ╭─[tests/fixtures/error/ms-asm-unexpected-operand-token.c:3:20]
// PARSE: 2 │ void f(void) {
// PARSE: 3 │   __asm mov eax, 1 1
// PARSE: ·                    ─
// PARSE: 4 │ }
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
