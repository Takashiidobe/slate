int         abs(int j);
extern void abort(void);

__attribute__((noinline)) int lisp_atan2(long dy, long dx) {
  if (dx <= 0)
    if (dy > 0)
      return abs(dx) <= abs(dy);
  return 0;
}

int main() {
  volatile long dy = 63, dx = -77;
  if (lisp_atan2(dy, dx))
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abs(j: i32) -> i32;
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn lisp_atan2(mut dy: i64, mut dx: i64) -> i32 {
// CHECK-NEXT:     if dx <= ((0 as i32) as i64) {
// CHECK-NEXT:         if dy > ((0 as i32) as i64) {
// CHECK-NEXT:             return ((unsafe { abs(dx as i32) }) <= unsafe { abs(dy as i32) }) as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut dy: i64 = (63 as i32) as i64;
// CHECK-NEXT:     let mut dx: i64 = -(77 as i32) as i64;
// CHECK-NEXT:     if lisp_atan2(
// CHECK-NEXT:         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(dy)) },
// CHECK-NEXT:         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(dx)) },
// CHECK-NEXT:     ) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
