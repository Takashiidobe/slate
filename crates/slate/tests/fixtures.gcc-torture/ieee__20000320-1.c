/* { dg-do run }

   # AVR doubles are floats
   { dg-skip-if "AVR doubles are floats" { avr-*-* } }

   # ColdFire FPUs require software handling of subnormals.  We are
   # not aware of any system that has this.
   { dg-xfail-if "" { m68k-*-* && coldfire_fpu } }

   # C6X floating point hardware turns denormals to zero in FP conversions.
   { dg-xfail-if "" { tic6x-*-* && ti_c67x } } */

void abort(void);
void exit(int);

#if __INT_MAX__ != 2147483647 ||                                               \
    (__LONG_LONG_MAX__ != 9223372036854775807ll &&                             \
     __LONG_MAX__ != 9223372036854775807ll)
int main(void) { exit(0); }
#else
#if __LONG_MAX__ != 9223372036854775807ll
typedef unsigned long long ull;
#else
typedef unsigned long ull;
#endif
typedef unsigned ul;

union fl {
  float f;
  ul    l;
} uf;
union dl {
  double d;
  ull    ll;
} ud;

int failed = 0;

void c(ull d, ul f) {
  ud.ll = d;
  uf.f  = (float)ud.d;
  if (uf.l != f) {
    failed++;
  }
}

int main() {
  if (sizeof(float) != sizeof(ul) || sizeof(double) != sizeof(ull))
    exit(0);

#if (defined __arm__ || defined __thumb__) &&                                  \
    !(defined __ARMEB__ || defined __VFP_FP__)
  /* The ARM always stores FP numbers in big-wordian format,
     even when running in little-byteian mode.  */
  c(0x0000000036900000ULL, 0x00000000U);
  c(0x0000000136900000ULL, 0x00000001U);
  c(0xffffffff369fffffULL, 0x00000001U);
  c(0x0000000036A00000ULL, 0x00000001U);
  c(0xffffffff36A7ffffULL, 0x00000001U);
  c(0x0000000036A80000ULL, 0x00000002U);
  c(0xffffffff36AfffffULL, 0x00000002U);
  c(0x0000000036b00000ULL, 0x00000002U);
  c(0x0000000136b00000ULL, 0x00000002U);

  c(0xdfffffff380fffffULL, 0x007fffffU);
  c(0xe0000000380fffffULL, 0x00800000U);
  c(0xe0000001380fffffULL, 0x00800000U);
  c(0xffffffff380fffffULL, 0x00800000U);
  c(0x0000000038100000ULL, 0x00800000U);
  c(0x0000000138100000ULL, 0x00800000U);
  c(0x1000000038100000ULL, 0x00800000U);
  c(0x1000000138100000ULL, 0x00800001U);
  c(0x2fffffff38100000ULL, 0x00800001U);
  c(0x3000000038100000ULL, 0x00800002U);
  c(0x5000000038100000ULL, 0x00800002U);
  c(0x5000000138100000ULL, 0x00800003U);
#else
  c(0x3690000000000000ULL, 0x00000000U);
  c(0x3690000000000001ULL, 0x00000001U);
  c(0x369fffffffffffffULL, 0x00000001U);
  c(0x36A0000000000000ULL, 0x00000001U);
  c(0x36A7ffffffffffffULL, 0x00000001U);
  c(0x36A8000000000000ULL, 0x00000002U);
  c(0x36AfffffffffffffULL, 0x00000002U);
  c(0x36b0000000000000ULL, 0x00000002U);
  c(0x36b0000000000001ULL, 0x00000002U);

  c(0x380fffffdfffffffULL, 0x007fffffU);
  c(0x380fffffe0000000ULL, 0x00800000U);
  c(0x380fffffe0000001ULL, 0x00800000U);
  c(0x380fffffffffffffULL, 0x00800000U);
  c(0x3810000000000000ULL, 0x00800000U);
  c(0x3810000000000001ULL, 0x00800000U);
  c(0x3810000010000000ULL, 0x00800000U);
  c(0x3810000010000001ULL, 0x00800001U);
  c(0x381000002fffffffULL, 0x00800001U);
  c(0x3810000030000000ULL, 0x00800002U);
  c(0x3810000050000000ULL, 0x00800002U);
  c(0x3810000050000001ULL, 0x00800003U);
#endif

  if (failed)
    abort();
  else
    exit(0);
}
#endif

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-do run }
// CHECK-NEXT: //!
// CHECK-NEXT: //! # AVR doubles are floats
// CHECK-NEXT: //! { dg-skip-if "AVR doubles are floats" { avr-*-* } }
// CHECK-NEXT: //!
// CHECK-NEXT: //! # ColdFire FPUs require software handling of subnormals.  We are
// CHECK-NEXT: //! # not aware of any system that has this.
// CHECK-NEXT: //! { dg-xfail-if "" { m68k-*-* && coldfire_fpu } }
// CHECK-NEXT: //!
// CHECK-NEXT: //! # C6X floating point hardware turns denormals to zero in FP conversions.
// CHECK-NEXT: //! { dg-xfail-if "" { tic6x-*-* && ti_c67x } }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union fl {
// CHECK-NEXT:     f: f32,
// CHECK-NEXT:     l: u32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut uf: fl = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union dl {
// CHECK-NEXT:     d: f64,
// CHECK-NEXT:     ll: u64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut ud: dl = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut failed: i32 = 0 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: fn c(mut d: u64, mut f: u32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         ud.ll = d;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         uf.f = (unsafe { ud.d }) as f32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { uf.l }) != f {
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = unsafe { failed };
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             failed = [[V_2]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (4 as u64) != (4 as u64) || (8 as u64) != (8 as u64) {
// CHECK-NEXT:         unsafe { exit(0 as i32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     c(3931642474694443008 as u64, 0 as u32);
// CHECK-NEXT:     c(3931642474694443009 as u64, 1 as u32);
// CHECK-NEXT:     c(3936146074321813503 as u64, 1 as u32);
// CHECK-NEXT:     c(3936146074321813504 as u64, 1 as u32);
// CHECK-NEXT:     c(3938397874135498751 as u64, 1 as u32);
// CHECK-NEXT:     c(3938397874135498752 as u64, 2 as u32);
// CHECK-NEXT:     c(3940649673949183999 as u64, 2 as u32);
// CHECK-NEXT:     c(3940649673949184000 as u64, 2 as u32);
// CHECK-NEXT:     c(3940649673949184001 as u64, 2 as u32);
// CHECK-NEXT:     c(4039728865214463999 as u64, 8388607 as u32);
// CHECK-NEXT:     c(4039728865214464000 as u64, 8388608 as u32);
// CHECK-NEXT:     c(4039728865214464001 as u64, 8388608 as u32);
// CHECK-NEXT:     c(4039728865751334911 as u64, 8388608 as u32);
// CHECK-NEXT:     c(4039728865751334912 as u64, 8388608 as u32);
// CHECK-NEXT:     c(4039728865751334913 as u64, 8388608 as u32);
// CHECK-NEXT:     c(4039728866019770368 as u64, 8388608 as u32);
// CHECK-NEXT:     c(4039728866019770369 as u64, 8388609 as u32);
// CHECK-NEXT:     c(4039728866556641279 as u64, 8388609 as u32);
// CHECK-NEXT:     c(4039728866556641280 as u64, 8388610 as u32);
// CHECK-NEXT:     c(4039728867093512192 as u64, 8388610 as u32);
// CHECK-NEXT:     c(4039728867093512193 as u64, 8388611 as u32);
// CHECK-NEXT:     if (unsafe { failed }) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe { exit(0 as i32) };
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
