long long seconds(void) {
  long long s = 60 * 60;
  return s;
}

unsigned mask(void) { return 1u << 31; }

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @seconds() -> i64 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_S:[0-9]+]] s: i64 [storage=automatic] = widen<i64, reason=assign>(mul<i32, overflow=ub>(const<i32>(60), const<i32>(60)));
// CHECK-NEXT:     return read<i64>(%[[V_S]]);
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @mask() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     return shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(1), const<i32>(31));
// CHECK: {{^}}}
// SLATE-FILECHECK-END
