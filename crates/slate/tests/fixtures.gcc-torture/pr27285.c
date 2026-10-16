/* PR tree-optimization/27285 */

extern void abort(void);

struct S {
  unsigned char a, b, c, d[16];
};

void __attribute__((noinline)) foo(struct S *x, struct S *y) {
  int           a, b;
  unsigned char c, *d, *e;

  b = x->b;
  d = x->d;
  e = y->d;
  a = 0;
  while (b) {
    if (b >= 8) {
      c  = 0xff;
      b -= 8;
    } else {
      c = 0xff << (8 - b);
      b = 0;
    }

    e[a] = d[a] & c;
    a++;
  }
}

int main(void) {
  struct S x = {0, 25, 0, {0xaa, 0xbb, 0xcc, 0xdd}};
  struct S y = {0, 0, 0, {0}};

  foo(&x, &y);
  if (x.d[0] != y.d[0] || x.d[1] != y.d[1] || x.d[2] != y.d[2] ||
      (x.d[3] & 0x80) != y.d[3])
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/27285
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     a: u8,
// CHECK-NEXT:     b: u8,
// CHECK-NEXT:     c: u8,
// CHECK-NEXT:     d: [u8; 16],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: &mut S, mut y: &mut S) {
// CHECK-NEXT:     let mut a: i32 = 0 as i32;
// CHECK-NEXT:     let mut b: i32 = ((unsafe { (*(x as *mut S)).b }) as u32) as i32;
// CHECK-NEXT:     let mut c: u8 = 0 as u8;
// CHECK-NEXT:     let mut d: *mut u8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut e: *mut u8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     d = unsafe { (*(x as *mut S)).d.as_mut_ptr() as *mut u8 };
// CHECK-NEXT:     e = unsafe { (*(y as *mut S)).d.as_mut_ptr() as *mut u8 };
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while b != (0 as i32) {
// CHECK-NEXT:         if b >= (8 as i32) {
// CHECK-NEXT:             c = ((255 as i32) as i8) as u8;
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: i32 = b;
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] - (8 as i32);
// CHECK-NEXT:             b = [[V_1]];
// CHECK-NEXT:         } else {
// CHECK-NEXT:             c = (((255 as i32) << (8 as i32) - b) as i8) as u8;
// CHECK-NEXT:             b = 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { e.offset(a as isize) } = (((((unsafe { *unsafe { d.offset(a as isize) } })
// CHECK-NEXT:                 as u32) as i32)
// CHECK-NEXT:                 & ((c as u32) as i32)) as i8) as u8;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = a;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         a = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: S = S {
// CHECK-NEXT:         a: ((0 as i32) as i8) as u8,
// CHECK-NEXT:         b: ((25 as i32) as i8) as u8,
// CHECK-NEXT:         c: ((0 as i32) as i8) as u8,
// CHECK-NEXT:         d: {
// CHECK-NEXT:             let mut [[T_0:__t[0-9]+]]: [u8; 16] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_0]][0] = ((170 as i32) as i8) as u8;
// CHECK-NEXT:             [[T_0]][1] = ((187 as i32) as i8) as u8;
// CHECK-NEXT:             [[T_0]][2] = ((204 as i32) as i8) as u8;
// CHECK-NEXT:             [[T_0]][3] = ((221 as i32) as i8) as u8;
// CHECK-NEXT:             [[T_0]]
// CHECK-NEXT:         },
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut y: S = S {
// CHECK-NEXT:         a: ((0 as i32) as i8) as u8,
// CHECK-NEXT:         b: ((0 as i32) as i8) as u8,
// CHECK-NEXT:         c: ((0 as i32) as i8) as u8,
// CHECK-NEXT:         d: {
// CHECK-NEXT:             let mut [[T_1:__t[0-9]+]]: [u8; 16] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_1]][0] = ((0 as i32) as i8) as u8;
// CHECK-NEXT:             [[T_1]]
// CHECK-NEXT:         },
// CHECK-NEXT:     };
// CHECK-NEXT:     foo(unsafe { &mut (*std::ptr::addr_of_mut!(x)) }, unsafe {
// CHECK-NEXT:         &mut (*std::ptr::addr_of_mut!(y))
// CHECK-NEXT:     });
// CHECK-NEXT:     if (((unsafe { *unsafe { (x.d.as_mut_ptr() as *mut u8).offset((0 as i32) as isize) } }) as u32)
// CHECK-NEXT:         as i32)
// CHECK-NEXT:         != (((unsafe { *unsafe { (y.d.as_mut_ptr() as *mut u8).offset((0 as i32) as isize) } })
// CHECK-NEXT:             as u32) as i32)
// CHECK-NEXT:         || (((unsafe { *unsafe { (x.d.as_mut_ptr() as *mut u8).offset((1 as i32) as isize) } })
// CHECK-NEXT:             as u32) as i32)
// CHECK-NEXT:             != (((unsafe { *unsafe { (y.d.as_mut_ptr() as *mut u8).offset((1 as i32) as isize) } })
// CHECK-NEXT:                 as u32) as i32)
// CHECK-NEXT:         || (((unsafe { *unsafe { (x.d.as_mut_ptr() as *mut u8).offset((2 as i32) as isize) } })
// CHECK-NEXT:             as u32) as i32)
// CHECK-NEXT:             != (((unsafe { *unsafe { (y.d.as_mut_ptr() as *mut u8).offset((2 as i32) as isize) } })
// CHECK-NEXT:                 as u32) as i32)
// CHECK-NEXT:         || (((unsafe { *unsafe { (x.d.as_mut_ptr() as *mut u8).offset((3 as i32) as isize) } })
// CHECK-NEXT:             as u32) as i32)
// CHECK-NEXT:             & (128 as i32)
// CHECK-NEXT:             != (((unsafe { *unsafe { (y.d.as_mut_ptr() as *mut u8).offset((3 as i32) as isize) } })
// CHECK-NEXT:                 as u32) as i32)
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
