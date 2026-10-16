#include <stdio.h>

struct First {
  int value;
};

struct Second {
  int value;
};

typedef union {
  struct First  *first;
  struct Second *second;
} PointerArgument __attribute__((transparent_union));

__attribute__((noinline)) static int read_value(PointerArgument argument) {
  return argument.first->value;
}

// @check-fn-begin
int main(void) {
  struct First  first  = {.value = 17};
  struct Second second = {.value = 29};
  printf("%d %d\n", read_value(&first), read_value(&second));
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut first: First = First { value: 17 as i32 };
// CHECK-NEXT:     let mut second: Second = Second { value: 29 as i32 };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %d\n".as_ptr() as *const i8,
// CHECK-NEXT:             read_value({
// CHECK-NEXT:                 let mut [[T_0:__t[0-9]+]]: [[SLATERECORD_0:__SlateRecord[0-9]+]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                 [[T_0]].first = std::ptr::addr_of_mut!(first);
// CHECK-NEXT:                 [[T_0]]
// CHECK-NEXT:             }),
// CHECK-NEXT:             read_value({
// CHECK-NEXT:                 let mut [[T_1:__t[0-9]+]]: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                 [[T_1]].second = std::ptr::addr_of_mut!(second);
// CHECK-NEXT:                 [[T_1]]
// CHECK-NEXT:             }),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     0 as i32
// CHECK: {{^}}}
// SLATE-FILECHECK-END
