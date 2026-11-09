// SLATE-FILECHECK-ERROR RESOLVE
// SLATE-FILECHECK-ARGS --dump-ir

void spin(void) {
  __builtin_ia32_pause();
}

// SLATE-FILECHECK-BEGIN RESOLVE
// RESOLVE: Error:   × semantic analysis failed
// RESOLVE: Error: -Wimplicit-function-declaration
// RESOLVE:   × use of unknown builtin '__builtin_ia32_pause'
// RESOLVE:    ╭─[tests/fixtures/error/clang/linux/aarch64/x86_target_builtin.c:3:3]
// RESOLVE:  2 │ void spin(void) {
// RESOLVE:  3 │   __builtin_ia32_pause();
// RESOLVE:    ·   ────────────────────
// RESOLVE:  4 │ }
// RESOLVE:    ╰────
// SLATE-FILECHECK-END RESOLVE
