#define TRUE 1
#define FALSE 0
#define TRUTH(x) ((x) != 0)

int explicit_kept(int a, int b) { return (int)(a < b) != 0; }

int object_macro_arms_folded(int a) { return a ? TRUE : FALSE; }

int macro_test_kept(int a, int b) { return TRUTH(a < b); }

int user_variable_kept(int a) {
  _Bool flag;
  if (a)
    flag = 1;
  else
    flag = a > 3;
  return flag;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @explicit_kept(%{{[0-9]+}} a: i32, %{{[0-9]+}} b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     return from_bool<i32, reason=return>(ne<i32>(from_bool<i32, reason=explicit>(lt<i32>(read<i32>(%[[V_A:[0-9]+]]), read<i32>(%[[V_B:[0-9]+]]))), const<i32>(0)));
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @object_macro_arms_folded(%{{[0-9]+}} a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     return from_bool<i32, reason=assign>(ne<i32>(read<i32>(%[[V_A:[0-9]+]]), const<i32>(0)));
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @macro_test_kept(%{{[0-9]+}} a: i32, %{{[0-9]+}} b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     return from_bool<i32, reason=return>(ne<i32>(from_bool<i32, reason=promotion>(lt<i32>(read<i32>(%[[V_A:[0-9]+]]), read<i32>(%[[V_B:[0-9]+]]))), const<i32>(0)));
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @user_variable_kept(%{{[0-9]+}} a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_FLAG:[0-9]+]] flag: bool [storage=automatic];
// CHECK-NEXT:     if ne<i32>(read<i32>(%[[V_A:[0-9]+]]), const<i32>(0))
// CHECK-NEXT:         write<bool>(%[[V_FLAG]], ne<i32, reason=assign>(const<i32>(1), const<i32>(0)));
// CHECK-NEXT:     else
// CHECK-NEXT:         write<bool>(%[[V_FLAG]], gt<i32>(read<i32>(%[[V_A]]), const<i32>(3)));
// CHECK-NEXT:     return from_bool<i32, reason=return>(read<bool>(%[[V_FLAG]]));
// CHECK: {{^}}}
// SLATE-FILECHECK-END
