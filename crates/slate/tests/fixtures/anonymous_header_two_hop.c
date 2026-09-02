#include "anonymous_header_wrapper.h"

int main(void) {
  anonymous_header_state state = {0};
  state.value.wide             = 7;
  return state.count + (state.value.wide != 7);
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![allow(
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
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: union {{anon_[0-9]+}} {
// LOWERING-NEXT:     wide: i32,
// LOWERING-NEXT:     bytes: [i8; 4],
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct anonymous_header_state {
// LOWERING-NEXT:     count: i32,
// LOWERING-NEXT:     value: {{anon_[0-9]+}},
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut state: anonymous_header_state = anonymous_header_state {
// LOWERING-NEXT:         count: 0,
// LOWERING-NEXT:         value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     state = anonymous_header_state {
// LOWERING-NEXT:         count: 0,
// LOWERING-NEXT:         value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 7;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         state.value.wide = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = state.count;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { state.value.wide };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 7;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![allow(
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
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: union {{anon_[0-9]+}} {
// REWRITES-NEXT:     wide: i32,
// REWRITES-NEXT:     bytes: [i8; 4],
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct anonymous_header_state {
// REWRITES-NEXT:     count: i32,
// REWRITES-NEXT:     value: {{anon_[0-9]+}},
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut state: anonymous_header_state = anonymous_header_state {
// REWRITES-NEXT:         count: 0,
// REWRITES-NEXT:         value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// REWRITES-NEXT:     };
// REWRITES-NEXT:     state = anonymous_header_state {
// REWRITES-NEXT:         count: 0,
// REWRITES-NEXT:         value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// REWRITES-NEXT:     };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         state.value.wide = 7;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = (unsafe { state.value.wide }) != 7;
// REWRITES-NEXT:     std::process::exit((state.count + ({{_v[0-9]+}} as i32)) as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
