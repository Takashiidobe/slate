// SLATE-FILECHECK-ERROR PARSE

void f(void) {
  __asm {
    nop
  }
}

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × `__asm` is not supported on this architecture
// PARSE: ╰─▶ `__asm` is not supported on this architecture
// PARSE: ╭─[tests/fixtures/error/msvc/windows/x86_64/ms-asm-unsupported-architecture.c:3:3]
// PARSE: 2 │ void f(void) {
// PARSE: 3 │   __asm {
// PARSE: ·   ─────
// PARSE: 4 │     nop
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
