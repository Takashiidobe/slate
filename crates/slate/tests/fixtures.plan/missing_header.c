#include <no_such_header.h>

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu x86_64-pc-windows-msvc
// X86_64-UNKNOWN-LINUX-GNU: {{^}}target linux-x86_64: 0 compiles from 1 preprocessor runs, naive 16, 1 units unplanned
// X86_64-PC-WINDOWS-MSVC: {{^}}target windows-x86_64: 0 compiles from 1 preprocessor runs, naive 16, 1 units unplanned
// CHECK-NEXT:   missing_header.c: 0 compiles, 0 relevant, 0 elements, 1 runs, 1 failed preprocessing
// CHECK-NEXT:     error: parse {{.*}}tests/fixtures.plan/missing_header.c: header not found in search path: <no_such_header.h>
// CHECK-NEXT: total: 0 compiles across 1 targets
// CHECK-NEXT: error: 1 translation units could not be planned
// CHECK-NEXT: exit: 1
// CHECK-NEXT: exhaustive: 0 compiles 0 elements 0 missed
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
