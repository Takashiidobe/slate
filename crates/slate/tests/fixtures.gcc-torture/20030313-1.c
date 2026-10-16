struct A {
  unsigned long p, q, r, s;
} x = {13, 14, 15, 16};

extern void abort(void);
extern void exit(int);

static inline struct A *bar(void) {
  struct A *r;

  switch (8) {
  case 2:
    abort();
    break;
  case 8:
    r = &x;
    break;
  default:
    abort();
    break;
  }
  return r;
}

void foo(unsigned long *x, int y) {
  if (y != 12)
    abort();
  if (x[0] != 1 || x[1] != 11)
    abort();
  if (x[2] != 2 || x[3] != 12)
    abort();
  if (x[4] != 3 || x[5] != 13)
    abort();
  if (x[6] != 4 || x[7] != 14)
    abort();
  if (x[8] != 5 || x[9] != 15)
    abort();
  if (x[10] != 6 || x[11] != 16)
    abort();
}

int main(void) {
  unsigned long a[40];
  int           b = 0;

  a[b++] = 1;
  a[b++] = 11;
  a[b++] = 2;
  a[b++] = 12;
  a[b++] = 3;
  a[b++] = bar()->p;
  a[b++] = 4;
  a[b++] = bar()->q;
  a[b++] = 5;
  a[b++] = bar()->r;
  a[b++] = 6;
  a[b++] = bar()->s;
  foo(a, b);
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
// CHECK-NEXT: struct A {
// CHECK-NEXT:     p: u64,
// CHECK-NEXT:     q: u64,
// CHECK-NEXT:     r: u64,
// CHECK-NEXT:     s: u64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut x: A = A {
// CHECK-NEXT:     p: ((13 as i32) as i64) as u64,
// CHECK-NEXT:     q: ((14 as i32) as i64) as u64,
// CHECK-NEXT:     r: ((15 as i32) as i64) as u64,
// CHECK-NEXT:     s: ((16 as i32) as i64) as u64,
// CHECK-NEXT: };
// CHECK-NEXT: fn bar() -> *mut A {
// CHECK-NEXT:     let mut r: *mut A = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: {
// CHECK-NEXT:         match 8 as i32 {
// CHECK-NEXT:             2 => {
// CHECK-NEXT:                 unsafe { abort() };
// CHECK-NEXT:             }
// CHECK-NEXT:             8 => {
// CHECK-NEXT:                 r = unsafe { std::ptr::addr_of_mut!(x) };
// CHECK-NEXT:             }
// CHECK-NEXT:             _ => {
// CHECK-NEXT:                 unsafe { abort() };
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     r
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut [[X_0:x_[0-9]+]]: *mut u64, mut y: i32) {
// CHECK-NEXT:     if y != (12 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { *unsafe { [[X_0]].offset((0 as i32) as isize) } })
// CHECK-NEXT:         != (((1 as i32) as i64) as u64)
// CHECK-NEXT:         || (unsafe { *unsafe { [[X_0]].offset((1 as i32) as isize) } })
// CHECK-NEXT:             != (((11 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { *unsafe { [[X_0]].offset((2 as i32) as isize) } })
// CHECK-NEXT:         != (((2 as i32) as i64) as u64)
// CHECK-NEXT:         || (unsafe { *unsafe { [[X_0]].offset((3 as i32) as isize) } })
// CHECK-NEXT:             != (((12 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { *unsafe { [[X_0]].offset((4 as i32) as isize) } })
// CHECK-NEXT:         != (((3 as i32) as i64) as u64)
// CHECK-NEXT:         || (unsafe { *unsafe { [[X_0]].offset((5 as i32) as isize) } })
// CHECK-NEXT:             != (((13 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { *unsafe { [[X_0]].offset((6 as i32) as isize) } })
// CHECK-NEXT:         != (((4 as i32) as i64) as u64)
// CHECK-NEXT:         || (unsafe { *unsafe { [[X_0]].offset((7 as i32) as isize) } })
// CHECK-NEXT:             != (((14 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { *unsafe { [[X_0]].offset((8 as i32) as isize) } })
// CHECK-NEXT:         != (((5 as i32) as i64) as u64)
// CHECK-NEXT:         || (unsafe { *unsafe { [[X_0]].offset((9 as i32) as isize) } })
// CHECK-NEXT:             != (((15 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { *unsafe { [[X_0]].offset((10 as i32) as isize) } })
// CHECK-NEXT:         != (((6 as i32) as i64) as u64)
// CHECK-NEXT:         || (unsafe { *unsafe { [[X_0]].offset((11 as i32) as isize) } })
// CHECK-NEXT:             != (((16 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: __SlateAlign16<[u64; 40]> = __SlateAlign16([0 as u64; 40]);
// CHECK-NEXT:     let mut b: i32 = 0 as i32;
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i32 = b;
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:     b = [[V_2]];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (a.0.as_mut_ptr() as *mut u64).offset([[V_1]] as isize) } = ((1 as i32)
// CHECK-NEXT:             as i64) as u64;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: i32 = b;
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] + (1 as i32);
// CHECK-NEXT:     b = [[V_4]];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (a.0.as_mut_ptr() as *mut u64).offset([[V_3]] as isize) } = ((11 as i32)
// CHECK-NEXT:             as i64) as u64;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: i32 = b;
// CHECK-NEXT:     let mut [[V_6:__v[0-9]+]]: i32 = [[V_5]] + (1 as i32);
// CHECK-NEXT:     b = [[V_6]];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (a.0.as_mut_ptr() as *mut u64).offset([[V_5]] as isize) } = ((2 as i32)
// CHECK-NEXT:             as i64) as u64;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_7:__v[0-9]+]]: i32 = b;
// CHECK-NEXT:     let mut [[V_8:__v[0-9]+]]: i32 = [[V_7]] + (1 as i32);
// CHECK-NEXT:     b = [[V_8]];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (a.0.as_mut_ptr() as *mut u64).offset([[V_7]] as isize) } = ((12 as i32)
// CHECK-NEXT:             as i64) as u64;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_9:__v[0-9]+]]: i32 = b;
// CHECK-NEXT:     let mut [[V_10:__v[0-9]+]]: i32 = [[V_9]] + (1 as i32);
// CHECK-NEXT:     b = [[V_10]];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (a.0.as_mut_ptr() as *mut u64).offset([[V_9]] as isize) } = ((3 as i32)
// CHECK-NEXT:             as i64) as u64;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_11:__v[0-9]+]]: i32 = b;
// CHECK-NEXT:     let mut [[V_12:__v[0-9]+]]: i32 = [[V_11]] + (1 as i32);
// CHECK-NEXT:     b = [[V_12]];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (a.0.as_mut_ptr() as *mut u64).offset([[V_11]] as isize) } = unsafe {
// CHECK-NEXT:             (*bar()).p
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_13:__v[0-9]+]]: i32 = b;
// CHECK-NEXT:     let mut [[V_14:__v[0-9]+]]: i32 = [[V_13]] + (1 as i32);
// CHECK-NEXT:     b = [[V_14]];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (a.0.as_mut_ptr() as *mut u64).offset([[V_13]] as isize) } = ((4 as i32)
// CHECK-NEXT:             as i64) as u64;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_15:__v[0-9]+]]: i32 = b;
// CHECK-NEXT:     let mut [[V_16:__v[0-9]+]]: i32 = [[V_15]] + (1 as i32);
// CHECK-NEXT:     b = [[V_16]];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (a.0.as_mut_ptr() as *mut u64).offset([[V_15]] as isize) } = unsafe {
// CHECK-NEXT:             (*bar()).q
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_17:__v[0-9]+]]: i32 = b;
// CHECK-NEXT:     let mut [[V_18:__v[0-9]+]]: i32 = [[V_17]] + (1 as i32);
// CHECK-NEXT:     b = [[V_18]];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (a.0.as_mut_ptr() as *mut u64).offset([[V_17]] as isize) } = ((5 as i32)
// CHECK-NEXT:             as i64) as u64;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_19:__v[0-9]+]]: i32 = b;
// CHECK-NEXT:     let mut [[V_20:__v[0-9]+]]: i32 = [[V_19]] + (1 as i32);
// CHECK-NEXT:     b = [[V_20]];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (a.0.as_mut_ptr() as *mut u64).offset([[V_19]] as isize) } = unsafe {
// CHECK-NEXT:             (*bar()).r
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_21:__v[0-9]+]]: i32 = b;
// CHECK-NEXT:     let mut [[V_22:__v[0-9]+]]: i32 = [[V_21]] + (1 as i32);
// CHECK-NEXT:     b = [[V_22]];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (a.0.as_mut_ptr() as *mut u64).offset([[V_21]] as isize) } = ((6 as i32)
// CHECK-NEXT:             as i64) as u64;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_23:__v[0-9]+]]: i32 = b;
// CHECK-NEXT:     let mut [[V_24:__v[0-9]+]]: i32 = [[V_23]] + (1 as i32);
// CHECK-NEXT:     b = [[V_24]];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (a.0.as_mut_ptr() as *mut u64).offset([[V_23]] as isize) } = unsafe {
// CHECK-NEXT:             (*bar()).s
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     foo(a.0.as_mut_ptr() as *mut u64, b);
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
