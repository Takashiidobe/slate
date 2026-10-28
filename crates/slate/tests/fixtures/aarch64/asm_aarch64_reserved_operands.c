#include <stdio.h>

static long bump_x19(long value) {
    register long reserved __asm__("x19") = value;
    __asm__("add %0, %0, #1" : "+r"(reserved));
    return reserved;
}

static long read_x19(long value, long other) {
    register long reserved __asm__("x19") = value;
    long result;
    __asm__("add %0, %1, %2" : "=r"(result) : "r"(reserved), "r"(other));
    return result;
}

static long write_x19(void) {
    register long reserved __asm__("x19");
    __asm__("mov %0, #7" : "=r"(reserved));
    return reserved;
}

static int narrow_w19(int value) {
    register int reserved __asm__("w19") = value;
    __asm__("add %w0, %w0, #3" : "+r"(reserved));
    return reserved;
}

static long bump_x29(long value) {
    register long frame __asm__("x29") = value;
    __asm__("add %0, %0, #1" : "+r"(frame));
    return frame;
}

static long both(long left, long right) {
    register long low __asm__("x19") = left;
    register long high __asm__("x29") = right;
    long result;
    __asm__("add %0, %1, %2" : "=&r"(result) : "r"(low), "r"(high));
    return result;
}

int main(void) {
    printf("%ld %ld %ld\n", bump_x19(41), read_x19(40, 2), write_x19());
    printf("%d %ld %ld\n", narrow_w19(39), bump_x29(41), both(20, 22));
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
// CHECK-NEXT: fn bump_x19(mut reserved: i64) -> i64 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "mov {1:x}, x19\nmov x19, {0:x}\nadd x19, x19, #1\nmov {0:x}, x19\nmov x19, {1:x}",
// CHECK-NEXT:             inout(reg) reserved as u64 => [[T_0]], out(reg) _, options(pure, nomem, nostack,
// CHECK-NEXT:             preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         reserved = unsafe { std::mem::transmute::<u64, i64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     reserved
// CHECK-NEXT: }
// CHECK-NEXT: fn read_x19(mut reserved: i64, mut other: i64) -> i64 {
// CHECK-NEXT:     let mut result: i64 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "mov {3:x}, x19\nmov x19, {1:x}\nadd {0:x}, x19, {2:x}\nmov x19, {3:x}",
// CHECK-NEXT:             lateout(reg) [[T_0]], in (reg) reserved as u64, in (reg) other as u64, out(reg)
// CHECK-NEXT:             _, options(pure, nomem, nostack, preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         result = unsafe { std::mem::transmute::<u64, i64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     result
// CHECK-NEXT: }
// CHECK-NEXT: fn write_x19() -> i64 {
// CHECK-NEXT:     let mut reserved: i64 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "mov {1:x}, x19\nmov x19, #7\nmov {0:x}, x19\nmov x19, {1:x}", out(reg) [[T_0]],
// CHECK-NEXT:             out(reg) _, options(pure, nomem, nostack, preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         reserved = unsafe { std::mem::transmute::<u64, i64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     reserved
// CHECK-NEXT: }
// CHECK-NEXT: fn narrow_w19(mut reserved: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "mov {1:x}, x19\nmov x19, {0:x}\nadd w19, w19, #3\nmov {0:x}, x19\nmov x19, {1:x}",
// CHECK-NEXT:             inout(reg) reserved as u32 => [[T_0]], out(reg) _, options(pure, nomem, nostack,
// CHECK-NEXT:             preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         reserved = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     reserved
// CHECK-NEXT: }
// CHECK-NEXT: fn bump_x29(mut frame: i64) -> i64 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "mov {1:x}, x29\nmov x29, {0:x}\nadd x29, x29, #1\nmov {0:x}, x29\nmov x29, {1:x}",
// CHECK-NEXT:             inout(reg) frame as u64 => [[T_0]], out(reg) _, options(pure, nomem, nostack,
// CHECK-NEXT:             preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         frame = unsafe { std::mem::transmute::<u64, i64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     frame
// CHECK-NEXT: }
// CHECK-NEXT: fn both(mut low: i64, mut high: i64) -> i64 {
// CHECK-NEXT:     let mut result: i64 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "mov {4:x}, x29\nmov x29, {2:x}\nmov {3:x}, x19\nmov x19, {1:x}\nadd {0:x}, x19, x29\nmov x19, {3:x}\nmov x29, {4:x}",
// CHECK-NEXT:             out(reg) [[T_0]], in (reg) low as u64, in (reg) high as u64, out(reg) _,
// CHECK-NEXT:             out(reg) _, options(pure, nomem, nostack, preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         result = unsafe { std::mem::transmute::<u64, i64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     result
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%ld %ld %ld\n".as_ptr(),
// CHECK-NEXT:             bump_x19(41i64),
// CHECK-NEXT:             read_x19(40i64, 2i64),
// CHECK-NEXT:             write_x19(),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %ld %ld\n".as_ptr(),
// CHECK-NEXT:             narrow_w19(39i32),
// CHECK-NEXT:             bump_x29(41i64),
// CHECK-NEXT:             both(20i64, 22i64),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
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
