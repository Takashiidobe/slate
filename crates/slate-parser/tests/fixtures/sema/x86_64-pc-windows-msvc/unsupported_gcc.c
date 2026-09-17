// SLATE-FILECHECK-DEFINES CHECK
// SLATE-FILECHECK-ERROR CHECK
// SLATE-FILECHECK-ARGS --flavor=gcc

int value;

// SLATE-FILECHECK-BEGIN CHECK
// CHECK: Error:   × no predefines for Gcc on x86_64-pc-windows-msvc
// CHECK: ╰─▶ no predefines for Gcc on x86_64-pc-windows-msvc
// SLATE-FILECHECK-END CHECK
