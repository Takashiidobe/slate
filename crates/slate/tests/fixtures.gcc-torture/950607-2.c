void abort(void);
void exit(int);

typedef struct {
  long int p_x, p_y;
} Point;

int f(Point basePt, Point pt1, Point pt2) {
  long long vector;

  vector =
      (long long)(pt1.p_x - basePt.p_x) * (long long)(pt2.p_y - basePt.p_y) -
      (long long)(pt1.p_y - basePt.p_y) * (long long)(pt2.p_x - basePt.p_x);

  if (vector > (long long)0)
    return 0;
  else if (vector < (long long)0)
    return 1;
  else
    return 2;
}

int main(void) {
  Point b, p1, p2;
  int   answer;

  b.p_x = -23250;
  b.p_y = 23250;

  p1.p_x = 23250;
  p1.p_y = -23250;

  p2.p_x = -23250;
  p2.p_y = -23250;

  answer = f(b, p1, p2);

  if (answer != 1)
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
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     p_x: i64,
// CHECK-NEXT:     p_y: i64,
// CHECK-NEXT: }
// CHECK-NEXT: fn f(
// CHECK-NEXT:     mut basePt: [[SLATERECORD_0]],
// CHECK-NEXT:     mut pt1: [[SLATERECORD_0]],
// CHECK-NEXT:     mut pt2: [[SLATERECORD_0]],
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     let mut vector: i64 = (pt1.p_x - basePt.p_x) * (pt2.p_y - basePt.p_y)
// CHECK-NEXT:         - (pt1.p_y - basePt.p_y) * (pt2.p_x - basePt.p_x);
// CHECK-NEXT:     if vector > ((0 as i32) as i64) {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if vector < ((0 as i32) as i64) {
// CHECK-NEXT:             return 1 as i32;
// CHECK-NEXT:         } else {
// CHECK-NEXT:             return 2 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut b: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut p1: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut p2: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut answer: i32 = 0 as i32;
// CHECK-NEXT:     b.p_x = -(23250 as i32) as i64;
// CHECK-NEXT:     b.p_y = (23250 as i32) as i64;
// CHECK-NEXT:     p1.p_x = (23250 as i32) as i64;
// CHECK-NEXT:     p1.p_y = -(23250 as i32) as i64;
// CHECK-NEXT:     p2.p_x = -(23250 as i32) as i64;
// CHECK-NEXT:     p2.p_y = -(23250 as i32) as i64;
// CHECK-NEXT:     answer = f(b, p1, p2);
// CHECK-NEXT:     if answer != (1 as i32) {
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
