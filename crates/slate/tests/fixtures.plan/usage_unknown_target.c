// plan-args: --plan --matrix {matrix} --matrix-target nope
int unused;

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu x86_64-pc-windows-msvc
// CHECK: {{^}}error: unknown matrix target nope
// CHECK-NEXT: exit: 1
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
