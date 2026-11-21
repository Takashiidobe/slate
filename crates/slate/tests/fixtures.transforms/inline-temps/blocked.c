int g(int);

int add_call(int *p) {
  int r = (*p += g(1));
  return r;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @add_call(%{{[0-9]+}} p: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_R:[0-9]+]] r: i32 [storage=automatic];
// CHECK-NEXT:     let %[[V0:[0-9]+]]: i32 [synthetic] = call<i32, signature=fn(i32) -> i32>(%[[V1:[0-9]+]], const<i32>(1));
// CHECK-NEXT:     let %[[V2:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[V_P:[0-9]+]]);
// CHECK-NEXT:     let %[[V3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(deref(read<ptr<i32>>(%[[V2]]))), read<i32>(%[[V0]]));
// CHECK-NEXT:     write<i32>(deref(read<ptr<i32>>(%[[V2]])), read<i32>(%[[V3]]));
// CHECK-NEXT:     write<i32>(%[[V_R]], read<i32>(%[[V3]]));
// CHECK-NEXT:     return read<i32>(%[[V_R]]);
// CHECK: {{^}}}
// SLATE-FILECHECK-END
