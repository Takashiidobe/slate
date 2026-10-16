#include <stdio.h>

static int add(int a, int unused) { return a + 1; }

static int get_used(int a, int b) { return a + b; }

static int remove_two(int a, int unused_a, int unused_b) { return a + 2; }

static int address_taken(int a, int unused) { return a + 4; }

// @check-fn-begin
int main(void) {
  int (*indirect)(int, int) = address_taken;
  printf("%d %d %d %d\n", add(5, 10), get_used(1, 2), remove_two(3, 4, 5),
         indirect(8, 9));
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut indirect: Option<unsafe extern "C-unwind" fn(i32, i32) -> i32> = Some(
// CHECK-NEXT:         address_taken,
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %d %d %d\n".as_ptr() as *const i8,
// CHECK-NEXT:             add(5 as i32, 10 as i32),
// CHECK-NEXT:             get_used(1 as i32, 2 as i32),
// CHECK-NEXT:             remove_two(3 as i32, 4 as i32, 5 as i32),
// CHECK-NEXT:             unsafe { indirect.unwrap()(8 as i32, 9 as i32) },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     0 as i32
// CHECK: {{^}}}
// SLATE-FILECHECK-END
