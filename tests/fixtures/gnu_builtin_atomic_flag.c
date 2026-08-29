#include <stdbool.h>
#include <stdio.h>

int main(void) {
  bool flag   = false;
  int  first  = __atomic_test_and_set(&flag, __ATOMIC_SEQ_CST);
  int  second = __atomic_test_and_set(&flag, __ATOMIC_SEQ_CST);
  __atomic_clear(&flag, __ATOMIC_SEQ_CST);
  printf("%d %d %d\n", first, second, !flag);
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut flag: bool = false;
// LOWERING-NEXT:     let mut first: i32 = 0;
// LOWERING-NEXT:     let mut second: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: bool = false;
// LOWERING-NEXT:     flag = _v1;
// LOWERING-NEXT:     let _v2: *mut i8 = std::ptr::addr_of_mut!(flag) as *mut i8;
// LOWERING-NEXT:     let _v3: bool = (unsafe { std::sync::atomic::AtomicI8::from_ptr(_v2).swap(1, std::sync::atomic::Ordering::SeqCst) }) != 0;
// LOWERING-NEXT:     let _v4: i32 = _v3 as i32;
// LOWERING-NEXT:     first = _v4;
// LOWERING-NEXT:     let _v5: *mut i8 = std::ptr::addr_of_mut!(flag) as *mut i8;
// LOWERING-NEXT:     let _v6: bool = (unsafe { std::sync::atomic::AtomicI8::from_ptr(_v5).swap(1, std::sync::atomic::Ordering::SeqCst) }) != 0;
// LOWERING-NEXT:     let _v7: i32 = _v6 as i32;
// LOWERING-NEXT:     second = _v7;
// LOWERING-NEXT:     let _v8: *mut i8 = std::ptr::addr_of_mut!(flag) as *mut i8;
// LOWERING-NEXT:     unsafe { std::sync::atomic::AtomicI8::from_ptr(_v8).store(0, std::sync::atomic::Ordering::SeqCst) };
// LOWERING-NEXT:     let _v9: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v10: i32 = first;
// LOWERING-NEXT:     let _v11: i32 = second;
// LOWERING-NEXT:     let _v12: bool = flag;
// LOWERING-NEXT:     let _v13: bool = !_v12;
// LOWERING-NEXT:     let _v14: i32 = _v13 as i32;
// LOWERING-NEXT:     let _v15: i32 = unsafe { printf(_v9 as *const i8, _v10, _v11, _v14) };
// LOWERING-NEXT:     let _v16: i32 = 0;
// LOWERING-NEXT:     __retval = _v16;
// LOWERING-NEXT:     let _v17: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v17 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut flag: bool = false;
// REWRITES-NEXT: let mut first: i32 = 0;
// REWRITES-NEXT: let mut second: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: flag = false;
// REWRITES-NEXT: let _v2: *mut i8 = std::ptr::addr_of_mut!(flag) as *mut i8;
// REWRITES-NEXT: let _v3: bool = (unsafe { std::sync::atomic::AtomicI8::from_ptr(_v2).swap(1, std::sync::atomic::Ordering::SeqCst) }) != 0;
// REWRITES-NEXT: first = _v3 as i32;
// REWRITES-NEXT: let _v5: *mut i8 = std::ptr::addr_of_mut!(flag) as *mut i8;
// REWRITES-NEXT: let _v6: bool = (unsafe { std::sync::atomic::AtomicI8::from_ptr(_v5).swap(1, std::sync::atomic::Ordering::SeqCst) }) != 0;
// REWRITES-NEXT: second = _v6 as i32;
// REWRITES-NEXT: let _v8: *mut i8 = std::ptr::addr_of_mut!(flag) as *mut i8;
// REWRITES-NEXT: unsafe { std::sync::atomic::AtomicI8::from_ptr(_v8).store(0, std::sync::atomic::Ordering::SeqCst) };
// REWRITES-NEXT: let _v9: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v10: i32 = first;
// REWRITES-NEXT: let _v11: i32 = second;
// REWRITES-NEXT: let _v13: bool = !flag;
// REWRITES-NEXT: let _v15: i32 = unsafe { printf(_v9 as *const i8, _v10, _v11, _v13 as i32) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
