/* { dg-options "-nostdinc" } */
int printf(const char *, ...);
void exit(int);
void abort(void);
char *getenv(const char *);

static int twice(int value) { return value * 2; }

int main(void) {
  printf("twice %d\n", twice(21));
  if (getenv("SLATE_NOSTDINC_UNSET"))
    abort();
  exit(twice(21) == 42 ? 0 : 1);
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![allow(suspicious_runtime_symbol_definitions)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT:     fn exit([[ARG_0]]: i32);
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn getenv([[ARG_0]]: *const i8) -> *mut i8;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-options "-nostdinc" }
// CHECK-NEXT: fn twice(value: i32) -> i32 {
// CHECK-NEXT:     value * 2
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe { printf(c"twice %d\n".as_ptr(), twice(21i32)) };
// CHECK-NEXT:     if !unsafe { getenv(c"SLATE_NOSTDINC_UNSET".as_ptr()) }.is_null() {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(if twice(21i32) == 42 { 0i32 } else { 1i32 }) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
