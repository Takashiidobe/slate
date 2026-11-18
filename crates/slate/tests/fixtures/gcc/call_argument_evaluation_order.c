#include <stdio.h>

int g;
int log_index;
int calls[16];

static int observe(int tag) {
    calls[log_index++] = tag * 10 + g;
    return g;
}

static int pair(int a, int b) { return a * 100 + b; }

static int (*pick(int tag))(int, int) {
    calls[log_index++] = tag * 10 + g;
    return pair;
}

int main(void) {
    int sum = observe(1) + (g = 1);
    int args = pair(observe(2), g = 2);
    int both = pair(observe(3), observe(4));
    int indirect = pick(5)(observe(6), g = 3);
    int nested = pair(observe(7) + (g = 4), observe(8));
    int arr[2] = {observe(9), (g = 5)};
    printf("%d %d %d %d %d %d %d\n", sum, args, both, indirect, nested, arr[0], arr[1]);
    for (int i = 0; i < log_index; i++)
        printf("%d ", calls[i]);
    printf("\n");
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: unsafe impl<T> Sync for __SlateAlign16<T> {}
// CHECK-NEXT: static mut g: i32 = unsafe { core::mem::zeroed() };
// CHECK-NEXT: static mut log_index: i32 = unsafe { core::mem::zeroed() };
// CHECK-NEXT: static mut calls: __SlateAlign16<[i32; 16]> = __SlateAlign16(unsafe {
// CHECK-NEXT:     core::mem::zeroed()
// CHECK-NEXT: });
// CHECK-NEXT: fn observe(tag: i32) -> i32 {
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: i32 = unsafe { log_index };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         log_index = [[V_0]] + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         calls.0[[[V_0]] as usize] = tag * 10 + g;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { g }
// CHECK-NEXT: }
// CHECK-NEXT: extern "C-unwind" fn pair(a: i32, b: i32) -> i32 {
// CHECK-NEXT:     a * 100 + b
// CHECK-NEXT: }
// CHECK-NEXT: fn pick(tag: i32) -> Option<unsafe extern "C-unwind" fn(i32, i32) -> i32> {
// CHECK-NEXT:     let [[V_0]]: i32 = unsafe { log_index };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         log_index = [[V_0]] + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         calls.0[[[V_0]] as usize] = tag * 10 + g;
// CHECK-NEXT:     }
// CHECK-NEXT:     Some(pair)
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut sum: i32 = 0;
// CHECK-NEXT:     let [[V_0]]: i32 = observe(1i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         g = 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     sum = [[V_0]] + 1;
// CHECK-NEXT:     let mut args: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         g = 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     args = pair(observe(2i32), 2i32);
// CHECK-NEXT:     let mut both: i32 = 0;
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: i32 = observe(4i32);
// CHECK-NEXT:     both = pair(observe(3i32), [[V_1]]);
// CHECK-NEXT:     let mut indirect: i32 = 0;
// CHECK-NEXT:     let [[V_2:__v[0-9]+]]: Option<unsafe extern "C-unwind" fn(i32, i32) -> i32> = pick(5i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         g = 3;
// CHECK-NEXT:     }
// CHECK-NEXT:     indirect = unsafe { [[V_2]].unwrap()(observe(6i32), 3i32) };
// CHECK-NEXT:     let mut nested: i32 = 0;
// CHECK-NEXT:     let [[V_3:__v[0-9]+]]: i32 = observe(8i32);
// CHECK-NEXT:     let [[V_4:__v[0-9]+]]: i32 = observe(7i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         g = 4;
// CHECK-NEXT:     }
// CHECK-NEXT:     nested = pair([[V_4]] + 4, [[V_3]]);
// CHECK-NEXT:     let mut arr: [i32; 2] = [0i32; 2];
// CHECK-NEXT:     let [[V_5:__v[0-9]+]]: i32 = observe(9i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         g = 5;
// CHECK-NEXT:     }
// CHECK-NEXT:     arr = [[[V_5]], 5];
// CHECK-NEXT:     println!("{} {} {} {} {} {} {}", sum, args, both, indirect, nested, arr[0], arr[1]);
// CHECK-NEXT:     let mut i: i32 = 0;
// CHECK-NEXT:     while i < unsafe { log_index } {
// CHECK-NEXT:         print!("{} ", unsafe { calls.0[i as usize] });
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         i += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("");
// CHECK-NEXT:     0
// CHECK-NEXT: }
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
