#include <stdio.h>

static int *identity_mut(int *value) { return value; }

static int *forward_mut(int *value) { return identity_mut(value); }

static const int *identity_const(const int *value) { return value; }

static int *choose_value(int *first, int *second, int choose_first) {
  if (choose_first)
    return first;
  return second;
}

int main(void) {
  int first = 20;
  int second = 22;
  int *alias = forward_mut(&first);
  *alias += 2;
  const int *read_alias = identity_const(&second);
  int *ambiguous = choose_value(&first, &second, 1);
  printf("%d %d %d\n", first, *read_alias, *ambiguous);
  return 0;
}

// REWRITES-DAG: fn identity_mut(arg{{[0-9]+}}: &mut i32) -> *mut i32
// REWRITES-DAG: fn forward_mut(arg{{[0-9]+}}: &mut i32) -> *mut i32
// REWRITES-DAG: fn identity_const(arg{{[0-9]+}}: &i32) -> *mut i32
// REWRITES-DAG: fn choose_value(arg{{[0-9]+}}: *mut i32, arg{{[0-9]+}}: *mut i32, arg{{[0-9]+}}: i32) -> *mut i32
