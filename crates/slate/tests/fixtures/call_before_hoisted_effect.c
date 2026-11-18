#include <stdio.h>

int g;
int log_index;
int calls[8];

static int observe(int tag) {
    calls[log_index++] = tag * 10 + g;
    return g;
}

static int pair(int a, int b) { return a * 100 + b; }

int main(void) {
    int sum = observe(1) + (g = 1);
    int cmp = observe(2) < (g = 5);
    int args = pair(observe(3), g = 7);
    int post = observe(4) + g++;
    int arr[2] = {observe(5), (g = 9)};
    printf("%d %d %d %d %d %d\n", sum, cmp, args, post, arr[0], arr[1]);
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
// CHECK-NEXT: static mut calls: __SlateAlign16<[i32; 8]> = __SlateAlign16(unsafe {
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
// CHECK-NEXT: fn pair(a: i32, b: i32) -> i32 {
// CHECK-NEXT:     a * 100 + b
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut sum: i32 = 0;
// CHECK-NEXT:     let [[V_0]]: i32 = observe(1i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         g = 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     sum = [[V_0]] + 1;
// CHECK-NEXT:     let mut cmp: i32 = 0;
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: i32 = observe(2i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         g = 5;
// CHECK-NEXT:     }
// CHECK-NEXT:     cmp = ([[V_1]] < 5) as i32;
// CHECK-NEXT:     let mut args: i32 = 0;
// CHECK-NEXT:     let [[V_2:__v[0-9]+]]: i32 = observe(3i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         g = 7;
// CHECK-NEXT:     }
// CHECK-NEXT:     args = pair([[V_2]], 7i32);
// CHECK-NEXT:     let mut post: i32 = 0;
// CHECK-NEXT:     let [[V_3:__v[0-9]+]]: i32 = observe(4i32);
// CHECK-NEXT:     let [[V_4:__v[0-9]+]]: i32 = unsafe { g };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         g = [[V_4]] + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     post = [[V_3]] + [[V_4]];
// CHECK-NEXT:     let mut arr: [i32; 2] = [0i32; 2];
// CHECK-NEXT:     let [[V_5:__v[0-9]+]]: i32 = observe(5i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         g = 9;
// CHECK-NEXT:     }
// CHECK-NEXT:     arr = [[[V_5]], 9];
// CHECK-NEXT:     println!("{} {} {} {} {} {}", sum, cmp, args, post, arr[0], arr[1]);
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
