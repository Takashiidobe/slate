// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ERROR IR
// SLATE-FILECHECK-ARGS --dump-ir

long f(signed char *ap) { return __builtin_va_arg(ap, long); }

// SLATE-FILECHECK-BEGIN IR
// IR: Error:   × semantic analysis failed
// IR: Error:
// IR: × va_arg of non-va_list
// IR: ╭─[tests/fixtures/error/clang/linux/x86_64/va_arg_signed_char_pointer.c:2:34]
// IR: 1 │
// IR: 2 │ long f(signed char *ap) { return __builtin_va_arg(ap, long); }
// IR: ·                                  ──────────────────────────
// IR: ╰────
// SLATE-FILECHECK-END IR
