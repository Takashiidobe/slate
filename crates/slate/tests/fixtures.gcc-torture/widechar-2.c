#include <stddef.h>

void abort(void);
void exit(int);

const wchar_t ws[] = L"foo";

int main(void) {
  if (ws[0] != L'f' || ws[1] != L'o' || ws[2] != L'o' || ws[3] != L'\0')
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut ws: __SlateAlign16<[i32; 4]> = __SlateAlign16([
// CHECK-NEXT:     102 as i32,
// CHECK-NEXT:     111 as i32,
// CHECK-NEXT:     111 as i32,
// CHECK-NEXT:     0 as i32,
// CHECK-NEXT: ]);
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of!(ws.0) as *const i32 }.offset((0 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     }) != (102 as i32)
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of!(ws.0) as *const i32 }
// CHECK-NEXT:                     .offset((1 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) != (111 as i32)
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of!(ws.0) as *const i32 }
// CHECK-NEXT:                     .offset((2 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) != (111 as i32)
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of!(ws.0) as *const i32 }
// CHECK-NEXT:                     .offset((3 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
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
