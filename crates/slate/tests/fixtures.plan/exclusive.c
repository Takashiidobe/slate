#ifdef GAMMA
int exclusive_gamma = GAMMA;
#endif
#ifdef DELTA
int exclusive_delta = 1;
#endif

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu x86_64-pc-windows-msvc
// X86_64-UNKNOWN-LINUX-GNU: {{^}}target linux-x86_64: 2 compiles from 3 preprocessor runs, naive 16
// X86_64-PC-WINDOWS-MSVC: {{^}}target windows-x86_64: 2 compiles from 3 preprocessor runs, naive 16
// CHECK-NEXT:   exclusive.c: 2 compiles, 2 relevant, 2 elements, 3 runs
// CHECK-NEXT:     [gamma]
// CHECK-NEXT:     [delta]
// CHECK-NEXT: total: 2 compiles across 1 targets
// CHECK-NEXT: exit: 0
// CHECK-NEXT: exhaustive: 2 compiles 2 elements 0 missed
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
