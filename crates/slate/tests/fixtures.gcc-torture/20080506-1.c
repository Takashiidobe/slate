/* PR middle-end/36137 */
extern void abort(void);

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

int main() {
  unsigned int u;
  int          i = -1;

  u = MAX((unsigned int)MAX(i, 0), 1);
  if (u != 1)
    abort();

  u = MIN((unsigned int)MAX(i, 0), (unsigned int)i);
  if (u != 0)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // PR middle-end/36137
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut u: u32 = 0 as u32;
// CHECK-NEXT:     let mut i: i32 = -(1 as i32);
// CHECK-NEXT:     u = if ((if i > (0 as i32) { i } else { 0 as i32 }) as u32) > ((1 as i32) as u32) {
// CHECK-NEXT:         (if i > (0 as i32) { i } else { 0 as i32 }) as u32
// CHECK-NEXT:     } else {
// CHECK-NEXT:         (1 as i32) as u32
// CHECK-NEXT:     };
// CHECK-NEXT:     if u != ((1 as i32) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     u = if ((if i > (0 as i32) { i } else { 0 as i32 }) as u32) < (i as u32) {
// CHECK-NEXT:         (if i > (0 as i32) { i } else { 0 as i32 }) as u32
// CHECK-NEXT:     } else {
// CHECK-NEXT:         i as u32
// CHECK-NEXT:     };
// CHECK-NEXT:     if u != ((0 as i32) as u32) {
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
