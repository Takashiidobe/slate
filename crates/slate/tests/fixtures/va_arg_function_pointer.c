#include <stdarg.h>
#include <stdio.h>

typedef int (*Callback)(int);
static int twice(int x) { return 2 * x; }
static int invoke(int x, ...) {
    va_list ap;
    va_start(ap, x);
    Callback first = va_arg(ap, Callback);
    Callback second = va_arg(ap, Callback);
    va_end(ap);
    return first(x) + (second ? second(x + 1) : 3);
}
int main(void) {
    printf("%d %d\n", invoke(4, twice, twice), invoke(7, twice, (Callback)0));
    return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: extern "C-unwind" fn twice(x: i32) -> i32 {
// CHECK-NEXT:     2 * x
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C-unwind" fn invoke(x: i32, mut __va_args: ...) -> i32 {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     let first: Option<unsafe extern "C-unwind" fn(i32) -> i32> = unsafe {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 *const u8,
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(i32) -> i32>,
// CHECK-NEXT:             >(ap.next_arg::<*const u8>())
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let second: Option<unsafe extern "C-unwind" fn(i32) -> i32> = unsafe {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 *const u8,
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(i32) -> i32>,
// CHECK-NEXT:             >(ap.next_arg::<*const u8>())
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     {}
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: i32;
// CHECK-NEXT:     if second != None {
// CHECK-NEXT:         [[V_0]] = unsafe { second.unwrap()(x + 1) };
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_0]] = 3;
// CHECK-NEXT:     }
// CHECK-NEXT:     (unsafe { first.unwrap()(x) }) + [[V_0]]
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}", unsafe { invoke(4 as i32, unsafe { std::mem::transmute::< Option <
// CHECK-NEXT:         unsafe extern "C-unwind" fn (i32) -> i32 >, * const u8 > (Some(twice)) }, unsafe
// CHECK-NEXT:         { std::mem::transmute::< Option < unsafe extern "C-unwind" fn (i32) -> i32 >, *
// CHECK-NEXT:         const u8 > (Some(twice)) }) }, unsafe { invoke(7 as i32, unsafe {
// CHECK-NEXT:         std::mem::transmute::< Option < unsafe extern "C-unwind" fn (i32) -> i32 >, *
// CHECK-NEXT:         const u8 > (Some(twice)) }, unsafe { std::mem::transmute::< Option < unsafe
// CHECK-NEXT:         extern "C-unwind" fn (i32) -> i32 >, * const u8 > (None) }) }
// CHECK-NEXT:     );
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
