int none(void) { return 1; }

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu x86_64-pc-windows-msvc
// X86_64-UNKNOWN-LINUX-GNU: {{^}}target linux-x86_64: 1 compiles from 1 preprocessor runs, naive 16
// X86_64-PC-WINDOWS-MSVC: {{^}}target windows-x86_64: 1 compiles from 1 preprocessor runs, naive 16
// CHECK-NEXT:   none.c: 1 compiles, 0 relevant, 3 elements, 1 runs
// CHECK-NEXT:     []
// CHECK-NEXT: total: 1 compiles across 1 targets
// CHECK-NEXT: exit: 0
// CHECK-NEXT: exhaustive: 1 compiles 3 elements 0 missed
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
