// SLATE-FILECHECK-FLAVOR msvc
// SLATE-FILECHECK-ARGS -target=i686-pc-windows-msvc
// SLATE-FILECHECK-ERROR PARSE

void f(void) {
  __asm fld st(8)
}

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × expected x87 stack register index 0-7
// PARSE: ╰─▶ expected x87 stack register index 0-7
// PARSE: ╭─[tests/fixtures/error/ms-asm-st-index.c:3:16]
// PARSE: 2 │ void f(void) {
// PARSE: 3 │   __asm fld st(8)
// PARSE: ·                ─
// PARSE: 4 │ }
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
