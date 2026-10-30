#include <stdio.h>

/* typedef doc */
typedef struct Foo Foo;

/* struct doc */
struct Foo { int a; };

/* only typedef doc */
typedef struct Bar Bar;
struct Bar { int b; };

/* prototype doc */
int f(int x);
/* prototype only doc */
int h(int x);

/* definition doc */
int f(int x) { return x + 1; }
int h(int x) { return x * 2; }

extern int counter;
/* counter definition */
int counter = 4;

int main(void) {
    Foo foo = {1};
    Bar bar = {2};
    printf("%d %d %d\n", f(foo.a), h(bar.b), counter);
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: /// struct doc
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Foo {
// CHECK-NEXT:     a: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: /// only typedef doc
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Bar {
// CHECK-NEXT:     b: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: /// definition doc
// CHECK-NEXT: fn f(x: i32) -> i32 {
// CHECK-NEXT:     x + 1
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: /// prototype only doc
// CHECK-NEXT: fn h(x: i32) -> i32 {
// CHECK-NEXT:     x * 2
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: /// counter definition
// CHECK-NEXT: static mut counter: i32 = 4;
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let foo: Foo = Foo { a: 1 };
// CHECK-NEXT:     let bar: Bar = Bar { b: 2 };
// CHECK-NEXT:     println!("{} {} {}", f(foo.a), h(bar.b), unsafe { counter });
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
