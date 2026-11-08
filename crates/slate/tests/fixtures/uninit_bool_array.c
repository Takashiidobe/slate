#include <stdbool.h>
#include <stdio.h>

static int sieve(int limit) {
    bool composite[64];
    int count = 0;
    for (int i = 0; i < limit; i++)
        composite[i] = false;
    for (int i = 2; i < limit; i++) {
        if (composite[i])
            continue;
        count++;
        for (int j = i * i; j < limit; j += i)
            composite[j] = true;
    }
    return count;
}

int main(void) {
    bool flags[4];
    for (int i = 0; i < 4; i++)
        flags[i] = i % 2;
    int set = 0;
    for (int i = 0; i < 4; i++)
        set += flags[i];
    printf("%d %d\n", set, sieve(64));
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn sieve(limit: i32) -> i32 {
// CHECK-NEXT:     let mut composite: __SlateAlign16<[bool; 64]> = __SlateAlign16([false; 64]);
// CHECK-NEXT:     let mut count: i32 = 0;
// CHECK-NEXT:     for i in 0..limit {
// CHECK-NEXT:         composite.0[i as usize] = false;
// CHECK-NEXT:     }
// CHECK-NEXT:     for i in 2..limit {
// CHECK-NEXT:         if composite.0[i as usize] {
// CHECK-NEXT:             continue;
// CHECK-NEXT:         }
// CHECK-NEXT:         count += 1;
// CHECK-NEXT:         let mut j: i32 = i * i;
// CHECK-NEXT:         while j < limit {
// CHECK-NEXT:             composite.0[j as usize] = true;
// CHECK-NEXT:             j += i;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     count
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut flags: [bool; 4] = [false; 4];
// CHECK-NEXT:     for i in 0i32..4i32 {
// CHECK-NEXT:         flags[i as usize] = i % 2 != 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut set: i32 = 0;
// CHECK-NEXT:     for i in flags.iter().copied() {
// CHECK-NEXT:         set += i as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("{} {}", set, sieve(64i32));
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
