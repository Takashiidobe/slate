#ifdef ALPHA
#define LEFT 1
#else
#define LEFT 0
#endif
#ifdef BETA
#define RIGHT 1
#else
#define RIGHT 0
#endif
#ifdef DELTA
#define OTHER 1
#else
#define OTHER 0
#endif
int combined = LEFT * 100 + RIGHT * 10 + OTHER;

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu x86_64-pc-windows-msvc
// X86_64-UNKNOWN-LINUX-GNU: {{^}}target linux-x86_64: 8 compiles from 8 preprocessor runs, naive 16
// X86_64-PC-WINDOWS-MSVC: {{^}}target windows-x86_64: 8 compiles from 8 preprocessor runs, naive 16
// CHECK-NEXT:   combined.c: 8 compiles, 3 relevant, 8 elements, 8 runs
// CHECK-NEXT:     []
// CHECK-NEXT:     [alpha]
// CHECK-NEXT:     [beta]
// CHECK-NEXT:     [delta]
// CHECK-NEXT:     [alpha,beta]
// CHECK-NEXT:     [alpha,delta]
// CHECK-NEXT:     [beta,delta]
// CHECK-NEXT:     [alpha,beta,delta]
// CHECK-NEXT: total: 8 compiles across 1 targets
// CHECK-NEXT: exit: 0
// CHECK-NEXT: exhaustive: 8 compiles 8 elements 0 missed
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
