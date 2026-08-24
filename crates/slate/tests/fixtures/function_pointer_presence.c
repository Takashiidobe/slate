static int bump(int value) { return value + 1; }

static int maybe_apply(int (*op)(int), int value) {
  if (op) {
    return op(value);
  }
  return value;
}

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
// REWRITES-DAG: .is_some()
// REWRITES-DAG: if op.is_some()
// REWRITES-DAG: .is_none()
// REWRITES-DAG: return unsafe { op.unwrap()(value) };
// REWRITES-NOT: != None
// REWRITES-NOT: == None
// REWRITES-NOT: std::ptr::null_mut()
// REWRITES-NOT: let _v{{[0-9]+}}: Option<fn(i32) -> i32> = op;
// REWRITES-NOT: __retval
