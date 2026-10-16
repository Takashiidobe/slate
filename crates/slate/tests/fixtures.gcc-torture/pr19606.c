/* PR c/19606
   The C front end used to shorten the type of a division to a type
   that does not preserve the semantics of the original computation.
   Make sure that won't happen.  */

void abort(void);
void exit(int);

signed char a = -4;

int foo(void) { return ((unsigned int)(signed int)a) / 2LL; }

int bar(void) { return ((unsigned int)(signed int)a) % 5LL; }

int main(void) {
  int r;

  r = foo();
  if (r != ((unsigned int)(signed int)(signed char)-4) / 2LL)
    abort();

  r = bar();
  if (r != ((unsigned int)(signed int)(signed char)-4) % 5LL)
    abort();

  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR c/19606
// CHECK-NEXT: //! The C front end used to shorten the type of a division to a type
// CHECK-NEXT: //! that does not preserve the semantics of the original computation.
// CHECK-NEXT: //! Make sure that won't happen.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: i8 = -(4 as i32) as i8;
// CHECK-EMPTY:
// CHECK-NEXT: fn foo() -> i32 {
// CHECK-NEXT:     ((((((unsafe { a }) as i32) as u32) as u64) as i64) / (2 as i64)) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar() -> i32 {
// CHECK-NEXT:     ((((((unsafe { a }) as i32) as u32) as u64) as i64) % (5 as i64)) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut r: i32 = foo();
// CHECK-NEXT:     if (r as i64) != (((((-(4 as i32) as i8) as i32) as u32) as u64) as i64) / (2 as i64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     r = bar();
// CHECK-NEXT:     if (r as i64) != (((((-(4 as i32) as i8) as i32) as u32) as u64) as i64) % (5 as i64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
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
// SLATE-FILECHECK-END
