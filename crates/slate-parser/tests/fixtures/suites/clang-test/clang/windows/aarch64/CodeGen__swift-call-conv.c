

void __attribute__((__swiftcall__)) f(void) {}

void __attribute__((__swiftasynccall__)) f_async(void) {}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ERROR DEFAULT
// SLATE-FILECHECK-STD DEFAULT gnu17

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × unsupported calling convention `swiftcall`
// DEFAULT:   ╰─▶ unsupported calling convention `swiftcall`
// DEFAULT:    ╭─[tests/fixtures/suites/clang-test/clang/windows/aarch64/CodeGen__swift-call-conv.c:3:6]
// DEFAULT:  2 │
// DEFAULT:  3 │ void __attribute__((__swiftcall__)) f(void) {}
// DEFAULT:    ·      ─────────────
// DEFAULT:  4 │
// DEFAULT:    ╰────
// SLATE-FILECHECK-END DEFAULT
