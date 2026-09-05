#include "anonymous_header_wrapper.h"

int main(void) {
  anonymous_header_state state = {0};
  state.value.wide             = 7;
  return state.count + (state.value.wide != 7);
}

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING: #![allow(
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
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: union {{anon_[0-9]+}} {
// COMMON-LOWERING-NEXT:     wide: i32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct anonymous_header_state {
// COMMON-LOWERING-NEXT:     count: i32,
// COMMON-LOWERING-NEXT:     value: {{anon_[0-9]+}},
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut state: anonymous_header_state = anonymous_header_state {
// COMMON-LOWERING-NEXT:         count: 0,
// COMMON-LOWERING-NEXT:         value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: anonymous_header_state = anonymous_header_state {
// COMMON-LOWERING-NEXT:         count: 0,
// COMMON-LOWERING-NEXT:         value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     state = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 7;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         state.value.wide = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = state.count;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { state.value.wide };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 7;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     bytes: [i8; 4],
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     bytes: [u8; 4],
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES: #![allow(
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
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: union {{anon_[0-9]+}} {
// COMMON-REWRITES-NEXT:     wide: i32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct anonymous_header_state {
// COMMON-REWRITES-NEXT:     count: i32,
// COMMON-REWRITES-NEXT:     value: {{anon_[0-9]+}},
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let mut state: anonymous_header_state = anonymous_header_state {
// COMMON-REWRITES-NEXT:         count: 0,
// COMMON-REWRITES-NEXT:         value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     state = anonymous_header_state {
// COMMON-REWRITES-NEXT:         count: 0,
// COMMON-REWRITES-NEXT:         value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         state.value.wide = 7;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = state.count + (((unsafe { state.value.wide }) != 7) as i32);
// COMMON-REWRITES-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT:     bytes: [i8; 4],
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT:     bytes: [u8; 4],
// SLATE-FILECHECK-END rewrites-aarch64-gnu
