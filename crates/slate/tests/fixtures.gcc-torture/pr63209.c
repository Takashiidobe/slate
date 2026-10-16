static int Sub(int a, int b) { return b - a; }

static unsigned Select(unsigned a, unsigned b, unsigned c) {
  const int pa_minus_pb = Sub((a >> 8) & 0xff, (b >> 8) & 0xff) +
                          Sub((a >> 0) & 0xff, (b >> 0) & 0xff);
  return (pa_minus_pb <= 0) ? a : b;
}

__attribute__((noinline)) unsigned Predictor(unsigned              left,
                                             const unsigned *const top) {
  const unsigned pred = Select(top[1], left, top[0]);
  return pred;
}

int main(void) {
  const unsigned top[2] = {0xff7a7a7a, 0xff7a7a7a};
  const unsigned left   = 0xff7b7b7b;
  const unsigned pred   = Predictor(left, top /*+ 1*/);
  if (pred == left)
    return 0;
  return 1;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: fn Sub(mut a: i32, mut b: i32) -> i32 {
// CHECK-NEXT:     b - a
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn Select(mut a: u32, mut b: u32, mut c: u32) -> u32 {
// CHECK-NEXT:     let mut pa_minus_pb: i32 = Sub(
// CHECK-NEXT:         (a >> (8 as i32) & ((255 as i32) as u32)) as i32,
// CHECK-NEXT:         (b >> (8 as i32) & ((255 as i32) as u32)) as i32,
// CHECK-NEXT:     ) + Sub(
// CHECK-NEXT:         (a >> (0 as i32) & ((255 as i32) as u32)) as i32,
// CHECK-NEXT:         (b >> (0 as i32) & ((255 as i32) as u32)) as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     if pa_minus_pb <= (0 as i32) { a } else { b }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn Predictor(mut left: u32, mut top: *const u32) -> u32 {
// CHECK-NEXT:     let mut pred: u32 = Select(
// CHECK-NEXT:         unsafe { *unsafe { top.offset((1 as i32) as isize) } },
// CHECK-NEXT:         left,
// CHECK-NEXT:         unsafe { *unsafe { top.offset((0 as i32) as isize) } },
// CHECK-NEXT:     );
// CHECK-NEXT:     pred
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut top: [u32; 2] = [4286216826 as u32, 4286216826 as u32];
// CHECK-NEXT:     let mut left: u32 = 4286282619 as u32;
// CHECK-NEXT:     let mut pred: u32 = Predictor(left, top.as_ptr() as *const u32); // + 1
// CHECK-NEXT:     if pred == left {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     1 as i32
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
