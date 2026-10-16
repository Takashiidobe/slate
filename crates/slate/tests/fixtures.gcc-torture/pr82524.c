/* PR target/82524 */

struct S {
  unsigned char b, g, r, a;
};
union U {
  struct S c;
  unsigned v;
};

static inline unsigned char foo(unsigned char a, unsigned char b) {
  return ((a + 1) * b) >> 8;
}

__attribute__((noinline, noclone)) unsigned bar(union U *x, union U *y) {
  union U       z;
  unsigned char v = x->c.a;
  unsigned char w = foo(y->c.a, 255 - v);
  z.c.r           = foo(x->c.r, v) + foo(y->c.r, w);
  z.c.g           = foo(x->c.g, v) + foo(y->c.g, w);
  z.c.b           = foo(x->c.b, v) + foo(y->c.b, w);
  z.c.a           = 0;
  return z.v;
}

int main() {
  union U a, b, c;
  if ((unsigned char)~0 != 255 || sizeof(unsigned) != 4)
    return 0;
  a.c = (struct S){255, 255, 255, 0};
  b.c = (struct S){255, 255, 255, 255};
  c.v = bar(&a, &b);
  if (c.c.b != 255 || c.c.g != 255 || c.c.r != 255 || c.c.a != 0)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR target/82524
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     b: u8,
// CHECK-NEXT:     g: u8,
// CHECK-NEXT:     r: u8,
// CHECK-NEXT:     a: u8,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union U {
// CHECK-NEXT:     c: S,
// CHECK-NEXT:     v: u32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut a: u8, mut b: u8) -> u8 {
// CHECK-NEXT:     (((((a as u32) as i32) + (1 as i32)) * ((b as u32) as i32) >> (8 as i32)) as i8) as u8
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut x: &U, mut y: &U) -> u32 {
// CHECK-NEXT:     let mut z: U = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut v: u8 = unsafe { (*(x as *const U)).c.a };
// CHECK-NEXT:     let mut w: u8 = foo(
// CHECK-NEXT:         unsafe { (*(y as *const U)).c.a },
// CHECK-NEXT:         (((255 as i32) - ((v as u32) as i32)) as i8) as u8,
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         z.c.r = ((((foo(unsafe { (*(x as *const U)).c.r }, v) as u32) as i32)
// CHECK-NEXT:             + ((foo(unsafe { (*(y as *const U)).c.r }, w) as u32) as i32)) as i8)
// CHECK-NEXT:             as u8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         z.c.g = ((((foo(unsafe { (*(x as *const U)).c.g }, v) as u32) as i32)
// CHECK-NEXT:             + ((foo(unsafe { (*(y as *const U)).c.g }, w) as u32) as i32)) as i8)
// CHECK-NEXT:             as u8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         z.c.b = ((((foo(unsafe { (*(x as *const U)).c.b }, v) as u32) as i32)
// CHECK-NEXT:             + ((foo(unsafe { (*(y as *const U)).c.b }, w) as u32) as i32)) as i8)
// CHECK-NEXT:             as u8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         z.c.a = ((0 as i32) as i8) as u8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { z.v }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_1:__slate_compound_[0-9]+]]: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut a: U = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut b: U = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut c: U = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     if ((((!(0 as i32) as i8) as u8) as u32) as i32) != (255 as i32)
// CHECK-NEXT:         || (4 as u64) != (((4 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         a.c = unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_0]] = S {
// CHECK-NEXT:                     b: ((255 as i32) as i8) as u8,
// CHECK-NEXT:                     g: ((255 as i32) as i8) as u8,
// CHECK-NEXT:                     r: ((255 as i32) as i8) as u8,
// CHECK-NEXT:                     a: ((0 as i32) as i8) as u8,
// CHECK-NEXT:                 };
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         b.c = unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_1]] = S {
// CHECK-NEXT:                     b: ((255 as i32) as i8) as u8,
// CHECK-NEXT:                     g: ((255 as i32) as i8) as u8,
// CHECK-NEXT:                     r: ((255 as i32) as i8) as u8,
// CHECK-NEXT:                     a: ((255 as i32) as i8) as u8,
// CHECK-NEXT:                 };
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_1]])
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         c.v = bar(unsafe { &(*std::ptr::addr_of_mut!(a)) }, unsafe {
// CHECK-NEXT:             &(*std::ptr::addr_of_mut!(b))
// CHECK-NEXT:         });
// CHECK-NEXT:     }
// CHECK-NEXT:     if (((unsafe { c.c.b }) as u32) as i32) != (255 as i32)
// CHECK-NEXT:         || (((unsafe { c.c.g }) as u32) as i32) != (255 as i32)
// CHECK-NEXT:         || (((unsafe { c.c.r }) as u32) as i32) != (255 as i32)
// CHECK-NEXT:         || (((unsafe { c.c.a }) as u32) as i32) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
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
