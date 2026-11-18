/* { dg-options "-masm=intel" } */
#include <stdio.h>

long word = 41;
long arr[4] = {1, 2, 3, 4};

static long read_word(void) {
    long v;
    __asm__("mov %0, QWORD PTR %a1" : "=r"(v) : "i"(&word));
    return v;
}

static long read_third(void) {
    long v;
    __asm__("mov %0, QWORD PTR %a1" : "=r"(v) : "i"(&arr[2]));
    return v;
}

static long *address_of_last(void) {
    long *p;
    __asm__("lea %0, %a1" : "=r"(p) : "i"(&arr[3]));
    return p;
}

static long immediate_address(void) {
    long v;
    __asm__("lea %0, %a1" : "=r"(v) : "i"(8));
    return v;
}

static long through_register(long *q) {
    long v;
    __asm__("mov %0, %a1" : "=r"(v) : "r"(q));
    return v;
}

int main(void) {
    printf("%ld %ld %d %ld %ld\n", read_word(), read_third(), address_of_last() == &arr[3],
           immediate_address(), through_register(&arr[1]));
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-masm=intel" }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe impl<T> Sync for __SlateAlign16<T> {}
// CHECK-EMPTY:
// CHECK-NEXT: static mut word: i64 = 41;
// CHECK-EMPTY:
// CHECK-NEXT: static mut arr: __SlateAlign16<[i64; 4]> = __SlateAlign16([1i64, 2i64, 3i64, 4i64]);
// CHECK-EMPTY:
// CHECK-NEXT: fn read_word() -> i64 {
// CHECK-NEXT:     let mut v: i64 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u64;
// CHECK-NEXT:         core::arch::asm!("mov {0:r}, QWORD PTR [rip + {1}]", lateout(reg) [[T_0]], sym word, options(pure, nomem, nostack));
// CHECK-NEXT:         v = unsafe { core::mem::transmute::<u64, i64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     v
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn read_third() -> i64 {
// CHECK-NEXT:     let mut v: i64 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u64;
// CHECK-NEXT:         core::arch::asm!("mov {0:r}, QWORD PTR [rip + {1}+16]", lateout(reg) [[T_0]], sym arr, options(pure, nomem, nostack));
// CHECK-NEXT:         v = unsafe { core::mem::transmute::<u64, i64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     v
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn address_of_last() -> *mut i64 {
// CHECK-NEXT:     let mut p: *mut i64 = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u64;
// CHECK-NEXT:         core::arch::asm!("lea {0:r}, [rip + {1}+24]", lateout(reg) [[T_0]], sym arr, options(pure, nomem, nostack));
// CHECK-NEXT:         p = unsafe { core::mem::transmute::<u64, *mut i64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     p
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn immediate_address() -> i64 {
// CHECK-NEXT:     let mut v: i64 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u64;
// CHECK-NEXT:         core::arch::asm!("lea {0:r}, [{1}]", lateout(reg) [[T_0]], const 8 as i32, options(pure, nomem, nostack));
// CHECK-NEXT:         v = unsafe { core::mem::transmute::<u64, i64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     v
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn through_register(mut q: *mut i64) -> i64 {
// CHECK-NEXT:     let mut v: i64 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u64;
// CHECK-NEXT:         core::arch::asm!("mov {0:r}, [{1:r}]", lateout(reg) [[T_0]], in(reg) q as u64, options(pure, nomem, nostack));
// CHECK-NEXT:         v = unsafe { core::mem::transmute::<u64, i64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     v
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: i64 =
// CHECK-NEXT:         through_register(unsafe { (core::ptr::addr_of_mut!(arr.0) as *mut i64).add(1) });
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: i64 = immediate_address();
// CHECK-NEXT:     let [[V_2:__v[0-9]+]]: i32 = (address_of_last()
// CHECK-NEXT:         == unsafe { (core::ptr::addr_of_mut!(arr.0) as *mut i64).add(3) })
// CHECK-NEXT:         as i32;
// CHECK-NEXT:     let [[V_3:__v[0-9]+]]: i64 = read_third();
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%ld %ld %d %ld %ld\n".as_ptr(),
// CHECK-NEXT:             read_word(),
// CHECK-NEXT:             [[V_3]],
// CHECK-NEXT:             [[V_2]],
// CHECK-NEXT:             [[V_1]],
// CHECK-NEXT:             [[V_0]],
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
