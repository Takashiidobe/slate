#ifdef GAMMA
int broken(void) { return undeclared_name; }
#else
int broken(void) { return 0; }
#endif
#ifdef ALPHA
int broken_alpha = 1;
#endif

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu x86_64-pc-windows-msvc
// X86_64-UNKNOWN-LINUX-GNU: {{^}}target linux-x86_64: 1 compiles from 4 preprocessor runs, naive 16
// X86_64-PC-WINDOWS-MSVC: {{^}}target windows-x86_64: 1 compiles from 4 preprocessor runs, naive 16
// CHECK-NEXT:   broken.c: 1 compiles, 2 relevant, 4 elements, 4 runs, 2 failed sema
// CHECK-NEXT:     [alpha]
// CHECK-NEXT:     rejected [alpha,gamma]: lower {{.*}}tests/fixtures.plan/broken.c to slate-parser IR: semantic analysis failed
// CHECK-NEXT:     rejected [gamma]: lower {{.*}}tests/fixtures.plan/broken.c to slate-parser IR: semantic analysis failed
// CHECK-NEXT: total: 1 compiles across 1 targets
// CHECK-NEXT: exit: 0
// CHECK-NEXT: exhaustive: 1 compiles 4 elements 0 missed
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
