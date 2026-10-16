/* PR middle-end/50865 */

#define INT64_MIN (-__LONG_LONG_MAX__ - 1)

int main() {
  volatile long long l1 = 1;
  volatile long long l2 = -1;
  volatile long long l3 = -1;

  if ((INT64_MIN % 1LL) != 0)
    __builtin_abort();
  if ((INT64_MIN % l1) != 0)
    __builtin_abort();
  if (l2 == -1) {
    if ((INT64_MIN % 1LL) != 0)
      __builtin_abort();
  } else if ((INT64_MIN % -l2) != 0)
    __builtin_abort();
  if ((INT64_MIN % -l3) != 0)
    __builtin_abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR middle-end/50865
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut l1: i64 = (1 as i32) as i64;
// CHECK-NEXT:     let mut l2: i64 = -(1 as i32) as i64;
// CHECK-NEXT:     let mut l3: i64 = -(1 as i32) as i64;
// CHECK-NEXT:     if (-(9223372036854775807 as i64) - ((1 as i32) as i64)) % (1 as i64) != ((0 as i32) as i64) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (-(9223372036854775807 as i64) - ((1 as i32) as i64))
// CHECK-NEXT:         % unsafe { std::ptr::read_volatile(std::ptr::addr_of!(l1)) }
// CHECK-NEXT:         != ((0 as i32) as i64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(l2)) }) == (-(1 as i32) as i64) {
// CHECK-NEXT:         if (-(9223372036854775807 as i64) - ((1 as i32) as i64)) % (1 as i64) != ((0 as i32) as i64)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if (-(9223372036854775807 as i64) - ((1 as i32) as i64))
// CHECK-NEXT:             % -unsafe { std::ptr::read_volatile(std::ptr::addr_of!(l2)) }
// CHECK-NEXT:             != ((0 as i32) as i64)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if (-(9223372036854775807 as i64) - ((1 as i32) as i64))
// CHECK-NEXT:         % -unsafe { std::ptr::read_volatile(std::ptr::addr_of!(l3)) }
// CHECK-NEXT:         != ((0 as i32) as i64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
