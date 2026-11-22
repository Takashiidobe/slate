#include <stdio.h>

void release(int *p) { printf("release %d\n", *p); }

int into_block(int skip) {
    int result = 1;
    if (skip)
        goto entered;
    {
        int value = result * 2;
    entered:
        value = 3;
        result += value;
    }
    return result;
}

int into_then(int a, int b) {
    int total = 0;
    if (b)
        goto inside;
    if (a > 2) {
        total += 10;
    inside:
        total += 1;
    } else {
        total += 100;
    }
    return total;
}

int into_else_from_then(int x) {
    int hits = 0;
    if (x == 1)
        goto other;
    else {
    other:
        hits += 1;
    }
    return hits;
}

int into_returning_arm(int a, int b) {
    if (b)
        goto bail;
    if (a) {
        printf("a\n");
    bail:
        return -1;
    }
    return a;
}

int into_scoped_tail(int a) {
    if (a)
        goto entered;
    {
        printf("prefix\n");
    entered:;
        int guard __attribute__((cleanup(release))) = a;
        printf("tail\n");
    }
    printf("after\n");
    return a;
}

int forwarded(int a) {
    if (a)
        goto first;
    a += 5;
    goto done;
first:
second:
    goto done;
done:
    return a;
}

int main(void) {
    printf("%d %d\n", into_block(0), into_block(1));
    printf("%d %d %d\n", into_then(3, 0), into_then(1, 0), into_then(0, 1));
    printf("%d %d\n", into_else_from_then(1), into_else_from_then(2));
    printf("%d %d %d\n", into_returning_arm(1, 0), into_returning_arm(0, 0), into_returning_arm(0, 1));
    printf("%d %d\n", into_scoped_tail(0), into_scoped_tail(4));
    printf("%d %d\n", forwarded(0), forwarded(1));
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: fn release(p: *mut i32) {
// CHECK-NEXT:     println!("release {}", unsafe { *p });
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn into_block(skip: i32) -> i32 {
// CHECK-NEXT:     let mut result: i32 = 1;
// CHECK-NEXT:     let mut value: i32 = 0;
// CHECK-NEXT:     if skip == 0 {
// CHECK-NEXT:         value = result * 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     value = 3;
// CHECK-NEXT:     result += value;
// CHECK-NEXT:     result
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn into_then(a: i32, b: i32) -> i32 {
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: {
// CHECK-NEXT:         if b == 0 {
// CHECK-NEXT:             if a > 2 {
// CHECK-NEXT:                 total += 10;
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 total += 100;
// CHECK-NEXT:                 break '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         total += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn into_else_from_then(x: i32) -> i32 {
// CHECK-NEXT:     let mut hits: i32 = 0;
// CHECK-NEXT:     hits += 1;
// CHECK-NEXT:     hits
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn into_returning_arm(a: i32, b: i32) -> i32 {
// CHECK-NEXT:     '[[SLATE_BREAK_0]]: {
// CHECK-NEXT:         if b == 0 {
// CHECK-NEXT:             if a != 0 {
// CHECK-NEXT:                 println!("a");
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 break '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         return -1;
// CHECK-NEXT:     }
// CHECK-NEXT:     a
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn into_scoped_tail(a: i32) -> i32 {
// CHECK-NEXT:     if a == 0 {
// CHECK-NEXT:         println!("prefix");
// CHECK-NEXT:     }
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut guard: i32 = a;
// CHECK-NEXT:         let __slate_cleanup_ptr_1 = core::ptr::addr_of_mut!(guard) as *mut _;
// CHECK-NEXT:         let __slate_cleanup_1 = ::slate_support::Guard(
// CHECK-NEXT:             &mut || release(__slate_cleanup_ptr_1 as *mut _),
// CHECK-NEXT:         );
// CHECK-NEXT:         println!("tail");
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("after");
// CHECK-NEXT:     a
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn forwarded(mut a: i32) -> i32 {
// CHECK-NEXT:     if a == 0 {
// CHECK-NEXT:         a += 5;
// CHECK-NEXT:     }
// CHECK-NEXT:     a
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{} {}", into_block(0i32), into_block(1i32));
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}",
// CHECK-NEXT:         into_then(3i32, 0i32),
// CHECK-NEXT:         into_then(1i32, 0i32),
// CHECK-NEXT:         into_then(0i32, 1i32)
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{} {}", into_else_from_then(1i32), into_else_from_then(2i32));
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}",
// CHECK-NEXT:         into_returning_arm(1i32, 0i32),
// CHECK-NEXT:         into_returning_arm(0i32, 0i32),
// CHECK-NEXT:         into_returning_arm(0i32, 1i32)
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{} {}", into_scoped_tail(0i32), into_scoped_tail(4i32));
// CHECK-NEXT:     println!("{} {}", forwarded(0i32), forwarded(1i32));
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
