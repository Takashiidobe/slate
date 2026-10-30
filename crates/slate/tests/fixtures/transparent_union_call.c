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

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut first: First = First { value: 17 };
// CHECK-NEXT:     let mut second: Second = Second { value: 29 };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}", read_value({ let mut [[T_0:__t[0-9]+]] : __SlateRecord_transparent_union_call_c_14 =
// CHECK-NEXT:         unsafe { std::mem::zeroed() }; [[T_0]].first = std::ptr::addr_of_mut!(first); [[T_0]]
// CHECK-NEXT:         }), read_value({ let mut [[T_1:__t[0-9]+]] : __SlateRecord_transparent_union_call_c_14 =
// CHECK-NEXT:         unsafe { std::mem::zeroed() }; [[T_1]].second = std::ptr::addr_of_mut!(second); [[T_1]]
// CHECK-NEXT:         })
// CHECK-NEXT:     );
// CHECK-NEXT:     0
// CHECK: {{^}}}
// SLATE-FILECHECK-END
