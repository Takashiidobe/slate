/* Origin: PR c/92 from Simon Marlow <t-simonm@microsoft.com>, adapted
   to a testcase by Joseph Myers <jsm28@cam.ac.uk>.
*/

void abort(void);
void exit(int);

typedef struct {
} empty;

typedef struct {
  int   i;
  empty e;
  int   i2;
} st;

st s = {.i = 0, .i2 = 1};

extern void abort(void);

int main(void) {
  if (s.i2 == 1)
    exit(0);
  else
    abort();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Origin: PR c/92 from Simon Marlow <t-simonm@microsoft.com>, adapted
// CHECK-NEXT: //! to a testcase by Joseph Myers <jsm28@cam.ac.uk>.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {}
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_1:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     i: i32,
// CHECK-NEXT:     e: [[SLATERECORD_0]],
// CHECK-NEXT:     i2: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut s: [[SLATERECORD_1]] = {
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: [[SLATERECORD_1]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].i = 0 as i32;
// CHECK-NEXT:     [[T_0]].i2 = 1 as i32;
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (unsafe { s.i2 }) == (1 as i32) {
// CHECK-NEXT:         unsafe { exit(0 as i32) };
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
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
