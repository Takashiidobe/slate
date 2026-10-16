/* PR24716, scalar evolution returning the wrong result
   for pdest.  */

int Link[] = {-1};
int W[]    = {2};

extern void abort(void);

int f(int k, int p) {
  int pdest, j, D1361;
  j     = 0;
  pdest = 0;
  for (;;) {
    if (pdest > 2)
      do
        j--, pdest++;
      while (j > 2);

    if (j == 1)
      break;

    while (pdest > p)
      if (j == p)
        pdest++;

    do {
      D1361 = W[k];
      do
        if (D1361 != 0)
          pdest = 1, W[k] = D1361 = 0;
      while (p < 1);
    } while (k > 0);

    do {
      p = 0;
      k = Link[k];
      while (p < j)
        if (k != -1)
          pdest++, p++;
    } while (k != -1);
    j = 1;
  }

  /* The correct return value should be pdest (1 in the call from main).
     DOM3 is mistaken and propagates a 0 here.  */
  return pdest;
}

int main() {
  if (!f(0, 2))
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR24716, scalar evolution returning the wrong result
// CHECK-NEXT: //! for pdest.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut Link: [i32; 1] = [-(1 as i32)];
// CHECK-EMPTY:
// CHECK-NEXT: static mut W: [i32; 1] = [2 as i32];
// CHECK-EMPTY:
// CHECK-NEXT: fn f(mut k: i32, mut p: i32) -> i32 {
// CHECK-NEXT:     let mut pdest: i32 = 0 as i32;
// CHECK-NEXT:     let mut j: i32 = 0 as i32;
// CHECK-NEXT:     let mut D1361: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         if pdest > (2 as i32) {
// CHECK-NEXT:             '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 let mut [[V_0:__v[0-9]+]]: i32 = j;
// CHECK-NEXT:                 let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] - (1 as i32);
// CHECK-NEXT:                 j = [[V_1]];
// CHECK-NEXT:                 let mut [[V_2:__v[0-9]+]]: i32 = pdest;
// CHECK-NEXT:                 let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:                 pdest = [[V_3]];
// CHECK-NEXT:                 if !(j > (2 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         if j == (1 as i32) {
// CHECK-NEXT:             break '[[SLATE_BREAK_0]];
// CHECK-NEXT:         }
// CHECK-NEXT:         '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while pdest > p {
// CHECK-NEXT:             if j == p {
// CHECK-NEXT:                 let mut [[V_4:__v[0-9]+]]: i32 = pdest;
// CHECK-NEXT:                 let mut [[V_5:__v[0-9]+]]: i32 = [[V_4]] + (1 as i32);
// CHECK-NEXT:                 pdest = [[V_5]];
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:             D1361 = unsafe {
// CHECK-NEXT:                 *unsafe { unsafe { std::ptr::addr_of_mut!(W) as *mut i32 }.offset(k as isize) }
// CHECK-NEXT:             };
// CHECK-NEXT:             '[[SLATE_BREAK_4:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 if D1361 != (0 as i32) {
// CHECK-NEXT:                     pdest = 1 as i32;
// CHECK-NEXT:                     D1361 = 0 as i32;
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of_mut!(W) as *mut i32 }.offset(k as isize)
// CHECK-NEXT:                         } = 0 as i32;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !(p < (1 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             if !(k > (0 as i32)) {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         '[[SLATE_BREAK_5:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:             p = 0 as i32;
// CHECK-NEXT:             k = unsafe {
// CHECK-NEXT:                 *unsafe { unsafe { std::ptr::addr_of_mut!(Link) as *mut i32 }.offset(k as isize) }
// CHECK-NEXT:             };
// CHECK-NEXT:             '[[SLATE_BREAK_6:__slate_break_[0-9]+]]: while p < j {
// CHECK-NEXT:                 if k != -(1 as i32) {
// CHECK-NEXT:                     let mut [[V_6:__v[0-9]+]]: i32 = pdest;
// CHECK-NEXT:                     let mut [[V_7:__v[0-9]+]]: i32 = [[V_6]] + (1 as i32);
// CHECK-NEXT:                     pdest = [[V_7]];
// CHECK-NEXT:                     let mut [[V_8:__v[0-9]+]]: i32 = p;
// CHECK-NEXT:                     let mut [[V_9:__v[0-9]+]]: i32 = [[V_8]] + (1 as i32);
// CHECK-NEXT:                     p = [[V_9]];
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             if !(k != -(1 as i32)) {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         j = 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     // The correct return value should be pdest (1 in the call from main).
// CHECK-NEXT:     // DOM3 is mistaken and propagates a 0 here.
// CHECK-NEXT:     pdest
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if !(f(0 as i32, 2 as i32) != (0 as i32)) {
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
