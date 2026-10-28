#include <stdio.h>

static int bump(int *p) {
    int before = *p;
    __asm__ volatile("addl $1, %0" : "+m"(*p));
    return *p - before;
}

static int store(int *p) {
    int before = *p;
    __asm__ volatile("movl $7, (%0)" : : "r"(p) : "memory");
    return *p - before;
}

int main(void) {
    int value = 41;
    printf("%d\n", bump(&value));
    printf("%d\n", value);
    printf("%d\n", store(&value));
    printf("%d\n", value);
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn bump(mut p: &mut i32) -> i32 {
// CHECK-NEXT:     let before: i32 = unsafe { *(p as *mut i32) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "addl $1, ({0:r})", in (reg) p as * mut i32, options(nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     (unsafe { *(p as *mut i32) }) - before
// CHECK-NEXT: }
// CHECK-NEXT: fn store(mut p: &mut i32) -> i32 {
// CHECK-NEXT:     let before: i32 = unsafe { *(p as *mut i32) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "movl $7, ({0:r})", in (reg) (p as * mut i32) as u64, options(nostack,
// CHECK-NEXT:             att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     (unsafe { *(p as *mut i32) }) - before
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut value: i32 = 41;
// CHECK-NEXT:     let __slate_printf_arg0 = bump(unsafe { &mut (*std::ptr::addr_of_mut!(value)) });
// CHECK-NEXT:     println!("{}", __slate_printf_arg0);
// CHECK-NEXT:     println!("{}", value);
// CHECK-NEXT:     let __slate_printf_arg0 = store(unsafe { &mut (*std::ptr::addr_of_mut!(value)) });
// CHECK-NEXT:     println!("{}", __slate_printf_arg0);
// CHECK-NEXT:     println!("{}", value);
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
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
