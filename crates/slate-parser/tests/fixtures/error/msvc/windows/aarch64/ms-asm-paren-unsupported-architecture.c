// SLATE-FILECHECK-ERROR PARSE

void f(void) {
  __asm volatile("nop");
}

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × `__asm` is not supported on this architecture
// PARSE:   ╰─▶ `__asm` is not supported on this architecture
// PARSE:    ╭─[tests/fixtures/error/msvc/windows/aarch64/ms-asm-paren-unsupported-architecture.c:3:3]
// PARSE:  2 │ void f(void) {
// PARSE:  3 │   __asm volatile("nop");
// PARSE:    ·   ─────
// PARSE:  4 │ }
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
