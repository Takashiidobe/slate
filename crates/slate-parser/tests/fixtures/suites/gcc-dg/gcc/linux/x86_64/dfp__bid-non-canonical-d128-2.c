/* Test non-canonical BID significands: _Decimal128, case where
   combination field starts 11.  Bug 91226.  */
/* { dg-do run { target { lp64 && dfprt } } } */
/* { dg-require-effective-target dfp_bid } */
/* { dg-options "-std=gnu23 -O2" } */

extern void abort (void);
extern void exit (int);

union u
{
  _Decimal128 d128;
  unsigned __int128 u128;
};

#define U128(hi, lo) (((unsigned __int128) lo) \
		      | (((unsigned __int128) hi) << 64))

int
main (void)
{
  unsigned __int128 i = U128 (0x6e79000000000000ULL, 0x1ULL);
  union u x;
  _Decimal128 d128;
  x.u128 = i;
  d128 = x.d128;
  volatile double d = d128;
  if (d != 0)
    abort ();
  /* The above number should have quantum exponent 1234.  */
  _Decimal128 t1233 = 0.e1233DL, t1234 = 0.e1234DL, t1235 = 0.e1235DL;
  _Decimal128 dx;
  dx = d128 + t1233;
  if (__builtin_memcmp (&dx, &t1233, 16) != 0)
    abort ();
  dx = d128 + t1234;
  if (__builtin_memcmp (&dx, &t1234, 16) != 0)
    abort ();
  dx = d128 + t1235;
  if (__builtin_memcmp (&dx, &t1234, 16) != 0)
    abort ();
  exit (0);
}

// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     type @type0 u = union {
// DEFAULT-NEXT:         field0 d128: d128;
// DEFAULT-NEXT:         field1 u128: u128;
// DEFAULT-NEXT:     } [size=16, align=16, offsets=[0, 0]];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%12 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %16 @__builtin_memcmp(%13 <unnamed>: ptr<const void>, %14 <unnamed>: ptr<const void>, %15 <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %4 i: u128 [storage=automatic] = or<u128>(widen<u128, reason=explicit>(const<u64>(1)), shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(7960393816354062336)), const<i32>(64)));
// DEFAULT-NEXT:         let %5 x: @type0 [storage=automatic];
// DEFAULT-NEXT:         let %6 d128: d128 [storage=automatic];
// DEFAULT-NEXT:         write<u128>(field1(%5), read<u128>(%4));
// DEFAULT-NEXT:         write<d128>(%6, read<d128>(field0(%5)));
// DEFAULT-NEXT:         let %7 d: volatile f64 [storage=automatic] = float_convert<f64, reason=assign, rounding=nearest_even, exceptions=observable>(read<d128>(%6));
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(read<f64, volatile>(%7), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %8 t1233: d128 [storage=automatic] = const<d128>(0.e1233);
// DEFAULT-NEXT:         let %9 t1234: d128 [storage=automatic] = const<d128>(0.e1234);
// DEFAULT-NEXT:         let %10 t1235: d128 [storage=automatic] = const<d128>(0.e1235);
// DEFAULT-NEXT:         let %11 dx: d128 [storage=automatic];
// DEFAULT-NEXT:         write<d128>(%11, add<d128, rounding=nearest_even, exceptions=observable, contract=fast>(read<d128>(%6), read<d128>(%8)));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%16, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%11)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%8)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<d128>(%11, add<d128, rounding=nearest_even, exceptions=observable, contract=fast>(read<d128>(%6), read<d128>(%9)));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%16, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%11)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<d128>(%11, add<d128, rounding=nearest_even, exceptions=observable, contract=fast>(read<d128>(%6), read<d128>(%10)));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%16, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%11)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
