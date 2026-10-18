struct counter {
    int value;
};

void counter_add(struct counter *c, int by) { c->value += by; }

int counter_get(const struct counter *c) { return c->value; }

int counter_apply(int (*fn)(struct counter *), struct counter *c) { return fn(c); }

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct counter {
// CHECK-NEXT:     value: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn counter_add(c: &mut counter, by: i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*c).value = (*c).value + by;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn counter_get(c: &counter) -> i32 {
// CHECK-NEXT:     unsafe { (*(c as *const counter)).value }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn counter_apply(
// CHECK-NEXT:     r#fn: Option<unsafe extern "C-unwind" fn(*mut counter) -> i32>,
// CHECK-NEXT:     c: *mut counter,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { r#fn.unwrap()(c) }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
