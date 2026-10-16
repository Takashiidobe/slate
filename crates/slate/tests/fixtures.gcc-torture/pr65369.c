/* PR tree-optimization/65369 */
#include <stdint.h>

static const char data[] = "12345678901234567890123456789012345678901234567890"
                           "123456789012345678901234567890";

__attribute__((noinline)) static void foo(const uint32_t *buf) {
  if (__builtin_memcmp(buf, data, 64))
    __builtin_abort();
}

__attribute__((noinline)) static void bar(const unsigned char *block) {
  uint32_t buf[16];
  __builtin_memcpy(buf + 0, block + 0, 4);
  __builtin_memcpy(buf + 1, block + 4, 4);
  __builtin_memcpy(buf + 2, block + 8, 4);
  __builtin_memcpy(buf + 3, block + 12, 4);
  __builtin_memcpy(buf + 4, block + 16, 4);
  __builtin_memcpy(buf + 5, block + 20, 4);
  __builtin_memcpy(buf + 6, block + 24, 4);
  __builtin_memcpy(buf + 7, block + 28, 4);
  __builtin_memcpy(buf + 8, block + 32, 4);
  __builtin_memcpy(buf + 9, block + 36, 4);
  __builtin_memcpy(buf + 10, block + 40, 4);
  __builtin_memcpy(buf + 11, block + 44, 4);
  __builtin_memcpy(buf + 12, block + 48, 4);
  __builtin_memcpy(buf + 13, block + 52, 4);
  __builtin_memcpy(buf + 14, block + 56, 4);
  __builtin_memcpy(buf + 15, block + 60, 4);
  foo(buf);
}

int main() {
  unsigned char input[sizeof data + 16] __attribute__((aligned(16)));
  __builtin_memset(input, 0, sizeof input);
  __builtin_memcpy(input + 1, data, sizeof data);
  bar(input + 1);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/65369
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "memcmp"]
// CHECK-NEXT:     fn __builtin_memcmp([[V_0:__v[0-9]+]]: *const (), [[V_1:__v[0-9]+]]: *const (), [[V_2:__v[0-9]+]]: u64) -> i32;
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT:     #[link_name = "memcpy"]
// CHECK-NEXT:     fn __builtin_memcpy([[V_3:__v[0-9]+]]: *mut (), [[V_4:__v[0-9]+]]: *const (), [[V_5:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT:     #[link_name = "memset"]
// CHECK-NEXT:     fn __builtin_memset([[V_6:__v[0-9]+]]: *mut (), [[V_7:__v[0-9]+]]: i32, [[V_8:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut data: __SlateAlign16<[i8; 81]> = __SlateAlign16([
// CHECK-NEXT:     49 as i8, 50 as i8, 51 as i8, 52 as i8, 53 as i8, 54 as i8, 55 as i8, 56 as i8, 57 as i8,
// CHECK-NEXT:     48 as i8, 49 as i8, 50 as i8, 51 as i8, 52 as i8, 53 as i8, 54 as i8, 55 as i8, 56 as i8,
// CHECK-NEXT:     57 as i8, 48 as i8, 49 as i8, 50 as i8, 51 as i8, 52 as i8, 53 as i8, 54 as i8, 55 as i8,
// CHECK-NEXT:     56 as i8, 57 as i8, 48 as i8, 49 as i8, 50 as i8, 51 as i8, 52 as i8, 53 as i8, 54 as i8,
// CHECK-NEXT:     55 as i8, 56 as i8, 57 as i8, 48 as i8, 49 as i8, 50 as i8, 51 as i8, 52 as i8, 53 as i8,
// CHECK-NEXT:     54 as i8, 55 as i8, 56 as i8, 57 as i8, 48 as i8, 49 as i8, 50 as i8, 51 as i8, 52 as i8,
// CHECK-NEXT:     53 as i8, 54 as i8, 55 as i8, 56 as i8, 57 as i8, 48 as i8, 49 as i8, 50 as i8, 51 as i8,
// CHECK-NEXT:     52 as i8, 53 as i8, 54 as i8, 55 as i8, 56 as i8, 57 as i8, 48 as i8, 49 as i8, 50 as i8,
// CHECK-NEXT:     51 as i8, 52 as i8, 53 as i8, 54 as i8, 55 as i8, 56 as i8, 57 as i8, 48 as i8, 0 as i8,
// CHECK-NEXT: ]);
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut buf: *const u32) {
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         __builtin_memcmp(
// CHECK-NEXT:             buf as *const (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of!(data.0) as *const i8 }) as *const (),
// CHECK-NEXT:             ((64 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut block: *const u8) {
// CHECK-NEXT:     let mut buf: __SlateAlign16<[u32; 16]> = __SlateAlign16([0 as u32; 16]);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memcpy(
// CHECK-NEXT:             (unsafe { (buf.0.as_mut_ptr() as *mut u32).offset((0 as i32) as isize) }) as *mut (),
// CHECK-NEXT:             (unsafe { block.offset((0 as i32) as isize) }) as *const (),
// CHECK-NEXT:             ((4 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memcpy(
// CHECK-NEXT:             (unsafe { (buf.0.as_mut_ptr() as *mut u32).offset((1 as i32) as isize) }) as *mut (),
// CHECK-NEXT:             (unsafe { block.offset((4 as i32) as isize) }) as *const (),
// CHECK-NEXT:             ((4 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memcpy(
// CHECK-NEXT:             (unsafe { (buf.0.as_mut_ptr() as *mut u32).offset((2 as i32) as isize) }) as *mut (),
// CHECK-NEXT:             (unsafe { block.offset((8 as i32) as isize) }) as *const (),
// CHECK-NEXT:             ((4 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memcpy(
// CHECK-NEXT:             (unsafe { (buf.0.as_mut_ptr() as *mut u32).offset((3 as i32) as isize) }) as *mut (),
// CHECK-NEXT:             (unsafe { block.offset((12 as i32) as isize) }) as *const (),
// CHECK-NEXT:             ((4 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memcpy(
// CHECK-NEXT:             (unsafe { (buf.0.as_mut_ptr() as *mut u32).offset((4 as i32) as isize) }) as *mut (),
// CHECK-NEXT:             (unsafe { block.offset((16 as i32) as isize) }) as *const (),
// CHECK-NEXT:             ((4 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memcpy(
// CHECK-NEXT:             (unsafe { (buf.0.as_mut_ptr() as *mut u32).offset((5 as i32) as isize) }) as *mut (),
// CHECK-NEXT:             (unsafe { block.offset((20 as i32) as isize) }) as *const (),
// CHECK-NEXT:             ((4 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memcpy(
// CHECK-NEXT:             (unsafe { (buf.0.as_mut_ptr() as *mut u32).offset((6 as i32) as isize) }) as *mut (),
// CHECK-NEXT:             (unsafe { block.offset((24 as i32) as isize) }) as *const (),
// CHECK-NEXT:             ((4 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memcpy(
// CHECK-NEXT:             (unsafe { (buf.0.as_mut_ptr() as *mut u32).offset((7 as i32) as isize) }) as *mut (),
// CHECK-NEXT:             (unsafe { block.offset((28 as i32) as isize) }) as *const (),
// CHECK-NEXT:             ((4 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memcpy(
// CHECK-NEXT:             (unsafe { (buf.0.as_mut_ptr() as *mut u32).offset((8 as i32) as isize) }) as *mut (),
// CHECK-NEXT:             (unsafe { block.offset((32 as i32) as isize) }) as *const (),
// CHECK-NEXT:             ((4 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memcpy(
// CHECK-NEXT:             (unsafe { (buf.0.as_mut_ptr() as *mut u32).offset((9 as i32) as isize) }) as *mut (),
// CHECK-NEXT:             (unsafe { block.offset((36 as i32) as isize) }) as *const (),
// CHECK-NEXT:             ((4 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memcpy(
// CHECK-NEXT:             (unsafe { (buf.0.as_mut_ptr() as *mut u32).offset((10 as i32) as isize) }) as *mut (),
// CHECK-NEXT:             (unsafe { block.offset((40 as i32) as isize) }) as *const (),
// CHECK-NEXT:             ((4 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memcpy(
// CHECK-NEXT:             (unsafe { (buf.0.as_mut_ptr() as *mut u32).offset((11 as i32) as isize) }) as *mut (),
// CHECK-NEXT:             (unsafe { block.offset((44 as i32) as isize) }) as *const (),
// CHECK-NEXT:             ((4 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memcpy(
// CHECK-NEXT:             (unsafe { (buf.0.as_mut_ptr() as *mut u32).offset((12 as i32) as isize) }) as *mut (),
// CHECK-NEXT:             (unsafe { block.offset((48 as i32) as isize) }) as *const (),
// CHECK-NEXT:             ((4 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memcpy(
// CHECK-NEXT:             (unsafe { (buf.0.as_mut_ptr() as *mut u32).offset((13 as i32) as isize) }) as *mut (),
// CHECK-NEXT:             (unsafe { block.offset((52 as i32) as isize) }) as *const (),
// CHECK-NEXT:             ((4 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memcpy(
// CHECK-NEXT:             (unsafe { (buf.0.as_mut_ptr() as *mut u32).offset((14 as i32) as isize) }) as *mut (),
// CHECK-NEXT:             (unsafe { block.offset((56 as i32) as isize) }) as *const (),
// CHECK-NEXT:             ((4 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memcpy(
// CHECK-NEXT:             (unsafe { (buf.0.as_mut_ptr() as *mut u32).offset((15 as i32) as isize) }) as *mut (),
// CHECK-NEXT:             (unsafe { block.offset((60 as i32) as isize) }) as *const (),
// CHECK-NEXT:             ((4 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     foo(buf.0.as_mut_ptr() as *const u32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut input: __SlateAlign16<[u8; 97]> = __SlateAlign16([0 as u8; 97]);
// CHECK-NEXT:     unsafe { __builtin_memset(input.0.as_mut_ptr() as *mut (), 0 as i32, 97 as u64) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memcpy(
// CHECK-NEXT:             (unsafe { (input.0.as_mut_ptr() as *mut u8).offset((1 as i32) as isize) }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of!(data.0) as *const i8 }) as *const (),
// CHECK-NEXT:             81 as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     bar((unsafe { (input.0.as_mut_ptr() as *mut u8).offset((1 as i32) as isize) }) as *const u8);
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
