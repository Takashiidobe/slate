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

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn twice(x: i32) -> i32 {
// CHECK-NEXT:     2 * x
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn invoke(x: i32, mut __va_args: ...) -> i32 {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'static> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     ap = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::ffi::VaList<'_>, core::ffi::VaList<'static>>(
// CHECK-NEXT:             __va_args.clone(),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let first: Option<unsafe extern "C-unwind" fn(i32) -> i32> = unsafe {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::mem::transmute::<*const u8, Option<unsafe extern "C-unwind" fn(i32) -> i32>>(
// CHECK-NEXT:                 ap.next_arg::<*const u8>(),
// CHECK-NEXT:             )
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let second: Option<unsafe extern "C-unwind" fn(i32) -> i32> = unsafe {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::mem::transmute::<*const u8, Option<unsafe extern "C-unwind" fn(i32) -> i32>>(
// CHECK-NEXT:                 ap.next_arg::<*const u8>(),
// CHECK-NEXT:             )
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: i32 = unsafe { first.unwrap()(x) };
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: i32 = if second != None {
// CHECK-NEXT:         unsafe { second.unwrap()(x + 1) }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         3
// CHECK-NEXT:     };
// CHECK-NEXT:     [[V_0]] + [[V_1]]
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}",
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             invoke(
// CHECK-NEXT:                 4i32,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     core::mem::transmute::<Option<unsafe extern "C-unwind" fn(i32) -> i32>, *const u8>(
// CHECK-NEXT:                         Some(twice),
// CHECK-NEXT:                     )
// CHECK-NEXT:                 },
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     core::mem::transmute::<Option<unsafe extern "C-unwind" fn(i32) -> i32>, *const u8>(
// CHECK-NEXT:                         Some(twice),
// CHECK-NEXT:                     )
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         },
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             invoke(
// CHECK-NEXT:                 7i32,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     core::mem::transmute::<Option<unsafe extern "C-unwind" fn(i32) -> i32>, *const u8>(
// CHECK-NEXT:                         Some(twice),
// CHECK-NEXT:                     )
// CHECK-NEXT:                 },
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     core::mem::transmute::<Option<unsafe extern "C-unwind" fn(i32) -> i32>, *const u8>(
// CHECK-NEXT:                         None,
// CHECK-NEXT:                     )
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         }
// CHECK-NEXT:     );
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
