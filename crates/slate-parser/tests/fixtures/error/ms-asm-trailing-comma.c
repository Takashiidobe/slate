// SLATE-FILECHECK-FLAVOR msvc
// SLATE-FILECHECK-ARGS -target=i686-pc-windows-msvc
// SLATE-FILECHECK-ERROR PARSE

void f(void) {
  __asm mov eax,
}

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × expected `__asm` operand
// PARSE: ╰─▶ expected `__asm` operand
// PARSE: ╭─[tests/fixtures/error/ms-asm-trailing-comma.c:3:16]
// PARSE: 2 │ void f(void) {
// PARSE: 3 │   __asm mov eax,
// PARSE: ·                ─
// PARSE: 4 │ }
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
