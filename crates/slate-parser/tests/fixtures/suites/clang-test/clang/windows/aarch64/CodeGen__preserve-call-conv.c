

// Check that the preserve_most calling convention attribute at the source level
// is lowered to the corresponding calling convention attrribute at the LLVM IR
// level.
void foo(void) __attribute__((preserve_most)) {
}

// Check that the preserve_most calling convention attribute at the source level
// is lowered to the corresponding calling convention attrribute at the LLVM IR
// level.
void boo(void) __attribute__((preserve_all)) {
}

// Check that the preserve_none calling convention attribute at the source level
// is lowered to the corresponding calling convention attrribute at the LLVM IR
// level.
void bar(void) __attribute__((preserve_none)) {
}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT gnu17
// SLATE-FILECHECK-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × unsupported calling convention `preserve_most`
// DEFAULT:   ╰─▶ unsupported calling convention `preserve_most`
// DEFAULT:    ╭─[tests/fixtures/suites/clang-test/clang/windows/aarch64/CodeGen__preserve-call-conv.c:6:16]
// DEFAULT:  5 │ // level.
// DEFAULT:  6 │ void foo(void) __attribute__((preserve_most)) {
// DEFAULT:    ·                ─────────────
// DEFAULT:  7 │ }
// DEFAULT:    ╰────
// SLATE-FILECHECK-END DEFAULT
