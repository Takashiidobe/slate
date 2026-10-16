void abort(void);
void exit(int);

int var = 0;

void g() { var = 1; }

void f() {
  int f2 = 0;

  if (f2 == 0)
    ;

  g();
}

int main(void) {
  f();
  if (var != 1)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: static mut var: i32 = 0 as i32;
// CHECK-NEXT: fn g() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         var = 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn f() {
// CHECK-NEXT:     let mut f2: i32 = 0 as i32;
// CHECK-NEXT:     if f2 == (0 as i32) {
// CHECK-NEXT:         {}
// CHECK-NEXT:     }
// CHECK-NEXT:     g();
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     f();
// CHECK-NEXT:     if (unsafe { var }) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
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
// SLATE-FILECHECK-END
