/* { dg-options "-std=c2y" } */
#include <stdio.h>

static int grid(int limit) {
    int hits = 0;
rows:
    for (int i = 0; i < 6; ++i) {
cols:
        for (int j = 0; j < 6; ++j) {
            if (j > i)
                continue rows;
            if (i * j > limit)
                break rows;
            if ((i + j) % 3 == 0)
                continue cols;
            hits += i * 10 + j;
        }
    }
    return hits;
}

static int dispatch(int n) {
    int total = 0;
    int k = 0;
outer:
    while (k < 8) {
        ++k;
choice:
        switch (n + k) {
        case 3:
            total += 1;
            break choice;
        case 5:
            total += 100;
            continue outer;
        case 9:
            total += 1000;
            break outer;
        default:
            break;
        }
        total += 10;
    }
    return total * 16 + k;
}

static int countdown(int n) {
    int steps = 0;
again:
    do {
        --n;
        ++steps;
        if (n % 2)
            continue again;
        if (n < 3)
            break again;
        steps += 5;
    } while (n > 0);
    return steps * 100 + n;
}

int main(void) {
    for (int limit = 0; limit < 30; limit += 7)
        printf("grid %d = %d\n", limit, grid(limit));
    for (int n = 0; n < 6; ++n)
        printf("dispatch %d = %d\n", n, dispatch(n));
    for (int n = 1; n < 12; n += 3)
        printf("countdown %d = %d\n", n, countdown(n));
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-std=c2y" }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn grid(limit: i32) -> i32 {
// CHECK-NEXT:     let mut j: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut i: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut hits: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         hits = 0;
// CHECK-NEXT:         i = 0;
// CHECK-NEXT:         '__loop_1: loop {
// CHECK-NEXT:             if i < 6 {
// CHECK-NEXT:                 j = 0;
// CHECK-NEXT:                 loop {
// CHECK-NEXT:                     if j < 6 {
// CHECK-NEXT:                         if j > i {
// CHECK-NEXT:                             break;
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             if i * j > limit {
// CHECK-NEXT:                                 break '__loop_1;
// CHECK-NEXT:                             } else {
// CHECK-NEXT:                                 if (i + j) % 3 != 0 {
// CHECK-NEXT:                                     hits += i * 10 + j;
// CHECK-NEXT:                                 }
// CHECK-NEXT:                                 j += 1;
// CHECK-NEXT:                             }
// CHECK-NEXT:                         }
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 i += 1;
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         return hits;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn dispatch(n: i32) -> i32 {
// CHECK-NEXT:     let mut k: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut total: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         total = 0;
// CHECK-NEXT:         k = 0;
// CHECK-NEXT:         '[[JOIN_0:__join_[0-9]+]]: {
// CHECK-NEXT:             loop {
// CHECK-NEXT:                 if k < 8 {
// CHECK-NEXT:                     k += 1;
// CHECK-NEXT:                     let [[T_0:__t[0-9]+]]: i32 = n + k;
// CHECK-NEXT:                     if [[T_0]] == 3 {
// CHECK-NEXT:                         total += 1;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         if [[T_0]] == 5 {
// CHECK-NEXT:                             total += 100;
// CHECK-NEXT:                             continue;
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             if [[T_0]] == 9 {
// CHECK-NEXT:                                 break;
// CHECK-NEXT:                             }
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                     total += 10;
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     break '[[JOIN_0]];
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             total += 1000;
// CHECK-NEXT:         }
// CHECK-NEXT:         return total * 16 + k;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn countdown(mut n: i32) -> i32 {
// CHECK-NEXT:     let mut steps: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         steps = 0;
// CHECK-NEXT:         loop {
// CHECK-NEXT:             n -= 1;
// CHECK-NEXT:             steps += 1;
// CHECK-NEXT:             if n % 2 == 0 {
// CHECK-NEXT:                 if n < 3 {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     steps += 5;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             if !(n > 0) {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         return steps * 100 + n;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut limit: i32 = 0;
// CHECK-NEXT:     while limit < 30 {
// CHECK-NEXT:         println!("grid {} = {}", limit, grid(limit));
// CHECK-NEXT:         limit += 7;
// CHECK-NEXT:     }
// CHECK-NEXT:     for n in 0i32..6i32 {
// CHECK-NEXT:         println!("dispatch {} = {}", n, dispatch(n));
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut n: i32 = 1;
// CHECK-NEXT:     while n < 12 {
// CHECK-NEXT:         println!("countdown {} = {}", n, countdown(n));
// CHECK-NEXT:         n += 3;
// CHECK-NEXT:     }
// CHECK-NEXT:     0
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
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
