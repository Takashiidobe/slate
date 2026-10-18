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
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn wcslen(__s: *const i32) -> u64;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut __slate_str19: [i32; 12] = [
// CHECK-NEXT:     104 as i32,
// CHECK-NEXT:     101 as i32,
// CHECK-NEXT:     108 as i32,
// CHECK-NEXT:     108 as i32,
// CHECK-NEXT:     111 as i32,
// CHECK-NEXT:     32 as i32,
// CHECK-NEXT:     119 as i32,
// CHECK-NEXT:     111 as i32,
// CHECK-NEXT:     114 as i32,
// CHECK-NEXT:     108 as i32,
// CHECK-NEXT:     100 as i32,
// CHECK-NEXT:     0 as i32,
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
// CHECK-NEXT:         108 as i32,
// CHECK-NEXT:         111 as i32,
// CHECK-NEXT:         99 as i32,
// CHECK-NEXT:         97 as i32,
// CHECK-NEXT:         108 as i32,
// CHECK-NEXT:         0 as i32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     buf.0[0] = 76;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%zu %d %d\n".as_ptr(),
// CHECK-NEXT:             unsafe { wcslen(unsafe { greeting }) },
// CHECK-NEXT:             unsafe { *unsafe { unsafe { greeting }.offset(5 as isize) } },
// CHECK-NEXT:             unsafe { *unsafe { unsafe { greeting }.offset(10 as isize) } },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%x %x %x %x\n".as_ptr(),
// CHECK-NEXT:             ((unsafe { *unsafe { u16.offset(0 as isize) } }) as u32) as i32,
// CHECK-NEXT:             ((unsafe { *unsafe { u16.offset(2 as isize) } }) as u32) as i32,
// CHECK-NEXT:             unsafe { *unsafe { u32.offset(0 as isize) } },
// CHECK-NEXT:             unsafe { *unsafe { u32.offset(1 as isize) } },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%ls %zu\n".as_ptr(), buf.0.as_mut_ptr() as *mut i32, 16 as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}", unsafe { __slate_str26[1] }, (unsafe { * unsafe { ((unsafe {
// CHECK-NEXT:         std::ptr::addr_of_mut!(__slate_str27) as * mut i32 }) as * const i8).offset(4 as
// CHECK-NEXT:         isize) } }) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: static mut __slate_str20: [u16; 4] = [233 as u16, 116 as u16, 233 as u16, 0 as u16];
// CHECK-NEXT: static mut __slate_str21: [u32; 3] = [128512 as u32, 33 as u32, 0 as u32];
// CHECK-NEXT: static mut __slate_str26: [i32; 3] = [97 as i32, 98 as i32, 0 as i32];
// CHECK-NEXT: static mut __slate_str27: [i32; 4] = [120 as i32, 121 as i32, 122 as i32, 0 as i32];
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
