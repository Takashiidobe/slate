#include <stdio.h>

long word = 41;
long arr[4] = {1, 2, 3, 4};

#define READ_STABLE(_var)                                             \
    ({                                                               \
        long __val;                                                  \
        __asm__("movq %%gs:%a[var], %[val]"                          \
                : [val] "=r"(__val)                                  \
                : [var] "i"(&(_var)));                                \
        __val;                                                       \
    })

static long read_offset(void) {
    long value;
    __asm__("movq %a1, %0" : "=r"(value) : "i"(&arr[2]));
    return value;
}

static long *address_of(void) {
    long *address;
    __asm__("leaq %a1, %0" : "=r"(address) : "i"(&arr[1]));
    return address;
}

static long immediate(void) {
    long value;
    __asm__("leaq %a1, %0" : "=r"(value) : "i"(8));
    return value;
}

static long through_register(long *pointer) {
    long value;
    __asm__("movq %a1, %0" : "=r"(value) : "r"(pointer));
    return value;
}

int main(void) {
    printf("%ld %ld\n", READ_STABLE(word), READ_STABLE(arr[3]));
    printf("%ld %ld\n", read_offset(), *address_of());
    printf("%ld %ld\n", immediate(), through_register(&arr[0]));
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: static mut word: i64 = 41;
// CHECK-NEXT: static mut arr: __SlateAlign16<[i64; 4]> = __SlateAlign16([1i64, 2i64, 3i64, 4i64]);
// CHECK-NEXT: fn read_offset() -> i64 {
// CHECK-NEXT:     let mut value: i64 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "movq {1}+16(%rip), {0:r}", lateout(reg) [[T_0]], sym arr, options(pure, nomem,
// CHECK-NEXT:             nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         value = unsafe { std::mem::transmute::<u64, i64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     value
// CHECK-NEXT: }
// CHECK-NEXT: fn address_of() -> *mut i64 {
// CHECK-NEXT:     let mut address: *mut i64 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "leaq {1}+8(%rip), {0:r}", lateout(reg) [[T_0]], sym arr, options(pure, nomem,
// CHECK-NEXT:             nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         address = unsafe { std::mem::transmute::<u64, *mut i64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     address
// CHECK-NEXT: }
// CHECK-NEXT: fn immediate() -> i64 {
// CHECK-NEXT:     let mut value: i64 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "leaq {1}, {0:r}", lateout(reg) [[T_0]], const 8 as i32, options(pure, nomem,
// CHECK-NEXT:             nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         value = unsafe { std::mem::transmute::<u64, i64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     value
// CHECK-NEXT: }
// CHECK-NEXT: fn through_register(mut pointer: *mut i64) -> i64 {
// CHECK-NEXT:     let mut value: i64 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "movq ({1:r}), {0:r}", lateout(reg) [[T_0]], in (reg) pointer as u64,
// CHECK-NEXT:             options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         value = unsafe { std::mem::transmute::<u64, i64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     value
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: i64;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __val: i64 = 0;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             let [[T_0]]: u64;
// CHECK-NEXT:             core::arch::asm!(
// CHECK-NEXT:                 "movq %gs:{1}(%rip), {0:r}", lateout(reg) [[T_0]], sym word, options(pure,
// CHECK-NEXT:                 nomem, nostack, att_syntax)
// CHECK-NEXT:             );
// CHECK-NEXT:             __val = unsafe { std::mem::transmute::<u64, i64>([[T_0]] as u64) };
// CHECK-NEXT:         }
// CHECK-NEXT:         [[V_0]] = __val;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i64;
// CHECK-NEXT:     let mut __val: i64 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_1:__t[0-9]+]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "movq %gs:{1}+24(%rip), {0:r}", lateout(reg) [[T_1]], sym arr, options(pure,
// CHECK-NEXT:             nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         __val = unsafe { std::mem::transmute::<u64, i64>([[T_1]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     [[V_1]] = __val;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%ld %ld\n".as_ptr(), [[V_0]], [[V_1]]) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(c"%ld %ld\n".as_ptr(), read_offset(), *address_of())
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%ld %ld\n".as_ptr(),
// CHECK-NEXT:             immediate(),
// CHECK-NEXT:             through_register((std::ptr::addr_of_mut!(arr.0) as *mut i64).add(0)),
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
