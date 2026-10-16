extern void abort(void);
extern void exit(int);

void do_exit(void) { exit(0); }
void do_abort(void) { abort(); }

void foo(int x, int a) {
  if (x < a)
    goto doit;
  do_exit();
  if (x != a)
    goto doit;

  /* else */
  do_abort();
  return;

doit:
  do_abort();
}

int main() {
  foo(1, 0);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn do_exit() {
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn do_abort() {
// CHECK-NEXT:     unsafe { abort() };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: i32, mut a: i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         '[[JOIN_0:__join_[0-9]+]]: {
// CHECK-NEXT:             if x < a {
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 do_exit();
// CHECK-NEXT:                 if x != a {
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     // else
// CHECK-NEXT:                     do_abort();
// CHECK-NEXT:                     return;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         do_abort();
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     foo(1 as i32, 0 as i32);
// CHECK-NEXT:     0 as i32
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
// SLATE-FILECHECK-END
