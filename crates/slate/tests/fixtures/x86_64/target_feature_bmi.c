// @check-fn-begin
__attribute__((target("bmi"))) int bmi_probe(int x) { return x + 1; }
// @check-fn-end

int main(void) { return bmi_probe(41); }

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}unsafe fn bmi_probe(x: i32) -> i32 {
// CHECK-NEXT:     x + 1
// CHECK: {{^}}}
// SLATE-FILECHECK-END
