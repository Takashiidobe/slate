// plan-args: --plan --matrix {matrix} crate-dir
int unused;

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu x86_64-pc-windows-msvc
// CHECK: {{^}}error: --plan requires only a project directory
// CHECK-NEXT: exit: 1
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
