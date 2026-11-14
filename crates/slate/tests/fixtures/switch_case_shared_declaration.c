#include <stdio.h>

static int branch(int kind, int next) {
    switch (kind) {
    case 0:
    case 1:
        next += 1;
        int taken = next + 1;
        return taken * 2;
    case 2:
        taken = next + 5;
        return taken * 3;
    default:
        return 0;
    }
}

static int fallthrough(int kind) {
    int total = 0;
    switch (kind) {
    case 0:
        total += 1;
        int shared = 10;
        total += shared;
    case 1:
        shared = 20;
        total += shared;
        break;
    case 2: {
        int inner = 7;
        total += inner;
        break;
    }
    }
    return total;
}

int main(void) {
    for (int kind = 0; kind < 4; kind++) {
        printf("%d %d\n", branch(kind, 2), fallthrough(kind));
    }
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: fn branch(kind: i32, mut next: i32) -> i32 {
// CHECK-NEXT:     let mut taken: i32 = 0;
// CHECK-NEXT:     match kind {
// CHECK-NEXT:         0 | 1 => {
// CHECK-NEXT:             next += 1;
// CHECK-NEXT:             taken = next + 1;
// CHECK-NEXT:             return taken * 2;
// CHECK-NEXT:         }
// CHECK-NEXT:         2 => {
// CHECK-NEXT:             taken = next + 5;
// CHECK-NEXT:             return taken * 3;
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {
// CHECK-NEXT:             return 0;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn fallthrough(kind: i32) -> i32 {
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     let mut shared: i32 = 0;
// CHECK-NEXT:     match kind {
// CHECK-NEXT:         0 => {
// CHECK-NEXT:             total += 1;
// CHECK-NEXT:             shared = 10;
// CHECK-NEXT:             total += shared;
// CHECK-NEXT:             shared = 20;
// CHECK-NEXT:             let [[V_0:_v[0-9]+]]: i32 = total;
// CHECK-NEXT:             let [[V_1:_v[0-9]+]]: i32 = [[V_0]] + shared;
// CHECK-NEXT:             total = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:         1 => {
// CHECK-NEXT:             shared = 20;
// CHECK-NEXT:             total += shared;
// CHECK-NEXT:         }
// CHECK-NEXT:         2 => {
// CHECK-NEXT:             let inner: i32 = 7;
// CHECK-NEXT:             total += inner;
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {}
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     for kind in 0i32..4i32 {
// CHECK-NEXT:         println!("{} {}", branch(kind, 2i32), fallthrough(kind));
// CHECK-NEXT:     }
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
