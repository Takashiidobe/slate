/* { dg-require-effective-target indirect_calls } */
typedef __UINT8_TYPE__  uint8_t;
typedef __UINT16_TYPE__ uint16_t;

typedef uint8_t (*fn1)(void *a);
typedef void    (*fn2)(void *a, int *arg);

struct S {
  uint8_t  buffer[64];
  uint16_t n;
  fn2      f2;
  void    *a;
  fn1      f1;
};

volatile uint16_t x;

void __attribute__((__noinline__, __noclone__)) foo(uint16_t n) { x = n; }

void __attribute__((__noinline__, __noclone__)) testfn(struct S *self) {
  int arg;

  foo(self->n);
  self->n++;
  self->f2(self->a, &arg);
  self->buffer[0] = self->f1(self->a);
}

static unsigned char myfn2_called = 0;

static void myfn2(void *a, int *arg) { myfn2_called = 1; }

static uint8_t myfn1(void *a) { return 0; }

int main(void) {
  struct S s;
  s.n  = 0;
  s.f2 = myfn2;
  s.f1 = myfn1;
  testfn(&s);
  if (myfn2_called != 1)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: /// { dg-require-effective-target indirect_calls }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     buffer: [u8; 64],
// CHECK-NEXT:     n: u16,
// CHECK-NEXT:     f2: Option<unsafe extern "C-unwind" fn(*mut (), *mut i32)>,
// CHECK-NEXT:     a: *mut (),
// CHECK-NEXT:     f1: Option<unsafe extern "C-unwind" fn(*mut ()) -> u8>,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut x: u16 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut n: u16) {
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(x), n) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn testfn(mut self_: &mut S) {
// CHECK-NEXT:     let mut arg: i32 = 0 as i32;
// CHECK-NEXT:     foo(unsafe { (*(self_ as *mut S)).n });
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: u16 = unsafe { (*self_).n };
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: u16 = (((([[V_0]] as u32) as i32) + (1 as i32)) as i16) as u16;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*self_).n = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         unsafe { (*(self_ as *mut S)).f2 }.unwrap()(
// CHECK-NEXT:             unsafe { (*(self_ as *mut S)).a },
// CHECK-NEXT:             std::ptr::addr_of_mut!(arg),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { (*(self_ as *mut S)).buffer.as_mut_ptr() as *mut u8 }
// CHECK-NEXT:                 .offset((0 as i32) as isize)
// CHECK-NEXT:         } = unsafe {
// CHECK-NEXT:             unsafe { (*(self_ as *mut S)).f1 }.unwrap()(unsafe { (*(self_ as *mut S)).a })
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut myfn2_called: u8 = ((0 as i32) as i8) as u8;
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn myfn2(mut a: *mut (), mut arg: *mut i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         myfn2_called = ((1 as i32) as i8) as u8;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn myfn1(mut a: *mut ()) -> u8 {
// CHECK-NEXT:     ((0 as i32) as i8) as u8
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut s: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     s.n = ((0 as i32) as i16) as u16;
// CHECK-NEXT:     s.f2 = Some(myfn2);
// CHECK-NEXT:     s.f1 = Some(myfn1);
// CHECK-NEXT:     testfn(unsafe { &mut (*std::ptr::addr_of_mut!(s)) });
// CHECK-NEXT:     if (((unsafe { myfn2_called }) as u32) as i32) != (1 as i32) {
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
