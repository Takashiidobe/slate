#ifndef ALPHA
#define VIA_ALPHA
#endif
#ifdef VIA_ALPHA
#define STATE_SIZE 8
#else
#define STATE_SIZE 4
#endif
int state[STATE_SIZE];

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu x86_64-pc-windows-msvc
// X86_64-UNKNOWN-LINUX-GNU: {{^}}target linux-x86_64: 2 compiles from 2 preprocessor runs, naive 16
// X86_64-PC-WINDOWS-MSVC: {{^}}target windows-x86_64: 2 compiles from 2 preprocessor runs, naive 16
// CHECK-NEXT:   chain.c: 2 compiles, 1 relevant, 2 elements, 2 runs
// CHECK-NEXT:     []
// CHECK-NEXT:     [alpha]
// CHECK-NEXT: total: 2 compiles across 1 targets
// CHECK-NEXT: exit: 0
// CHECK-NEXT: exhaustive: 2 compiles 2 elements 0 missed
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
