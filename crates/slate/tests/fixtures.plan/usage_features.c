// plan-args: --plan --matrix {matrix} --features alpha
int unused;

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu x86_64-pc-windows-msvc
// CHECK: {{^}}error: --plan chooses features itself; remove --features
// CHECK-NEXT: exit: 1
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
