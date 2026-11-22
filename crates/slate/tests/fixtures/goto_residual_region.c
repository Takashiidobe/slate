#include <stdio.h>

int irreducible(int n, int start) {
    int total = 0;
    for (int i = 0; i < 3; i++)
        total += i;
    if (start)
        goto b;
a:
    total += 1;
b:
    total += 2;
    if (--n > 0)
        goto a;
    for (int i = 0; i < 2; i++)
        total *= 2;
    return total;
}

int nested(int rows, int start) {
    int sum = 0;
    for (int r = 0; r < rows; r++) {
        int k = r;
        if (r == 4)
            break;
        if (start)
            goto inner;
    outer:
        sum += 10;
    inner:
        sum += 1;
        if (sum > 200)
            break;
        if (sum % 7 == 0)
            continue;
        if (k-- > 0)
            goto outer;
        sum += k;
    }
    return sum;
}

int declared(int n) {
    int out = 1;
    if (n & 1)
        goto odd;
even:
    out *= 2;
odd:;
    int seen = out + n;
    if (--n > 0)
        goto even;
    return seen * 3 + out;
}

int main(void) {
    printf("%d %d %d\n", irreducible(3, 0), irreducible(3, 1), irreducible(1, 1));
    printf("%d %d %d\n", nested(3, 0), nested(6, 1), nested(30, 0));
    printf("%d %d %d\n", declared(4), declared(5), declared(1));
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: fn irreducible(mut n: i32, start: i32) -> i32 {
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     for i in 0i32..3i32 {
// CHECK-NEXT:         total += i;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut __slate_state: usize = 3usize;
// CHECK-NEXT:     if start != 0 {
// CHECK-NEXT:         __slate_state = 1;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         __slate_state = 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         match __slate_state {
// CHECK-NEXT:             1 => {
// CHECK-NEXT:                 total += 2;
// CHECK-NEXT:                 let [[V_0:__v[0-9]+]]: i32 = n - 1;
// CHECK-NEXT:                 n = [[V_0]];
// CHECK-NEXT:                 if [[V_0]] > 0 {
// CHECK-NEXT:                     __slate_state = 2;
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             _ => {
// CHECK-NEXT:                 total += 1;
// CHECK-NEXT:                 __slate_state = 1;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     for i in 0i32..2i32 {
// CHECK-NEXT:         total *= 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn nested(rows: i32, start: i32) -> i32 {
// CHECK-NEXT:     let mut sum: i32 = 0;
// CHECK-NEXT:     let mut r: i32 = 0;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while r < rows {
// CHECK-NEXT:         '[[SLATE_CONTINUE_0:__slate_continue_[0-9]+]]: {
// CHECK-NEXT:             let mut k: i32 = r;
// CHECK-NEXT:             if r == 4 {
// CHECK-NEXT:                 break '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut __slate_state: usize = 7usize;
// CHECK-NEXT:             loop {
// CHECK-NEXT:                 match __slate_state {
// CHECK-NEXT:                     0 => {
// CHECK-NEXT:                         break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     1 => {
// CHECK-NEXT:                         let [[V_0]]: i32 = k;
// CHECK-NEXT:                         k = [[V_0]] - 1;
// CHECK-NEXT:                         if [[V_0]] > 0 {
// CHECK-NEXT:                             __slate_state = 6usize;
// CHECK-NEXT:                             continue;
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             __slate_state = 0usize;
// CHECK-NEXT:                             continue;
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                     2 => {
// CHECK-NEXT:                         break '[[SLATE_CONTINUE_0]];
// CHECK-NEXT:                     }
// CHECK-NEXT:                     3 => {
// CHECK-NEXT:                         if sum % 7 == 0 {
// CHECK-NEXT:                             __slate_state = 2usize;
// CHECK-NEXT:                             continue;
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             __slate_state = 1usize;
// CHECK-NEXT:                             continue;
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                     4 => {
// CHECK-NEXT:                         break '[[SLATE_BREAK_0]];
// CHECK-NEXT:                     }
// CHECK-NEXT:                     5 => {
// CHECK-NEXT:                         sum += 1;
// CHECK-NEXT:                         if sum > 200 {
// CHECK-NEXT:                             __slate_state = 4usize;
// CHECK-NEXT:                             continue;
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             __slate_state = 3usize;
// CHECK-NEXT:                             continue;
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                     6 => {
// CHECK-NEXT:                         sum += 10;
// CHECK-NEXT:                         __slate_state = 5usize;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     7 => {
// CHECK-NEXT:                         if start != 0 {
// CHECK-NEXT:                             __slate_state = 5usize;
// CHECK-NEXT:                             continue;
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             __slate_state = 6usize;
// CHECK-NEXT:                             continue;
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                     _ => {
// CHECK-NEXT:                         unreachable!();
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             sum += k;
// CHECK-NEXT:         }
// CHECK-NEXT:         r += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     sum
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn declared(mut n: i32) -> i32 {
// CHECK-NEXT:     let mut out: i32 = 1;
// CHECK-NEXT:     let mut seen: i32 = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     let mut __slate_state: usize = 4usize;
// CHECK-NEXT:     if n & 1 != 0 {
// CHECK-NEXT:         __slate_state = 1;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         __slate_state = 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         match __slate_state {
// CHECK-NEXT:             1 => {
// CHECK-NEXT:                 seen = out + n;
// CHECK-NEXT:                 let [[V_1:__v[0-9]+]]: i32 = n - 1;
// CHECK-NEXT:                 n = [[V_1]];
// CHECK-NEXT:                 if [[V_1]] > 0 {
// CHECK-NEXT:                     __slate_state = 2;
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             _ => {
// CHECK-NEXT:                 out *= 2;
// CHECK-NEXT:                 __slate_state = 1;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     seen * 3 + out
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}",
// CHECK-NEXT:         irreducible(3i32, 0i32),
// CHECK-NEXT:         irreducible(3i32, 1i32),
// CHECK-NEXT:         irreducible(1i32, 1i32)
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{} {} {}", nested(3i32, 0i32), nested(6i32, 1i32), nested(30i32, 0i32));
// CHECK-NEXT:     println!("{} {} {}", declared(4i32), declared(5i32), declared(1i32));
// CHECK-NEXT:     0
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
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
