#include <stdint.h>
#include <stdio.h>

static uint32_t values[3]                      = {1, 2, 3};
_Alignas(32) static uint32_t aligned_values[3] = {4, 5, 6};

static uint32_t update(uint32_t *items) {
  items[0] += 10;
  return items[0] + items[2];
}

static uint32_t middle(uint32_t (*items)[3]) { return (*items)[1]; }

int main(void) {
  uint32_t first  = update(values);
  uint32_t second = update(aligned_values);
  printf("%u %u %u\n", first, second, middle(&values));
}

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING: #![feature(c_variadic)]
// COMMON-LOWERING-NEXT: #![allow(
// COMMON-LOWERING-NEXT:     dead_code,
// COMMON-LOWERING-NEXT:     unused,
// COMMON-LOWERING-NEXT:     non_camel_case_types,
// COMMON-LOWERING-NEXT:     non_snake_case,
// COMMON-LOWERING-NEXT:     non_upper_case_globals,
// COMMON-LOWERING-NEXT:     arithmetic_overflow,
// COMMON-LOWERING-NEXT:     unconditional_panic,
// COMMON-LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-LOWERING-NEXT:     unused_comparisons
// COMMON-LOWERING-NEXT: )]
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: static mut aligned_values: aligned::Aligned<aligned::A32, [u32; 3]> = aligned::Aligned([4, 5, 6]);
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: static mut values: [u32; 3] = [1, 2, 3];
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u32 = std::ptr::addr_of_mut!(values).cast::<u32>();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = update({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u32 = std::ptr::addr_of_mut!(aligned_values).cast::<u32>();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = update({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = middle(std::ptr::addr_of_mut!(values));
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn update({{arg[0-9]+}}: *mut u32) -> u32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 10;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u32 = unsafe { {{arg[0-9]+}}.add(0) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u32 = unsafe { {{arg[0-9]+}}.add(0) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u32 = unsafe { {{arg[0-9]+}}.add(2) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn middle({{arg[0-9]+}}: *mut [u32; 3]) -> u32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = unsafe { (*{{arg[0-9]+}})[({{__v[0-9]+}} as usize)] };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%u %u %u\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%u %u %u\n\0".as_ptr() as *mut u8;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES: #![feature(c_variadic)]
// COMMON-REWRITES-NEXT: #![allow(
// COMMON-REWRITES-NEXT:     dead_code,
// COMMON-REWRITES-NEXT:     unused,
// COMMON-REWRITES-NEXT:     non_camel_case_types,
// COMMON-REWRITES-NEXT:     non_snake_case,
// COMMON-REWRITES-NEXT:     non_upper_case_globals,
// COMMON-REWRITES-NEXT:     arithmetic_overflow,
// COMMON-REWRITES-NEXT:     unconditional_panic,
// COMMON-REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-REWRITES-NEXT:     unused_comparisons
// COMMON-REWRITES-NEXT: )]
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: static mut aligned_values: aligned::Aligned<aligned::A32, [u32; 3]> = aligned::Aligned([4, 5, 6]);
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: static mut values: [u32; 3] = [1, 2, 3];
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u32 = std::ptr::addr_of_mut!(values).cast::<u32>();
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u32 = update({{__v[0-9]+}});
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u32 = std::ptr::addr_of_mut!(aligned_values).cast::<u32>();
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%u %u %u\n".as_ptr(),
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             update({{__v[0-9]+}}),
// COMMON-REWRITES-NEXT:             middle(unsafe { &(*std::ptr::addr_of_mut!(values)) }),
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn update({{arg[0-9]+}}: *mut u32) -> u32 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u32 = 10;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u32 = unsafe { {{arg[0-9]+}}.add(0) };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         *{{__v[0-9]+}} = (unsafe { *{{__v[0-9]+}} }) + {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u32 = unsafe { {{arg[0-9]+}}.add(0) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u32 = unsafe { *{{__v[0-9]+}} };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u32 = unsafe { {{arg[0-9]+}}.add(2) };
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}} + unsafe { *{{__v[0-9]+}} }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn middle({{arg[0-9]+}}: &[u32; 3]) -> u32 {
// COMMON-REWRITES-NEXT:     unsafe { (*({{arg[0-9]+}} as *const [u32; 3]))[1] }
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites
