/* PR tree-optimization/58277 */

extern void abort(void);
static int  a[1], b, c, e, i, j, k, m, q[] = {1, 1}, t;
int volatile d;
int **r;
static int ***volatile s = &r;
int f, g, o, x;
static int *volatile h = &f, *p;
char n;

static void fn1() {
  b = a[a[a[a[a[a[a[a[b]]]]]]]];
  b = a[a[a[a[a[a[a[a[b]]]]]]]];
  b = a[a[b]];
  b = a[a[a[a[a[a[a[a[b]]]]]]]];
  b = a[a[a[a[a[a[a[a[b]]]]]]]];
}

static int fn2() {
  n = 0;
  for (; g; t++) {
    for (;; m++) {
      d;
      int  *u;
      int **v[] = {
          0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
          0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
          0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
          0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
          0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
          0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
          0,  0,  0,  &u, 0,  0,  0,  0,  &u, &u, &u, &u, &u, &u, &u, 0,  &u, 0,
          &u, &u, &u, 0,  &u, &u, 0,  &u, &u, &u, &u, 0,  &u, &u, &u, &u, &u, 0,
          &u, &u, 0,  &u, 0,  &u, &u, 0,  &u, &u, &u, &u, &u, 0,  &u, 0,  0,  0,
          &u, &u, &u, 0,  0,  &u, &u, &u, 0,  &u, 0,  &u, &u};
      int ***w[] = {&v[0]};
      if (*p)
        break;
      return 0;
    }
    *h = 0;
  }
  return 1;
}

static void fn3() {
  int *y[] = {0, 0, 0, 0, 0, 0, 0, 0};
  for (; i; i++)
    x = 0;
  if (fn2()) {
    int *z[6] = {};
    for (; n < 1; n++)
      *h = 0;
    int t1[7];
    for (; c; c++)
      o = t1[0];
    for (; e; e--) {
      int  **t2 = &y[0];
      int ***t3 = &t2;
      *t3       = &z[0];
    }
  }
  *s = 0;
  for (n = 0;; n = 0) {
    int t4 = 0;
    if (q[n])
      break;
    *r = &t4;
  }
}

int main() {
  for (; j; j--)
    a[0] = 0;
  fn3();
  for (; k; k++)
    fn1();
  fn1();

  if (n)
    abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/58277
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: [i32; 1] = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut e: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut i: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut j: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut k: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut m: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut q: [i32; 2] = [1 as i32, 1 as i32];
// CHECK-EMPTY:
// CHECK-NEXT: static mut t: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut d: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut r: *mut *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut s: *mut *mut *mut i32 = unsafe { std::ptr::addr_of_mut!(r) };
// CHECK-EMPTY:
// CHECK-NEXT: static mut f: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut g: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut o: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut x: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut h: *mut i32 = unsafe { std::ptr::addr_of_mut!(f) };
// CHECK-EMPTY:
// CHECK-NEXT: static mut p: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut n: i8 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         b = unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }.offset((unsafe { *unsafe { unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }.offset((unsafe { *unsafe { unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }.offset((unsafe { *unsafe { unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }.offset((unsafe { *unsafe { unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }.offset((unsafe { *unsafe { unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }.offset((unsafe { *unsafe { unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }.offset((unsafe { *unsafe { unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }.offset((unsafe { b }) as isize) } }) as isize) } }) as isize) } }) as isize) } }) as isize) } }) as isize) } }) as isize) } }) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         b = unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }.offset((unsafe { *unsafe { unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }.offset((unsafe { *unsafe { unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }.offset((unsafe { *unsafe { unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }.offset((unsafe { *unsafe { unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }.offset((unsafe { *unsafe { unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }.offset((unsafe { *unsafe { unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }.offset((unsafe { *unsafe { unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }.offset((unsafe { b }) as isize) } }) as isize) } }) as isize) } }) as isize) } }) as isize) } }) as isize) } }) as isize) } }) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         b = unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }.offset(
// CHECK-NEXT:                     (unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }
// CHECK-NEXT:                                 .offset((unsafe { b }) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }) as isize,
// CHECK-NEXT:                 )
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         b = unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }.offset((unsafe { *unsafe { unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }.offset((unsafe { *unsafe { unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }.offset((unsafe { *unsafe { unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }.offset((unsafe { *unsafe { unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }.offset((unsafe { *unsafe { unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }.offset((unsafe { *unsafe { unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }.offset((unsafe { *unsafe { unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }.offset((unsafe { b }) as isize) } }) as isize) } }) as isize) } }) as isize) } }) as isize) } }) as isize) } }) as isize) } }) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         b = unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }.offset((unsafe { *unsafe { unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }.offset((unsafe { *unsafe { unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }.offset((unsafe { *unsafe { unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }.offset((unsafe { *unsafe { unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }.offset((unsafe { *unsafe { unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }.offset((unsafe { *unsafe { unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }.offset((unsafe { *unsafe { unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }.offset((unsafe { b }) as isize) } }) as isize) } }) as isize) } }) as isize) } }) as isize) } }) as isize) } }) as isize) } }) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         n = (0 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (unsafe { g }) != (0 as i32) {
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:             unsafe { std::ptr::read_volatile(std::ptr::addr_of!(d)) };
// CHECK-NEXT:             let mut u: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             let mut v: __SlateAlign16<[*mut *mut i32; 175]> = __SlateAlign16([
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(u),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(u),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(u),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(u),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(u),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(u),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(u),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(u),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(u),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(u),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(u),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(u),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(u),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(u),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(u),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(u),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(u),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(u),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(u),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(u),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(u),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(u),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(u),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(u),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(u),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(u),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(u),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(u),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(u),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(u),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(u),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(u),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(u),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(u),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(u),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(u),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(u),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(u),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(u),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(u),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(u),
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i32>(),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(u),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(u),
// CHECK-NEXT:             ]);
// CHECK-NEXT:             [unsafe { (v.0.as_mut_ptr() as *mut *mut *mut i32).offset((0 as i32) as isize) }];
// CHECK-NEXT:             if (unsafe { *unsafe { p } }) != (0 as i32) {
// CHECK-NEXT:                 break '[[SLATE_BREAK_1]];
// CHECK-NEXT:             }
// CHECK-NEXT:             return 0 as i32;
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: i32 = unsafe { m };
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 m = [[V_1]];
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { std::ptr::read_volatile(std::ptr::addr_of!(h)) } = 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = unsafe { t };
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             t = [[V_3]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     1 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3() {
// CHECK-NEXT:     let mut y: __SlateAlign16<[*mut i32; 8]> = __SlateAlign16([
// CHECK-NEXT:         std::ptr::null_mut::<i32>(),
// CHECK-NEXT:         std::ptr::null_mut::<i32>(),
// CHECK-NEXT:         std::ptr::null_mut::<i32>(),
// CHECK-NEXT:         std::ptr::null_mut::<i32>(),
// CHECK-NEXT:         std::ptr::null_mut::<i32>(),
// CHECK-NEXT:         std::ptr::null_mut::<i32>(),
// CHECK-NEXT:         std::ptr::null_mut::<i32>(),
// CHECK-NEXT:         std::ptr::null_mut::<i32>(),
// CHECK-NEXT:     ]);
// CHECK-NEXT:     '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while (unsafe { i }) != (0 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             x = 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = unsafe { i };
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = [[V_4]] + (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             i = [[V_5]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if fn2() != (0 as i32) {
// CHECK-NEXT:         let mut z: __SlateAlign16<[*mut i32; 6]> = __SlateAlign16({
// CHECK-NEXT:             let mut [[T_0:__t[0-9]+]]: [*mut i32; 6] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_0]]
// CHECK-NEXT:         });
// CHECK-NEXT:         '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: while ((unsafe { n }) as i32) < (1 as i32) {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe { std::ptr::read_volatile(std::ptr::addr_of!(h)) } = 0 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_6:__v[0-9]+]]: i8 = unsafe { n };
// CHECK-NEXT:             let mut [[V_7:__v[0-9]+]]: i8 = (([[V_6]] as i32) + (1 as i32)) as i8;
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 n = [[V_7]];
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut t1: __SlateAlign16<[i32; 7]> = __SlateAlign16([0 as i32; 7]);
// CHECK-NEXT:         '[[SLATE_BREAK_4:__slate_break_[0-9]+]]: while (unsafe { c }) != (0 as i32) {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 o = unsafe {
// CHECK-NEXT:                     *unsafe { (t1.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) }
// CHECK-NEXT:                 };
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_8:__v[0-9]+]]: i32 = unsafe { c };
// CHECK-NEXT:             let mut [[V_9:__v[0-9]+]]: i32 = [[V_8]] + (1 as i32);
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 c = [[V_9]];
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         '[[SLATE_BREAK_5:__slate_break_[0-9]+]]: while (unsafe { e }) != (0 as i32) {
// CHECK-NEXT:             let mut t2: *mut *mut i32 =
// CHECK-NEXT:                 unsafe { (y.0.as_mut_ptr() as *mut *mut i32).offset((0 as i32) as isize) };
// CHECK-NEXT:             let mut t3: *mut *mut *mut i32 = std::ptr::addr_of_mut!(t2);
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *t3 = unsafe { (z.0.as_mut_ptr() as *mut *mut i32).offset((0 as i32) as isize) };
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_10:__v[0-9]+]]: i32 = unsafe { e };
// CHECK-NEXT:             let mut [[V_11:__v[0-9]+]]: i32 = [[V_10]] - (1 as i32);
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 e = [[V_11]];
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { std::ptr::read_volatile(std::ptr::addr_of!(s)) } =
// CHECK-NEXT:             std::ptr::null_mut::<*mut i32>();
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         n = (0 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_6:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t4: i32 = 0 as i32;
// CHECK-NEXT:         if (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(q) as *mut i32 }
// CHECK-NEXT:                     .offset(((unsafe { n }) as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) != (0 as i32)
// CHECK-NEXT:         {
// CHECK-NEXT:             break '[[SLATE_BREAK_6]];
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { r } = std::ptr::addr_of_mut!(t4);
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             n = (0 as i32) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     '[[SLATE_BREAK_7:__slate_break_[0-9]+]]: while (unsafe { j }) != (0 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }.offset((0 as i32) as isize)
// CHECK-NEXT:             } = 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_12:__v[0-9]+]]: i32 = unsafe { j };
// CHECK-NEXT:         let mut [[V_13:__v[0-9]+]]: i32 = [[V_12]] - (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             j = [[V_13]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     fn3();
// CHECK-NEXT:     '[[SLATE_BREAK_8:__slate_break_[0-9]+]]: while (unsafe { k }) != (0 as i32) {
// CHECK-NEXT:         fn1();
// CHECK-NEXT:         let mut [[V_14:__v[0-9]+]]: i32 = unsafe { k };
// CHECK-NEXT:         let mut [[V_15:__v[0-9]+]]: i32 = [[V_14]] + (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             k = [[V_15]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     fn1();
// CHECK-NEXT:     if (unsafe { n }) != (0 as i8) {
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
