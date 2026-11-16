#include "api.h"

int add(int a, int b) { return a + b; }

int sub(int a, int b) { return a - b; }

void bump(struct counter *c, int by) { c->value += by; }

const char *name(void) { return "routines"; }

const struct api_routines api = {add, sub, bump, name};

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}use crate::__slate_types::api_routines;
// CHECK-NEXT: use crate::__slate_types::counter;
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(transparent)]
// CHECK-NEXT: struct __SlateConst<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: unsafe impl<T> Sync for __SlateConst<T> {}
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.routines.add")]
// CHECK-NEXT: pub extern "C-unwind" fn add(a: i32, b: i32) -> i32 {
// CHECK-NEXT:     a + b
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.routines.sub")]
// CHECK-NEXT: pub extern "C-unwind" fn sub(a: i32, b: i32) -> i32 {
// CHECK-NEXT:     a - b
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: pub extern "C-unwind" fn bump([[V_0:__v[0-9]+]]: *mut counter, by: i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*[[V_0]]).value = (*[[V_0]]).value + by;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: pub extern "C-unwind" fn name() -> *const i8 {
// CHECK-NEXT:     c"routines".as_ptr()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: static api: __SlateConst<api_routines> = __SlateConst(api_routines {
// CHECK-NEXT:     add: Some(add),
// CHECK-NEXT:     sub: Some(sub),
// CHECK-NEXT:     bump: Some(bump),
// CHECK-NEXT:     name: Some(name),
// CHECK-NEXT: });
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
