int decrement(int n) {
  n--;
  return n;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @decrement(%{{[0-9]+}} n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     write<i32>(%[[V_N:[0-9]+]], sub<i32, overflow=ub>(read<i32>(%[[V_N]]), const<i32>(1)));
// CHECK-NEXT:     return read<i32>(%[[V_N]]);
// CHECK: {{^}}}
// SLATE-FILECHECK-END
