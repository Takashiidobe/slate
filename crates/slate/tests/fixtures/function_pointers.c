#include <stdio.h>

static int add_pair(int lhs, int rhs) { return lhs + rhs; }

static int mul_pair(int lhs, int rhs) { return lhs * rhs; }

static int apply_binary(int (*op)(int, int), int lhs, int rhs) {
  return op(lhs, rhs);
}

int main(void) {
  int (*chosen)(int, int) = add_pair;
  printf("%d\n", apply_binary(chosen, 4, 5));
  chosen = mul_pair;
  printf("%d\n", chosen(3, 6));
  return 0;
}
// REWRITES-DAG: add_pair as *const ()
// REWRITES-DAG: Option<unsafe extern "C" fn(i32, i32) -> i32>>(add_pair as *const ())
// REWRITES-DAG: .unwrap()(
// REWRITES-DAG: lhs + rhs
// REWRITES-DAG: lhs * rhs
// REWRITES-NOT: return _v
// REWRITES-NOT: let mut __retval: i32 = lhs
