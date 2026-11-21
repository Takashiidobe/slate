int tested(int a, int b) { return (a < b) != 0; }

int negated(int a, int b) { return (a < b) == 0; }

int double_not(int a, int b) { return !!(a < b); }

int inverted(int a, int b) { return !(a < b); }

int float_eq_inverted(double a, double b) { return !(a == b); }

int float_lt_kept(double a, double b) { return !(a < b); }

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @tested(%{{[0-9]+}} a: i32, %{{[0-9]+}} b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     return from_bool<i32, reason=return>(lt<i32>(read<i32>(%[[V_A:[0-9]+]]), read<i32>(%[[V_B:[0-9]+]])));
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @negated(%{{[0-9]+}} a: i32, %{{[0-9]+}} b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     return from_bool<i32, reason=return>(ge<i32>(read<i32>(%[[V_A:[0-9]+]]), read<i32>(%[[V_B:[0-9]+]])));
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @double_not(%{{[0-9]+}} a: i32, %{{[0-9]+}} b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     return from_bool<i32, reason=return>(lt<i32>(read<i32>(%[[V_A:[0-9]+]]), read<i32>(%[[V_B:[0-9]+]])));
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @inverted(%{{[0-9]+}} a: i32, %{{[0-9]+}} b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     return from_bool<i32, reason=return>(ge<i32>(read<i32>(%[[V_A:[0-9]+]]), read<i32>(%[[V_B:[0-9]+]])));
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @float_eq_inverted(%{{[0-9]+}} a: f64, %{{[0-9]+}} b: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     return from_bool<i32, reason=return>(ne<f64, exceptions=ignore>(read<f64>(%[[V_A:[0-9]+]]), read<f64>(%[[V_B:[0-9]+]])));
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @float_lt_kept(%{{[0-9]+}} a: f64, %{{[0-9]+}} b: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     return from_bool<i32, reason=return>(not<bool>(lt<f64, exceptions=ignore>(read<f64>(%[[V_A:[0-9]+]]), read<f64>(%[[V_B:[0-9]+]]))));
// CHECK: {{^}}}
// SLATE-FILECHECK-END
