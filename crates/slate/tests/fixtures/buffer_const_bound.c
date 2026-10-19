#include <stdio.h>
#include <stdlib.h>

static int sum_fixed(int *p) {
  int total = 0;
  for (int i = 0; i < 4; i++) {
    total += p[i];
  }
  return total;
}

static void scale_fixed(int *p) {
  for (int i = 0; i < 3; i++) {
    p[i] = p[i] * 2;
  }
}

static int sum3(int *p) {
  int t = 0;
  for (int i = 0; i < 3; i++) {
    t += p[i];
  }
  return t;
}

static int mix(int *p) {
  int t = 0;
  for (int i = 0; i < 3; i++) {
    t += p[i];
  }
  t += p[5];
  return t;
}

// @check-fn-begin
int main(void) {
  int arr[4] = {10, 20, 30, 40};
  int r      = sum_fixed(arr);

  int a[3] = {1, 2, 3};
  scale_fixed(a);

  int *m = malloc(3 * sizeof(int));
  m[0]   = 5;
  m[1]   = 6;
  m[2]   = 7;
  int r2 = sum3(m);
  free(m);

  int big[6] = {1, 2, 3, 4, 5, 6};
  int r3     = mix(big);

  printf("%d %d %d %d %d\n", r, a[0], a[2], r2, r3);
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut arr: __SlateAlign16<[i32; 4]> = __SlateAlign16([10i32, 20i32, 30i32, 40i32]);
// CHECK-NEXT:     let r: i32 = sum_fixed(unsafe {
// CHECK-NEXT:         std::slice::from_raw_parts(arr.0.as_mut_ptr() as *const i32, 4usize)
// CHECK-NEXT:     });
// CHECK-NEXT:     let mut a: [i32; 3] = [1, 2, 3];
// CHECK-NEXT:     scale_fixed(unsafe {
// CHECK-NEXT:         std::slice::from_raw_parts_mut(a.as_mut_ptr() as *mut i32, 3usize)
// CHECK-NEXT:     });
// CHECK-NEXT:     let m: *mut i32 = (unsafe { malloc(12u64) }) as *mut i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *m.offset(0 as isize) = 5;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *m.offset(1 as isize) = 6;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *m.offset(2 as isize) = 7;
// CHECK-NEXT:     }
// CHECK-NEXT:     let r2: i32 = sum3(unsafe { std::slice::from_raw_parts(m as *const i32, 3usize) });
// CHECK-NEXT:     unsafe { free(m as *mut ()) };
// CHECK-NEXT:     let mut big: __SlateAlign16<[i32; 6]> = __SlateAlign16([
// CHECK-NEXT:         1i32, 2i32, 3i32, 4i32, 5i32, 6i32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let r3: i32 = mix(big.0.as_mut_ptr() as *mut i32);
// CHECK-NEXT:     println!("{} {} {} {} {}", r, a[0], a[2], r2, r3);
// CHECK-NEXT:     0
// CHECK: {{^}}}
// SLATE-FILECHECK-END
