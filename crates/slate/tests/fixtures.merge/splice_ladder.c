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
// CHECK-NEXT: unit splice_ladder: 5 items, 0 gated, 0 duplicated functions
// CHECK-NEXT:   spliced fn:run: 4 variants
// CHECK-NEXT: macros: 0 function-like, 0 lifted at least once (expansions counted per distinct compile)
// CHECK-NEXT: projection: 12 checked, 0 not translatable, 0 mismatches
// CHECK-NEXT: signatures: 0 shared symbols, 0 mismatches
// CHECK-NEXT: exit: 0
// CHECK-NEXT: file main.rs
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(not(any(
// CHECK-NEXT:     all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"),
// CHECK-NEXT:     all(target_arch = "x86_64", windows, target_env = "msvc")
// CHECK-NEXT: )))]
// CHECK-NEXT: compile_error!("this crate was not translated for this target");
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn run(mut mode: i32, mut n: i32) -> i32 {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: bool = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut out: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         out = 0;
// CHECK-NEXT:         '__loop_14: loop {
// CHECK-NEXT:             #[cfg(all(not(feature = "alpha"), not(feature = "beta")))]
// CHECK-NEXT:             let [[T_0:__t[0-9]+]]: i32 = mode;
// CHECK-NEXT:             '[[JOIN_0:__join_[0-9]+]]: {
// CHECK-NEXT:                 '[[JOIN_1:__join_[0-9]+]]: {
// CHECK-NEXT:                     #[cfg(all(feature = "alpha", not(feature = "beta")))]
// CHECK-NEXT:                     let [[T_0]]: i32 = mode;
// CHECK-NEXT:                     '[[JOIN_2:__join_[0-9]+]]: {
// CHECK-NEXT:                         #[cfg(feature = "beta")]
// CHECK-NEXT:                         let [[T_0]]: i32 = mode;
// CHECK-NEXT:                         if [[T_0]] == 0 {
// CHECK-NEXT:                             out += 1;
// CHECK-NEXT:                             let [[V_1:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:                             n = [[V_1]] - 1;
// CHECK-NEXT:                             if [[V_1]] == 0 {
// CHECK-NEXT:                                 break '__loop_14;
// CHECK-NEXT:                             } else {
// CHECK-NEXT:                                 #[cfg(feature = "beta")]
// CHECK-NEXT:                                 {
// CHECK-NEXT:                                     break '[[JOIN_2]];
// CHECK-NEXT:                                 }
// CHECK-NEXT:                             }
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             #[cfg(all(feature = "alpha", feature = "beta"))]
// CHECK-NEXT:                             {
// CHECK-NEXT:                                 if [[T_0]] == 1 {
// CHECK-NEXT:                                     break '[[JOIN_2]];
// CHECK-NEXT:                                 } else {
// CHECK-NEXT:                                     if [[T_0]] == 2 {
// CHECK-NEXT:                                         break '[[JOIN_1]];
// CHECK-NEXT:                                     } else {
// CHECK-NEXT:                                         if [[T_0]] == 3 {
// CHECK-NEXT:                                             break '[[JOIN_0]];
// CHECK-NEXT:                                         } else {
// CHECK-NEXT:                                             if [[T_0]] == 4 {
// CHECK-NEXT:                                                 out -= 2;
// CHECK-NEXT:                                                 if out < -50 {
// CHECK-NEXT:                                                     [[V_0]] = true;
// CHECK-NEXT:                                                 } else {
// CHECK-NEXT:                                                     let [[V_2:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:                                                     n = [[V_2]] - 1;
// CHECK-NEXT:                                                     [[V_0]] = [[V_2]] == 0;
// CHECK-NEXT:                                                 }
// CHECK-NEXT:                                                 if [[V_0]] {
// CHECK-NEXT:                                                     break '__loop_14;
// CHECK-NEXT:                                                 } else {
// CHECK-NEXT:                                                     mode = 0;
// CHECK-NEXT:                                                     continue '__loop_14;
// CHECK-NEXT:                                                 }
// CHECK-NEXT:                                             } else {
// CHECK-NEXT:                                                 if [[T_0]] == 5 {
// CHECK-NEXT:                                                     out += 11;
// CHECK-NEXT:                                                 } else {
// CHECK-NEXT:                                                     if [[T_0]] != 6 {
// CHECK-NEXT:                                                         break '__loop_14;
// CHECK-NEXT:                                                     }
// CHECK-NEXT:                                                 }
// CHECK-NEXT:                                             }
// CHECK-NEXT:                                         }
// CHECK-NEXT:                                     }
// CHECK-NEXT:                                 }
// CHECK-NEXT:                             }
// CHECK-NEXT:                             #[cfg(all(feature = "alpha", not(feature = "beta")))]
// CHECK-NEXT:                             {
// CHECK-NEXT:                                 if [[T_0]] != 1 {
// CHECK-NEXT:                                     if [[T_0]] == 2 {
// CHECK-NEXT:                                         break '[[JOIN_1]];
// CHECK-NEXT:                                     } else {
// CHECK-NEXT:                                         if [[T_0]] == 3 {
// CHECK-NEXT:                                             break '[[JOIN_0]];
// CHECK-NEXT:                                         } else {
// CHECK-NEXT:                                             if [[T_0]] == 4 {
// CHECK-NEXT:                                                 out -= 2;
// CHECK-NEXT:                                                 if out < -50 {
// CHECK-NEXT:                                                     [[V_0]] = true;
// CHECK-NEXT:                                                 } else {
// CHECK-NEXT:                                                     let [[V_3:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:                                                     n = [[V_3]] - 1;
// CHECK-NEXT:                                                     [[V_0]] = [[V_3]] == 0;
// CHECK-NEXT:                                                 }
// CHECK-NEXT:                                                 if [[V_0]] {
// CHECK-NEXT:                                                     break '__loop_14;
// CHECK-NEXT:                                                 } else {
// CHECK-NEXT:                                                     mode = 0;
// CHECK-NEXT:                                                     continue '__loop_14;
// CHECK-NEXT:                                                 }
// CHECK-NEXT:                                             } else {
// CHECK-NEXT:                                                 if [[T_0]] == 6 {
// CHECK-NEXT:                                                     out *= 2;
// CHECK-NEXT:                                                     let [[V_4:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:                                                     n = [[V_4]] - 1;
// CHECK-NEXT:                                                     if [[V_4]] == 0 {
// CHECK-NEXT:                                                         break '__loop_14;
// CHECK-NEXT:                                                     } else {
// CHECK-NEXT:                                                         mode = 3;
// CHECK-NEXT:                                                         continue '__loop_14;
// CHECK-NEXT:                                                     }
// CHECK-NEXT:                                                 } else {
// CHECK-NEXT:                                                     break '__loop_14;
// CHECK-NEXT:                                                 }
// CHECK-NEXT:                                             }
// CHECK-NEXT:                                         }
// CHECK-NEXT:                                     }
// CHECK-NEXT:                                 }
// CHECK-NEXT:                             }
// CHECK-NEXT:                             #[cfg(all(not(feature = "alpha"), feature = "beta"))]
// CHECK-NEXT:                             {
// CHECK-NEXT:                                 if [[T_0]] == 3 {
// CHECK-NEXT:                                     break '[[JOIN_2]];
// CHECK-NEXT:                                 } else {
// CHECK-NEXT:                                     if [[T_0]] == 4 {
// CHECK-NEXT:                                         out -= 2;
// CHECK-NEXT:                                         if out < -50 {
// CHECK-NEXT:                                             [[V_0]] = true;
// CHECK-NEXT:                                         } else {
// CHECK-NEXT:                                             let [[V_5:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:                                             n = [[V_5]] - 1;
// CHECK-NEXT:                                             [[V_0]] = [[V_5]] == 0;
// CHECK-NEXT:                                         }
// CHECK-NEXT:                                         if [[V_0]] {
// CHECK-NEXT:                                             break '__loop_14;
// CHECK-NEXT:                                         } else {
// CHECK-NEXT:                                             mode = 0;
// CHECK-NEXT:                                             continue '__loop_14;
// CHECK-NEXT:                                         }
// CHECK-NEXT:                                     } else {
// CHECK-NEXT:                                         if [[T_0]] == 5 {
// CHECK-NEXT:                                             out += 11;
// CHECK-NEXT:                                         } else {
// CHECK-NEXT:                                             if [[T_0]] != 6 {
// CHECK-NEXT:                                                 break '__loop_14;
// CHECK-NEXT:                                             }
// CHECK-NEXT:                                         }
// CHECK-NEXT:                                     }
// CHECK-NEXT:                                 }
// CHECK-NEXT:                             }
// CHECK-NEXT:                             #[cfg(all(not(feature = "alpha"), not(feature = "beta")))]
// CHECK-NEXT:                             {
// CHECK-NEXT:                                 if [[T_0]] != 3 {
// CHECK-NEXT:                                     if [[T_0]] == 4 {
// CHECK-NEXT:                                         out -= 2;
// CHECK-NEXT:                                         if out < -50 {
// CHECK-NEXT:                                             [[V_0]] = true;
// CHECK-NEXT:                                         } else {
// CHECK-NEXT:                                             let [[V_6:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:                                             n = [[V_6]] - 1;
// CHECK-NEXT:                                             [[V_0]] = [[V_6]] == 0;
// CHECK-NEXT:                                         }
// CHECK-NEXT:                                         if [[V_0]] {
// CHECK-NEXT:                                             break '__loop_14;
// CHECK-NEXT:                                         } else {
// CHECK-NEXT:                                             mode = 0;
// CHECK-NEXT:                                             continue '__loop_14;
// CHECK-NEXT:                                         }
// CHECK-NEXT:                                     } else {
// CHECK-NEXT:                                         if [[T_0]] == 6 {
// CHECK-NEXT:                                             out *= 2;
// CHECK-NEXT:                                             let [[V_7:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:                                             n = [[V_7]] - 1;
// CHECK-NEXT:                                             if [[V_7]] == 0 {
// CHECK-NEXT:                                                 break '__loop_14;
// CHECK-NEXT:                                             } else {
// CHECK-NEXT:                                                 mode = 3;
// CHECK-NEXT:                                                 continue '__loop_14;
// CHECK-NEXT:                                             }
// CHECK-NEXT:                                         } else {
// CHECK-NEXT:                                             break '__loop_14;
// CHECK-NEXT:                                         }
// CHECK-NEXT:                                     }
// CHECK-NEXT:                                 }
// CHECK-NEXT:                             }
// CHECK-NEXT:                         }
// CHECK-NEXT:                         #[cfg(feature = "beta")]
// CHECK-NEXT:                         {
// CHECK-NEXT:                             out *= 2;
// CHECK-NEXT:                         }
// CHECK-NEXT:                         #[cfg(feature = "beta")]
// CHECK-NEXT:                         let [[V_8:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:                         #[cfg(feature = "beta")]
// CHECK-NEXT:                         {
// CHECK-NEXT:                             n = [[V_8]] - 1;
// CHECK-NEXT:                             if [[V_8]] == 0 {
// CHECK-NEXT:                                 break '__loop_14;
// CHECK-NEXT:                             } else {
// CHECK-NEXT:                                 mode = 3;
// CHECK-NEXT:                                 continue '__loop_14;
// CHECK-NEXT:                             }
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                     #[cfg(feature = "alpha")]
// CHECK-NEXT:                     {
// CHECK-NEXT:                         out *= 3;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     #[cfg(feature = "alpha")]
// CHECK-NEXT:                     let [[V_9:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:                     #[cfg(feature = "alpha")]
// CHECK-NEXT:                     {
// CHECK-NEXT:                         n = [[V_9]] - 1;
// CHECK-NEXT:                         if [[V_9]] == 0 {
// CHECK-NEXT:                             break '__loop_14;
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 #[cfg(feature = "alpha")]
// CHECK-NEXT:                 {
// CHECK-NEXT:                     out += 5;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             out ^= 7;
// CHECK-NEXT:             let [[V_10:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:             n = [[V_10]] - 1;
// CHECK-NEXT:             if [[V_10]] == 0 {
// CHECK-NEXT:                 #[cfg(any(feature = "alpha", feature = "beta"))]
// CHECK-NEXT:                 {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 #[cfg(all(not(feature = "alpha"), not(feature = "beta")))]
// CHECK-NEXT:                 {
// CHECK-NEXT:                     break '__loop_14;
// CHECK-NEXT:                 }
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 mode = 4;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         return out;
// CHECK-NEXT:     }
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
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut total: u32 = 0;
// CHECK-NEXT:     for m in 0i32..8i32 {
// CHECK-NEXT:         for n in 0i32..9i32 {
// CHECK-NEXT:             total = total.wrapping_mul(31).wrapping_add(run(m, n) as u32);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%u\n".as_ptr(), total) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
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
