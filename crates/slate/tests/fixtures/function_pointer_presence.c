static int bump(int value) { return value + 1; }

// @check-fn-begin
static int maybe_apply(int (*op)(int), int value) {
  if (op) {
    return op(value);
  }
  return value;
}
// @check-fn-end

// @check-fn-begin
int main(void) {
  int (*op)(int) = 0;
  int total      = maybe_apply(op, 4);
  op             = bump;
  if (op != 0) {
    total = total + maybe_apply(op, 5);
  }
  if (op == 0) {
    return 2;
  }
  return total == 10 ? 0 : 1;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn maybe_apply(op: Option<unsafe extern "C-unwind" fn(i32) -> i32>, value: i32) -> i32 {
// CHECK-NEXT:     if op != None {
// CHECK-NEXT:         return unsafe { op.unwrap()(value) };
// CHECK-NEXT:     }
// CHECK-NEXT:     value
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut op: Option<unsafe extern "C-unwind" fn(i32) -> i32> = None;
// CHECK-NEXT:     let mut total: i32 = maybe_apply(op, 4 as i32);
// CHECK-NEXT:     op = Some(bump);
// CHECK-NEXT:     if op != None {
// CHECK-NEXT:         total += maybe_apply(op, 5 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if op == None {
// CHECK-NEXT:         return 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     if total == 10 { 0 } else { 1 }
// CHECK: {{^}}}
// SLATE-FILECHECK-END
