// merge-features: alpha beta gamma delta epsilon
#include <stdio.h>

struct state {
    int head;
#ifdef ALPHA
    long a;
#endif
#ifdef BETA
    long b;
#endif
};

int state_size(void) { return (int)sizeof(struct state); }

int nested(void) {
    int value = 0;
#ifdef GAMMA
    value += 1;
#ifdef DELTA
    value += 2;
#endif
#ifdef EPSILON
    value += 4;
#endif
#endif
    return value;
}

int joint(void) {
    int value = 0;
#if defined(ALPHA) && defined(GAMMA) && defined(EPSILON)
    value += 8;
#endif
#if defined(BETA) || (defined(DELTA) && defined(EPSILON))
    value += 16;
#endif
    return value;
}

#if defined(ALPHA) && defined(DELTA)
int shape(int x) { return x + 1; }
#elif defined(BETA)
int shape(int x) {
    for (int i = 0; i < 2; i++) x += i;
    return x;
}
#else
int shape(int x) {
    if (x > 0) return x * 3;
    return x;
}
#endif

int main(void) {
    printf("%d %d %d %d\n", state_size() > 8, nested(), joint(), shape(2));
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 24 compiles
// CHECK-NEXT:   cover_refine.c: [alpha,beta,gamma,delta,epsilon] 32 compiled
// CHECK-NEXT:     8 assignments preprocess like a compiled one
// CHECK-NEXT: target windows-x86_64: 24 compiles
// CHECK-NEXT:   cover_refine.c: [alpha,beta,gamma,delta,epsilon] 32 compiled
// CHECK-NEXT:     8 assignments preprocess like a compiled one
// CHECK-NEXT: total: 48 compiles across 2 targets
// CHECK-NEXT: unit cover_refine: 13 items, 3 gated, 0 duplicated functions
// CHECK-NEXT:   spliced record:state: 4 variants
// CHECK-NEXT:   spliced fn:nested: 5 variants
// CHECK-NEXT:   spliced fn:joint: 4 variants
// CHECK-NEXT:   spliced fn:shape: 3 variants
// CHECK-NEXT:   spliced fn:__slate_main: 2 variants
// CHECK-NEXT: macros: 0 function-like, 0 lifted at least once (expansions counted per distinct compile)
// CHECK-NEXT: projection: 64 checked, 0 not translatable, 0 mismatches
// CHECK-NEXT: signatures: 0 shared symbols, 0 mismatches
// CHECK-NEXT: exit: 0
// CHECK-NEXT: file main.rs
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: mod __slate_oldnames {
// CHECK-NEXT:     #[link(name = "oldnames")]
// CHECK-NEXT:     unsafe extern "C" {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct state {
// CHECK-NEXT:     head: i32,
// CHECK-NEXT:     #[cfg(feature = "alpha")]
// CHECK-NEXT:     a: c_long,
// CHECK-NEXT:     #[cfg(feature = "beta")]
// CHECK-NEXT:     b: c_long,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn joint() -> i32 {
// CHECK-NEXT:     #[cfg(any(
// CHECK-NEXT:         feature = "beta",
// CHECK-NEXT:         all(feature = "delta", feature = "epsilon"),
// CHECK-NEXT:         all(feature = "alpha", feature = "gamma", feature = "epsilon")
// CHECK-NEXT:     ))]
// CHECK-NEXT:     let mut value: i32 = 0;
// CHECK-NEXT:     #[cfg(all(feature = "alpha", feature = "gamma", feature = "epsilon"))]
// CHECK-NEXT:     {
// CHECK-NEXT:         value += 8;
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(any(feature = "beta", all(feature = "delta", feature = "epsilon")))]
// CHECK-NEXT:     {
// CHECK-NEXT:         value += 16;
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(any(
// CHECK-NEXT:         all(not(feature = "beta"), not(feature = "epsilon")),
// CHECK-NEXT:         all(not(feature = "alpha"), not(feature = "beta"), not(feature = "delta")),
// CHECK-NEXT:         all(not(feature = "beta"), not(feature = "gamma"), not(feature = "delta"))
// CHECK-NEXT:     ))]
// CHECK-NEXT:     let value: i32 = 0;
// CHECK-NEXT:     value
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn nested() -> i32 {
// CHECK-NEXT:     #[cfg(feature = "gamma")]
// CHECK-NEXT:     let mut value: i32 = 0;
// CHECK-NEXT:     #[cfg(feature = "gamma")]
// CHECK-NEXT:     {
// CHECK-NEXT:         value += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(all(feature = "gamma", feature = "delta"))]
// CHECK-NEXT:     {
// CHECK-NEXT:         value += 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(all(feature = "gamma", feature = "epsilon"))]
// CHECK-NEXT:     {
// CHECK-NEXT:         value += 4;
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(not(feature = "gamma"))]
// CHECK-NEXT:     let value: i32 = 0;
// CHECK-NEXT:     value
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn shape(mut x: i32) -> i32 {
// CHECK-NEXT:     #[cfg(any(
// CHECK-NEXT:         all(not(feature = "alpha"), not(feature = "beta")),
// CHECK-NEXT:         all(not(feature = "beta"), not(feature = "delta"))
// CHECK-NEXT:     ))]
// CHECK-NEXT:     {
// CHECK-NEXT:         if x > 0 {
// CHECK-NEXT:             return x * 3;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(any(
// CHECK-NEXT:         all(not(feature = "alpha"), feature = "beta"),
// CHECK-NEXT:         all(feature = "beta", not(feature = "delta"))
// CHECK-NEXT:     ))]
// CHECK-NEXT:     {
// CHECK-NEXT:         for i in 0i32..2i32 {
// CHECK-NEXT:             x += i;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(any(not(feature = "alpha"), not(feature = "delta")))]
// CHECK-NEXT:     {
// CHECK-NEXT:         x
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(all(feature = "alpha", feature = "delta"))]
// CHECK-NEXT:     {
// CHECK-NEXT:         x + 1
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn state_size() -> i32 {
// CHECK-NEXT:     std::mem::size_of::<state>() as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: //! merge-features: alpha beta gamma delta epsilon
// CHECK-NEXT: #[cfg(not(any(
// CHECK-NEXT:     all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"),
// CHECK-NEXT:     all(target_arch = "x86_64", windows, target_env = "msvc")
// CHECK-NEXT: )))]
// CHECK-NEXT: compile_error!("this crate was not translated for this target");
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(
// CHECK-NEXT:                 c"%d %d %d %d\n".as_ptr(),
// CHECK-NEXT:                 (state_size() > 8) as i32,
// CHECK-NEXT:                 nested(),
// CHECK-NEXT:                 joint(),
// CHECK-NEXT:                 shape(2i32),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!(
// CHECK-NEXT:             "{} {} {} {}",
// CHECK-NEXT:             (state_size() > 8) as i32,
// CHECK-NEXT:             nested(),
// CHECK-NEXT:             joint(),
// CHECK-NEXT:             shape(2i32)
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: use core::ffi::c_char;
// CHECK-NEXT: #[cfg(any(feature = "alpha", feature = "beta"))]
// CHECK-NEXT: use core::ffi::c_long;
// CHECK-NEXT: use slate_support::size_t;
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
