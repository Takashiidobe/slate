// RISC-V does not support swiftcall

#if __has_extension(swiftcc)
// expected-no-diagnostics
#else
// expected-warning@+2 {{'__swiftcall__' calling convention is not supported for this target}}
#endif
void __attribute__((__swiftcall__)) f(void) {}

#if __has_extension(swiftasynccc)
// expected-no-diagnostics
#else
// expected-warning@+2 {{'__swiftasynccall__' calling convention is not supported for this target}}
#endif
void __attribute__((__swiftasynccall__)) g(void) {}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ERROR DEFAULT
// SLATE-FILECHECK-STD DEFAULT gnu17

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × unsupported calling convention `swiftcall`
// DEFAULT:   ╰─▶ unsupported calling convention `swiftcall`
// DEFAULT:    ╭─[tests/fixtures/suites/clang-test/clang/windows/x86_64/Sema__swift-call-conv.c:8:6]
// DEFAULT:  7 │ #endif
// DEFAULT:  8 │ void __attribute__((__swiftcall__)) f(void) {}
// DEFAULT:    ·      ─────────────
// DEFAULT:  9 │
// DEFAULT:    ╰────
// SLATE-FILECHECK-END DEFAULT
