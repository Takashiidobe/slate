/* { dg-options "-fdollars-in-identifiers" } */
int printf(const char *, ...);

static int $counter = 3;

static int add$one(int value$in) { return value$in + 1; }

int main(void) {
  int local$ = add$one($counter);
  printf("%d\n", local$);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-options "-fdollars-in-identifiers" }
// CHECK-NEXT: static mut _dollar_counter: i32 = 3;
// CHECK-EMPTY:
// CHECK-NEXT: fn add_dollar_one(value_dollar_in: i32) -> i32 {
// CHECK-NEXT:     value_dollar_in + 1
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let local_dollar_: i32 = add_dollar_one(unsafe { _dollar_counter });
// CHECK-NEXT:     unsafe { printf(c"%d\n".as_ptr(), local_dollar_) };
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
