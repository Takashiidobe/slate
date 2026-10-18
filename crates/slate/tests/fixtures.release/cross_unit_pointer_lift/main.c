#include <stdio.h>

struct counter {
    int value;
};

void counter_add(struct counter *c, int by);
int counter_get(const struct counter *c);
int counter_apply(int (*fn)(struct counter *), struct counter *c);

int doubled(struct counter *c) { return c->value * 2; }

void add_twice(struct counter *c, int by) {
    counter_add(c, by);
    counter_add(c, by);
}

int main(void) {
    struct counter local = {1};
    add_twice(&local, 2);
    printf("%d\n", counter_get(&local));
    printf("%d\n", counter_apply(doubled, &local));
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: mod __slate_types;
// CHECK-NEXT: #[path = "counter.rs"]
// CHECK-NEXT: mod __slate_unit_counter;
// CHECK-EMPTY:
// CHECK-NEXT: use crate::__slate_types::counter;
// CHECK-NEXT: use crate::__slate_unit_counter::counter_add;
// CHECK-NEXT: use crate::__slate_unit_counter::counter_apply;
// CHECK-NEXT: use crate::__slate_unit_counter::counter_get;
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.doubled")]
// CHECK-NEXT: extern "C-unwind" fn doubled(c: *mut counter) -> i32 {
// CHECK-NEXT:     (unsafe { (*c).value }) * 2
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn add_twice(c: &mut counter, by: i32) {
// CHECK-NEXT:     unsafe { counter_add(c, by) };
// CHECK-NEXT:     unsafe { counter_add(c, by) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut local: counter = counter { value: 1 };
// CHECK-NEXT:     add_twice(unsafe { &mut (*std::ptr::addr_of_mut!(local)) }, 2 as i32);
// CHECK-NEXT:     println!("{}", unsafe {
// CHECK-NEXT:         counter_get(&(*(std::ptr::addr_of_mut!(local) as *const counter)))
// CHECK-NEXT:     });
// CHECK-NEXT:     println!("{}", unsafe {
// CHECK-NEXT:         counter_apply(Some(doubled), std::ptr::addr_of_mut!(local))
// CHECK-NEXT:     });
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
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
