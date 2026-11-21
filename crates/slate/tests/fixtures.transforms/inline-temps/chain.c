int sum(int *p, int n) {
  int s = 0;
  while (n-- > 0)
    s += *p++;
  return s;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @sum(%{{[0-9]+}} p: ptr<i32>, %{{[0-9]+}} n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_S:[0-9]+]] s: i32 [storage=automatic] = const<i32>(0);
// CHECK-NEXT:     while %[[V0:[0-9]+]] {
// CHECK-NEXT:         let %[[V1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[V_N:[0-9]+]]);
// CHECK-NEXT:         write<i32>(%[[V_N]], sub<i32, overflow=ub>(read<i32>(%[[V1]]), const<i32>(1)));
// CHECK-NEXT:         yield gt<i32>(read<i32>(%[[V1]]), const<i32>(0));
// CHECK-NEXT:     }
// CHECK-NEXT:         let %[[V2:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[V_P:[0-9]+]]);
// CHECK-NEXT:         write<ptr<i32>>(%[[V_P]], ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[V2]]), const<i32>(1)));
// CHECK-NEXT:         write<i32>(%[[V_S]], add<i32, overflow=ub>(read<i32>(%[[V_S]]), read<i32>(deref(read<ptr<i32>>(%[[V2]])))));
// CHECK-NEXT:     return read<i32>(%[[V_S]]);
// CHECK: {{^}}}
// SLATE-FILECHECK-END
