#define BUMP(x) \
  do {             \
    (x)++;         \
    (x)++;         \
  } while (0)

int bump_twice(int x) {
  BUMP(x);
  return x;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @bump_twice(%{{[0-9]+}} x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     macro_body #0 {
// CHECK-NEXT:         do %[[V0:[0-9]+]]
// CHECK-NEXT:             {
// CHECK-NEXT:                 write<i32>(%[[V_X:[0-9]+]], add<i32, overflow=ub>(read<i32>(%[[V_X]]), const<i32>(1)));
// CHECK-NEXT:                 write<i32>(%[[V_X]], add<i32, overflow=ub>(read<i32>(%[[V_X]]), const<i32>(1)));
// CHECK-NEXT:             }
// CHECK-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// CHECK-NEXT:     }
// CHECK-NEXT:     return read<i32>(%[[V_X]]);
// CHECK: {{^}}}
// SLATE-FILECHECK-END
