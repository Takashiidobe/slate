/* PR middle-end/77718 */

char a[64] __attribute__((aligned(8)));

__attribute__((noinline, noclone)) int foo(void) {
  return __builtin_memcmp("bbbbbb", a, 6);
}

__attribute__((noinline, noclone)) int bar(void) {
  return __builtin_memcmp(a, "bbbbbb", 6);
}

int main() {
  __builtin_memset(a, 'a', sizeof(a));
  if (((foo() < 0) ^ ('a' > 'b')) || ((bar() < 0) ^ ('a' < 'b')))
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR middle-end/77718
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "memcmp"]
// CHECK-NEXT:     fn __builtin_memcmp([[V_0:__v[0-9]+]]: *const (), [[V_1:__v[0-9]+]]: *const (), [[V_2:__v[0-9]+]]: u64) -> i32;
// CHECK-NEXT:     #[link_name = "memset"]
// CHECK-NEXT:     fn __builtin_memset([[V_3:__v[0-9]+]]: *mut (), [[V_4:__v[0-9]+]]: i32, [[V_5:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(8))]
// CHECK-NEXT: struct __SlateAlign8<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: __SlateAlign8<[i8; 64]> = __SlateAlign8(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: fn foo() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memcmp(
// CHECK-NEXT:             b"bbbbbb\0".as_ptr() as *const (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(a.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             ((6 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memcmp(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(a.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             b"bbbbbb\0".as_ptr() as *const (),
// CHECK-NEXT:             ((6 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(a.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             97 as i32,
// CHECK-NEXT:             64 as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[V_6:__v[0-9]+]]: bool;
// CHECK-NEXT:     if ((foo() < (0 as i32)) as i32) ^ (((97 as i32) > (98 as i32)) as i32) != (0 as i32) {
// CHECK-NEXT:         [[V_6]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_6]] = ((bar() < (0 as i32)) as i32) ^ (((97 as i32) < (98 as i32)) as i32) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_6]] {
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
