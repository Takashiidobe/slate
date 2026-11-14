#include <stdio.h>

struct gen {
    int gen;
    int try;
};

typedef struct gen gen_t;

static int gen_global = 3;
static int gen(int gen) { return gen * 2 + gen_global; }

static int advance(struct gen *gen, int send) {
    gen->gen += send;
    return gen->gen;
}

#define gen_macro(gen) ((gen) + 1)

int main(void) {
    struct gen state = {.gen = 1, .try = 2};
    int gen_local = gen(4);
    int (*fp)(int) = gen;
    printf("%d %d %d\n", advance(&state, 5), gen_local, fp(1));
    {
        int gen = 7;
        gen += gen_macro(gen);
        printf("%d %d\n", gen, state.try);
    }
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: macro_rules! gen_macro {
// CHECK-NEXT:     ($gen:expr) => {
// CHECK-NEXT:         ($gen) + 1
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct r#gen {
// CHECK-NEXT:     r#gen: i32,
// CHECK-NEXT:     r#try: i32,
// CHECK-NEXT: }
// CHECK-NEXT: static mut gen_global: i32 = 3;
// CHECK-NEXT: extern "C-unwind" fn r#gen(r#gen: i32) -> i32 {
// CHECK-NEXT:     r#gen * 2 + unsafe { gen_global }
// CHECK-NEXT: }
// CHECK-NEXT: fn advance(r#gen: &mut r#gen, send: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*r#gen).r#gen = (*r#gen).r#gen + send;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { (*(r#gen as *mut r#gen)).r#gen }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut state: r#gen = r#gen { r#gen: 1, r#try: 2 };
// CHECK-NEXT:     let gen_local: i32 = r#gen(4i32);
// CHECK-NEXT:     let fp: Option<unsafe extern "C-unwind" fn(i32) -> i32> = Some(r#gen);
// CHECK-NEXT:     let __slate_printf_arg0 = advance(
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(state)) },
// CHECK-NEXT:         5i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{} {} {}", __slate_printf_arg0, gen_local, unsafe { fp.unwrap() (1i32) });
// CHECK-NEXT:     let mut r#gen: i32 = 7;
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: i32 = r#gen;
// CHECK-NEXT:     r#gen = [[V_0]] + gen_macro!(r#gen);
// CHECK-NEXT:     println!("{} {}", r#gen, state.r#try);
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
