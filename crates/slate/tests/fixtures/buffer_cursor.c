#include <stdio.h>

struct cursor {
  int *ptr;
};

// @check-fn-begin
int main(void) {
  int values[4];
  for (int i = 0; i < 4; i++) {
    values[i] = i + getchar();
  }
  struct cursor c;
  c.ptr = values + 1;
  struct cursor d;
  d.ptr = values + 3;
  printf("%d %ld\n", *c.ptr + *d.ptr, d.ptr - c.ptr);
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut values: __SlateAlign16<[i32; 4]> = __SlateAlign16([0 as i32; 4]);
// CHECK-NEXT:     for i in 0 as i32..4 as i32 {
// CHECK-NEXT:         values.0[i as usize] = i + unsafe { getchar() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut c: cursor = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     c.ptr = unsafe { (values.0.as_mut_ptr() as *mut i32).offset(1 as isize) };
// CHECK-NEXT:     let mut d: cursor = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     d.ptr = unsafe { (values.0.as_mut_ptr() as *mut i32).offset(3 as isize) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %ld\n".as_ptr(),
// CHECK-NEXT:             (unsafe { *c.ptr }) + unsafe { *d.ptr },
// CHECK-NEXT:             (unsafe { d.ptr.offset_from(c.ptr as *mut i32) }) as i64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK: {{^}}}
// SLATE-FILECHECK-END
