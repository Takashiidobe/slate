#include <stdio.h>

int run(int mode, int n) {
    int out = 0;
    for (;;) {
        switch (mode) {
        case 0:
            out += 1;
            if (n-- == 0)
                goto leave;
#ifdef ALPHA
        case 1:
            out *= 3;
            if (n-- == 0)
                goto leave;
        case 2:
            out += 5;
#endif
        case 3:
            out ^= 7;
            if (n-- == 0)
                goto leave;
            mode = 4;
            break;
        case 4:
            out -= 2;
            if (out < -50 || n-- == 0)
                goto leave;
            mode = 0;
            break;
#ifdef BETA
        case 5:
            out += 11;
#endif
        case 6:
            out *= 2;
            if (n-- == 0)
                goto leave;
            mode = 3;
            break;
        default:
            goto leave;
        }
    }
leave:
    return out;
}

int main(void) {
    unsigned total = 0;
    for (int m = 0; m < 8; m++)
        for (int n = 0; n < 9; n++)
            total = total * 31u + (unsigned)run(m, n);
    printf("%u\n", total);
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 4 compiles
// CHECK-NEXT:   splice_ladder.c: [alpha,beta] 4 compiled
// CHECK-NEXT: target windows-x86_64: 4 compiles
// CHECK-NEXT:   splice_ladder.c: [alpha,beta] 4 compiled
// CHECK-NEXT: total: 8 compiles across 2 targets
// CHECK-NEXT: unit splice_ladder: 6 items, 1 gated, 0 duplicated functions
// CHECK-NEXT:   spliced fn:run: 4 variants
// CHECK-NEXT: macros: 0 function-like, 0 lifted at least once (expansions counted per distinct compile)
// CHECK-NEXT: projection: 12 checked, 0 not translatable, 0 mismatches
// CHECK-NEXT: signatures: 0 shared symbols, 0 mismatches
// CHECK-NEXT: exit: 0
// CHECK-NEXT: file main.rs
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(not(any(all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"), all(target_arch = "x86_64", windows, target_env = "msvc"))))]
// CHECK-NEXT: compile_error!("this crate was not translated for this target");
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: mod __slate_oldnames {
// CHECK-NEXT:     #[link(name = "oldnames")]
// CHECK-NEXT:     unsafe extern "C" {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn run(mut mode: i32, mut n: i32) -> i32 {
// CHECK-NEXT:     let mut out: i32 = 0;
// CHECK-NEXT:     loop {
// CHECK-NEXT:         #[cfg(all(feature = "alpha", feature = "beta"))]
// CHECK-NEXT:         {
// CHECK-NEXT:             match mode {
// CHECK-NEXT:                 0 => {
// CHECK-NEXT:                     out += 1;
// CHECK-NEXT:                     let [[V_0:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:                     n = [[V_0]] - 1;
// CHECK-NEXT:                     if [[V_0]] == 0 {
// CHECK-NEXT:                         break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     out *= 3;
// CHECK-NEXT:                     let [[V_1:_v[0-9]+]]: i32 = n;
// CHECK-NEXT:                     n = [[V_1]] - 1;
// CHECK-NEXT:                     if [[V_1]] == 0 {
// CHECK-NEXT:                         break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     out += 5;
// CHECK-NEXT:                     out ^= 7;
// CHECK-NEXT:                     let [[V_2:_v[0-9]+]]: i32 = n;
// CHECK-NEXT:                     n = [[V_2]] - 1;
// CHECK-NEXT:                     if [[V_2]] == 0 {
// CHECK-NEXT:                         break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     mode = 4;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 1 => {
// CHECK-NEXT:                     out *= 3;
// CHECK-NEXT:                     let [[V_3:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:                     n = [[V_3]] - 1;
// CHECK-NEXT:                     if [[V_3]] == 0 {
// CHECK-NEXT:                         break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     out += 5;
// CHECK-NEXT:                     out ^= 7;
// CHECK-NEXT:                     let [[V_4:_v[0-9]+]]: i32 = n;
// CHECK-NEXT:                     n = [[V_4]] - 1;
// CHECK-NEXT:                     if [[V_4]] == 0 {
// CHECK-NEXT:                         break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     mode = 4;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 2 => {
// CHECK-NEXT:                     out += 5;
// CHECK-NEXT:                     out ^= 7;
// CHECK-NEXT:                     let [[V_5:_v[0-9]+]]: i32 = n;
// CHECK-NEXT:                     n = [[V_5]] - 1;
// CHECK-NEXT:                     if [[V_5]] == 0 {
// CHECK-NEXT:                         break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     mode = 4;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 3 => {
// CHECK-NEXT:                     out ^= 7;
// CHECK-NEXT:                     let [[V_6:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:                     n = [[V_6]] - 1;
// CHECK-NEXT:                     if [[V_6]] == 0 {
// CHECK-NEXT:                         break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     mode = 4;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 4 => {
// CHECK-NEXT:                     out -= 2;
// CHECK-NEXT:                     let [[V_7:__v[0-9]+]]: bool = if out < -50 {
// CHECK-NEXT:                         true
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         let [[V_8:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:                         n = [[V_8]] - 1;
// CHECK-NEXT:                         [[V_8]] == 0
// CHECK-NEXT:                     };
// CHECK-NEXT:                     if [[V_7]] {
// CHECK-NEXT:                         break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     mode = 0;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 5 => {
// CHECK-NEXT:                     out += 11;
// CHECK-NEXT:                     out *= 2;
// CHECK-NEXT:                     let [[V_9:_v[0-9]+]]: i32 = n;
// CHECK-NEXT:                     n = [[V_9]] - 1;
// CHECK-NEXT:                     if [[V_9]] == 0 {
// CHECK-NEXT:                         break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     mode = 3;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 6 => {
// CHECK-NEXT:                     out *= 2;
// CHECK-NEXT:                     let [[V_10:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:                     n = [[V_10]] - 1;
// CHECK-NEXT:                     if [[V_10]] == 0 {
// CHECK-NEXT:                         break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     mode = 3;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 _ => {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         #[cfg(all(feature = "alpha", not(feature = "beta")))]
// CHECK-NEXT:         {
// CHECK-NEXT:             match mode {
// CHECK-NEXT:                 0 => {
// CHECK-NEXT:                     out += 1;
// CHECK-NEXT:                     let [[V_11:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:                     n = [[V_11]] - 1;
// CHECK-NEXT:                     if [[V_11]] == 0 {
// CHECK-NEXT:                         break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     out *= 3;
// CHECK-NEXT:                     let [[V_1]]: i32 = n;
// CHECK-NEXT:                     n = [[V_1]] - 1;
// CHECK-NEXT:                     if [[V_1]] == 0 {
// CHECK-NEXT:                         break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     out += 5;
// CHECK-NEXT:                     out ^= 7;
// CHECK-NEXT:                     let [[V_2]]: i32 = n;
// CHECK-NEXT:                     n = [[V_2]] - 1;
// CHECK-NEXT:                     if [[V_2]] == 0 {
// CHECK-NEXT:                         break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     mode = 4;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 1 => {
// CHECK-NEXT:                     out *= 3;
// CHECK-NEXT:                     let [[V_12:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:                     n = [[V_12]] - 1;
// CHECK-NEXT:                     if [[V_12]] == 0 {
// CHECK-NEXT:                         break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     out += 5;
// CHECK-NEXT:                     out ^= 7;
// CHECK-NEXT:                     let [[V_4]]: i32 = n;
// CHECK-NEXT:                     n = [[V_4]] - 1;
// CHECK-NEXT:                     if [[V_4]] == 0 {
// CHECK-NEXT:                         break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     mode = 4;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 2 => {
// CHECK-NEXT:                     out += 5;
// CHECK-NEXT:                     out ^= 7;
// CHECK-NEXT:                     let [[V_5]]: i32 = n;
// CHECK-NEXT:                     n = [[V_5]] - 1;
// CHECK-NEXT:                     if [[V_5]] == 0 {
// CHECK-NEXT:                         break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     mode = 4;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 3 => {
// CHECK-NEXT:                     out ^= 7;
// CHECK-NEXT:                     let [[V_13:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:                     n = [[V_13]] - 1;
// CHECK-NEXT:                     if [[V_13]] == 0 {
// CHECK-NEXT:                         break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     mode = 4;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 4 => {
// CHECK-NEXT:                     out -= 2;
// CHECK-NEXT:                     let [[V_14:__v[0-9]+]]: bool = if out < -50 {
// CHECK-NEXT:                         true
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         let [[V_15:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:                         n = [[V_15]] - 1;
// CHECK-NEXT:                         [[V_15]] == 0
// CHECK-NEXT:                     };
// CHECK-NEXT:                     if [[V_14]] {
// CHECK-NEXT:                         break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     mode = 0;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 6 => {
// CHECK-NEXT:                     out *= 2;
// CHECK-NEXT:                     let [[V_16:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:                     n = [[V_16]] - 1;
// CHECK-NEXT:                     if [[V_16]] == 0 {
// CHECK-NEXT:                         break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     mode = 3;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 _ => {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         #[cfg(all(not(feature = "alpha"), feature = "beta"))]
// CHECK-NEXT:         {
// CHECK-NEXT:             match mode {
// CHECK-NEXT:                 0 => {
// CHECK-NEXT:                     out += 1;
// CHECK-NEXT:                     let [[V_17:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:                     n = [[V_17]] - 1;
// CHECK-NEXT:                     if [[V_17]] == 0 {
// CHECK-NEXT:                         break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     out ^= 7;
// CHECK-NEXT:                     let [[V_18:_v[0-9]+]]: i32 = n;
// CHECK-NEXT:                     n = [[V_18]] - 1;
// CHECK-NEXT:                     if [[V_18]] == 0 {
// CHECK-NEXT:                         break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     mode = 4;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 3 => {
// CHECK-NEXT:                     out ^= 7;
// CHECK-NEXT:                     let [[V_19:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:                     n = [[V_19]] - 1;
// CHECK-NEXT:                     if [[V_19]] == 0 {
// CHECK-NEXT:                         break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     mode = 4;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 4 => {
// CHECK-NEXT:                     out -= 2;
// CHECK-NEXT:                     let [[V_20:__v[0-9]+]]: bool = if out < -50 {
// CHECK-NEXT:                         true
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         let [[V_21:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:                         n = [[V_21]] - 1;
// CHECK-NEXT:                         [[V_21]] == 0
// CHECK-NEXT:                     };
// CHECK-NEXT:                     if [[V_20]] {
// CHECK-NEXT:                         break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     mode = 0;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 5 => {
// CHECK-NEXT:                     out += 11;
// CHECK-NEXT:                     out *= 2;
// CHECK-NEXT:                     let [[V_1]]: i32 = n;
// CHECK-NEXT:                     n = [[V_1]] - 1;
// CHECK-NEXT:                     if [[V_1]] == 0 {
// CHECK-NEXT:                         break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     mode = 3;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 6 => {
// CHECK-NEXT:                     out *= 2;
// CHECK-NEXT:                     let [[V_22:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:                     n = [[V_22]] - 1;
// CHECK-NEXT:                     if [[V_22]] == 0 {
// CHECK-NEXT:                         break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     mode = 3;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 _ => {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         #[cfg(all(not(feature = "alpha"), not(feature = "beta")))]
// CHECK-NEXT:         {
// CHECK-NEXT:             match mode {
// CHECK-NEXT:                 0 => {
// CHECK-NEXT:                     out += 1;
// CHECK-NEXT:                     let [[V_23:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:                     n = [[V_23]] - 1;
// CHECK-NEXT:                     if [[V_23]] == 0 {
// CHECK-NEXT:                         break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     out ^= 7;
// CHECK-NEXT:                     let [[V_18]]: i32 = n;
// CHECK-NEXT:                     n = [[V_18]] - 1;
// CHECK-NEXT:                     if [[V_18]] == 0 {
// CHECK-NEXT:                         break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     mode = 4;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 3 => {
// CHECK-NEXT:                     out ^= 7;
// CHECK-NEXT:                     let [[V_24:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:                     n = [[V_24]] - 1;
// CHECK-NEXT:                     if [[V_24]] == 0 {
// CHECK-NEXT:                         break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     mode = 4;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 4 => {
// CHECK-NEXT:                     out -= 2;
// CHECK-NEXT:                     let [[V_25:__v[0-9]+]]: bool = if out < -50 {
// CHECK-NEXT:                         true
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         let [[V_26:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:                         n = [[V_26]] - 1;
// CHECK-NEXT:                         [[V_26]] == 0
// CHECK-NEXT:                     };
// CHECK-NEXT:                     if [[V_25]] {
// CHECK-NEXT:                         break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     mode = 0;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 6 => {
// CHECK-NEXT:                     out *= 2;
// CHECK-NEXT:                     let [[V_27:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:                     n = [[V_27]] - 1;
// CHECK-NEXT:                     if [[V_27]] == 0 {
// CHECK-NEXT:                         break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     mode = 3;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 _ => {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     out
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut total: u32 = 0;
// CHECK-NEXT:     for m in 0i32..8i32 {
// CHECK-NEXT:         for n in 0i32..9i32 {
// CHECK-NEXT:             total = total.wrapping_mul(31).wrapping_add(run(m, n) as u32);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%u\n".as_ptr(), total) };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: use core::ffi::c_char;
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
