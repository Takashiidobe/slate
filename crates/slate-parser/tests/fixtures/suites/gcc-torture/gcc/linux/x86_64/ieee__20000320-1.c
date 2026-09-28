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


// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type0 ull = u64;
// DEFAULT-NEXT:     type @type1 ul = u32;
// DEFAULT-NEXT:     type @type2 fl = union {
// DEFAULT-NEXT:         field0 f: f32;
// DEFAULT-NEXT:         field1 l: u32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type3 dl = union {
// DEFAULT-NEXT:         field0 d: f64;
// DEFAULT-NEXT:         field1 ll: u64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     global %5 uf: @type2 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 ud: @type3 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %8 failed: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%13 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %9 @c(%10 d: u64, %11 f: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<u64>(field1(%7), read<u64>(%10));
// DEFAULT-NEXT:         write<f32>(field0(%5), float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(read<f64>(field0(%7))));
// DEFAULT-NEXT:         if ne<u32>(read<u32>(field1(%5)), read<u32>(%11))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %14: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:                 let %15: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%14), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%8, read<i32>(%15));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(const<u64>(4), const<u64>(4)), ne<u64>(const<u64>(8), const<u64>(8)))
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(u64, u32) -> void>(%9, const<u64>(3931642474694443008), const<u32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(u64, u32) -> void>(%9, const<u64>(3931642474694443009), const<u32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(u64, u32) -> void>(%9, const<u64>(3936146074321813503), const<u32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(u64, u32) -> void>(%9, const<u64>(3936146074321813504), const<u32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(u64, u32) -> void>(%9, const<u64>(3938397874135498751), const<u32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(u64, u32) -> void>(%9, const<u64>(3938397874135498752), const<u32>(2));
// DEFAULT-NEXT:         call<void, signature=fn(u64, u32) -> void>(%9, const<u64>(3940649673949183999), const<u32>(2));
// DEFAULT-NEXT:         call<void, signature=fn(u64, u32) -> void>(%9, const<u64>(3940649673949184000), const<u32>(2));
// DEFAULT-NEXT:         call<void, signature=fn(u64, u32) -> void>(%9, const<u64>(3940649673949184001), const<u32>(2));
// DEFAULT-NEXT:         call<void, signature=fn(u64, u32) -> void>(%9, const<u64>(4039728865214463999), const<u32>(8388607));
// DEFAULT-NEXT:         call<void, signature=fn(u64, u32) -> void>(%9, const<u64>(4039728865214464000), const<u32>(8388608));
// DEFAULT-NEXT:         call<void, signature=fn(u64, u32) -> void>(%9, const<u64>(4039728865214464001), const<u32>(8388608));
// DEFAULT-NEXT:         call<void, signature=fn(u64, u32) -> void>(%9, const<u64>(4039728865751334911), const<u32>(8388608));
// DEFAULT-NEXT:         call<void, signature=fn(u64, u32) -> void>(%9, const<u64>(4039728865751334912), const<u32>(8388608));
// DEFAULT-NEXT:         call<void, signature=fn(u64, u32) -> void>(%9, const<u64>(4039728865751334913), const<u32>(8388608));
// DEFAULT-NEXT:         call<void, signature=fn(u64, u32) -> void>(%9, const<u64>(4039728866019770368), const<u32>(8388608));
// DEFAULT-NEXT:         call<void, signature=fn(u64, u32) -> void>(%9, const<u64>(4039728866019770369), const<u32>(8388609));
// DEFAULT-NEXT:         call<void, signature=fn(u64, u32) -> void>(%9, const<u64>(4039728866556641279), const<u32>(8388609));
// DEFAULT-NEXT:         call<void, signature=fn(u64, u32) -> void>(%9, const<u64>(4039728866556641280), const<u32>(8388610));
// DEFAULT-NEXT:         call<void, signature=fn(u64, u32) -> void>(%9, const<u64>(4039728867093512192), const<u32>(8388610));
// DEFAULT-NEXT:         call<void, signature=fn(u64, u32) -> void>(%9, const<u64>(4039728867093512193), const<u32>(8388611));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%8), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
