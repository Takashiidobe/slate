/* { dg-options "-fwide-exec-charset=UTF-32BE" } */
#include <stdio.h>
#include <wchar.h>

int main(void) {
  const wchar_t text[] = L"Azé";
  for (unsigned i = 0; i < sizeof text / sizeof text[0]; i++)
    printf("%08x ", (unsigned)text[i]);
  printf("%08x\n", (unsigned)L'Z');
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-fwide-exec-charset=UTF-32BE" }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe impl<T> Sync for __SlateAlign16<T> {}
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let text: __SlateAlign16<[i32; 4]> =
// CHECK-NEXT:         __SlateAlign16([1090519040i32, 2046820352i32, -385875968i32, 0i32]);
// CHECK-NEXT:     let mut i: u32 = 0;
// CHECK-NEXT:     while (i as u64)
// CHECK-NEXT:         < (core::mem::size_of::<[i32; 4]>() as u64) / (core::mem::size_of::<i32>() as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::printf(c"%08x ".as_ptr(), text.0[i as usize] as u32) };
// CHECK-NEXT:         unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:         i = i.wrapping_add(1);
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%08x\n".as_ptr(), 1509949440u32) };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
