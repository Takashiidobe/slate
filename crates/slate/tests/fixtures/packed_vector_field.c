#include <stdio.h>
#include <stdlib.h>

typedef long long V __attribute__((vector_size(16), may_alias));

struct __attribute__((packed)) T {
    char c;
    V v;
    int words[3];
};

static void bump(struct T *t) {
    t->v[1] += 5;
    t->words[1] = t->words[0] + 1;
}

int main(void) {
    struct T *t = calloc(2, sizeof(struct T));
    V seed = {3, 4};
    t->v = seed;
    t->words[0] = 9;
    bump(t);
    int *tail = t->words;
    printf("%lld %lld %d %d %d\n", t->v[0], t->v[1], t->words[0], t->words[1], *tail);
    free(t);
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn calloc(__nmemb: u64, __size: u64) -> *mut ();
// CHECK-NEXT:     fn free(__ptr: *mut ());
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct T {
// CHECK-NEXT:     c: i8,
// CHECK-NEXT:     v: std::simd::Simd<i64, 2>,
// CHECK-NEXT:     words: [i32; 3],
// CHECK-NEXT: }
// CHECK-NEXT: fn bump(t: *mut T) {
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: i32 = 1;
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: i64 = unsafe {
// CHECK-NEXT:         std::ptr::addr_of!((* t).v).read_unaligned()[[[V_0]] as usize]
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_2:__v[0-9]+]]: i64 = [[V_1]] + 5;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]] = [[V_2]];
// CHECK-NEXT:         let [[T_1:__t[0-9]+]] = std::ptr::addr_of_mut!((* t).v);
// CHECK-NEXT:         let mut [[T_2:__t[0-9]+]] = [[T_1]].read_unaligned();
// CHECK-NEXT:         [[T_2]][[[V_0]] as usize] = [[T_0]];
// CHECK-NEXT:         [[T_1]].write_unaligned([[T_2]]);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!((* t).words) as *mut i32 }.offset(1 as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:             .write_unaligned(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         unsafe { std::ptr::addr_of_mut!((* t).words) as *mut i32 }
// CHECK-NEXT:                             .offset(0 as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                         .read_unaligned()
// CHECK-NEXT:                 }) + 1,
// CHECK-NEXT:             );
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let t: *mut T = (unsafe { calloc(2 as u64, 29 as u64) }) as *mut T;
// CHECK-NEXT:     let seed: std::simd::Simd<i64, 2> = std::simd::Simd::from_array([
// CHECK-NEXT:         3 as i64,
// CHECK-NEXT:         4 as i64,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*t).v = seed;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!((* t).words) as *mut i32 }.offset(0 as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:             .write_unaligned(9 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     bump(t);
// CHECK-NEXT:     let tail: *mut i32 = unsafe { std::ptr::addr_of_mut!((* t).words) as *mut i32 };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%lld %lld %d %d %d\n".as_ptr(),
// CHECK-NEXT:             unsafe { std::ptr::addr_of!((* t).v).read_unaligned()[0] },
// CHECK-NEXT:             unsafe { std::ptr::addr_of!((* t).v).read_unaligned()[1] },
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!((* t).words) as *mut i32 }
// CHECK-NEXT:                         .offset(0 as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                     .read_unaligned()
// CHECK-NEXT:             },
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!((* t).words) as *mut i32 }
// CHECK-NEXT:                         .offset(1 as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                     .read_unaligned()
// CHECK-NEXT:             },
// CHECK-NEXT:             unsafe { *tail },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     unsafe { free(t as *mut ()) };
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
