/* { dg-do run }
   # C6X floating point hardware turns denormals to zero in multiplications.
   { dg-xfail-if "" { tic6x-*-* && ti_c67x } }

   # ColdFire FPUs require software handling of subnormals.  We are
   # not aware of any system that has this.
   { dg-xfail-if "" { m68k-*-* && coldfire_fpu } }

   # The C-SKY hardware FPU only supports flush-to-zero mode.
   { dg-xfail-if "" { csky-*-* && hard_float } }

   # The Epiphany single-precision floating point format does not
   # support subnormals.
   { dg-skip-if "" { epiphany-*-* } } */

/* Check that certain subnormal numbers (formerly known as denormalized
   numbers) are rounded to within 0.5 ulp.  PR other/14354.  */

/* This test requires that float and unsigned int are the same size and
   that the sign-bit of the float is at MSB of the unsigned int.  */

void abort(void);
void exit(int);

#if __INT_MAX__ != 2147483647L
int main() { exit(0); }
#else

union uf {
  unsigned int u;
  float        f;
};

static float u2f(unsigned int v) {
  union uf u;
  u.u = v;
  return u.f;
}

static unsigned int f2u(float v) {
  union uf u;
  u.f = v;
  return u.u;
}

int ok = 1;

static void tstmul(unsigned int ux, unsigned int uy, unsigned int ur) {
  float x = u2f(ux);
  float y = u2f(uy);

  if (f2u(x * y) != ur)
    /* Set a variable rather than aborting here, to simplify tracing when
       several computations are wrong.  */
    ok = 0;
}

/* We don't want to make this const and static, or else we risk inlining
   causing the test to fold as constants at compile-time.  */
struct {
  unsigned int p1, p2, res;
} expected[] = {{0xfff, 0x3f800400, 0xfff},
                {0xf, 0x3fc88888, 0x17},
                {0xf, 0x3f844444, 0xf}};

int main() {
  unsigned int i;

  for (i = 0; i < sizeof(expected) / sizeof(expected[0]); i++) {
    tstmul(expected[i].p1, expected[i].p2, expected[i].res);
    tstmul(expected[i].p2, expected[i].p1, expected[i].res);
  }

  if (!ok)
    abort();

  exit(0);
}
#endif

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-do run }
// CHECK-NEXT: //! # C6X floating point hardware turns denormals to zero in multiplications.
// CHECK-NEXT: //! { dg-xfail-if "" { tic6x-*-* && ti_c67x } }
// CHECK-NEXT: //!
// CHECK-NEXT: //! # ColdFire FPUs require software handling of subnormals.  We are
// CHECK-NEXT: //! # not aware of any system that has this.
// CHECK-NEXT: //! { dg-xfail-if "" { m68k-*-* && coldfire_fpu } }
// CHECK-NEXT: //!
// CHECK-NEXT: //! # The C-SKY hardware FPU only supports flush-to-zero mode.
// CHECK-NEXT: //! { dg-xfail-if "" { csky-*-* && hard_float } }
// CHECK-NEXT: //!
// CHECK-NEXT: //! # The Epiphany single-precision floating point format does not
// CHECK-NEXT: //! # support subnormals.
// CHECK-NEXT: //! { dg-skip-if "" { epiphany-*-* } }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // Check that certain subnormal numbers (formerly known as denormalized
// CHECK-NEXT: // numbers) are rounded to within 0.5 ulp.  PR other/14354.
// CHECK-NEXT: // This test requires that float and unsigned int are the same size and
// CHECK-NEXT: // that the sign-bit of the float is at MSB of the unsigned int.
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union uf {
// CHECK-NEXT:     u: u32,
// CHECK-NEXT:     f: f32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn u2f(mut v: u32) -> f32 {
// CHECK-NEXT:     let mut u: uf = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         u.u = v;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { u.f }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn f2u(mut v: f32) -> u32 {
// CHECK-NEXT:     let mut u: uf = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         u.f = v;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { u.u }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut ok: i32 = 1 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: fn tstmul(mut ux: u32, mut uy: u32, mut ur: u32) {
// CHECK-NEXT:     let mut x: f32 = u2f(ux);
// CHECK-NEXT:     let mut y: f32 = u2f(uy);
// CHECK-NEXT:     if f2u(x * y) != ur {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             ok = 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     // Set a variable rather than aborting here, to simplify tracing when
// CHECK-NEXT:     // several computations are wrong.
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: /// We don't want to make this const and static, or else we risk inlining
// CHECK-NEXT: /// causing the test to fold as constants at compile-time.
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     p1: u32,
// CHECK-NEXT:     p2: u32,
// CHECK-NEXT:     res: u32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut expected: __SlateAlign16<[[[SLATERECORD_0]]; 3]> = __SlateAlign16([
// CHECK-NEXT:     [[SLATERECORD_0]] {
// CHECK-NEXT:         p1: (4095 as i32) as u32,
// CHECK-NEXT:         p2: (1065354240 as i32) as u32,
// CHECK-NEXT:         res: (4095 as i32) as u32,
// CHECK-NEXT:     },
// CHECK-NEXT:     [[SLATERECORD_0]] {
// CHECK-NEXT:         p1: (15 as i32) as u32,
// CHECK-NEXT:         p2: (1070106760 as i32) as u32,
// CHECK-NEXT:         res: (23 as i32) as u32,
// CHECK-NEXT:     },
// CHECK-NEXT:     [[SLATERECORD_0]] {
// CHECK-NEXT:         p1: (15 as i32) as u32,
// CHECK-NEXT:         p2: (1065632836 as i32) as u32,
// CHECK-NEXT:         res: (15 as i32) as u32,
// CHECK-NEXT:     },
// CHECK-NEXT: ]);
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: u32 = (0 as i32) as u32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (i as u64) < (36 as u64) / (12 as u64) {
// CHECK-NEXT:         tstmul(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(expected.0) as *mut [[SLATERECORD_0]] }
// CHECK-NEXT:                         .offset(i as isize)
// CHECK-NEXT:                 })
// CHECK-NEXT:                 .p1
// CHECK-NEXT:             },
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(expected.0) as *mut [[SLATERECORD_0]] }
// CHECK-NEXT:                         .offset(i as isize)
// CHECK-NEXT:                 })
// CHECK-NEXT:                 .p2
// CHECK-NEXT:             },
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(expected.0) as *mut [[SLATERECORD_0]] }
// CHECK-NEXT:                         .offset(i as isize)
// CHECK-NEXT:                 })
// CHECK-NEXT:                 .res
// CHECK-NEXT:             },
// CHECK-NEXT:         );
// CHECK-NEXT:         tstmul(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(expected.0) as *mut [[SLATERECORD_0]] }
// CHECK-NEXT:                         .offset(i as isize)
// CHECK-NEXT:                 })
// CHECK-NEXT:                 .p2
// CHECK-NEXT:             },
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(expected.0) as *mut [[SLATERECORD_0]] }
// CHECK-NEXT:                         .offset(i as isize)
// CHECK-NEXT:                 })
// CHECK-NEXT:                 .p1
// CHECK-NEXT:             },
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(expected.0) as *mut [[SLATERECORD_0]] }
// CHECK-NEXT:                         .offset(i as isize)
// CHECK-NEXT:                 })
// CHECK-NEXT:                 .res
// CHECK-NEXT:             },
// CHECK-NEXT:         );
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: u32 = i;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: u32 = [[V_1]].wrapping_add((1 as i32) as u32);
// CHECK-NEXT:         i = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if !((unsafe { ok }) != (0 as i32)) {
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
