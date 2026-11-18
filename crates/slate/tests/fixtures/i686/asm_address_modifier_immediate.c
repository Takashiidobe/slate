#include <stdio.h>

static long immediate_address(void) {
    long v;
    __asm__("leal %a1, %0" : "=r"(v) : "i"(8));
    return v;
}

static int through_register(int *q) {
    int v;
    __asm__("movl %a1, %0" : "=r"(v) : "r"(q));
    return v;
}

int main(void) {
    int values[2] = {5, 9};
    printf("%ld %d\n", immediate_address(), through_register(&values[1]));
    return 0;
}

// SLATE-FILECHECK-BEGIN i686-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn immediate_address() -> i32 {
// CHECK-NEXT:     let mut v: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "leal {1}, {0:e}", lateout(reg) [[T_0]], const 8 as i32, options(pure, nomem,
// CHECK-NEXT:             nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         v = unsafe { core::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     v
// CHECK-NEXT: }
// CHECK-NEXT: fn through_register(mut q: *mut i32) -> i32 {
// CHECK-NEXT:     let mut v: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "movl ({1:e}), {0:e}", lateout(reg) [[T_0]], in (reg) q as u32, options(pure,
// CHECK-NEXT:             nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         v = unsafe { core::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     v
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut values: [i32; 2] = [5, 9];
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%ld %d\n".as_ptr(),
// CHECK-NEXT:             immediate_address(),
// CHECK-NEXT:             through_register((values.as_mut_ptr() as *mut i32).add(1)),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
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
