/* PR tree-optimization/20100
   Pure function being treated as const.
   Author: Hans-Peter Nilsson.  */

static unsigned short g = 0;
static unsigned short p = 0;
unsigned char         e;

static unsigned short next_g(void) { return g == e - 1 ? 0 : g + 1; }

static unsigned short curr_p(void) { return p; }

static unsigned short inc_g(void) { return g = next_g(); }

static unsigned short curr_g(void) { return g; }

static char ring_empty(void) {
  if (curr_p() == curr_g())
    return 1;
  else
    return 0;
}

char frob(unsigned short a, unsigned short b) {
  g = a;
  p = b;
  inc_g();
  return ring_empty();
}

unsigned short get_n(void) {
  unsigned short n = 0;
  unsigned short org_g;
  org_g = curr_g();
  while (!ring_empty() && n < 5) {
    inc_g();
    n++;
  }

  return n;
}

void abort(void);
void exit(int);
int  main(void) {
  e = 3;
  if (frob(0, 2) != 0 || g != 1 || p != 2 || e != 3 || get_n() != 1 || g != 2 ||
      p != 2)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/20100
// CHECK-NEXT: //! Pure function being treated as const.
// CHECK-NEXT: //! Author: Hans-Peter Nilsson.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut g: u16 = ((0 as i32) as i16) as u16;
// CHECK-EMPTY:
// CHECK-NEXT: static mut p: u16 = ((0 as i32) as i16) as u16;
// CHECK-EMPTY:
// CHECK-NEXT: static mut e: u8 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn next_g() -> u16 {
// CHECK-NEXT:     ((if (((unsafe { g }) as u32) as i32) == (((unsafe { e }) as u32) as i32) - (1 as i32) {
// CHECK-NEXT:         0 as i32
// CHECK-NEXT:     } else {
// CHECK-NEXT:         (((unsafe { g }) as u32) as i32) + (1 as i32)
// CHECK-NEXT:     }) as i16) as u16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn curr_p() -> u16 {
// CHECK-NEXT:     unsafe { p }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn inc_g() -> u16 {
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: u16 = next_g();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         g = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     [[V_1]]
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn curr_g() -> u16 {
// CHECK-NEXT:     unsafe { g }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ring_empty() -> i8 {
// CHECK-NEXT:     if ((curr_p() as u32) as i32) == ((curr_g() as u32) as i32) {
// CHECK-NEXT:         return (1 as i32) as i8;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         return (0 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn frob(mut a: u16, mut b: u16) -> i8 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         g = a;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         p = b;
// CHECK-NEXT:     }
// CHECK-NEXT:     inc_g();
// CHECK-NEXT:     ring_empty()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn get_n() -> u16 {
// CHECK-NEXT:     let mut n: u16 = ((0 as i32) as i16) as u16;
// CHECK-NEXT:     curr_g();
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while !(ring_empty() != (0 as i8)) && ((n as u32) as i32) < (5 as i32) {
// CHECK-NEXT:         inc_g();
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: u16 = n;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: u16 = (((([[V_2]] as u32) as i32) + (1 as i32)) as i16) as u16;
// CHECK-NEXT:         n = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     n
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         e = ((3 as i32) as i8) as u8;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: bool;
// CHECK-NEXT:     if (frob(((0 as i32) as i16) as u16, ((2 as i32) as i16) as u16) as i32) != (0 as i32)
// CHECK-NEXT:         || (((unsafe { g }) as u32) as i32) != (1 as i32)
// CHECK-NEXT:         || (((unsafe { p }) as u32) as i32) != (2 as i32)
// CHECK-NEXT:         || (((unsafe { e }) as u32) as i32) != (3 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         [[V_4]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_4]] = ((get_n() as u32) as i32) != (1 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_4]]
// CHECK-NEXT:         || (((unsafe { g }) as u32) as i32) != (2 as i32)
// CHECK-NEXT:         || (((unsafe { p }) as u32) as i32) != (2 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
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
