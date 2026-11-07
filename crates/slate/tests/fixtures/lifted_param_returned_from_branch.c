#include <stdlib.h>

static void *g_shared;

static void *pick(void *fresh) {
    void *previous = g_shared;
    return previous ? previous : fresh;
}

static int *pick_int(int *fresh, int which) {
    switch (which) {
    case 0:
        return (int *)g_shared;
    default:
        return which > 1 ? fresh : (int *)0;
    }
}

int main(void) {
    int a = 1, b = 2;
    if (pick(&a) != &a)
        abort();
    g_shared = &b;
    if (pick(&a) != &b || pick_int(&a, 2) != &a || pick_int(&a, 1) || pick_int(&a, 0) != &b)
        abort();
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: static mut g_shared: *mut () = unsafe { std::mem::zeroed() };
// CHECK-NEXT: use slate_support::abort;
// CHECK-NEXT: fn pick(fresh: &()) -> *mut () {
// CHECK-NEXT:     let previous: *mut () = unsafe { g_shared };
// CHECK-NEXT:     if !previous.is_null() { previous } else { (fresh as *const ()) as *mut () }
// CHECK-NEXT: }
// CHECK-NEXT: fn pick_int(fresh: &i32, which: i32) -> *mut i32 {
// CHECK-NEXT:     match which {
// CHECK-NEXT:         0 => {
// CHECK-NEXT:             return (unsafe { g_shared }) as *mut i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {
// CHECK-NEXT:             return if which > 1 {
// CHECK-NEXT:                 (fresh as *const i32) as *mut i32
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 std::ptr::null_mut::<i32>()
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: i32 = 1;
// CHECK-NEXT:     let mut b: i32 = 2;
// CHECK-NEXT:     if pick(unsafe { &(*(std::ptr::addr_of_mut!(a) as *mut ())) })
// CHECK-NEXT:         != (std::ptr::addr_of_mut!(a) as *mut ())
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { std::process::abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         g_shared = std::ptr::addr_of_mut!(b) as *mut ();
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: bool = pick(unsafe { &(*(std::ptr::addr_of_mut!(a) as *mut ())) })
// CHECK-NEXT:         != (std::ptr::addr_of_mut!(b) as *mut ())
// CHECK-NEXT:         || pick_int(unsafe { &(*std::ptr::addr_of_mut!(a)) }, 2i32)
// CHECK-NEXT:             != std::ptr::addr_of_mut!(a);
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: bool = [[V_0]]
// CHECK-NEXT:         || !pick_int(unsafe { &(*std::ptr::addr_of_mut!(a)) }, 1i32).is_null();
// CHECK-NEXT:     let [[V_2:__v[0-9]+]]: bool = [[V_1]]
// CHECK-NEXT:         || pick_int(unsafe { &(*std::ptr::addr_of_mut!(a)) }, 0i32)
// CHECK-NEXT:             != std::ptr::addr_of_mut!(b);
// CHECK-NEXT:     if [[V_2]] {
// CHECK-NEXT:         unsafe { std::process::abort() };
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
