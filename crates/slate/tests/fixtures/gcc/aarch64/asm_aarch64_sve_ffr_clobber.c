#include <stdio.h>

__attribute__((target("+sve"))) static int all_lanes_faulted_in(void) {
    long active;
    long total;
    __asm__ volatile("setffr\n\t"
                     "rdffr p2.b\n\t"
                     "ptrue p4.b\n\t"
                     "cntp %0, p4, p2.b\n\t"
                     "cntp %1, p4, p4.b"
                     : "=r"(active), "=r"(total)
                     :
                     : "p2", "p4", "ffr");
    return active == total;
}

int main(void) {
    printf("%d\n", all_lanes_faulted_in());
    return 0;
}

// SLATE-FILECHECK-BEGIN aarch64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const u8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sve")]
// CHECK-NEXT: unsafe fn all_lanes_faulted_in() -> i32 {
// CHECK-NEXT:     let mut active: i64 = 0;
// CHECK-NEXT:     let mut total: i64 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u64;
// CHECK-NEXT:         let [[T_1:__t[0-9]+]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "setffr\n\trdffr p2.b\n\tptrue p4.b\n\tcntp {0:x}, p4, p2.b\n\tcntp {1:x}, p4, p4.b",
// CHECK-NEXT:             lateout(reg) [[T_0]], lateout(reg) [[T_1]], lateout("p2") _, lateout("p4") _,
// CHECK-NEXT:             lateout("ffr") _, options(nomem, nostack, preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         active = unsafe { std::mem::transmute::<u64, i64>([[T_0]] as u64) };
// CHECK-NEXT:         total = unsafe { std::mem::transmute::<u64, i64>([[T_1]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     (active == total) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{}", unsafe { all_lanes_faulted_in() });
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
