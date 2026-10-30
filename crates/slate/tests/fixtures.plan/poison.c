#if defined(ALPHA) && defined(BETA)
#error alpha and beta are incompatible here
#endif
#ifdef ALPHA
int poison = 1;
#elif defined(BETA)
int poison = 2;
#else
int poison = 0;
#endif

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu x86_64-pc-windows-msvc
// X86_64-UNKNOWN-LINUX-GNU: {{^}}target linux-x86_64: 3 compiles from 4 preprocessor runs, naive 16
// X86_64-PC-WINDOWS-MSVC: {{^}}target windows-x86_64: 3 compiles from 4 preprocessor runs, naive 16
// CHECK-NEXT:   poison.c: 3 compiles, 2 relevant, 3 elements, 4 runs, 1 hit #error
// CHECK-NEXT:     []
// CHECK-NEXT:     [alpha]
// CHECK-NEXT:     [beta]
// CHECK-NEXT: total: 3 compiles across 1 targets
// CHECK-NEXT: exit: 0
// CHECK-NEXT: exhaustive: 3 compiles 3 elements 0 missed
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
