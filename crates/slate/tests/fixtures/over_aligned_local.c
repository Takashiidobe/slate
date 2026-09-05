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
// COMMON-LOWERING-NEXT: #[repr(C, align(32))]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct OverAligned {
// COMMON-LOWERING-NEXT:     value: i32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut object: aligned::Aligned<aligned::A32, OverAligned> =
// COMMON-LOWERING-NEXT:         aligned::Aligned(OverAligned { value: 0 });
// COMMON-LOWERING-NEXT:     let mut local: aligned::Aligned<aligned::A64, i32> = aligned::Aligned(0);
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: OverAligned = OverAligned { value: 7 };
// COMMON-LOWERING-NEXT:     *object = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 11;
// COMMON-LOWERING-NEXT:     *local = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = std::mem::align_of::<OverAligned>() as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = std::ptr::addr_of_mut!(*object) as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} % {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = std::ptr::addr_of_mut!(*local) as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} % {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = object.value;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = *local;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         printf(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%zu %zu %zu %d %d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%zu %zu %zu %d %d\n\0".as_ptr() as *mut u8;
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
// COMMON-REWRITES-NEXT: #[repr(C, align(32))]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct OverAligned {
// COMMON-REWRITES-NEXT:     value: i32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let mut object: aligned::Aligned<aligned::A32, OverAligned> =
// COMMON-REWRITES-NEXT:         aligned::Aligned(OverAligned { value: 0 });
// COMMON-REWRITES-NEXT:     let mut local: aligned::Aligned<aligned::A64, i32> = aligned::Aligned(0);
// COMMON-REWRITES-NEXT:     *object = OverAligned { value: 7 };
// COMMON-REWRITES-NEXT:     *local = 11;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = std::ptr::addr_of_mut!(*object) as u64;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} % 32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = std::ptr::addr_of_mut!(*local) as u64;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} % 64;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = object.value;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:             std::mem::align_of::<OverAligned>() as u64,
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             *local,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"%zu %zu %zu %d %d\n".as_ptr() as *mut i8;
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"%zu %zu %zu %d %d\n".as_ptr() as *mut u8;
// SLATE-FILECHECK-END rewrites-aarch64-gnu
