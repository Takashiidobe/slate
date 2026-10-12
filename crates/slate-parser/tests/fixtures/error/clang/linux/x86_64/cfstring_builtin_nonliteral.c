// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ERROR IR
// SLATE-FILECHECK-ARGS --dump-ir

const void *dynamic(const char *text) { return __builtin___CFStringMakeConstantString(text); }

// SLATE-FILECHECK-BEGIN IR
// IR: Error:   × semantic analysis failed
// IR: Error:
// IR: × CFString literal is not a string constant
// IR: ╭─[tests/fixtures/error/clang/linux/x86_64/cfstring_builtin_nonliteral.c:2:48]
// IR: 1 │
// IR: 2 │ const void *dynamic(const char *text) { return __builtin___CFStringMakeConstantString(text); }
// IR: ·                                                ────────────────────────────────────────────
// IR: ╰────
// SLATE-FILECHECK-END IR
