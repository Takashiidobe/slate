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

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let indirect: Option<unsafe extern "C-unwind" fn(i32, i32) -> i32> = Some(
// CHECK-NEXT:         address_taken,
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {}", add(5i32, 10i32), get_used(1i32, 2i32), remove_two(3i32, 4i32,
// CHECK-NEXT:         5i32), unsafe { indirect.unwrap() (8i32, 9i32) }
// CHECK-NEXT:     );
// CHECK-NEXT:     0
// CHECK: {{^}}}
// SLATE-FILECHECK-END
