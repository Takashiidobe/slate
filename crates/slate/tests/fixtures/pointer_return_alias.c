#include <stdio.h>

static int *identity_mut(int *value) { return value; }

static int *forward_mut(int *value) { return identity_mut(value); }

static const int *identity_const(const int *value) { return value; }

// @check-fn-begin
static int *choose_value(int *first, int *second, int choose_first) {
  if (choose_first)
    return first;
  return second;
}
// @check-fn-end

int main(void) {
  int  first             = 20;
  int  second            = 22;
  int *alias             = forward_mut(&first);
  *alias                += 2;
  const int *read_alias  = identity_const(&second);
  int       *ambiguous   = choose_value(&first, &second, 1);
  printf("%d %d %d\n", first, *read_alias, *ambiguous);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn choose_value(
// CHECK-NEXT:     mut first: *mut i32,
// CHECK-NEXT:     mut second: *mut i32,
// CHECK-NEXT:     mut choose_first: i32,
// CHECK-NEXT: ) -> *mut i32 {
// CHECK-NEXT:     if choose_first != (0 as i32) {
// CHECK-NEXT:         return first;
// CHECK-NEXT:     }
// CHECK-NEXT:     second
// CHECK: {{^}}}
// SLATE-FILECHECK-END
