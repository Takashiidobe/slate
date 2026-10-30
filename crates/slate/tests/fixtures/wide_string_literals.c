#include <stdio.h>
#include <uchar.h>
#include <wchar.h>

static const wchar_t *greeting = L"hello" L" " "world";

int main(void) {
  const char16_t *u16 = u"été";
  const char32_t *u32 = U"\U0001F600!";
  wchar_t         buf[] = L"local";
  buf[0]                = L'L';
  printf("%zu %d %d\n", wcslen(greeting), greeting[5], greeting[10]);
  printf("%x %x %x %x\n", u16[0], u16[2], u32[0], u32[1]);
  printf("%ls %zu\n", buf, sizeof(L"abc"));
  printf("%d %d\n", L"a" "b"[1], ((const char *)L"xyz")[4]);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT:     fn wcslen([[ARG_0]]: *const i32) -> u64;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut __slate_str19: [i32; 12] = [
// CHECK-NEXT:     104, 101, 108, 108, 111, 32, 119, 111, 114, 108, 100, 0,
// CHECK-NEXT: ];
// CHECK-NEXT: static mut greeting: *const i32 = (unsafe {
// CHECK-NEXT:     std::ptr::addr_of_mut!(__slate_str19) as *mut i32
// CHECK-NEXT: }) as *const i32;
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let u16: *const u16 = (unsafe { std::ptr::addr_of_mut!(__slate_str20) as *mut u16 })
// CHECK-NEXT:         as *const u16;
// CHECK-NEXT:     let u32: *const u32 = (unsafe { std::ptr::addr_of_mut!(__slate_str21) as *mut u32 })
// CHECK-NEXT:         as *const u32;
// CHECK-NEXT:     let mut buf: __SlateAlign16<[i32; 6]> = __SlateAlign16([
// CHECK-NEXT:         108i32, 111i32, 99i32, 97i32, 108i32, 0i32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     buf.0[0] = 76;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%zu %d %d\n".as_ptr(),
// CHECK-NEXT:             wcslen(greeting),
// CHECK-NEXT:             *unsafe { greeting }.add(5),
// CHECK-NEXT:             *unsafe { greeting }.add(10),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%x %x %x %x\n".as_ptr(),
// CHECK-NEXT:             (*u16.add(0) as u32) as i32,
// CHECK-NEXT:             (*u16.add(2) as u32) as i32,
// CHECK-NEXT:             *u32.add(0),
// CHECK-NEXT:             *u32.add(1),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%ls %zu\n".as_ptr(), buf.0.as_mut_ptr() as *mut i32, 16u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}", unsafe { __slate_str26[1] }, (unsafe { *
// CHECK-NEXT:         ((std::ptr::addr_of_mut!(__slate_str27) as * mut i32) as * const i8).add(4) }) as
// CHECK-NEXT:         i32
// CHECK-NEXT:     );
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: static mut __slate_str20: [u16; 4] = [233, 116, 233, 0];
// CHECK-NEXT: static mut __slate_str21: [u32; 3] = [128512, 33, 0];
// CHECK-NEXT: static mut __slate_str26: [i32; 3] = [97, 98, 0];
// CHECK-NEXT: static mut __slate_str27: [i32; 4] = [120, 121, 122, 0];
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
