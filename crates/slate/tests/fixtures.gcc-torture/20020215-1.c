/* Test failed on an architecture that:

   - had 16-bit registers,
   - passed 64-bit structures in registers,
   - only allowed SImode values in even numbered registers.

   Before reload, s.i2 in foo() was represented as:

        (subreg:SI (reg:DI 0) 2)

   find_dummy_reload would return (reg:SI 1) for the subreg reload,
   despite that not being a valid register.  */

void abort(void);
void exit(int);

struct s {
  short i1;
  long  i2;
  short i3;
};

struct s foo(struct s s) {
  s.i2++;
  return s;
}

int main() {
  struct s s = foo((struct s){1000, 2000L, 3000});
  if (s.i1 != 1000 || s.i2 != 2001L || s.i3 != 3000)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Test failed on an architecture that:
// CHECK-NEXT: //!
// CHECK-NEXT: //! - had 16-bit registers,
// CHECK-NEXT: //! - passed 64-bit structures in registers,
// CHECK-NEXT: //! - only allowed SImode values in even numbered registers.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Before reload, s.i2 in foo() was represented as:
// CHECK-NEXT: //!
// CHECK-NEXT: //!      (subreg:SI (reg:DI 0) 2)
// CHECK-NEXT: //!
// CHECK-NEXT: //! find_dummy_reload would return (reg:SI 1) for the subreg reload,
// CHECK-NEXT: //! despite that not being a valid register.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct s {
// CHECK-NEXT:     i1: i16,
// CHECK-NEXT:     i2: i64,
// CHECK-NEXT:     i3: i16,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut s: s) -> s {
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i64 = s.i2;
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: i64 = [[V_1]] + ((1 as i32) as i64);
// CHECK-NEXT:     s.i2 = [[V_2]];
// CHECK-NEXT:     return s;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: s = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut s: s = foo(unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_0]] = s {
// CHECK-NEXT:                 i1: (1000 as i32) as i16,
// CHECK-NEXT:                 i2: 2000 as i64,
// CHECK-NEXT:                 i3: (3000 as i32) as i16,
// CHECK-NEXT:             };
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:         }
// CHECK-NEXT:     });
// CHECK-NEXT:     if (s.i1 as i32) != (1000 as i32) || s.i2 != (2001 as i64) || (s.i3 as i32) != (3000 as i32) {
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
