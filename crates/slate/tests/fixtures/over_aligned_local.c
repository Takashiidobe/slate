#include <stdint.h>

// LOWERING-DAG: #[repr(C, align(32))]
// LOWERING-LABEL: {{^}}fn main() {
// LOWERING-DAG: aligned::Aligned<aligned::A32, OverAligned>
// LOWERING-DAG: aligned::Aligned<aligned::A64, i32>
// LOWERING-DAG: std::mem::align_of::<OverAligned>() as u64
// LOWERING-DAG: std::ptr::addr_of_mut!(*object) as u64
// LOWERING-DAG: std::ptr::addr_of_mut!(*local) as u64
// LOWERING-NOT: object as u64
// LOWERING-NOT: *local as u64
// LOWERING: {{^}}}

#include <stdio.h>

struct OverAligned {
  _Alignas(32) int value;
};

int main(void) {
  struct OverAligned object = {7};
  _Alignas(64) int   local  = 11;
  printf("%zu %zu %zu %d %d\n", _Alignof(struct OverAligned),
         (uintptr_t)&object % 32U, (uintptr_t)&local % 64U, object.value,
         local);
  return 0;
}

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
// REWRITES-NEXT: #[repr(C, align(32))]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct OverAligned {
// REWRITES-NEXT:     value: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut object: aligned::Aligned<aligned::A32, OverAligned> =
// REWRITES-NEXT:         aligned::Aligned(OverAligned { value: 0 });
// REWRITES-NEXT:     let mut local: aligned::Aligned<aligned::A64, i32> = aligned::Aligned(0);
// REWRITES-NEXT:     *object = OverAligned { value: 7 };
// REWRITES-NEXT:     *local = 11;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = c"%zu %zu %zu %d %d\n".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: u64 = std::ptr::addr_of_mut!(*object) as u64;
// REWRITES-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} % 32;
// REWRITES-NEXT:     let {{_v[0-9]+}}: u64 = std::ptr::addr_of_mut!(*local) as u64;
// REWRITES-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} % 64;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = object.value;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:             std::mem::align_of::<OverAligned>() as u64,
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             *local,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
