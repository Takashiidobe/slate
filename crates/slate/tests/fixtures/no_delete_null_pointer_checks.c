/* { dg-options "-fno-delete-null-pointer-checks" } */
#include <stdio.h>

struct pair {
    int first;
    int second;
};

struct __attribute__((packed)) wire {
    char tag;
    int length;
};

int load_then_check(int *p) {
    int value = *p;
    if (!p)
        return -1;
    return value;
}

int field_then_check(struct pair *pair) {
    int value = pair->second;
    if (pair == NULL)
        return -1;
    return value;
}

int index_then_check(const int *values, int count) {
    int total = values[count - 1];
    if (values)
        total += values[0];
    return total;
}

void store_then_check(int *out, int value) {
    *out = value;
    if (out != NULL)
        *out += 1;
}

int packed_then_check(struct wire *wire) {
    int length = wire->length;
    return wire ? length : -1;
}

int unchecked(int *p) {
    return *p + 1;
}

int call_or_zero(int (*callback)(int *), int *p) {
    return callback ? callback(p) : 0;
}

int main(void) {
    int values[3] = {4, 5, 6};
    struct pair pair = {7, 8};
    struct wire wire = {'w', 9};
    int out = 0;
    store_then_check(&out, 10);
    printf("%d %d %d %d %d %d %d\n", load_then_check(&values[1]), field_then_check(&pair),
           index_then_check(values, 3), out, packed_then_check(&wire),
           call_or_zero(unchecked, &values[0]), call_or_zero(NULL, &values[0]));
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-fno-delete-null-pointer-checks" }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct pair {
// CHECK-NEXT:     first: i32,
// CHECK-NEXT:     second: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct wire {
// CHECK-NEXT:     tag: i8,
// CHECK-NEXT:     length: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn load_then_check(p: *mut i32) -> i32 {
// CHECK-NEXT:     let value: i32 = unsafe { *p };
// CHECK-NEXT:     if !(!::slate_support::opaque_is_null(p)) {
// CHECK-NEXT:         return -1;
// CHECK-NEXT:     }
// CHECK-NEXT:     value
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn field_then_check(pair: *mut pair) -> i32 {
// CHECK-NEXT:     let value: i32 = unsafe { (*pair).second };
// CHECK-NEXT:     if ::slate_support::opaque_is_null(pair) {
// CHECK-NEXT:         return -1;
// CHECK-NEXT:     }
// CHECK-NEXT:     value
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn index_then_check(values: *const i32, count: i32) -> i32 {
// CHECK-NEXT:     let mut total: i32 = unsafe { *values.offset((count - 1) as isize) };
// CHECK-NEXT:     if !::slate_support::opaque_is_null(values) {
// CHECK-NEXT:         total += unsafe { *values.add(0) };
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn store_then_check(out: *mut i32, value: i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *out = value;
// CHECK-NEXT:     }
// CHECK-NEXT:     if !::slate_support::opaque_is_null(out) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *out = *out + 1;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn packed_then_check(wire: *mut wire) -> i32 {
// CHECK-NEXT:     let length: i32 = unsafe { (*wire).length };
// CHECK-NEXT:     if !::slate_support::opaque_is_null(wire) { length } else { -1 }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.unchecked")]
// CHECK-NEXT: extern "C-unwind" fn unchecked(p: *mut i32) -> i32 {
// CHECK-NEXT:     (unsafe { *p }) + 1
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn call_or_zero(
// CHECK-NEXT:     callback: Option<unsafe extern "C-unwind" fn(*mut i32) -> i32>,
// CHECK-NEXT:     p: *mut i32,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: i32 = if callback != None {
// CHECK-NEXT:         unsafe { callback.unwrap()(p) }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0
// CHECK-NEXT:     };
// CHECK-NEXT:     [[V_0]]
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut values: [i32; 3] = [4, 5, 6];
// CHECK-NEXT:     let mut pair: pair = pair { first: 7, second: 8 };
// CHECK-NEXT:     let mut wire: wire = wire { tag: 119, length: 9 };
// CHECK-NEXT:     let mut out: i32 = 0;
// CHECK-NEXT:     store_then_check(core::ptr::addr_of_mut!(out), 10i32);
// CHECK-NEXT:     let __slate_printf_arg0 = load_then_check(
// CHECK-NEXT:         unsafe { (values.as_mut_ptr() as *mut i32).add(1) },
// CHECK-NEXT:     );
// CHECK-NEXT:     let __slate_printf_arg2 = index_then_check(values.as_mut_ptr() as *const i32, 3i32);
// CHECK-NEXT:     let __slate_printf_arg5 = call_or_zero(
// CHECK-NEXT:         Some(unchecked),
// CHECK-NEXT:         unsafe { (values.as_mut_ptr() as *mut i32).add(0) },
// CHECK-NEXT:     );
// CHECK-NEXT:     let __slate_printf_arg6 = call_or_zero(
// CHECK-NEXT:         None,
// CHECK-NEXT:         unsafe { (values.as_mut_ptr() as *mut i32).add(0) },
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {} {} {} {}",
// CHECK-NEXT:         __slate_printf_arg0,
// CHECK-NEXT:         field_then_check(core::ptr::addr_of_mut!(pair)),
// CHECK-NEXT:         __slate_printf_arg2,
// CHECK-NEXT:         out,
// CHECK-NEXT:         packed_then_check(core::ptr::addr_of_mut!(wire)),
// CHECK-NEXT:         __slate_printf_arg5,
// CHECK-NEXT:         __slate_printf_arg6
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
