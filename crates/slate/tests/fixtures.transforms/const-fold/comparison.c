int folded(void) {
  _Bool flag = 1;
  if (2 < 1)
    return 0;
  return flag;
}

int explicit_kept(void) { return (_Bool)2 == 1; }

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @folded() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_FLAG:[0-9]+]] flag: bool [storage=automatic] = const<bool>(true);
// CHECK-NEXT:     if const<bool>(false) [folded=false]
// CHECK-NEXT:         return const<i32>(0);
// CHECK-NEXT:     return from_bool<i32, reason=return>(read<bool>(%[[V_FLAG]]));
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @explicit_kept() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     return from_bool<i32, reason=return>(eq<i32>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(const<i32>(2), const<i32>(0))), const<i32>(1)));
// CHECK: {{^}}}
// SLATE-FILECHECK-END
