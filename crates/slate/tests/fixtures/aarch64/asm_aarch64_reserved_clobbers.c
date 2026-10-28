#include <stdio.h>

static long through_x19(long value) {
    long result;
    __asm__("mov x19, %1\n\tadd x19, x19, #1\n\tmov %0, x19" : "=r"(result) : "r"(value) : "x19");
    return result;
}

static long through_x29(long value) {
    long result;
    __asm__("mov x29, %1\n\tadd %0, x29, #2" : "=&r"(result) : "r"(value) : "x29");
    return result;
}

static long through_fp(long value) {
    long result;
    __asm__("mov x29, %1\n\tadd %0, x29, #3" : "=&r"(result) : "r"(value) : "fp");
    return result;
}

static long through_both(long left, long right) {
    long result;
    __asm__ volatile("mov x19, %1\n\tmov x29, %2\n\tadd %0, x19, x29"
                     : "=&r"(result)
                     : "r"(left), "r"(right)
                     : "x19", "x29", "memory");
    return result;
}

static long recurse(long depth) {
    if (depth == 0) {
        return 0;
    }
    long local = depth * 3;
    __asm__ volatile("mov x19, #0" ::: "x19");
    return local + recurse(depth - 1);
}

int main(void) {
    printf("%ld\n", through_x19(41));
    printf("%ld\n", through_x29(40));
    printf("%ld\n", through_fp(39));
    printf("%ld\n", through_both(20, 22));
    printf("%ld\n", recurse(5));
    return 0;
}

// SLATE-FILECHECK-BEGIN aarch64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const u8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn through_x19(mut value: i64) -> i64 {
// CHECK-NEXT:     let mut result: i64 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "mov {2:x}, x19\nmov x19, {1:x}\n\tadd x19, x19, #1\n\tmov {0:x}, x19\nmov x19, {2:x}",
// CHECK-NEXT:             lateout(reg) [[T_0]], in (reg) value as u64, out(reg) _, options(pure, nomem,
// CHECK-NEXT:             nostack, preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         result = unsafe { std::mem::transmute::<u64, i64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     result
// CHECK-NEXT: }
// CHECK-NEXT: fn through_x29(mut value: i64) -> i64 {
// CHECK-NEXT:     let mut result: i64 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "mov {2:x}, x29\nmov x29, {1:x}\n\tadd {0:x}, x29, #2\nmov x29, {2:x}",
// CHECK-NEXT:             out(reg) [[T_0]], in (reg) value as u64, out(reg) _, options(pure, nomem,
// CHECK-NEXT:             nostack, preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         result = unsafe { std::mem::transmute::<u64, i64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     result
// CHECK-NEXT: }
// CHECK-NEXT: fn through_fp(mut value: i64) -> i64 {
// CHECK-NEXT:     let mut result: i64 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "mov {2:x}, x29\nmov x29, {1:x}\n\tadd {0:x}, x29, #3\nmov x29, {2:x}",
// CHECK-NEXT:             out(reg) [[T_0]], in (reg) value as u64, out(reg) _, options(pure, nomem,
// CHECK-NEXT:             nostack, preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         result = unsafe { std::mem::transmute::<u64, i64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     result
// CHECK-NEXT: }
// CHECK-NEXT: fn through_both(mut left: i64, mut right: i64) -> i64 {
// CHECK-NEXT:     let mut result: i64 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "mov {4:x}, x29\nmov {3:x}, x19\nmov x19, {1:x}\n\tmov x29, {2:x}\n\tadd {0:x}, x19, x29\nmov x19, {3:x}\nmov x29, {4:x}",
// CHECK-NEXT:             out(reg) [[T_0]], in (reg) left as u64, in (reg) right as u64, out(reg) _,
// CHECK-NEXT:             out(reg) _, options(nostack, preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         result = unsafe { std::mem::transmute::<u64, i64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     result
// CHECK-NEXT: }
// CHECK-NEXT: fn recurse(depth: i64) -> i64 {
// CHECK-NEXT:     if depth == 0 {
// CHECK-NEXT:         return 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     let local: i64 = depth * 3;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "mov {0:x}, x19\nmov x19, #0\nmov x19, {0:x}", out(reg) _, options(nostack,
// CHECK-NEXT:             preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     local + recurse(depth - 1)
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%ld\n".as_ptr(), through_x19(41i64)) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%ld\n".as_ptr(), through_x29(40i64)) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%ld\n".as_ptr(), through_fp(39i64)) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%ld\n".as_ptr(), through_both(20i64, 22i64)) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%ld\n".as_ptr(), recurse(5i64)) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
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
