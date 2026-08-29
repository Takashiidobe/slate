#include <stdio.h>

extern __thread int counter;
__thread int counter __attribute__((tls_model("global-dynamic"))) = 5;

#pragma GCC visibility push(hidden)
int f$oo(void) { return 1; }
#pragma GCC visibility pop

static const char *pretty(void) { return __PRETTY_FUNCTION__; }

int main(void) {
  if (0)
    goto done;
  printf("%d\n", counter);
  printf("%d\n", f$oo());
  printf("%s\n", pretty());
  printf("%d\n", (int)sizeof(void));
done:
  __attribute__((unused));
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(thread_local)]
// LOWERING-NEXT: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[thread_local]
// LOWERING-NEXT: static mut counter: i32 = 5;
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn f_oo() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 1;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: i32 = __retval;
// LOWERING-NEXT:     return _v1;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn pretty() -> *mut i8 {
// LOWERING-NEXT:     let mut __retval: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let _v0: *mut i8 = b"const char *pretty(void)\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = __retval;
// LOWERING-NEXT:     return _v1;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut __state0: i32 = 0;
// LOWERING-NEXT:     '__dispatch0: loop {
// LOWERING-NEXT:         match __state0 {
// LOWERING-NEXT:             0 => {
// LOWERING-NEXT:                 let _v0: i32 = 0;
// LOWERING-NEXT:                 __retval = _v0;
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let _v1: i32 = 0;
// LOWERING-NEXT:                     let _v2: bool = _v1 != 0;
// LOWERING-NEXT:                     if _v2 {
// LOWERING-NEXT:                         __state0 = 1;
// LOWERING-NEXT:                         continue '__dispatch0;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 let _v3: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:                 let _v4: i32 = unsafe { counter };
// LOWERING-NEXT:                 let _v5: i32 = unsafe { printf(_v3 as *const i8, _v4) };
// LOWERING-NEXT:                 let _v6: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:                 let _v7: i32 = f_oo();
// LOWERING-NEXT:                 let _v8: i32 = unsafe { printf(_v6 as *const i8, _v7) };
// LOWERING-NEXT:                 let _v9: *mut i8 = b"%s\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:                 let _v10: *mut i8 = pretty();
// LOWERING-NEXT:                 let _v11: i32 = unsafe { printf(_v9 as *const i8, _v10) };
// LOWERING-NEXT:                 let _v12: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:                 let _v13: i32 = 1;
// LOWERING-NEXT:                 let _v14: i32 = unsafe { printf(_v12 as *const i8, _v13) };
// LOWERING-NEXT:                 __state0 = 1;
// LOWERING-NEXT:                 continue '__dispatch0;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             1 => {
// LOWERING-NEXT:                 let _v15: i32 = 0;
// LOWERING-NEXT:                 __retval = _v15;
// LOWERING-NEXT:                 let _v16: i32 = __retval;
// LOWERING-NEXT:                 std::process::exit(_v16 as i32);
// LOWERING-NEXT:             }
// LOWERING-NEXT:             _ => {
// LOWERING-NEXT:                 break '__dispatch0;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(thread_local)]
// REWRITES-NEXT: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[thread_local]
// REWRITES-NEXT: static mut counter: i32 = 5;
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn f_oo() -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 1;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn pretty() -> *mut i8 {
// REWRITES-NEXT: let mut __retval: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT: __retval = b"const char *pretty(void)\0".as_ptr() as *mut i8;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut __state0: i32 = 0;
// REWRITES-NEXT: '__dispatch0: loop {
// REWRITES-NEXT:         match __state0 {
// REWRITES-NEXT:             0 => {
// REWRITES-NEXT:                         __retval = 0;
// REWRITES-NEXT:                         {
// REWRITES-NEXT:                                         let _v1: i32 = 0;
// REWRITES-NEXT:                                         let _v2: bool = _v1 != 0;
// REWRITES-NEXT:                                         if _v2 {
// REWRITES-NEXT:                                                             __state0 = 1;
// REWRITES-NEXT:                                                             continue '__dispatch0;
// REWRITES-NEXT:                                         }
// REWRITES-NEXT:                         }
// REWRITES-NEXT:                         let _v3: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT:                         let _v5: i32 = unsafe { printf(_v3 as *const i8, unsafe { counter }) };
// REWRITES-NEXT:                         let _v6: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT:                         let _v7: i32 = f_oo();
// REWRITES-NEXT:                         let _v8: i32 = unsafe { printf(_v6 as *const i8, _v7) };
// REWRITES-NEXT:                         let _v9: *mut i8 = b"%s\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT:                         let _v10: *mut i8 = pretty();
// REWRITES-NEXT:                         let _v11: i32 = unsafe { printf(_v9 as *const i8, _v10) };
// REWRITES-NEXT:                         let _v12: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT:                         let _v13: i32 = 1;
// REWRITES-NEXT:                         let _v14: i32 = unsafe { printf(_v12 as *const i8, _v13) };
// REWRITES-NEXT:                         __state0 = 1;
// REWRITES-NEXT:                         continue '__dispatch0;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             1 => {
// REWRITES-NEXT:                         __retval = 0;
// REWRITES-NEXT:                         std::process::exit(__retval as i32);
// REWRITES-NEXT:             }
// REWRITES-NEXT:             _ => {
// REWRITES-NEXT:                         break '__dispatch0;
// REWRITES-NEXT:             }
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
