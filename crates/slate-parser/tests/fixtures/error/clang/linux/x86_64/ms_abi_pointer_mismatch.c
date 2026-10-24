// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ERROR IR
// SLATE-FILECHECK-ARGS --dump-ir

int __attribute__((ms_abi)) windows(int a);
int (*native)(int) = windows;

// SLATE-FILECHECK-BEGIN IR
// IR: Error:   × semantic analysis failed
// IR: Error: -Wincompatible-pointer-types
// IR:   × incompatible pointer types
// IR:    ╭─[tests/fixtures/error/clang/linux/x86_64/ms_abi_pointer_mismatch.c:3:22]
// IR:  2 │ int __attribute__((ms_abi)) windows(int a);
// IR:  3 │ int (*native)(int) = windows;
// IR:    ·                      ───────
// IR:  4 │
// IR:    ╰────
// SLATE-FILECHECK-END IR
