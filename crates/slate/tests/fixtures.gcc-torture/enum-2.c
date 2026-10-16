/* Copyright (C) 2000 Free Software Foundation */
/* by Alexandre Oliva  <aoliva@redhat.com> */

void abort(void);
void exit(int);

enum foo { FOO, BAR };

/* Even though the underlying type of an enum is unspecified, the type
   of enumeration constants is explicitly defined as int (6.4.4.3/2 in
   the C99 Standard).  Therefore, `i' must not be promoted to
   `unsigned' in the comparison below; we must exit the loop when it
   becomes negative. */

int main() {
  int i;
  for (i = BAR; i >= FOO; --i)
    if (i == -1)
      abort();

  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Copyright (C) 2000 Free Software Foundation
// CHECK-NEXT: //!
// CHECK-NEXT: //! by Alexandre Oliva  <aoliva@redhat.com>
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // Even though the underlying type of an enum is unspecified, the type
// CHECK-NEXT: // of enumeration constants is explicitly defined as int (6.4.4.3/2 in
// CHECK-NEXT: // the C99 Standard).  Therefore, `i' must not be promoted to
// CHECK-NEXT: // `unsigned' in the comparison below; we must exit the loop when it
// CHECK-NEXT: // becomes negative.
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 1 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i >= (0 as i32) {
// CHECK-NEXT:         if i == -(1 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] - (1 as i32);
// CHECK-NEXT:         i = [[V_2]];
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
