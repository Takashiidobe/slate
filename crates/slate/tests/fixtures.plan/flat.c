int flags(void) {
    int value = 0;
#ifdef ALPHA
    value |= 1;
#endif
#ifdef BETA
    value |= 2;
#endif
#ifndef GAMMA
    value |= 4;
#endif
    return value;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu x86_64-pc-windows-msvc
// X86_64-UNKNOWN-LINUX-GNU: {{^}}target linux-x86_64: 1 compiles from 8 preprocessor runs, naive 16
// X86_64-PC-WINDOWS-MSVC: {{^}}target windows-x86_64: 1 compiles from 8 preprocessor runs, naive 16
// CHECK-NEXT:   flat.c: 1 compiles, 3 relevant, 7 elements, 8 runs
// CHECK-NEXT:     [alpha,beta]
// CHECK-NEXT: total: 1 compiles across 1 targets
// CHECK-NEXT: exit: 0
// CHECK-NEXT: exhaustive: 1 compiles 7 elements 0 missed
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
