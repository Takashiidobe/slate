#include <stdio.h>

static long wide = 7;
static int narrow = 9;
static long pair[2] = {3, 4};

static void store_long(long *target, long value) {
    __asm__ volatile("str %x1, %0" : "=m"(*target) : "rZ"(value));
}

int main(void) {
    __asm__ volatile("str %x1, %0" : "=m"(wide) : "rZ"(0L));
    __asm__ volatile("str %w1, %0" : "=m"(narrow) : "rZ"(0));
    printf("%ld %d\n", wide, narrow);
    __asm__ volatile("stp %x1, %x2, %0" : "=Q"(pair) : "rZ"(0L), "rZ"(11L));
    printf("%ld %ld\n", pair[0], pair[1]);
    long sum;
    __asm__("add %0, %1, %x2" : "=r"(sum) : "r"(5L), "i"(0));
    printf("%ld\n", sum);
    int moved;
    __asm__("mov %w0, %w1" : "=r"(moved) : "rZ"(0));
    printf("%d\n", moved);
    store_long(&wide, 42);
    printf("%ld\n", wide);
    return 0;
}

// SLATE-FILECHECK-BEGIN aarch64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const u8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: static mut wide: i64 = 7;
// CHECK-NEXT: static mut narrow: i32 = 9;
// CHECK-NEXT: static mut pair: [i64; 2] = [3, 4];
// CHECK-NEXT: fn store_long(mut target: *mut i64, mut value: i64) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "str {1:x}, [{0:x}]", in (reg) target, in (reg) value as u64,
// CHECK-NEXT:             options(nostack, preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "str xzr, [{0:x}]", in (reg) std::ptr::addr_of_mut!(wide), options(nostack,
// CHECK-NEXT:             preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "str wzr, [{0:x}]", in (reg) std::ptr::addr_of_mut!(narrow), options(nostack,
// CHECK-NEXT:             preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%ld %d\n".as_ptr(), wide, narrow) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "stp xzr, {1:x}, [{0:x}]", in (reg) std::ptr::addr_of_mut!(pair), in (reg)
// CHECK-NEXT:             (11 as i64) as u64, options(nostack, preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%ld %ld\n".as_ptr(), pair[0], pair[1]) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut sum: i64 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "add {0:x}, {1:x}, xzr", lateout(reg) [[T_0]], in (reg) (5 as i64) as u64,
// CHECK-NEXT:             options(pure, nomem, nostack, preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         sum = unsafe { std::mem::transmute::<u64, i64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%ld\n".as_ptr(), sum) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut moved: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_1:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "mov {0:w}, wzr", lateout(reg) [[T_1]], options(pure, nomem, nostack,
// CHECK-NEXT:             preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         moved = unsafe { std::mem::transmute::<u32, i32>([[T_1]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("{}", moved);
// CHECK-NEXT:     store_long(unsafe { std::ptr::addr_of_mut!(wide) }, 42i64);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%ld\n".as_ptr(), wide) };
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
