#include <stddef.h>
#include <stdio.h>

static void *first_word(void *p) {
  void *q = __builtin_assume_aligned(p, 32);
  return q;
}

static void *first_word_offset(void *p, size_t off) {
  void *q = __builtin_assume_aligned(p, 32, off);
  return q;
}

int main(void) {
  _Alignas(32) static unsigned char buf[64];
  void                             *a = first_word(buf);
  void                             *b = first_word_offset(buf + 8, 8);
  printf("%d %d\n", a == buf, b == buf + 8);
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(
// LOWERING-NEXT:     dead_code,
// LOWERING-NEXT:     unused,
// LOWERING-NEXT:     non_camel_case_types,
// LOWERING-NEXT:     non_snake_case,
// LOWERING-NEXT:     non_upper_case_globals,
// LOWERING-NEXT:     arithmetic_overflow,
// LOWERING-NEXT:     unconditional_panic,
// LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-NEXT:     unused_comparisons
// LOWERING-NEXT: )]
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut main_buf: aligned::Aligned<aligned::A32, [u8; 64]> = aligned::Aligned([0; 64]);
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(main_buf).cast::<u8>();
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = first_word({{__v[0-9]+}} as *mut core::ffi::c_void);
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(main_buf).cast::<u8>();
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(8) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = first_word_offset({{__v[0-9]+}} as *mut core::ffi::c_void, {{__v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(main_buf).cast::<u8>();
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(main_buf).cast::<u8>();
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(8) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn first_word({{arg[0-9]+}}: *mut core::ffi::c_void) -> *mut core::ffi::c_void {
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = true;
// LOWERING-NEXT:     unsafe { core::hint::assert_unchecked(({{arg[0-9]+}} as usize) % ({{__v[0-9]+}} as usize) == 0usize) };
// LOWERING-NEXT:     return {{arg[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn first_word_offset({{arg[0-9]+}}: *mut core::ffi::c_void, {{arg[0-9]+}}: u64) -> *mut core::ffi::c_void {
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = true;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         core::hint::assert_unchecked(
// LOWERING-NEXT:             ({{arg[0-9]+}} as usize).wrapping_sub({{arg[0-9]+}} as usize) % ({{__v[0-9]+}} as usize) == 0usize,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     return {{arg[0-9]+}};
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(
// REWRITES-NEXT:     dead_code,
// REWRITES-NEXT:     unused,
// REWRITES-NEXT:     non_camel_case_types,
// REWRITES-NEXT:     non_snake_case,
// REWRITES-NEXT:     non_upper_case_globals,
// REWRITES-NEXT:     arithmetic_overflow,
// REWRITES-NEXT:     unconditional_panic,
// REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-NEXT:     unused_comparisons
// REWRITES-NEXT: )]
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut main_buf: aligned::Aligned<aligned::A32, [u8; 64]> = aligned::Aligned([0; 64]);
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(main_buf).cast::<u8>();
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = first_word({{__v[0-9]+}} as *mut core::ffi::c_void);
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(main_buf).cast::<u8>();
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(8) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = first_word_offset({{__v[0-9]+}} as *mut core::ffi::c_void, 8);
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"%d %d\n".as_ptr() as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"%d %d\n".as_ptr() as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(main_buf).cast::<u8>();
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} == ({{__v[0-9]+}} as *mut core::ffi::c_void)) as i32;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(main_buf).cast::<u8>();
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(8) };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             ({{__v[0-9]+}} == ({{__v[0-9]+}} as *mut core::ffi::c_void)) as i32,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn first_word({{arg[0-9]+}}: *mut core::ffi::c_void) -> *mut core::ffi::c_void {
// REWRITES-NEXT:     unsafe { core::hint::assert_unchecked(({{arg[0-9]+}} as usize) % ((32 as u64) as usize) == 0usize) };
// REWRITES-NEXT:     {{arg[0-9]+}}
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn first_word_offset({{arg[0-9]+}}: *mut core::ffi::c_void, {{arg[0-9]+}}: u64) -> *mut core::ffi::c_void {
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         core::hint::assert_unchecked(
// REWRITES-NEXT:             ({{arg[0-9]+}} as usize).wrapping_sub({{arg[0-9]+}} as usize) % ((32 as u64) as usize) == 0usize,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     {{arg[0-9]+}}
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
