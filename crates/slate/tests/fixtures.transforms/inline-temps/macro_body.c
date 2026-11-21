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
// CHECK-NEXT:                 let %[[V1:[0-9]+]]: i32 [synthetic] = read<i32>(macro_arg(#0, 0, %[[V_X:[0-9]+]]));
// CHECK-NEXT:                 let %[[V2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[V1]]), const<i32>(1));
// CHECK-NEXT:                 write<i32>(macro_arg(#0, 0, %[[V_X]]), read<i32>(%[[V2]]));
// CHECK-NEXT:                 let %[[V3:[0-9]+]]: i32 [synthetic] = read<i32>(macro_arg(#0, 0, %[[V_X]]));
// CHECK-NEXT:                 let %[[V4:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[V3]]), const<i32>(1));
// CHECK-NEXT:                 write<i32>(macro_arg(#0, 0, %[[V_X]]), read<i32>(%[[V4]]));
// CHECK-NEXT:             }
// CHECK-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// CHECK-NEXT:     }
// CHECK-NEXT:     return read<i32>(%[[V_X]]);
// CHECK: {{^}}}
// SLATE-FILECHECK-END
