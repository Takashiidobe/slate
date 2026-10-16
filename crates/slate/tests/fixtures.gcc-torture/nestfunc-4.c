/* { dg-add-options stack_size } */

/* Origin: hp@bitrange.com
   Test that return values come out right from a 1000-level call chain to
   functions without parameters that each need at least one "long"
   preserved.  Exposed problems related to the MMIX port.  */

void abort(void);
void exit(int);

long        level = 0;
extern long foo(void);
extern long bar(void);

#ifdef STACK_SIZE
#define DEPTH ((STACK_SIZE) / 512 + 1)
#else
#define DEPTH 500
#endif

int main(void) {
  if (foo() == -42)
    exit(0);

  abort();
}

long foo(void) {
  long tmp = ++level;
  return bar() + tmp;
}

long bar(void) {
  long tmp = level;
  return tmp > DEPTH - 1 ? -42 - tmp : foo() - tmp;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-add-options stack_size }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // Origin: hp@bitrange.com
// CHECK-NEXT: // Test that return values come out right from a 1000-level call chain to
// CHECK-NEXT: // functions without parameters that each need at least one "long"
// CHECK-NEXT: // preserved.  Exposed problems related to the MMIX port.
// CHECK-NEXT: static mut level: i64 = (0 as i32) as i64;
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if foo() == (-(42 as i32) as i64) {
// CHECK-NEXT:         unsafe { exit(0 as i32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { abort() };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo() -> i64 {
// CHECK-NEXT:     let mut tmp: i64 = 0 as i64;
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i64 = unsafe { level };
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: i64 = [[V_1]] + ((1 as i32) as i64);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         level = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT:     tmp = [[V_2]];
// CHECK-NEXT:     bar() + tmp
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar() -> i64 {
// CHECK-NEXT:     let mut tmp: i64 = unsafe { level };
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: i64;
// CHECK-NEXT:     if tmp > (((500 as i32) - (1 as i32)) as i64) {
// CHECK-NEXT:         [[V_3]] = (-(42 as i32) as i64) - tmp;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_3]] = foo() - tmp;
// CHECK-NEXT:     }
// CHECK-NEXT:     [[V_3]]
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
