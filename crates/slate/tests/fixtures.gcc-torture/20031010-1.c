/* A reminder to process ops in generate_expr_as_of_bb exactly once.  */

void abort(void);

long __attribute__((noinline)) foo(long ct, long cf, _Bool p1, _Bool p2,
                                   _Bool p3) {
  long diff;

  diff = ct - cf;

  if (p1) {
    if (p2) {
      if (p3) {
        long tmp = ct;
        ct       = cf;
        cf       = tmp;
      }
      diff = ct - cf;
    }

    return diff;
  }

  abort();
}

int main() {
  if (foo(2, 3, 1, 1, 1) == 0)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! A reminder to process ops in generate_expr_as_of_bb exactly once.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut ct: i64, mut cf: i64, mut p1: bool, mut p2: bool, mut p3: bool) -> i64 {
// CHECK-NEXT:     let mut diff: i64 = ct - cf;
// CHECK-NEXT:     if p1 {
// CHECK-NEXT:         if p2 {
// CHECK-NEXT:             if p3 {
// CHECK-NEXT:                 let mut tmp: i64 = ct;
// CHECK-NEXT:                 ct = cf;
// CHECK-NEXT:                 cf = tmp;
// CHECK-NEXT:             }
// CHECK-NEXT:             diff = ct - cf;
// CHECK-NEXT:         }
// CHECK-NEXT:         return diff;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { abort() };
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if foo(
// CHECK-NEXT:         (2 as i32) as i64,
// CHECK-NEXT:         (3 as i32) as i64,
// CHECK-NEXT:         (1 as i32) != (0 as i32),
// CHECK-NEXT:         (1 as i32) != (0 as i32),
// CHECK-NEXT:         (1 as i32) != (0 as i32),
// CHECK-NEXT:     ) == ((0 as i32) as i64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
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
