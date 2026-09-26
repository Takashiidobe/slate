/* PR c/102989 */
/* { dg-do run { target bitint } } */
/* { dg-options "-std=c23 -pedantic-errors" } */
/* { dg-skip-if "" { ! run_expensive_tests }  { "*" } { "-O0" "-O2" } } */
/* { dg-skip-if "" { ! run_expensive_tests } { "-flto" } { "" } } */

__attribute__((noipa)) void foo(unsigned _BitInt(25) a, unsigned _BitInt(27) b,
                                unsigned _BitInt(25) * p,
                                unsigned _BitInt(27) * q, float c) {
  p[0]   = b;
  q[0]   = a;
  q[1]   = (signed _BitInt(25))a;
  q[2]   = (_BitInt(12))a;
  q[3]   = c;
  q[4]  += a;
  q[5]   = a + b;
  q[6]   = a - b;
  q[7]   = a * b;
  q[8]   = a / b;
  q[9]   = a % b;
  q[10]  = b << (24320393uwb - a);
  q[11]  = (b * 131uwb) >> (24320393uwb - a);
  q[12]++;
  ++q[13];
  q[14]--;
  --q[15];
  q[16]   = a == b;
  q[17]   = a != b;
  q[18]   = a > b;
  q[19]   = a < b;
  q[20]   = a >= b;
  q[21]   = a <= b;
  q[22]   = a && b;
  q[23]   = a || b;
  q[24]   = !a;
  q[25]   = a & b;
  q[26]   = a | b;
  q[27]   = a ^ b;
  q[28]   = ~a;
  q[29]  -= a;
  q[30]  *= b;
  q[31]  /= b;
  q[32]  %= a;
  q[33] <<= (125900uwb - b);
  q[34] >>= (125901uwb - b);
  q[35]  &= a;
  q[36]  |= b;
  q[37]  ^= a;
  q[38]   = sizeof(a);
  q[39]   = q[39] ? a : b;
  q[40]   = 24196214uwb;
  switch (a) {
  case 12345641uwb:
    if (b != 1244wb)
      __builtin_abort();
    break;
  case 11821400uwb:
    if (b != 133445uwb)
      __builtin_abort();
    break;
  case 12145uwb:
    if (b != 1212uwb)
      __builtin_abort();
    break;
  case 24320389uwb:
    if (b != 125897uwb)
      __builtin_abort();
    break;
  case 7128412uwb:
    if (b != 150uwb)
      __builtin_abort();
    break;
  default:
    __builtin_abort();
  }
}

int
main() {
  unsigned _BitInt(25) p;
  unsigned _BitInt(27) q[41];
  static unsigned _BitInt(27) qe[41] = {
      24320389uwb, 124983685uwb, 134216069uwb, 42uwb,      24320407uwb,
      24446286uwb, 24194492uwb,  89202797uwb,  193uwb,     22268uwb,
      2014352uwb,  1030781uwb,   8uwb,         10uwb,      10uwb,
      12uwb,       0uwb,         1uwb,         1uwb,       0uwb,
      1uwb,        0uwb,         1uwb,         1uwb,       0uwb,
      67969uwb,    24378317uwb,  24310348uwb,  9234042uwb, 109897346uwb,
      125897uwb,   0uwb,         52uwb,        40uwb,      771uwb,
      5uwb,        125917uwb,    24320384uwb,  1uwb,       24320389uwb,
      24196214uwb};
  q[4]  = 18uwb;
  q[12] = 7uwb;
  q[13] = 9uwb;
  q[14] = 11uwb;
  q[15] = 13uwb;
  q[29] = 7uwb;
  q[30] = -1uwb;
  q[31] = -13uwb;
  q[32] = 52uwb;
  q[33] = 5uwb;
  q[34] = 12345uwb;
  q[35] = 15uwb;
  q[36] = 28uwb;
  q[37] = 5uwb;
  q[39] = 2uwb;
  foo(24320389uwb, 125897uwb, &p, q, 42.0f);
  if (p != 125897uwb)
    __builtin_abort();
  q[38] -= sizeof(p) - 1;
  for (int i = 0; i < 41; ++i)
    if (q[i] != qe[i])
      __builtin_abort();
  return 0;
}



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
// DEFAULT-NEXT:     global %9 qe: array<u27b, 41> [storage=static] [align=16] = aggregate<array<u27b, 41>, zero_fill=false>(index0 = widen<u27b, reason=assign>(const<u25b>(24320389)), index1 = const<u27b>(124983685), index2 = const<u27b>(134216069), index3 = widen<u27b, reason=assign>(const<u6b>(42)), index4 = widen<u27b, reason=assign>(const<u25b>(24320407)), index5 = widen<u27b, reason=assign>(const<u25b>(24446286)), index6 = widen<u27b, reason=assign>(const<u25b>(24194492)), index7 = const<u27b>(89202797), index8 = widen<u27b, reason=assign>(const<u8b>(193)), index9 = widen<u27b, reason=assign>(const<u15b>(22268)), index10 = widen<u27b, reason=assign>(const<u21b>(2014352)), index11 = widen<u27b, reason=assign>(const<u20b>(1030781)), index12 = widen<u27b, reason=assign>(const<u4b>(8)), index13 = widen<u27b, reason=assign>(const<u4b>(10)), index14 = widen<u27b, reason=assign>(const<u4b>(10)), index15 = widen<u27b, reason=assign>(const<u4b>(12)), index16 = widen<u27b, reason=assign>(const<u1b>(0)), index17 = widen<u27b, reason=assign>(const<u1b>(1)), index18 = widen<u27b, reason=assign>(const<u1b>(1)), index19 = widen<u27b, reason=assign>(const<u1b>(0)), index20 = widen<u27b, reason=assign>(const<u1b>(1)), index21 = widen<u27b, reason=assign>(const<u1b>(0)), index22 = widen<u27b, reason=assign>(const<u1b>(1)), index23 = widen<u27b, reason=assign>(const<u1b>(1)), index24 = widen<u27b, reason=assign>(const<u1b>(0)), index25 = widen<u27b, reason=assign>(const<u17b>(67969)), index26 = widen<u27b, reason=assign>(const<u25b>(24378317)), index27 = widen<u27b, reason=assign>(const<u25b>(24310348)), index28 = widen<u27b, reason=assign>(const<u24b>(9234042)), index29 = const<u27b>(109897346), index30 = widen<u27b, reason=assign>(const<u17b>(125897)), index31 = widen<u27b, reason=assign>(const<u1b>(0)), index32 = widen<u27b, reason=assign>(const<u6b>(52)), index33 = widen<u27b, reason=assign>(const<u6b>(40)), index34 = widen<u27b, reason=assign>(const<u10b>(771)), index35 = widen<u27b, reason=assign>(const<u3b>(5)), index36 = widen<u27b, reason=assign>(const<u17b>(125917)), index37 = widen<u27b, reason=assign>(const<u25b>(24320384)), index38 = widen<u27b, reason=assign>(const<u1b>(1)), index39 = widen<u27b, reason=assign>(const<u25b>(24320389)), index40 = widen<u27b, reason=assign>(const<u25b>(24196214))) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @foo(%1 a: u25b, %2 b: u27b, %3 p: ptr<u25b>, %4 q: ptr<u27b>, %5 c: f32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<u25b>(deref(ptr_offset<ptr<u25b>, subtract=false, element=u25b, overflow=ub>(read<ptr<u25b>>(%3), const<i32>(0))), truncate<u25b, reason=assign, fits=unknown>(read<u27b>(%2)));
// DEFAULT-NEXT:         write<u27b>(deref(ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(read<ptr<u27b>>(%4), const<i32>(0))), widen<u27b, reason=assign>(read<u25b>(%1)));
// DEFAULT-NEXT:         write<u27b>(deref(ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(read<ptr<u27b>>(%4), const<i32>(1))), reinterpret<u27b, reason=assign, fits=unknown>(widen<i27b, reason=assign>(reinterpret<i25b, reason=explicit, fits=unknown>(read<u25b>(%1)))));
// DEFAULT-NEXT:         write<u27b>(deref(ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(read<ptr<u27b>>(%4), const<i32>(2))), reinterpret<u27b, reason=assign, fits=unknown>(widen<i27b, reason=assign>(reinterpret<i12b, reason=explicit, fits=unknown>(truncate<u12b, reason=explicit, fits=unknown>(read<u25b>(%1))))));
// DEFAULT-NEXT:         write<u27b>(deref(ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(read<ptr<u27b>>(%4), const<i32>(3))), float_to_int<u27b, reason=assign, out_of_range=ub, exceptions=ignore>(read<f32>(%5)));
// DEFAULT-NEXT:         let %13: ptr<u27b> [synthetic] = ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(read<ptr<u27b>>(%4), const<i32>(4));
// DEFAULT-NEXT:         let %14: u27b [synthetic] = read<u27b>(deref(read<ptr<u27b>>(%13)));
// DEFAULT-NEXT:         let %15: u27b [synthetic] = add<u27b, overflow=wrap>(read<u27b>(%14), widen<u27b, reason=usual_arith>(read<u25b>(%1)));
// DEFAULT-NEXT:         write<u27b>(deref(read<ptr<u27b>>(%13)), read<u27b>(%15));
// DEFAULT-NEXT:         write<u27b>(deref(ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(read<ptr<u27b>>(%4), const<i32>(5))), add<u27b, overflow=wrap>(widen<u27b, reason=usual_arith>(read<u25b>(%1)), read<u27b>(%2)));
// DEFAULT-NEXT:         write<u27b>(deref(ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(read<ptr<u27b>>(%4), const<i32>(6))), sub<u27b, overflow=wrap>(widen<u27b, reason=usual_arith>(read<u25b>(%1)), read<u27b>(%2)));
// DEFAULT-NEXT:         write<u27b>(deref(ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(read<ptr<u27b>>(%4), const<i32>(7))), mul<u27b, overflow=wrap>(widen<u27b, reason=usual_arith>(read<u25b>(%1)), read<u27b>(%2)));
// DEFAULT-NEXT:         write<u27b>(deref(ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(read<ptr<u27b>>(%4), const<i32>(8))), div<u27b, by_zero=ub>(widen<u27b, reason=usual_arith>(read<u25b>(%1)), read<u27b>(%2)));
// DEFAULT-NEXT:         write<u27b>(deref(ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(read<ptr<u27b>>(%4), const<i32>(9))), rem<u27b, by_zero=ub>(widen<u27b, reason=usual_arith>(read<u25b>(%1)), read<u27b>(%2)));
// DEFAULT-NEXT:         write<u27b>(deref(ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(read<ptr<u27b>>(%4), const<i32>(10))), shl<u27b, overflow=wrap, amount_out_of_range=ub>(read<u27b>(%2), sub<u25b, overflow=wrap>(const<u25b>(24320393), read<u25b>(%1))));
// DEFAULT-NEXT:         write<u27b>(deref(ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(read<ptr<u27b>>(%4), const<i32>(11))), shr<u27b, amount_out_of_range=ub, fill=zero_extend>(mul<u27b, overflow=wrap>(read<u27b>(%2), widen<u27b, reason=usual_arith>(const<u8b>(131))), sub<u25b, overflow=wrap>(const<u25b>(24320393), read<u25b>(%1))));
// DEFAULT-NEXT:         let %16: ptr<u27b> [synthetic] = ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(read<ptr<u27b>>(%4), const<i32>(12));
// DEFAULT-NEXT:         let %17: u27b [synthetic] = read<u27b>(deref(read<ptr<u27b>>(%16)));
// DEFAULT-NEXT:         let %18: u27b [synthetic] = reinterpret<u27b, reason=assign, fits=unknown>(truncate<i27b, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=usual_arith, fits=unknown>(widen<u32, reason=usual_arith>(read<u27b>(%17))), const<i32>(1))));
// DEFAULT-NEXT:         write<u27b>(deref(read<ptr<u27b>>(%16)), read<u27b>(%18));
// DEFAULT-NEXT:         let %19: ptr<u27b> [synthetic] = ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(read<ptr<u27b>>(%4), const<i32>(13));
// DEFAULT-NEXT:         let %20: u27b [synthetic] = read<u27b>(deref(read<ptr<u27b>>(%19)));
// DEFAULT-NEXT:         let %21: u27b [synthetic] = reinterpret<u27b, reason=assign, fits=unknown>(truncate<i27b, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=usual_arith, fits=unknown>(widen<u32, reason=usual_arith>(read<u27b>(%20))), const<i32>(1))));
// DEFAULT-NEXT:         write<u27b>(deref(read<ptr<u27b>>(%19)), read<u27b>(%21));
// DEFAULT-NEXT:         let %22: ptr<u27b> [synthetic] = ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(read<ptr<u27b>>(%4), const<i32>(14));
// DEFAULT-NEXT:         let %23: u27b [synthetic] = read<u27b>(deref(read<ptr<u27b>>(%22)));
// DEFAULT-NEXT:         let %24: u27b [synthetic] = reinterpret<u27b, reason=assign, fits=unknown>(truncate<i27b, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=usual_arith, fits=unknown>(widen<u32, reason=usual_arith>(read<u27b>(%23))), const<i32>(1))));
// DEFAULT-NEXT:         write<u27b>(deref(read<ptr<u27b>>(%22)), read<u27b>(%24));
// DEFAULT-NEXT:         let %25: ptr<u27b> [synthetic] = ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(read<ptr<u27b>>(%4), const<i32>(15));
// DEFAULT-NEXT:         let %26: u27b [synthetic] = read<u27b>(deref(read<ptr<u27b>>(%25)));
// DEFAULT-NEXT:         let %27: u27b [synthetic] = reinterpret<u27b, reason=assign, fits=unknown>(truncate<i27b, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=usual_arith, fits=unknown>(widen<u32, reason=usual_arith>(read<u27b>(%26))), const<i32>(1))));
// DEFAULT-NEXT:         write<u27b>(deref(read<ptr<u27b>>(%25)), read<u27b>(%27));
// DEFAULT-NEXT:         write<u27b>(deref(ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(read<ptr<u27b>>(%4), const<i32>(16))), from_bool<u27b, reason=assign>(eq<u27b>(widen<u27b, reason=usual_arith>(read<u25b>(%1)), read<u27b>(%2))));
// DEFAULT-NEXT:         write<u27b>(deref(ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(read<ptr<u27b>>(%4), const<i32>(17))), from_bool<u27b, reason=assign>(ne<u27b>(widen<u27b, reason=usual_arith>(read<u25b>(%1)), read<u27b>(%2))));
// DEFAULT-NEXT:         write<u27b>(deref(ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(read<ptr<u27b>>(%4), const<i32>(18))), from_bool<u27b, reason=assign>(gt<u27b>(widen<u27b, reason=usual_arith>(read<u25b>(%1)), read<u27b>(%2))));
// DEFAULT-NEXT:         write<u27b>(deref(ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(read<ptr<u27b>>(%4), const<i32>(19))), from_bool<u27b, reason=assign>(lt<u27b>(widen<u27b, reason=usual_arith>(read<u25b>(%1)), read<u27b>(%2))));
// DEFAULT-NEXT:         write<u27b>(deref(ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(read<ptr<u27b>>(%4), const<i32>(20))), from_bool<u27b, reason=assign>(ge<u27b>(widen<u27b, reason=usual_arith>(read<u25b>(%1)), read<u27b>(%2))));
// DEFAULT-NEXT:         write<u27b>(deref(ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(read<ptr<u27b>>(%4), const<i32>(21))), from_bool<u27b, reason=assign>(le<u27b>(widen<u27b, reason=usual_arith>(read<u25b>(%1)), read<u27b>(%2))));
// DEFAULT-NEXT:         write<u27b>(deref(ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(read<ptr<u27b>>(%4), const<i32>(22))), from_bool<u27b, reason=assign>(logical_and<bool>(ne<u25b>(read<u25b>(%1), const<u25b>(0)), ne<u27b>(read<u27b>(%2), const<u27b>(0)))));
// DEFAULT-NEXT:         write<u27b>(deref(ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(read<ptr<u27b>>(%4), const<i32>(23))), from_bool<u27b, reason=assign>(logical_or<bool>(ne<u25b>(read<u25b>(%1), const<u25b>(0)), ne<u27b>(read<u27b>(%2), const<u27b>(0)))));
// DEFAULT-NEXT:         write<u27b>(deref(ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(read<ptr<u27b>>(%4), const<i32>(24))), from_bool<u27b, reason=assign>(not<bool>(ne<u25b>(read<u25b>(%1), const<u25b>(0)))));
// DEFAULT-NEXT:         write<u27b>(deref(ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(read<ptr<u27b>>(%4), const<i32>(25))), and<u27b>(widen<u27b, reason=usual_arith>(read<u25b>(%1)), read<u27b>(%2)));
// DEFAULT-NEXT:         write<u27b>(deref(ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(read<ptr<u27b>>(%4), const<i32>(26))), or<u27b>(widen<u27b, reason=usual_arith>(read<u25b>(%1)), read<u27b>(%2)));
// DEFAULT-NEXT:         write<u27b>(deref(ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(read<ptr<u27b>>(%4), const<i32>(27))), xor<u27b>(widen<u27b, reason=usual_arith>(read<u25b>(%1)), read<u27b>(%2)));
// DEFAULT-NEXT:         write<u27b>(deref(ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(read<ptr<u27b>>(%4), const<i32>(28))), widen<u27b, reason=assign>(not<u25b>(read<u25b>(%1))));
// DEFAULT-NEXT:         let %28: ptr<u27b> [synthetic] = ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(read<ptr<u27b>>(%4), const<i32>(29));
// DEFAULT-NEXT:         let %29: u27b [synthetic] = read<u27b>(deref(read<ptr<u27b>>(%28)));
// DEFAULT-NEXT:         let %30: u27b [synthetic] = sub<u27b, overflow=wrap>(read<u27b>(%29), widen<u27b, reason=usual_arith>(read<u25b>(%1)));
// DEFAULT-NEXT:         write<u27b>(deref(read<ptr<u27b>>(%28)), read<u27b>(%30));
// DEFAULT-NEXT:         let %31: ptr<u27b> [synthetic] = ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(read<ptr<u27b>>(%4), const<i32>(30));
// DEFAULT-NEXT:         let %32: u27b [synthetic] = read<u27b>(deref(read<ptr<u27b>>(%31)));
// DEFAULT-NEXT:         let %33: u27b [synthetic] = mul<u27b, overflow=wrap>(read<u27b>(%32), read<u27b>(%2));
// DEFAULT-NEXT:         write<u27b>(deref(read<ptr<u27b>>(%31)), read<u27b>(%33));
// DEFAULT-NEXT:         let %34: ptr<u27b> [synthetic] = ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(read<ptr<u27b>>(%4), const<i32>(31));
// DEFAULT-NEXT:         let %35: u27b [synthetic] = read<u27b>(deref(read<ptr<u27b>>(%34)));
// DEFAULT-NEXT:         let %36: u27b [synthetic] = div<u27b, by_zero=ub>(read<u27b>(%35), read<u27b>(%2));
// DEFAULT-NEXT:         write<u27b>(deref(read<ptr<u27b>>(%34)), read<u27b>(%36));
// DEFAULT-NEXT:         let %37: ptr<u27b> [synthetic] = ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(read<ptr<u27b>>(%4), const<i32>(32));
// DEFAULT-NEXT:         let %38: u27b [synthetic] = read<u27b>(deref(read<ptr<u27b>>(%37)));
// DEFAULT-NEXT:         let %39: u27b [synthetic] = rem<u27b, by_zero=ub>(read<u27b>(%38), widen<u27b, reason=usual_arith>(read<u25b>(%1)));
// DEFAULT-NEXT:         write<u27b>(deref(read<ptr<u27b>>(%37)), read<u27b>(%39));
// DEFAULT-NEXT:         let %40: ptr<u27b> [synthetic] = ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(read<ptr<u27b>>(%4), const<i32>(33));
// DEFAULT-NEXT:         let %41: u27b [synthetic] = read<u27b>(deref(read<ptr<u27b>>(%40)));
// DEFAULT-NEXT:         let %42: u27b [synthetic] = shl<u27b, overflow=wrap, amount_out_of_range=ub>(read<u27b>(%41), sub<u27b, overflow=wrap>(widen<u27b, reason=usual_arith>(const<u17b>(125900)), read<u27b>(%2)));
// DEFAULT-NEXT:         write<u27b>(deref(read<ptr<u27b>>(%40)), read<u27b>(%42));
// DEFAULT-NEXT:         let %43: ptr<u27b> [synthetic] = ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(read<ptr<u27b>>(%4), const<i32>(34));
// DEFAULT-NEXT:         let %44: u27b [synthetic] = read<u27b>(deref(read<ptr<u27b>>(%43)));
// DEFAULT-NEXT:         let %45: u27b [synthetic] = shr<u27b, amount_out_of_range=ub, fill=zero_extend>(read<u27b>(%44), sub<u27b, overflow=wrap>(widen<u27b, reason=usual_arith>(const<u17b>(125901)), read<u27b>(%2)));
// DEFAULT-NEXT:         write<u27b>(deref(read<ptr<u27b>>(%43)), read<u27b>(%45));
// DEFAULT-NEXT:         let %46: ptr<u27b> [synthetic] = ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(read<ptr<u27b>>(%4), const<i32>(35));
// DEFAULT-NEXT:         let %47: u27b [synthetic] = read<u27b>(deref(read<ptr<u27b>>(%46)));
// DEFAULT-NEXT:         let %48: u27b [synthetic] = and<u27b>(read<u27b>(%47), widen<u27b, reason=usual_arith>(read<u25b>(%1)));
// DEFAULT-NEXT:         write<u27b>(deref(read<ptr<u27b>>(%46)), read<u27b>(%48));
// DEFAULT-NEXT:         let %49: ptr<u27b> [synthetic] = ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(read<ptr<u27b>>(%4), const<i32>(36));
// DEFAULT-NEXT:         let %50: u27b [synthetic] = read<u27b>(deref(read<ptr<u27b>>(%49)));
// DEFAULT-NEXT:         let %51: u27b [synthetic] = or<u27b>(read<u27b>(%50), read<u27b>(%2));
// DEFAULT-NEXT:         write<u27b>(deref(read<ptr<u27b>>(%49)), read<u27b>(%51));
// DEFAULT-NEXT:         let %52: ptr<u27b> [synthetic] = ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(read<ptr<u27b>>(%4), const<i32>(37));
// DEFAULT-NEXT:         let %53: u27b [synthetic] = read<u27b>(deref(read<ptr<u27b>>(%52)));
// DEFAULT-NEXT:         let %54: u27b [synthetic] = xor<u27b>(read<u27b>(%53), widen<u27b, reason=usual_arith>(read<u25b>(%1)));
// DEFAULT-NEXT:         write<u27b>(deref(read<ptr<u27b>>(%52)), read<u27b>(%54));
// DEFAULT-NEXT:         write<u27b>(deref(ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(read<ptr<u27b>>(%4), const<i32>(38))), truncate<u27b, reason=assign, fits=always>(const<u64>(4)));
// DEFAULT-NEXT:         write<u27b>(deref(ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(read<ptr<u27b>>(%4), const<i32>(39))), conditional<u27b>(ne<u27b>(read<u27b>(deref(ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(read<ptr<u27b>>(%4), const<i32>(39)))), const<u27b>(0)), widen<u27b, reason=usual_arith>(read<u25b>(%1)), read<u27b>(%2)));
// DEFAULT-NEXT:         write<u27b>(deref(ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(read<ptr<u27b>>(%4), const<i32>(40))), widen<u27b, reason=assign>(const<u25b>(24196214)));
// DEFAULT-NEXT:         switch %11 read<u25b>(%1)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %11 const<u25b>(12345641):
// DEFAULT-NEXT:                     if ne<u27b>(read<u27b>(%2), reinterpret<u27b, reason=usual_arith, fits=unknown>(widen<i27b, reason=usual_arith>(const<i12b>(1244))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 break %11;
// DEFAULT-NEXT:                 case %11 const<u25b>(11821400):
// DEFAULT-NEXT:                     if ne<u27b>(read<u27b>(%2), widen<u27b, reason=usual_arith>(const<u18b>(133445)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 break %11;
// DEFAULT-NEXT:                 case %11 const<u25b>(12145):
// DEFAULT-NEXT:                     if ne<u27b>(read<u27b>(%2), widen<u27b, reason=usual_arith>(const<u11b>(1212)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 break %11;
// DEFAULT-NEXT:                 case %11 const<u25b>(24320389):
// DEFAULT-NEXT:                     if ne<u27b>(read<u27b>(%2), widen<u27b, reason=usual_arith>(const<u17b>(125897)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 break %11;
// DEFAULT-NEXT:                 case %11 const<u25b>(7128412):
// DEFAULT-NEXT:                     if ne<u27b>(read<u27b>(%2), widen<u27b, reason=usual_arith>(const<u8b>(150)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 break %11;
// DEFAULT-NEXT:                 default %11:
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %7 p: u25b [storage=automatic];
// DEFAULT-NEXT:         let %8 q: array<u27b, 41> [storage=automatic] [align=16];
// DEFAULT-NEXT:         write<u27b>(deref(ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(array_decay<ptr<u27b>, length=Some(41)>(%8), const<i32>(4))), widen<u27b, reason=assign>(const<u5b>(18)));
// DEFAULT-NEXT:         write<u27b>(deref(ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(array_decay<ptr<u27b>, length=Some(41)>(%8), const<i32>(12))), widen<u27b, reason=assign>(const<u3b>(7)));
// DEFAULT-NEXT:         write<u27b>(deref(ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(array_decay<ptr<u27b>, length=Some(41)>(%8), const<i32>(13))), widen<u27b, reason=assign>(const<u4b>(9)));
// DEFAULT-NEXT:         write<u27b>(deref(ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(array_decay<ptr<u27b>, length=Some(41)>(%8), const<i32>(14))), widen<u27b, reason=assign>(const<u4b>(11)));
// DEFAULT-NEXT:         write<u27b>(deref(ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(array_decay<ptr<u27b>, length=Some(41)>(%8), const<i32>(15))), widen<u27b, reason=assign>(const<u4b>(13)));
// DEFAULT-NEXT:         write<u27b>(deref(ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(array_decay<ptr<u27b>, length=Some(41)>(%8), const<i32>(29))), widen<u27b, reason=assign>(const<u3b>(7)));
// DEFAULT-NEXT:         write<u27b>(deref(ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(array_decay<ptr<u27b>, length=Some(41)>(%8), const<i32>(30))), widen<u27b, reason=assign>(neg<u1b, overflow=wrap>(const<u1b>(1))));
// DEFAULT-NEXT:         write<u27b>(deref(ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(array_decay<ptr<u27b>, length=Some(41)>(%8), const<i32>(31))), widen<u27b, reason=assign>(neg<u4b, overflow=wrap>(const<u4b>(13))));
// DEFAULT-NEXT:         write<u27b>(deref(ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(array_decay<ptr<u27b>, length=Some(41)>(%8), const<i32>(32))), widen<u27b, reason=assign>(const<u6b>(52)));
// DEFAULT-NEXT:         write<u27b>(deref(ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(array_decay<ptr<u27b>, length=Some(41)>(%8), const<i32>(33))), widen<u27b, reason=assign>(const<u3b>(5)));
// DEFAULT-NEXT:         write<u27b>(deref(ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(array_decay<ptr<u27b>, length=Some(41)>(%8), const<i32>(34))), widen<u27b, reason=assign>(const<u14b>(12345)));
// DEFAULT-NEXT:         write<u27b>(deref(ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(array_decay<ptr<u27b>, length=Some(41)>(%8), const<i32>(35))), widen<u27b, reason=assign>(const<u4b>(15)));
// DEFAULT-NEXT:         write<u27b>(deref(ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(array_decay<ptr<u27b>, length=Some(41)>(%8), const<i32>(36))), widen<u27b, reason=assign>(const<u5b>(28)));
// DEFAULT-NEXT:         write<u27b>(deref(ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(array_decay<ptr<u27b>, length=Some(41)>(%8), const<i32>(37))), widen<u27b, reason=assign>(const<u3b>(5)));
// DEFAULT-NEXT:         write<u27b>(deref(ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(array_decay<ptr<u27b>, length=Some(41)>(%8), const<i32>(39))), widen<u27b, reason=assign>(const<u2b>(2)));
// DEFAULT-NEXT:         call<void, signature=fn(u25b, u27b, ptr<u25b>, ptr<u27b>, f32) -> void>(%0, const<u25b>(24320389), widen<u27b, reason=arg>(const<u17b>(125897)), addr_of<ptr<u25b>>(%7), array_decay<ptr<u27b>, length=Some(41)>(%8), const<f32>(42.0));
// DEFAULT-NEXT:         if ne<u25b>(read<u25b>(%7), widen<u25b, reason=usual_arith>(const<u17b>(125897)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         let %55: ptr<u27b> [synthetic] = ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(array_decay<ptr<u27b>, length=Some(41)>(%8), const<i32>(38));
// DEFAULT-NEXT:         let %56: u27b [synthetic] = read<u27b>(deref(read<ptr<u27b>>(%55)));
// DEFAULT-NEXT:         let %57: u27b [synthetic] = truncate<u27b, reason=assign, fits=unknown>(sub<u64, overflow=wrap>(widen<u64, reason=usual_arith>(read<u27b>(%56)), sub<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))));
// DEFAULT-NEXT:         write<u27b>(deref(read<ptr<u27b>>(%55)), read<u27b>(%57));
// DEFAULT-NEXT:         for %12
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %10 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%10), const<i32>(41))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %58: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                 let %59: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%58), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%10, read<i32>(%59));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u27b>(read<u27b>(deref(ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(array_decay<ptr<u27b>, length=Some(41)>(%8), read<i32>(%10)))), read<u27b>(deref(ptr_offset<ptr<u27b>, subtract=false, element=u27b, overflow=ub>(array_decay<ptr<u27b>, length=Some(41)>(%9), read<i32>(%10)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
