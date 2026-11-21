int fill(int *p, int n) {
again:
  if (n-- > 0) {
    *p++ = n;
    goto again;
  }
  return *p;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @fill(%{{[0-9]+}} p: ptr<i32>, %{{[0-9]+}} n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     label %[[V_AGAIN:[0-9]+]] again:
// CHECK-NEXT:         let %[[V0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[V_N:[0-9]+]]);
// CHECK-NEXT:         write<i32>(%[[V_N]], sub<i32, overflow=ub>(read<i32>(%[[V0]]), const<i32>(1)));
// CHECK-NEXT:         if gt<i32>(read<i32>(%[[V0]]), const<i32>(0))
// CHECK-NEXT:             {
// CHECK-NEXT:                 let %[[V1:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[V_P:[0-9]+]]);
// CHECK-NEXT:                 write<ptr<i32>>(%[[V_P]], ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[V1]]), const<i32>(1)));
// CHECK-NEXT:                 write<i32>(deref(read<ptr<i32>>(%[[V1]])), read<i32>(%[[V_N]]));
// CHECK-NEXT:                 goto %[[V_AGAIN]];
// CHECK-NEXT:             }
// CHECK-NEXT:     return read<i32>(deref(read<ptr<i32>>(%[[V_P]])));
// CHECK: {{^}}}
// SLATE-FILECHECK-END
