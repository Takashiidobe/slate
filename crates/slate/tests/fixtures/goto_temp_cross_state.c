#include <stdio.h>

typedef struct {
    int pad;
    int x;
    int y;
} inner_t;

typedef struct {
    int lead;
    inner_t in;
} outer_t;

typedef struct {
    outer_t *dict;
} state_t;

static int compute(const state_t *const ms, int flag) {
    const outer_t *const o = ms->dict;
    const inner_t *const q = &o->in;
    int acc = q->x;
    if (flag) {
        goto second;
    }
    acc += 100;
second:
    acc += q->y;
    return acc;
}

int main(void) {
    inner_t inr;
    inr.pad = 0;
    inr.x = 3;
    inr.y = 4;
    outer_t ou;
    ou.lead = 0;
    ou.in = inr;
    state_t s;
    s.dict = &ou;
    printf("%d %d\n", compute(&s, 0), compute(&s, 1));
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
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct inner_t {
// LOWERING-NEXT:     pad: i32,
// LOWERING-NEXT:     x: i32,
// LOWERING-NEXT:     y: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct outer_t {
// LOWERING-NEXT:     lead: i32,
// LOWERING-NEXT:     r#in: inner_t,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct state_t {
// LOWERING-NEXT:     dict: *mut outer_t,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn compute({{arg[0-9]+}}: *mut state_t, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let mut ms: *mut state_t = std::ptr::null_mut();
// LOWERING-NEXT:     let mut flag: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut o: *mut outer_t = std::ptr::null_mut();
// LOWERING-NEXT:     let mut q: *mut inner_t = std::ptr::null_mut();
// LOWERING-NEXT:     let mut acc: i32 = 0;
// LOWERING-NEXT:     let mut {{__state[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     '{{__dispatch[0-9]+}}: loop {
// LOWERING-NEXT:         match {{__state[0-9]+}} {
// LOWERING-NEXT:             0 => {
// LOWERING-NEXT:                 ms = {{arg[0-9]+}};
// LOWERING-NEXT:                 flag = {{arg[0-9]+}};
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut state_t = ms;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut outer_t = unsafe { (*{{_v[0-9]+}}).dict };
// LOWERING-NEXT:                 o = {{_v[0-9]+}};
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut outer_t = o;
// LOWERING-NEXT:                 q = unsafe { std::ptr::addr_of_mut!((*{{_v[0-9]+}}).r#in) };
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut inner_t = q;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { (*{{_v[0-9]+}}).x };
// LOWERING-NEXT:                 acc = {{_v[0-9]+}};
// LOWERING-NEXT:                 {{__state[0-9]+}} = 1;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             1 => {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = flag;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:                 if {{_v[0-9]+}} {
// LOWERING-NEXT:                     {{__state[0-9]+}} = 2;
// LOWERING-NEXT:                 } else {
// LOWERING-NEXT:                     {{__state[0-9]+}} = 3;
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             2 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 5;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             3 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 4;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             4 => {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 100;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = acc;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                 acc = {{_v[0-9]+}};
// LOWERING-NEXT:                 {{__state[0-9]+}} = 5;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             5 => {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut inner_t = q;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { (*{{_v[0-9]+}}).y };
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = acc;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                 acc = {{_v[0-9]+}};
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = acc;
// LOWERING-NEXT:                 __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                 return {{_v[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             _ => {
// LOWERING-NEXT:                 unreachable!();
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut inr: inner_t = inner_t { pad: 0, x: 0, y: 0 };
// LOWERING-NEXT:     let mut ou: outer_t = outer_t {
// LOWERING-NEXT:         lead: 0,
// LOWERING-NEXT:         r#in: inner_t { pad: 0, x: 0, y: 0 },
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut s: state_t = state_t {
// LOWERING-NEXT:         dict: std::ptr::null_mut(),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     inr.pad = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     inr.x = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:     inr.y = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     ou.lead = {{_v[0-9]+}};
// LOWERING-NEXT:     ou.r#in = inr;
// LOWERING-NEXT:     s.dict = std::ptr::addr_of_mut!(ou);
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = compute(std::ptr::addr_of_mut!(s), {{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = compute(std::ptr::addr_of_mut!(s), {{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
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
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct inner_t {
// REWRITES-NEXT:     pad: i32,
// REWRITES-NEXT:     x: i32,
// REWRITES-NEXT:     y: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct outer_t {
// REWRITES-NEXT:     lead: i32,
// REWRITES-NEXT:     r#in: inner_t,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct state_t {
// REWRITES-NEXT:     dict: *mut outer_t,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn compute({{arg[0-9]+}}: &state_t, {{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT:     let mut ms: *mut state_t = std::ptr::null_mut();
// REWRITES-NEXT:     let mut flag: i32 = 0;
// REWRITES-NEXT:     let mut __retval: i32 = 0;
// REWRITES-NEXT:     let mut o: *mut outer_t = std::ptr::null_mut();
// REWRITES-NEXT:     let mut q: *mut inner_t = std::ptr::null_mut();
// REWRITES-NEXT:     let mut acc: i32 = 0;
// REWRITES-NEXT:     let mut {{__state[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     '{{__dispatch[0-9]+}}: loop {
// REWRITES-NEXT:         match {{__state[0-9]+}} {
// REWRITES-NEXT:             0 => {
// REWRITES-NEXT:                 ms = ({{arg[0-9]+}} as *const state_t) as *mut state_t;
// REWRITES-NEXT:                 flag = {{arg[0-9]+}};
// REWRITES-NEXT:                 o = unsafe { (*ms).dict };
// REWRITES-NEXT:                 q = unsafe { std::ptr::addr_of_mut!((*o).r#in) };
// REWRITES-NEXT:                 acc = unsafe { (*q).x };
// REWRITES-NEXT:                 {{__state[0-9]+}} = 1;
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             1 => {
// REWRITES-NEXT:                 let {{_v[0-9]+}}: bool = flag != 0;
// REWRITES-NEXT:                 if {{_v[0-9]+}} {
// REWRITES-NEXT:                     {{__state[0-9]+}} = 2;
// REWRITES-NEXT:                 } else {
// REWRITES-NEXT:                     {{__state[0-9]+}} = 3;
// REWRITES-NEXT:                 }
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             2 => {
// REWRITES-NEXT:                 {{__state[0-9]+}} = 5;
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             3 => {
// REWRITES-NEXT:                 {{__state[0-9]+}} = 4;
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             4 => {
// REWRITES-NEXT:                 let {{_v[0-9]+}}: i32 = 100;
// REWRITES-NEXT:                 acc = acc + {{_v[0-9]+}};
// REWRITES-NEXT:                 {{__state[0-9]+}} = 5;
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             5 => {
// REWRITES-NEXT:                 acc = acc + unsafe { (*q).y };
// REWRITES-NEXT:                 __retval = acc;
// REWRITES-NEXT:                 return __retval;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             _ => {
// REWRITES-NEXT:                 unreachable!();
// REWRITES-NEXT:             }
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut inr: inner_t = inner_t { pad: 0, x: 0, y: 0 };
// REWRITES-NEXT:     let mut ou: outer_t = outer_t {
// REWRITES-NEXT:         lead: 0,
// REWRITES-NEXT:         r#in: inner_t { pad: 0, x: 0, y: 0 },
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut s: state_t = state_t {
// REWRITES-NEXT:         dict: std::ptr::null_mut(),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     inr.pad = 0;
// REWRITES-NEXT:     inr.x = 3;
// REWRITES-NEXT:     inr.y = 4;
// REWRITES-NEXT:     ou.lead = 0;
// REWRITES-NEXT:     ou.r#in = inr;
// REWRITES-NEXT:     s.dict = std::ptr::addr_of_mut!(ou);
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%d %d\n".as_ptr(),
// REWRITES-NEXT:             compute(unsafe { &(*std::ptr::addr_of_mut!(s)) }, 0),
// REWRITES-NEXT:             compute(unsafe { &(*std::ptr::addr_of_mut!(s)) }, 1),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
