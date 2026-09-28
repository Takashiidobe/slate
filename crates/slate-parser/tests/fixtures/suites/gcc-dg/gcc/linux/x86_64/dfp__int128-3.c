/* PR libgcc/65833 */
/* Test non-canonical BID significands.  */
/* { dg-require-effective-target int128 } */
/* { dg-require-effective-target bitint } */
/* { dg-options "-O2 -std=gnu2x" } */
/* { dg-require-effective-target dfp_bid } */

union U32
{
  _Decimal32 d;
  unsigned int u;
};

union U64
{
  _Decimal64 d;
  unsigned long long int u;
};

union U128
{
  _Decimal128 d;
  unsigned long long int u[2];
};

int
main ()
{
  volatile union U32 u32;
  u32.d = 0.9999999e+27DF;
  u32.u++;
  volatile union U64 u64;
  u64.d = 0.9999999999999999e+90DD;
  u64.u++;
  volatile union U128 u128;
  u128.d = 0.9999999999999999999999999999999999e+39DL;
  if (u128.u[0] == 0x378d8e63ffffffffULL)
    u128.u[0]++;
  else if (u128.u[1] == 0x378d8e63ffffffffULL)
    u128.u[1]++;
  else
    u128.d = 0.DL;
  if ((__int128) u32.d != 0
      || (unsigned __int128) u32.d != 0U
      || (__int128) u64.d != 0
      || (unsigned __int128) u64.d != 0U
      || (__int128) u128.d != 0
      || (unsigned __int128) u128.d != 0U)
    __builtin_abort ();
  u32.u = 0xe59fffffU;
  u64.u = 0xe3ffffffffffffffULL;
  if (u128.u[0] == 0x378d8e6400000000ULL)
    {
      u128.u[0] = -1ULL;
      u128.u[1] = 0xe1be7fffffffffffULL;
    }
  else if (u128.u[1] == 0x378d8e6400000000ULL)
    {
      u128.u[1] = -1ULL;
      u128.u[0] = 0xe1be7fffffffffffULL;
    }
  if ((__int128) u32.d != 0
      || (unsigned __int128) u32.d != 0U
      || (__int128) u64.d != 0
      || (unsigned __int128) u64.d != 0U
      || (__int128) u128.d != 0
      || (unsigned __int128) u128.d != 0U)
    __builtin_abort ();
  if (u128.u[0] == -1ULL)
    {
      u128.u[0] = 0;
      u128.u[1] = 0xe629800000000000ULL;
    }
  else if (u128.u[1] == -1ULL)
    {
      u128.u[1] = 0;
      u128.u[0] = 0xe629800000000000ULL;
    }
  if ((__int128) u128.d != 0
      || (unsigned __int128) u128.d != 0U)
    __builtin_abort ();
}

// SLATE-FILECHECK-STD DEFAULT gnu2x
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
// DEFAULT-NEXT:     type @type0 U32 = union {
// DEFAULT-NEXT:         field0 d: d32;
// DEFAULT-NEXT:         field1 u: u32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type1 U64 = union {
// DEFAULT-NEXT:         field0 d: d64;
// DEFAULT-NEXT:         field1 u: u64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type2 U128 = union {
// DEFAULT-NEXT:         field0 d: d128;
// DEFAULT-NEXT:         field1 u: array<u64, 2>;
// DEFAULT-NEXT:     } [size=16, align=16, offsets=[0, 0]];
// DEFAULT-NEXT:     fn %7 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %4 u32: volatile @type0 [storage=automatic];
// DEFAULT-NEXT:         write<d32, volatile>(field0(%4), const<d32>(0.9999999e+27));
// DEFAULT-NEXT:         let %8: u32 [synthetic] = read<u32, volatile>(field1(%4));
// DEFAULT-NEXT:         let %9: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%8), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32, volatile>(field1(%4), read<u32>(%9));
// DEFAULT-NEXT:         let %5 u64: volatile @type1 [storage=automatic];
// DEFAULT-NEXT:         write<d64, volatile>(field0(%5), const<d64>(0.9999999999999999e+90));
// DEFAULT-NEXT:         let %10: u64 [synthetic] = read<u64, volatile>(field1(%5));
// DEFAULT-NEXT:         let %11: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%10), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         write<u64, volatile>(field1(%5), read<u64>(%11));
// DEFAULT-NEXT:         let %6 u128: volatile @type2 [storage=automatic];
// DEFAULT-NEXT:         write<d128, volatile>(field0(%6), const<d128>(0.9999999999999999999999999999999999e+39));
// DEFAULT-NEXT:         if eq<u64>(read<u64, volatile>(deref(ptr_offset<ptr<volatile u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<volatile u64>, length=Some(2)>(field1(%6)), const<i32>(0)))), const<u64>(4003012203950112767))
// DEFAULT-NEXT:             let %12: ptr<volatile u64> [synthetic] = ptr_offset<ptr<volatile u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<volatile u64>, length=Some(2)>(field1(%6)), const<i32>(0));
// DEFAULT-NEXT:             let %13: u64 [synthetic] = read<u64, volatile>(deref(read<ptr<volatile u64>>(%12)));
// DEFAULT-NEXT:             let %14: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%13), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:             write<u64, volatile>(deref(read<ptr<volatile u64>>(%12)), read<u64>(%14));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if eq<u64>(read<u64, volatile>(deref(ptr_offset<ptr<volatile u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<volatile u64>, length=Some(2)>(field1(%6)), const<i32>(1)))), const<u64>(4003012203950112767))
// DEFAULT-NEXT:                 let %15: ptr<volatile u64> [synthetic] = ptr_offset<ptr<volatile u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<volatile u64>, length=Some(2)>(field1(%6)), const<i32>(1));
// DEFAULT-NEXT:                 let %16: u64 [synthetic] = read<u64, volatile>(deref(read<ptr<volatile u64>>(%15)));
// DEFAULT-NEXT:                 let %17: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%16), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                 write<u64, volatile>(deref(read<ptr<volatile u64>>(%15)), read<u64>(%17));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128, volatile>(field0(%6), const<d128>(0.));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i128>(float_to_int<i128, reason=explicit, out_of_range=ub, exceptions=observable>(read<d32, volatile>(field0(%4))), widen<i128, reason=usual_arith>(const<i32>(0))), ne<u128>(float_to_int<u128, reason=explicit, out_of_range=ub, exceptions=observable>(read<d32, volatile>(field0(%4))), widen<u128, reason=usual_arith>(const<u32>(0)))), ne<i128>(float_to_int<i128, reason=explicit, out_of_range=ub, exceptions=observable>(read<d64, volatile>(field0(%5))), widen<i128, reason=usual_arith>(const<i32>(0)))), ne<u128>(float_to_int<u128, reason=explicit, out_of_range=ub, exceptions=observable>(read<d64, volatile>(field0(%5))), widen<u128, reason=usual_arith>(const<u32>(0)))), ne<i128>(float_to_int<i128, reason=explicit, out_of_range=ub, exceptions=observable>(read<d128, volatile>(field0(%6))), widen<i128, reason=usual_arith>(const<i32>(0)))), ne<u128>(float_to_int<u128, reason=explicit, out_of_range=ub, exceptions=observable>(read<d128, volatile>(field0(%6))), widen<u128, reason=usual_arith>(const<u32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%7);
// DEFAULT-NEXT:         write<u32, volatile>(field1(%4), const<u32>(3852468223));
// DEFAULT-NEXT:         write<u64, volatile>(field1(%5), const<u64>(16429131440647569407));
// DEFAULT-NEXT:         if eq<u64>(read<u64, volatile>(deref(ptr_offset<ptr<volatile u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<volatile u64>, length=Some(2)>(field1(%6)), const<i32>(0)))), const<u64>(4003012203950112768))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<u64, volatile>(deref(ptr_offset<ptr<volatile u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<volatile u64>, length=Some(2)>(field1(%6)), const<i32>(0))), neg<u64, overflow=wrap>(const<u64>(1)));
// DEFAULT-NEXT:                 write<u64, volatile>(deref(ptr_offset<ptr<volatile u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<volatile u64>, length=Some(2)>(field1(%6)), const<i32>(1))), const<u64>(16266579641597165567));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if eq<u64>(read<u64, volatile>(deref(ptr_offset<ptr<volatile u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<volatile u64>, length=Some(2)>(field1(%6)), const<i32>(1)))), const<u64>(4003012203950112768))
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<u64, volatile>(deref(ptr_offset<ptr<volatile u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<volatile u64>, length=Some(2)>(field1(%6)), const<i32>(1))), neg<u64, overflow=wrap>(const<u64>(1)));
// DEFAULT-NEXT:                     write<u64, volatile>(deref(ptr_offset<ptr<volatile u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<volatile u64>, length=Some(2)>(field1(%6)), const<i32>(0))), const<u64>(16266579641597165567));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i128>(float_to_int<i128, reason=explicit, out_of_range=ub, exceptions=observable>(read<d32, volatile>(field0(%4))), widen<i128, reason=usual_arith>(const<i32>(0))), ne<u128>(float_to_int<u128, reason=explicit, out_of_range=ub, exceptions=observable>(read<d32, volatile>(field0(%4))), widen<u128, reason=usual_arith>(const<u32>(0)))), ne<i128>(float_to_int<i128, reason=explicit, out_of_range=ub, exceptions=observable>(read<d64, volatile>(field0(%5))), widen<i128, reason=usual_arith>(const<i32>(0)))), ne<u128>(float_to_int<u128, reason=explicit, out_of_range=ub, exceptions=observable>(read<d64, volatile>(field0(%5))), widen<u128, reason=usual_arith>(const<u32>(0)))), ne<i128>(float_to_int<i128, reason=explicit, out_of_range=ub, exceptions=observable>(read<d128, volatile>(field0(%6))), widen<i128, reason=usual_arith>(const<i32>(0)))), ne<u128>(float_to_int<u128, reason=explicit, out_of_range=ub, exceptions=observable>(read<d128, volatile>(field0(%6))), widen<u128, reason=usual_arith>(const<u32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%7);
// DEFAULT-NEXT:         if eq<u64>(read<u64, volatile>(deref(ptr_offset<ptr<volatile u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<volatile u64>, length=Some(2)>(field1(%6)), const<i32>(0)))), neg<u64, overflow=wrap>(const<u64>(1)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<u64, volatile>(deref(ptr_offset<ptr<volatile u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<volatile u64>, length=Some(2)>(field1(%6)), const<i32>(0))), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:                 write<u64, volatile>(deref(ptr_offset<ptr<volatile u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<volatile u64>, length=Some(2)>(field1(%6)), const<i32>(1))), const<u64>(16584927840256917504));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if eq<u64>(read<u64, volatile>(deref(ptr_offset<ptr<volatile u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<volatile u64>, length=Some(2)>(field1(%6)), const<i32>(1)))), neg<u64, overflow=wrap>(const<u64>(1)))
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<u64, volatile>(deref(ptr_offset<ptr<volatile u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<volatile u64>, length=Some(2)>(field1(%6)), const<i32>(1))), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:                     write<u64, volatile>(deref(ptr_offset<ptr<volatile u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<volatile u64>, length=Some(2)>(field1(%6)), const<i32>(0))), const<u64>(16584927840256917504));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if logical_or<bool>(ne<i128>(float_to_int<i128, reason=explicit, out_of_range=ub, exceptions=observable>(read<d128, volatile>(field0(%6))), widen<i128, reason=usual_arith>(const<i32>(0))), ne<u128>(float_to_int<u128, reason=explicit, out_of_range=ub, exceptions=observable>(read<d128, volatile>(field0(%6))), widen<u128, reason=usual_arith>(const<u32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%7);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
