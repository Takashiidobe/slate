void abort(void);
void exit(int);

static int  ap(int i);
static void testit(void) {
  int ir[4] = {0, 1, 2, 3};
  int ix, n, m;
  n = 1;
  m = 3;
  for (ix = 1; ix <= 4; ix++) {
    if (n == 1)
      m = 4;
    else
      m = n - 1;
    ap(ir[n - 1]);
    n = m;
  }
}

static int t = 0;
static int a[4];

static int ap(int i) {
  if (t > 3)
    abort();
  a[t++] = i;
  return 1;
}

int main(void) {
  testit();
  if (a[0] != 0)
    abort();
  if (a[1] != 3)
    abort();
  if (a[2] != 2)
    abort();
  if (a[3] != 1)
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
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn testit() {
// CHECK-NEXT:     let mut ir: __SlateAlign16<[i32; 4]> = __SlateAlign16([
// CHECK-NEXT:         0 as i32,
// CHECK-NEXT:         1 as i32,
// CHECK-NEXT:         2 as i32,
// CHECK-NEXT:         3 as i32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut ix: i32 = 1 as i32;
// CHECK-NEXT:     let mut n: i32 = 1 as i32;
// CHECK-NEXT:     let mut m: i32 = 3 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while ix <= (4 as i32) {
// CHECK-NEXT:         if n == (1 as i32) {
// CHECK-NEXT:             m = 4 as i32;
// CHECK-NEXT:         } else {
// CHECK-NEXT:             m = n - (1 as i32);
// CHECK-NEXT:         }
// CHECK-NEXT:         ap(unsafe {
// CHECK-NEXT:             *unsafe { (ir.0.as_mut_ptr() as *mut i32).offset((n - (1 as i32)) as isize) }
// CHECK-NEXT:         });
// CHECK-NEXT:         n = m;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = ix;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:         ix = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: static mut t: i32 = 0 as i32;
// CHECK-NEXT: static mut a: __SlateAlign16<[i32; 4]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-NEXT: fn ap(mut i: i32) -> i32 {
// CHECK-NEXT:     if (unsafe { t }) > (3 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: i32 = unsafe { t };
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] + (1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         t = [[V_4]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(a.0) as *mut i32 }.offset([[V_3]] as isize)
// CHECK-NEXT:         } = i;
// CHECK-NEXT:     }
// CHECK-NEXT:     1 as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     testit();
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(a.0) as *mut i32 }
// CHECK-NEXT:                 .offset((0 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     }) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(a.0) as *mut i32 }
// CHECK-NEXT:                 .offset((1 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     }) != (3 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(a.0) as *mut i32 }
// CHECK-NEXT:                 .offset((2 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     }) != (2 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(a.0) as *mut i32 }
// CHECK-NEXT:                 .offset((3 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     }) != (1 as i32)
// CHECK-NEXT:     {
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
