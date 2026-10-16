/* PR opt/7130 */
void abort(void);
#define TYPE long

void scale(TYPE *alpha, TYPE *x, int n) {
  int i, ix;

  if (*alpha != 1)
    for (i = 0, ix = 0; i < n; i++, ix += 2) {
      TYPE tmpr, tmpi;
      tmpr      = *alpha * x[ix];
      tmpi      = *alpha * x[ix + 1];
      x[ix]     = tmpr;
      x[ix + 1] = tmpi;
    }
}

int main(void) {
  int  i;
  TYPE x[10];
  TYPE alpha = 2;

  for (i = 0; i < 10; i++)
    x[i] = i;

  scale(&alpha, x, 5);

  if (x[9] != 18)
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
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: // PR opt/7130
// CHECK-NEXT: fn scale(mut alpha: &i64, mut x: *mut i64, mut n: i32) {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut ix: i32 = 0 as i32;
// CHECK-NEXT:     if (unsafe { *(alpha as *const i64) }) != ((1 as i32) as i64) {
// CHECK-NEXT:         i = 0 as i32;
// CHECK-NEXT:         ix = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < n {
// CHECK-NEXT:             let mut tmpr: i64 =
// CHECK-NEXT:                 (unsafe { *(alpha as *const i64) }) * unsafe { *unsafe { x.offset(ix as isize) } };
// CHECK-NEXT:             let mut tmpi: i64 = (unsafe { *(alpha as *const i64) })
// CHECK-NEXT:                 * unsafe { *unsafe { x.offset((ix + (1 as i32)) as isize) } };
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe { x.offset(ix as isize) } = tmpr;
// CHECK-NEXT:             }
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe { x.offset((ix + (1 as i32)) as isize) } = tmpi;
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:             i = [[V_1]];
// CHECK-NEXT:             let mut [[V_2:__v[0-9]+]]: i32 = ix;
// CHECK-NEXT:             let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (2 as i32);
// CHECK-NEXT:             ix = [[V_3]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut x: __SlateAlign16<[i64; 10]> = __SlateAlign16([0 as i64; 10]);
// CHECK-NEXT:     let mut alpha: i64 = (2 as i32) as i64;
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < (10 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (x.0.as_mut_ptr() as *mut i64).offset(i as isize) } = i as i64;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = [[V_4]] + (1 as i32);
// CHECK-NEXT:         i = [[V_5]];
// CHECK-NEXT:     }
// CHECK-NEXT:     scale(
// CHECK-NEXT:         unsafe { &(*std::ptr::addr_of_mut!(alpha)) },
// CHECK-NEXT:         x.0.as_mut_ptr() as *mut i64,
// CHECK-NEXT:         5 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     if (unsafe { *unsafe { (x.0.as_mut_ptr() as *mut i64).offset((9 as i32) as isize) } })
// CHECK-NEXT:         != ((18 as i32) as i64)
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
