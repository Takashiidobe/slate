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
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut flag: bool = false;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:     flag = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = std::ptr::addr_of_mut!(flag) as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = (unsafe { std::sync::atomic::AtomicI8::from_ptr({{_v[0-9]+}}).swap(1, std::sync::atomic::Ordering::SeqCst) }) != 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = std::ptr::addr_of_mut!(flag) as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = (unsafe { std::sync::atomic::AtomicI8::from_ptr({{_v[0-9]+}}).swap(1, std::sync::atomic::Ordering::SeqCst) }) != 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = std::ptr::addr_of_mut!(flag) as *mut i8;
// LOWERING-NEXT:     unsafe { std::sync::atomic::AtomicI8::from_ptr({{_v[0-9]+}}).store(0, std::sync::atomic::Ordering::SeqCst) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = flag;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut flag: bool = false;
// REWRITES-NEXT: flag = false;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = std::ptr::addr_of_mut!(flag) as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = (unsafe { std::sync::atomic::AtomicI8::from_ptr({{_v[0-9]+}}).swap(1, std::sync::atomic::Ordering::SeqCst) }) != 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = std::ptr::addr_of_mut!(flag) as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = (unsafe { std::sync::atomic::AtomicI8::from_ptr({{_v[0-9]+}}).swap(1, std::sync::atomic::Ordering::SeqCst) }) != 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = std::ptr::addr_of_mut!(flag) as *mut i8;
// REWRITES-NEXT: unsafe { std::sync::atomic::AtomicI8::from_ptr({{_v[0-9]+}}).store(0, std::sync::atomic::Ordering::SeqCst) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = !flag;
// REWRITES-NEXT: unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}} as i32) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
