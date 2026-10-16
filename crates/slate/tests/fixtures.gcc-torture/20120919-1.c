/* PR rtl-optimization/54290 */
/* Testcase by Eric Volk <eriksnga@gmail.com> */
/* { dg-require-effective-target int32plus } */

double  vd[2] = {1., 0.};
int     vi[2] = {1234567890, 0};
double *pd    = vd;
int    *pi    = vi;

extern void abort(void);

void init(int *n, int *dummy) __attribute__((noinline, noclone));

void init(int *n, int *dummy) {
  if (0 == n)
    dummy[0] = 0;
}

int main(void) {
  int dummy[1532];
  int i = -1, n = 1, s = 0;
  init(&n, dummy);
  while (i < n) {
    if (i == 0) {
      if (pd[i] > 0) {
        if (pi[i] > 0) {
          s += pi[i];
        }
      }
      pd[i] = pi[i];
    }
    ++i;
  }
  if (s != 1234567890)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/54290
// CHECK-NEXT: //!
// CHECK-NEXT: //! Testcase by Eric Volk <eriksnga@gmail.com>
// CHECK-NEXT: //!
// CHECK-NEXT: //! { dg-require-effective-target int32plus }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut vd: __SlateAlign16<[f64; 2]> = __SlateAlign16([1.0f64, 0.0f64]);
// CHECK-EMPTY:
// CHECK-NEXT: static mut vi: [i32; 2] = [1234567890 as i32, 0 as i32];
// CHECK-EMPTY:
// CHECK-NEXT: static mut pd: *mut f64 = unsafe { std::ptr::addr_of_mut!(vd.0) as *mut f64 };
// CHECK-EMPTY:
// CHECK-NEXT: static mut pi: *mut i32 = unsafe { std::ptr::addr_of_mut!(vi) as *mut i32 };
// CHECK-EMPTY:
// CHECK-NEXT: fn init(mut n: *mut i32, mut dummy: *mut i32) {
// CHECK-NEXT:     if std::ptr::null_mut::<i32>() == n {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { dummy.offset((0 as i32) as isize) } = 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut dummy: __SlateAlign16<[i32; 1532]> = __SlateAlign16([0 as i32; 1532]);
// CHECK-NEXT:     let mut i: i32 = -(1 as i32);
// CHECK-NEXT:     let mut n: i32 = 1 as i32;
// CHECK-NEXT:     let mut s: i32 = 0 as i32;
// CHECK-NEXT:     init(std::ptr::addr_of_mut!(n), dummy.0.as_mut_ptr() as *mut i32);
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < n {
// CHECK-NEXT:         if i == (0 as i32) {
// CHECK-NEXT:             if (unsafe { *unsafe { unsafe { pd }.offset(i as isize) } }) > ((0 as i32) as f64) {
// CHECK-NEXT:                 if (unsafe { *unsafe { unsafe { pi }.offset(i as isize) } }) > (0 as i32) {
// CHECK-NEXT:                     let mut [[V_0:__v[0-9]+]]: i32 = s;
// CHECK-NEXT:                     let mut [[V_1:__v[0-9]+]]: i32 =
// CHECK-NEXT:                         [[V_0]] + unsafe { *unsafe { unsafe { pi }.offset(i as isize) } };
// CHECK-NEXT:                     s = [[V_1]];
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe { unsafe { pd }.offset(i as isize) } =
// CHECK-NEXT:                     (unsafe { *unsafe { unsafe { pi }.offset(i as isize) } }) as f64;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         i = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if s != (1234567890 as i32) {
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
