#include <stdint.h>

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C, align(32))]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct OverAligned {
// REWRITES-NEXT:     value: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut object: aligned::Aligned<aligned::A32, OverAligned> = aligned::Aligned(OverAligned { value: 0 });
// REWRITES-NEXT: let mut local: aligned::Aligned<aligned::A64, i32> = aligned::Aligned(0);
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: *object = OverAligned { value: 7 };
// REWRITES-NEXT: *local = 11;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%zu %zu %zu %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = std::mem::align_of::<OverAligned>() as u64;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = std::ptr::addr_of_mut!(*object) as u64;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 32;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} % {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = std::ptr::addr_of_mut!(*local) as u64;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 64;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = object.value;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}} % {{_v[0-9]+}}, {{_v[0-9]+}}, *local) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites

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
