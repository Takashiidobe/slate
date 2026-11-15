// { dg-options "-O2" }
#include <stdio.h>

struct key {
    int enabled;
};

struct key k1 = {1};

static inline __attribute__((always_inline)) int branch(struct key *key) {
    __asm__ goto("jmp %l[yes]\n\t# %c0" : : "i"(key) : : yes);
    return 0;
yes:
    return 1;
}

static inline __attribute__((always_inline)) void unlock(int *p) {
    if (branch(&k1))
        printf("unlock %d\n", *p);
}

static inline __attribute__((always_inline)) void release(int **p) {
    unlock(*p);
}

static int guarded(int *x) {
    int *guard __attribute__((cleanup(release))) = x;
    printf("held %d\n", *guard);
    return *guard + 1;
}

int main(void) {
    int value = 4;
    printf("%d\n", guarded(&value));
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-O2" }
// CHECK-NEXT: mod __slate_cleanup {
// CHECK-NEXT:     pub struct Guard<F: FnMut()>(pub F);
// CHECK-EMPTY:
// CHECK-NEXT:     impl<F: FnMut()> Drop for Guard<F> {
// CHECK-NEXT:         fn drop(&mut self) {
// CHECK-NEXT:             (self.0)()
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct key {
// CHECK-NEXT:     enabled: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut k1: key = key { enabled: 1i32 };
// CHECK-EMPTY:
// CHECK-NEXT: fn branch__const1(key: *mut key) -> i32 {
// CHECK-NEXT:     let mut __slate_state: usize = 2usize;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             match __slate_state {
// CHECK-NEXT:                 0 => {
// CHECK-NEXT:                     return 1;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 1 => {
// CHECK-NEXT:                     return 0;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 2 => {
// CHECK-NEXT:                     __slate_state = 1usize;
// CHECK-NEXT:                     core::arch::asm!("jmp {1}\n\t# {0}", sym k1, label {
// CHECK-NEXT: __slate_state = 0usize;
// CHECK-NEXT: }, options(nostack, att_syntax));
// CHECK-NEXT:                     continue;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 _ => {
// CHECK-NEXT:                     unreachable!();
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { core::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn unlock(p: *mut i32) {
// CHECK-NEXT:     if branch__const1(unsafe { core::ptr::addr_of_mut!(k1) }) != 0 {
// CHECK-NEXT:         println!("unlock {}", unsafe { *p });
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn release(p: *mut *mut i32) {
// CHECK-NEXT:     unlock(unsafe { *p });
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn guarded(mut guard: *mut i32) -> i32 {
// CHECK-NEXT:     let __slate_cleanup_ptr_0 = core::ptr::addr_of_mut!(guard) as *mut _;
// CHECK-NEXT:     let __slate_cleanup_0 = __slate_cleanup::Guard(|| release(__slate_cleanup_ptr_0 as *mut _));
// CHECK-NEXT:     println!("held {}", unsafe { *guard });
// CHECK-NEXT:     (unsafe { *guard }) + 1
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut value: i32 = 4;
// CHECK-NEXT:     println!("{}", guarded(core::ptr::addr_of_mut!(value)));
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
