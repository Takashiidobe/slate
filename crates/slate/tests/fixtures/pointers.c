#include <stdio.h>

// @check-fn-begin
static int bump_through_pointer(int value) {
  int  local = value;
  int *ptr   = &local;
  *ptr       = *ptr + 3;
  return local;
}
// @check-fn-end

static int add_into_pointer(int *slot, int amount) {
  *slot = *slot + amount;
  return *slot;
}

static int pick_with_pointer_arithmetic(int index) {
  int  values[4];
  int *ptr  = values;
  values[0] = 4;
  values[1] = 8;
  values[2] = 12;
  values[3] = 16;
  return *(ptr + index);
}

int main(void) {
  int total = 10;
  printf("%d\n", bump_through_pointer(5));
  printf("%d\n", add_into_pointer(&total, 7));
  printf("%d\n", total);
  printf("%d\n", pick_with_pointer_arithmetic(2));
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn bump_through_pointer(mut local: i32) -> i32 {
// CHECK-NEXT:     let ptr: *mut i32 = std::ptr::addr_of_mut!(local);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *ptr = (unsafe { *ptr }) + (3 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     local
// CHECK: {{^}}}
// SLATE-FILECHECK-END
