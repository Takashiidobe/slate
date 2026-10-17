#include <stdio.h>

static int calls;

static unsigned long long next(void) {
    calls++;
    return 0x100000000ULL;
}

int main(void) {
    volatile unsigned long long values[] = {1, 2, 3, 0x80000000ULL, 0x100000000ULL, 0x8000000000000000ULL, ~0ULL};
    for (unsigned i = 0; i < sizeof(values) / sizeof(values[0]); i++)
        printf("%d ", __builtin_clzll(values[i]));
    volatile unsigned int small = 1;
    volatile unsigned long wide = 1;
    printf("%d %d %d ", __builtin_clz(small), __builtin_clzl(wide), __builtin_clzll(next()));
    printf("%d\n", calls);
    return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut calls: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn next() -> u64 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         calls = (unsafe { calls }) + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     4294967296
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut values: __SlateAlign16<[u64; 7]> = __SlateAlign16([
// CHECK-NEXT:         1 as u64,
// CHECK-NEXT:         2 as u64,
// CHECK-NEXT:         3 as u64,
// CHECK-NEXT:         2147483648 as u64,
// CHECK-NEXT:         4294967296 as u64,
// CHECK-NEXT:         9223372036854775808 as u64,
// CHECK-NEXT:         !(0 as u64),
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut i: u32 = 0;
// CHECK-NEXT:     while (i as u64) < 56 / 8 {
// CHECK-NEXT:         let __slate_printf_arg0 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                 (values.0.as_mut_ptr() as *mut u64).offset(i as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         }
// CHECK-NEXT:             .leading_zeros() as i32;
// CHECK-NEXT:         print!("{} ", __slate_printf_arg0);
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         i = i.wrapping_add(1);
// CHECK-NEXT:     }
// CHECK-NEXT:     let small: u32 = 1;
// CHECK-NEXT:     let wide: u64 = 1;
// CHECK-NEXT:     print!(
// CHECK-NEXT:         "{} {} {} ", unsafe { std::ptr::read_volatile(std::ptr::addr_of!(small)) }
// CHECK-NEXT:         .leading_zeros() as i32, unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(std::ptr::addr_of!(wide)) } .leading_zeros() as i32,
// CHECK-NEXT:         next().leading_zeros() as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     println!("{}", unsafe { calls });
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
