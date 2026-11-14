#include <stdio.h>

struct obj { int v; };
struct list { long n; int **items; };

static int sum(struct obj **args, int n) {
    int total = 0;
    for (int i = 0; i < n; i++)
        if (args[i]) total += args[i]->v;
    return total;
}

static int fire(struct obj *retval) {
    struct obj *args[2] = { NULL, retval };
    return sum(args, 2);
}

static long count(struct list *l) { return l->n + **l->items; }

static long wrap(long length, int **items) {
    struct list l = {.n = length, .items = items};
    return count(&l);
}

int main(void) {
    struct obj o = {7};
    int x = 3;
    int *p = &x;
    printf("%d %ld\n", fire(&o), wrap(2, &p));
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct obj {
// CHECK-NEXT:     v: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct list {
// CHECK-NEXT:     n: i64,
// CHECK-NEXT:     items: *mut *mut i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn sum(args: &[*mut obj]) -> i32 {
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     for i in 0..args.len() as i32 {
// CHECK-NEXT:         if !unsafe { args[i as usize] }.is_null() {
// CHECK-NEXT:             total += unsafe { (*args[i as usize]).v };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn fire(retval: *mut obj) -> i32 {
// CHECK-NEXT:     let args: __SlateAlign16<[*mut obj; 2]> = __SlateAlign16([
// CHECK-NEXT:         std::ptr::null_mut::<obj>(),
// CHECK-NEXT:         retval,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     sum(&args.0)
// CHECK-NEXT: }
// CHECK-NEXT: fn count(l: &list) -> i64 {
// CHECK-NEXT:     (unsafe { (*(l as *const list)).n })
// CHECK-NEXT:         + ((unsafe { *(*(*(l as *const list)).items) }) as i64)
// CHECK-NEXT: }
// CHECK-NEXT: fn wrap(length: i64, items: *mut *mut i32) -> i64 {
// CHECK-NEXT:     let mut l: list = list { n: length, items: items };
// CHECK-NEXT:     count(unsafe { &(*std::ptr::addr_of_mut!(l)) })
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut o: obj = obj { v: 7 };
// CHECK-NEXT:     let mut x: i32 = 3;
// CHECK-NEXT:     let mut p: *mut i32 = std::ptr::addr_of_mut!(x);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%d %ld\n".as_ptr(),
// CHECK-NEXT:             fire(std::ptr::addr_of_mut!(o)),
// CHECK-NEXT:             wrap(2i64, std::ptr::addr_of_mut!(p)),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
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
