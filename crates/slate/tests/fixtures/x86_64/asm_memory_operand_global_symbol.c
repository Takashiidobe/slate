/* { dg-options "-fPIE" } */
#include <stdio.h>

struct ops {
    long pad;
    long val;
    long arr[4];
};

struct ops ops = {1, 41, {0, 0, 7, 0}};
static struct ops local_ops = {2, 5, {0}};

static long load_field(void) {
    long v;
    __asm__ volatile("movq %1, %0" : "=r"(v) : "m"(ops.val));
    return v;
}

static long load_element(void) {
    long v;
    __asm__ volatile("movq %1, %0" : "=r"(v) : "m"(ops.arr[2]));
    return v;
}

static void bump_local(void) {
    __asm__ volatile("addq $1, %0" : "+m"(local_ops.val));
}

static void store_local(long v) {
    __asm__ volatile("movq %1, %0" : "=m"(local_ops.arr[3]) : "r"(v));
}

static long load_pointee(const long *p) {
    long v;
    __asm__ volatile("movq %1, %0" : "=r"(v) : "m"(*p));
    return v;
}

int main(void) {
    bump_local();
    bump_local();
    store_local(9);
    printf("%ld %ld %ld %ld %ld\n", load_field(), load_element(), local_ops.val,
           local_ops.arr[3], load_pointee(&ops.pad));
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-fPIE" }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct ops {
// CHECK-NEXT:     pad: i64,
// CHECK-NEXT:     val: i64,
// CHECK-NEXT:     arr: [i64; 4],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut ops: ops = ops {
// CHECK-NEXT:     pad: 1i64,
// CHECK-NEXT:     val: 41i64,
// CHECK-NEXT:     arr: [0i64, 0i64, 7i64, 0i64],
// CHECK-NEXT: };
// CHECK-EMPTY:
// CHECK-NEXT: static mut local_ops: ops = ops {
// CHECK-NEXT:     pad: 2i64,
// CHECK-NEXT:     val: 5i64,
// CHECK-NEXT:     arr: {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: [i64; 4] = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         [[T_0]][0] = 0i64;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     },
// CHECK-NEXT: };
// CHECK-EMPTY:
// CHECK-NEXT: fn load_field() -> i64 {
// CHECK-NEXT:     let mut v: i64 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "movq {1}+8(%rip), {0:r}",
// CHECK-NEXT:             lateout(reg) [[T_0]],
// CHECK-NEXT:             sym ops,
// CHECK-NEXT:             options(nostack, att_syntax),
// CHECK-NEXT:         );
// CHECK-NEXT:         v = unsafe { core::mem::transmute::<u64, i64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     v
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn load_element() -> i64 {
// CHECK-NEXT:     let mut v: i64 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "movq {1}+32(%rip), {0:r}",
// CHECK-NEXT:             lateout(reg) [[T_0]],
// CHECK-NEXT:             sym ops,
// CHECK-NEXT:             options(nostack, att_syntax),
// CHECK-NEXT:         );
// CHECK-NEXT:         v = unsafe { core::mem::transmute::<u64, i64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     v
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bump_local() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "addq $1, {0}+8(%rip)",
// CHECK-NEXT:             sym local_ops,
// CHECK-NEXT:             options(nostack, att_syntax),
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn store_local(mut v: i64) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "movq {1:r}, {0}+40(%rip)",
// CHECK-NEXT:             sym local_ops,
// CHECK-NEXT:             in(reg) v as u64,
// CHECK-NEXT:             options(nostack, att_syntax),
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn load_pointee(mut p: *const i64) -> i64 {
// CHECK-NEXT:     let mut v: i64 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "movq ({1:r}), {0:r}",
// CHECK-NEXT:             lateout(reg) [[T_0]],
// CHECK-NEXT:             in(reg) p,
// CHECK-NEXT:             options(nostack, att_syntax),
// CHECK-NEXT:         );
// CHECK-NEXT:         v = unsafe { core::mem::transmute::<u64, i64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     v
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     bump_local();
// CHECK-NEXT:     bump_local();
// CHECK-NEXT:     store_local(9i64);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%ld %ld %ld %ld %ld\n".as_ptr(),
// CHECK-NEXT:             load_field(),
// CHECK-NEXT:             load_element(),
// CHECK-NEXT:             local_ops.val,
// CHECK-NEXT:             local_ops.arr[3],
// CHECK-NEXT:             load_pointee(core::ptr::addr_of_mut!(ops.pad) as *const i64),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
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
