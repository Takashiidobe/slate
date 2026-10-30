int nested(void) {
#ifdef ALPHA
#ifdef BETA
    return 3;
#else
    return 1;
#endif
#else
    return 0;
#endif
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu x86_64-pc-windows-msvc
// X86_64-UNKNOWN-LINUX-GNU: {{^}}target linux-x86_64: 3 compiles from 4 preprocessor runs, naive 16
// X86_64-PC-WINDOWS-MSVC: {{^}}target windows-x86_64: 3 compiles from 4 preprocessor runs, naive 16
// CHECK-NEXT:   nested.c: 3 compiles, 2 relevant, 5 elements, 4 runs
// CHECK-NEXT:     []
// CHECK-NEXT:     [alpha]
// CHECK-NEXT:     [alpha,beta]
// CHECK-NEXT: total: 3 compiles across 1 targets
// CHECK-NEXT: exit: 0
// CHECK-NEXT: exhaustive: 3 compiles 5 elements 0 missed
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
