typedef struct creal_T {
  double re;
  double im;
} creal_T;

#define N 16
int main() {
  int     k;
  int     i;
  int     j;
  creal_T t2[N];
  double  inval;

  inval = 1.0;
  for (j = 0; j < N; ++j) {
    t2[j].re = 0;
    t2[j].im = 0;
  }

  for (j = 0; j < N / 4; j++) {
    i        = j * 4;
    t2[i].re = inval;
    t2[i].im = inval;
    k        = i + 3;
    t2[k].re = inval;
    t2[k].im = inval;
    t2[i]    = t2[k];
    t2[k].re = inval;
  }

  for (i = 0; i < 2; ++i)
    if (t2[i].re != !i || t2[i].im != !i)
      __builtin_abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct creal_T {
// CHECK-NEXT:     re: f64,
// CHECK-NEXT:     im: f64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut k: i32 = 0 as i32;
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut j: i32 = 0 as i32;
// CHECK-NEXT:     let mut t2: __SlateAlign16<[creal_T; 16]> = __SlateAlign16(unsafe {
// CHECK-NEXT:         std::mem::zeroed()
// CHECK-NEXT:     });
// CHECK-NEXT:     let mut inval: f64 = 1.0f64;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while j < (16 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*unsafe { (t2.0.as_mut_ptr() as *mut creal_T).offset(j as isize) }).re = (0
// CHECK-NEXT:                 as i32) as f64;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*unsafe { (t2.0.as_mut_ptr() as *mut creal_T).offset(j as isize) }).im = (0
// CHECK-NEXT:                 as i32) as f64;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = j;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         j = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     j = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while j < (16 as i32) / (4 as i32) {
// CHECK-NEXT:         i = j * (4 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*unsafe { (t2.0.as_mut_ptr() as *mut creal_T).offset(i as isize) }).re = inval;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*unsafe { (t2.0.as_mut_ptr() as *mut creal_T).offset(i as isize) }).im = inval;
// CHECK-NEXT:         }
// CHECK-NEXT:         k = i + (3 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*unsafe { (t2.0.as_mut_ptr() as *mut creal_T).offset(k as isize) }).re = inval;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*unsafe { (t2.0.as_mut_ptr() as *mut creal_T).offset(k as isize) }).im = inval;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (t2.0.as_mut_ptr() as *mut creal_T).offset(i as isize) } = unsafe {
// CHECK-NEXT:                 *unsafe { (t2.0.as_mut_ptr() as *mut creal_T).offset(k as isize) }
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*unsafe { (t2.0.as_mut_ptr() as *mut creal_T).offset(k as isize) }).re = inval;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = j;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         j = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     i = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while i < (2 as i32) {
// CHECK-NEXT:         if (unsafe {
// CHECK-NEXT:             (*unsafe { (t2.0.as_mut_ptr() as *mut creal_T).offset(i as isize) }).re
// CHECK-NEXT:         }) != ((!(i != (0 as i32)) as i32) as f64)
// CHECK-NEXT:             || (unsafe {
// CHECK-NEXT:                 (*unsafe { (t2.0.as_mut_ptr() as *mut creal_T).offset(i as isize) }).im
// CHECK-NEXT:             }) != ((!(i != (0 as i32)) as i32) as f64)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = [[V_4]] + (1 as i32);
// CHECK-NEXT:         i = [[V_5]];
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
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
