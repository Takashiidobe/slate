// SLATE-FILECHECK-FLAVOR msvc
// SLATE-FILECHECK-ARGS -target=i686-pc-windows-msvc
// SLATE-FILECHECK-ERROR PARSE

void f(void) {
  __asm 3
}

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × expected instruction mnemonic in `__asm`
// PARSE: ╰─▶ expected instruction mnemonic in `__asm`
// PARSE: ╭─[tests/fixtures/error/ms-asm-missing-mnemonic.c:3:9]
// PARSE: 2 │ void f(void) {
// PARSE: 3 │   __asm 3
// PARSE: ·         ─
// PARSE: 4 │ }
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
