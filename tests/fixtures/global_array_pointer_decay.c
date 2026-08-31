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
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut aligned_values: aligned::Aligned<aligned::A32, [u32; 3]> = aligned::Aligned([4, 5, 6]);
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut values: [u32; 3] = [1, 2, 3];
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn update({{arg[0-9]+}}: *mut u32) -> u32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 10;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u32 = unsafe { {{arg[0-9]+}}.add(0) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u32 = unsafe { {{arg[0-9]+}}.add(0) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u32 = unsafe { {{arg[0-9]+}}.add(2) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn middle({{arg[0-9]+}}: *mut [u32; 3]) -> u32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = unsafe { (*{{arg[0-9]+}})[({{_v[0-9]+}} as usize)] };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u32 = std::ptr::addr_of_mut!(values).cast::<u32>();
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = update({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u32 = std::ptr::addr_of_mut!(aligned_values).cast::<u32>();
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = update({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%u %u %u\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = middle(std::ptr::addr_of_mut!(values));
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut aligned_values: aligned::Aligned<aligned::A32, [u32; 3]> = aligned::Aligned([4, 5, 6]);
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut values: [u32; 3] = [1, 2, 3];
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn update({{arg[0-9]+}}: *mut u32) -> u32 {
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = 10;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut u32 = unsafe { {{arg[0-9]+}}.add(0) };
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *{{_v[0-9]+}} = (unsafe { *{{_v[0-9]+}} }) + {{_v[0-9]+}};
// REWRITES-NEXT: }
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut u32 = unsafe { {{arg[0-9]+}}.add(0) };
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 2;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut u32 = unsafe { {{arg[0-9]+}}.add(2) };
// REWRITES-NEXT: return {{_v[0-9]+}} + unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn middle({{arg[0-9]+}}: &[u32; 3]) -> u32 {
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 1;
// REWRITES-NEXT: return unsafe { (*({{arg[0-9]+}} as *const [u32; 3]))[({{_v[0-9]+}} as usize)] };
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut u32 = std::ptr::addr_of_mut!(values).cast::<u32>();
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = update({{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut u32 = std::ptr::addr_of_mut!(aligned_values).cast::<u32>();
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = update({{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%u %u %u\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = middle(unsafe { &(*std::ptr::addr_of_mut!(values)) });
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-NEXT: std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
