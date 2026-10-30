#ifndef ALPHA
#define QUALIFIER const
#else
#define QUALIFIER
#endif
#ifndef BETA
#define EXTRA + 1
#endif
#ifndef EXTRA
#define EXTRA
#endif
QUALIFIER int implicit = 2 EXTRA;

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu x86_64-pc-windows-msvc
// X86_64-UNKNOWN-LINUX-GNU: {{^}}target linux-x86_64: 4 compiles from 4 preprocessor runs, naive 16
// X86_64-PC-WINDOWS-MSVC: {{^}}target windows-x86_64: 4 compiles from 4 preprocessor runs, naive 16
// CHECK-NEXT:   implicit.c: 4 compiles, 2 relevant, 4 elements, 4 runs
// CHECK-NEXT:     []
// CHECK-NEXT:     [alpha]
// CHECK-NEXT:     [beta]
// CHECK-NEXT:     [alpha,beta]
// CHECK-NEXT: total: 4 compiles across 1 targets
// CHECK-NEXT: exit: 0
// CHECK-NEXT: exhaustive: 4 compiles 4 elements 0 missed
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
