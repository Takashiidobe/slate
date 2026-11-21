int g(void);

int select(int a, int b) { return a < b ? 1 : 0; }

int select_inverted(int a, int b) { return a < b ? 0 : 1; }

int tested_select(int a, int b) { return (a < b ? 1 : 0) != 0; }

int int_temporary_kept(int a) { return a ? 1 : g() != 0; }

int other_kept(int a) { return a ? 2 : 0; }

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @select(%{{[0-9]+}} a: i32, %{{[0-9]+}} b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     return from_bool<i32, reason=assign>(lt<i32>(read<i32>(%[[V_A:[0-9]+]]), read<i32>(%[[V_B:[0-9]+]])));
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @select_inverted(%{{[0-9]+}} a: i32, %{{[0-9]+}} b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     return from_bool<i32, reason=assign>(not<bool>(lt<i32>(read<i32>(%[[V_A:[0-9]+]]), read<i32>(%[[V_B:[0-9]+]]))));
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @tested_select(%{{[0-9]+}} a: i32, %{{[0-9]+}} b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     return from_bool<i32, reason=return>(lt<i32>(read<i32>(%[[V_A:[0-9]+]]), read<i32>(%[[V_B:[0-9]+]])));
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @int_temporary_kept(%{{[0-9]+}} a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V0:[0-9]+]]: i32 [synthetic];
// CHECK-NEXT:     if ne<i32>(read<i32>(%[[V_A:[0-9]+]]), const<i32>(0))
// CHECK-NEXT:         write<i32>(%[[V0]], const<i32>(1));
// CHECK-NEXT:     else
// CHECK-NEXT:         write<i32>(%[[V0]], from_bool<i32, reason=promotion>(ne<i32>(call<i32, signature=fn() -> i32>(%[[V1:[0-9]+]]), const<i32>(0))));
// CHECK-NEXT:     return read<i32>(%[[V0]]);
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @other_kept(%{{[0-9]+}} a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     return conditional<i32>(ne<i32>(read<i32>(%[[V_A:[0-9]+]]), const<i32>(0)), const<i32>(2), const<i32>(0));
// CHECK: {{^}}}
// SLATE-FILECHECK-END
