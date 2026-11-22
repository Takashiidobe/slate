#include <stdio.h>

struct Pair {
    int left;
    int right;
};

static int calls;

static int read_pair(struct Pair pair) {
    calls++;
    return pair.left + pair.right;
}

static int write_pair(struct Pair pair, int left) {
    calls += 10;
    pair.left = left;
    return pair.left + pair.right;
}

static int sum(const int *values, unsigned count) {
    int total = 0;
    for (unsigned i = 0; i < count; i++)
        total += values[i];
    return total;
}

static void fill(int *values, unsigned count) {
    for (unsigned i = 0; i < count; i++)
        values[i] = 2;
}

static int select_value(int choice, struct Pair pair) {
    int value;
    if (choice) {
        int total = read_pair(pair);
        value = total + 1;
    } else {
        value = write_pair(pair, 8);
    }
    return value;
}

int main(void) {
    int shared[2] = {2, 4};
    int exclusive[2] = {3, 5};
    struct Pair pair = {4, 7};
    fill(exclusive, 2);
    int zero = select_value(0, pair);
    int one = select_value(1, pair);
    printf("%d %d %d %d %d\n", sum(shared, 2), sum(exclusive, 2),
           zero, one, calls);
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Pair {
// CHECK-NEXT:     left: i32,
// CHECK-NEXT:     right: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut calls: i32 = unsafe { core::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn read_pair(pair: Pair) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         calls += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     pair.left + pair.right
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn write_pair(mut pair: Pair, left: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         calls += 10;
// CHECK-NEXT:     }
// CHECK-NEXT:     pair.left = left;
// CHECK-NEXT:     pair.left + pair.right
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn sum(values: &[i32]) -> i32 {
// CHECK-NEXT:     let count: u32 = values.len() as u32;
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     for i in 0..count {
// CHECK-NEXT:         total += values[i as usize];
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fill(values: &mut [i32]) {
// CHECK-NEXT:     let count: u32 = values.len() as u32;
// CHECK-NEXT:     for i in 0..count {
// CHECK-NEXT:         values[i as usize] = 2;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn select_value(choice: i32, pair: Pair) -> i32 {
// CHECK-NEXT:     let value: i32 = if choice != 0 {
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: i32 = read_pair(pair);
// CHECK-NEXT:         [[V_0]] + 1
// CHECK-NEXT:     } else {
// CHECK-NEXT:         write_pair(pair, 8i32)
// CHECK-NEXT:     };
// CHECK-NEXT:     value
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let shared: [i32; 2] = [2, 4];
// CHECK-NEXT:     let mut exclusive: [i32; 2] = [3, 5];
// CHECK-NEXT:     let pair: Pair = Pair { left: 4, right: 7 };
// CHECK-NEXT:     fill(exclusive.as_mut_slice());
// CHECK-NEXT:     let zero: i32 = select_value(0i32, pair);
// CHECK-NEXT:     let one: i32 = select_value(1i32, pair);
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {} {}",
// CHECK-NEXT:         sum(shared.as_slice()),
// CHECK-NEXT:         sum(exclusive.as_slice()),
// CHECK-NEXT:         zero,
// CHECK-NEXT:         one,
// CHECK-NEXT:         unsafe { calls }
// CHECK-NEXT:     );
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
