void abort(void);
void exit(int);

int gfbyte(void) { return 0; }

int main(void) {
  int i, j, k;

  i = gfbyte();

  i = i + 1;

  if (i == 0)
    k = -0;
  else
    k = i + 0;

  if (i != 1)
    abort();

  k = 1;
  if (k <= i)
    do
      j = gfbyte();
    while (k++ < i);

  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn gfbyte() -> i32 {
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = gfbyte();
// CHECK-NEXT:     let mut j: i32 = 0 as i32;
// CHECK-NEXT:     let mut k: i32 = 0 as i32;
// CHECK-NEXT:     i += 1 as i32;
// CHECK-NEXT:     if i == (0 as i32) {
// CHECK-NEXT:         k = -(0 as i32);
// CHECK-NEXT:     } else {
// CHECK-NEXT:         k = i + (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if i != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     k = 1 as i32;
// CHECK-NEXT:     if k <= i {
// CHECK-NEXT:         '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:             j = gfbyte();
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i32 = k;
// CHECK-NEXT:             let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:             k = [[V_2]];
// CHECK-NEXT:             if !([[V_1]] < i) {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
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
