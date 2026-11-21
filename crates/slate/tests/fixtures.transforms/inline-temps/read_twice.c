int advance(int *p) {
  int v = *p++;
  return v + *p;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @advance(%{{[0-9]+}} p: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_V:[0-9]+]] v: i32 [storage=automatic];
// CHECK-NEXT:     let %[[V0:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[V_P:[0-9]+]]);
// CHECK-NEXT:     write<ptr<i32>>(%[[V_P]], ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[V0]]), const<i32>(1)));
// CHECK-NEXT:     write<i32>(%[[V_V]], read<i32>(deref(read<ptr<i32>>(%[[V0]]))));
// CHECK-NEXT:     return add<i32, overflow=ub>(read<i32>(%[[V_V]]), read<i32>(deref(read<ptr<i32>>(%[[V_P]]))));
// CHECK: {{^}}}
// SLATE-FILECHECK-END
