// SLATE-FILECHECK-ERROR RESOLVE
// SLATE-FILECHECK-ARGS --dump-ir-names

void spin(void) {
  __builtin_ia32_pause();
}

// SLATE-FILECHECK-BEGIN RESOLVE
// RESOLVE: Error:   × unresolved ordinary name `__builtin_ia32_pause`
// SLATE-FILECHECK-END RESOLVE
