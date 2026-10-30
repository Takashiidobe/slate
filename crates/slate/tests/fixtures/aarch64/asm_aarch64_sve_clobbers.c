#include <stdio.h>

__attribute__((target("+sve"))) static void add_arrays(long *out, const long *left,
                                                        const long *right, long count) {
    __asm__ volatile("mov x9, #0\n"
                     "1:\n\t"
                     "whilelo p1.d, x9, %3\n\t"
                     "b.none 2f\n\t"
                     "ld1d z1.d, p1/z, [%1, x9, lsl #3]\n\t"
                     "ld1d z2.d, p1/z, [%2, x9, lsl #3]\n\t"
                     "add z1.d, z1.d, z2.d\n\t"
                     "st1d z1.d, p1, [%0, x9, lsl #3]\n\t"
                     "incd x9\n\t"
                     "b 1b\n"
                     "2:"
                     :
                     : "r"(out), "r"(left), "r"(right), "r"(count)
                     : "x9", "p1", "z1", "z2", "memory", "cc");
}

__attribute__((target("+sve"))) static int lanes_positive(void) {
    long lanes;
    __asm__ volatile("ptrue p3.d\n\tcntp %0, p3, p3.d" : "=r"(lanes) : : "p3");
    return lanes > 0;
}

__attribute__((target("+sve"))) static double keep_live(double value) {
    double scaled = value * 2.0;
    __asm__ volatile("mov z0.d, #0\n\tmov z31.d, #0" : : : "z0", "z31");
    return scaled + value;
}

int main(void) {
    long left[11];
    long right[11];
    long out[11];
    for (long index = 0; index < 11; index++) {
        left[index] = index;
        right[index] = 100 * index;
        out[index] = -1;
    }
    add_arrays(out, left, right, 11);
    for (long index = 0; index < 11; index++) {
        printf("%ld ", out[index]);
    }
    printf("\n%d\n", lanes_positive());
    printf("%g\n", keep_live(1.5));
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
// CHECK-NEXT: #[target_feature(enable = "sve")]
// CHECK-NEXT: unsafe fn add_arrays(
// CHECK-NEXT:     mut out: *mut i64,
// CHECK-NEXT:     mut left: *const i64,
// CHECK-NEXT:     mut right: *const i64,
// CHECK-NEXT:     mut count: i64,
// CHECK-NEXT: ) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "mov x9, #0\n1:\n\twhilelo p1.d, x9, {3:x}\n\tb.none 2f\n\tld1d z1.d, p1/z, [{1:x}, x9, lsl #3]\n\tld1d z2.d, p1/z, [{2:x}, x9, lsl #3]\n\tadd z1.d, z1.d, z2.d\n\tst1d z1.d, p1, [{0:x}, x9, lsl #3]\n\tincd x9\n\tb 1b\n2:",
// CHECK-NEXT:             in (reg) out as u64, in (reg) left as u64, in (reg) right as u64, in (reg)
// CHECK-NEXT:             count as u64, lateout("x9") _, lateout("p1") _, lateout("v1") _,
// CHECK-NEXT:             lateout("v2") _, options(nostack)
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sve")]
// CHECK-NEXT: unsafe fn lanes_positive() -> i32 {
// CHECK-NEXT:     let mut lanes: i64 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "ptrue p3.d\n\tcntp {0:x}, p3, p3.d", lateout(reg) [[T_0]], lateout("p3") _,
// CHECK-NEXT:             options(nostack, preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         lanes = unsafe { std::mem::transmute::<u64, i64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     (lanes > 0) as i32
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sve")]
// CHECK-NEXT: unsafe fn keep_live(value: f64) -> f64 {
// CHECK-NEXT:     let scaled: f64 = value * 2.0f64;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "mov z0.d, #0\n\tmov z31.d, #0", lateout("v0") _, lateout("v31") _,
// CHECK-NEXT:             options(nostack, preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     scaled + value
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut left: [i64; 11] = [0i64; 11];
// CHECK-NEXT:     let mut right: [i64; 11] = [0i64; 11];
// CHECK-NEXT:     let mut out: [i64; 11] = [0i64; 11];
// CHECK-NEXT:     for index in 0i64..11i64 {
// CHECK-NEXT:         left[index as usize] = index;
// CHECK-NEXT:         right[index as usize] = 100 * index;
// CHECK-NEXT:         out[index as usize] = -1;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         add_arrays(
// CHECK-NEXT:             out.as_mut_ptr() as *mut i64,
// CHECK-NEXT:             left.as_mut_ptr() as *const i64,
// CHECK-NEXT:             right.as_mut_ptr() as *const i64,
// CHECK-NEXT:             11i64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     for index in 0i64..11i64 {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { printf(c"%ld ".as_ptr(), out[index as usize]) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("\n{}", unsafe { lanes_positive() });
// CHECK-NEXT:     unsafe { printf(c"%g\n".as_ptr(), keep_live(1.5f64)) };
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
