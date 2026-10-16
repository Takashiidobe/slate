/* PR middle-end/110115 */

int         a;
signed char b;

static int foo(signed char *e, int f) {
  int d;
  for (d = 0; d < f; d++)
    e[d] = 0;
  return d;
}

int bar(signed char e, int f) {
  signed char h[20];
  int         i = foo(h, f);
  return i;
}

int baz() {
  switch (a) {
  case 'f':
    return 0;
  default:
    return ~0;
  }
}

int main() {
  {
    signed char *k[3];
    int          d;
    for (d = 0; bar(8, 15) - 15 + d < 1; d++)
      k[baz() + 1] = &b;
    *k[0] = -*k[0];
  }
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR middle-end/110115
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: i8 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut e: *mut i8, mut f: i32) -> i32 {
// CHECK-NEXT:     let mut d: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while d < f {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { e.offset(d as isize) } = (0 as i32) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = d;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         d = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     d
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut e: i8, mut f: i32) -> i32 {
// CHECK-NEXT:     let mut h: __SlateAlign16<[i8; 20]> = __SlateAlign16([0 as i8; 20]);
// CHECK-NEXT:     let mut i: i32 = foo(h.0.as_mut_ptr() as *mut i8, f);
// CHECK-NEXT:     i
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn baz() -> i32 {
// CHECK-NEXT:     match unsafe { a } {
// CHECK-NEXT:         102 => {
// CHECK-NEXT:             return 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {
// CHECK-NEXT:             return !(0 as i32);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut k: __SlateAlign16<[*mut i8; 3]> = __SlateAlign16([0 as *mut i8; 3]);
// CHECK-NEXT:     let mut d: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while bar((8 as i32) as i8, 15 as i32) - (15 as i32) + d < (1 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (k.0.as_mut_ptr() as *mut *mut i8).offset((baz() + (1 as i32)) as isize) } =
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(b) };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = d;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         d = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { *unsafe { (k.0.as_mut_ptr() as *mut *mut i8).offset((0 as i32) as isize) } } =
// CHECK-NEXT:             -((unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     *unsafe { (k.0.as_mut_ptr() as *mut *mut i8).offset((0 as i32) as isize) }
// CHECK-NEXT:                 }
// CHECK-NEXT:             }) as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     0
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
