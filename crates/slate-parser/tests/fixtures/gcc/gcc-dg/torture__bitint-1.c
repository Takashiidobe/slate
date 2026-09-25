/* PR c/102989 */
/* { dg-do run { target bitint } } */
/* { dg-options "-std=c23 -pedantic-errors" } */
/* { dg-skip-if "" { ! run_expensive_tests }  { "*" } { "-O0" "-O2" } } */
/* { dg-skip-if "" { ! run_expensive_tests } { "-flto" } { "" } } */

__attribute__((noipa)) void foo(_BitInt(6) a, _BitInt(27) b, _BitInt(6) * p,
                                _BitInt(27) * q, float c) {
  p[0]   = b;
  q[0]   = a;
  q[1]   = (unsigned _BitInt(6))a;
  q[2]   = (unsigned _BitInt(9))a;
  q[3]   = c;
  q[4]  += a;
  q[5]   = a + b;
  q[6]   = a - b;
  q[7]   = a * b;
  q[8]   = a / b;
  q[9]   = a % b;
  q[10]  = b << (-20wb - a);
  q[11]  = (b * 131wb) >> (-20wb - a);
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
  q[33] <<= b;
  q[34] >>= b;
  q[35]  &= a;
  q[36]  |= b;
  q[37]  ^= a;
  q[38]   = sizeof(a);
  q[39]   = q[39] ? a : b;
  q[40]   = 12345wb;
  switch (a) {
  case 31wb:
    if (b != 8wb)
      __builtin_abort();
    break;
  case -18wb:
    if (b != 9wb)
      __builtin_abort();
    break;
  case 26wb:
    if (b != 12wb)
      __builtin_abort();
    break;
  case -25wb:
    if (b != 6wb)
      __builtin_abort();
    break;
  case -19wb:
    if (b != 15wb)
      __builtin_abort();
    break;
  default:
    __builtin_abort();
  }
}

int
main() {
  _BitInt(6) p;
  _BitInt(27) q[41];
  static _BitInt(27) qe[41] = {
      -25wb, 39wb,  487wb, 17wb, -7wb,  -19wb, -31wb, -150wb, -4wb, -1,   192wb,
      24wb,  8wb,   10wb,  10wb, 12wb,  0,     1wb,   0wb,    1,    0,    1,
      1,     1wbu,  0,     6wb,  -25wb, -31wb, 24wb,  32wb,   -6wb, -2wb, 2uwb,
      320wb, 192wb, 7wb,   30wb, -30wb, 1,     -25wb, 12345wb};
  q[4]  = 18wb;
  q[12] = 7wb;
  q[13] = 9wb;
  q[14] = 11wb;
  q[15] = 13wb;
  q[29] = 7wb;
  q[30] = -1wb;
  q[31] = -13wb;
  q[32] = 52wb;
  q[33] = 5wb;
  q[34] = 12345wb;
  q[35] = 15wb;
  q[36] = 28wb;
  q[37] = 5wb;
  q[39] = 2wb;
  foo(-25wb, 6wb, &p, q, 17.0f);
  if (p != 6wb)
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
// DEFAULT-NEXT:     global %9 qe: array<i27b, 41> [storage=static] = aggregate<array<i27b, 41>, zero_fill=false>(index0 = widen<i27b, reason=assign>(neg<i6b, overflow=ub>(const<i6b>(25))), index1 = widen<i27b, reason=assign>(const<i7b>(39)), index2 = widen<i27b, reason=assign>(const<i10b>(487)), index3 = widen<i27b, reason=assign>(const<i6b>(17)), index4 = widen<i27b, reason=assign>(neg<i4b, overflow=ub>(const<i4b>(7))), index5 = widen<i27b, reason=assign>(neg<i6b, overflow=ub>(const<i6b>(19))), index6 = widen<i27b, reason=assign>(neg<i6b, overflow=ub>(const<i6b>(31))), index7 = widen<i27b, reason=assign>(neg<i9b, overflow=ub>(const<i9b>(150))), index8 = widen<i27b, reason=assign>(neg<i4b, overflow=ub>(const<i4b>(4))), index9 = truncate<i27b, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))), index10 = widen<i27b, reason=assign>(const<i9b>(192)), index11 = widen<i27b, reason=assign>(const<i6b>(24)), index12 = widen<i27b, reason=assign>(const<i5b>(8)), index13 = widen<i27b, reason=assign>(const<i5b>(10)), index14 = widen<i27b, reason=assign>(const<i5b>(10)), index15 = widen<i27b, reason=assign>(const<i5b>(12)), index16 = truncate<i27b, reason=assign, fits=always>(const<i32>(0)), index17 = widen<i27b, reason=assign>(const<i2b>(1)), index18 = widen<i27b, reason=assign>(const<i2b>(0)), index19 = truncate<i27b, reason=assign, fits=always>(const<i32>(1)), index20 = truncate<i27b, reason=assign, fits=always>(const<i32>(0)), index21 = truncate<i27b, reason=assign, fits=always>(const<i32>(1)), index22 = truncate<i27b, reason=assign, fits=always>(const<i32>(1)), index23 = reinterpret<i27b, reason=assign, fits=unknown>(widen<u27b, reason=assign>(const<u1b>(1))), index24 = truncate<i27b, reason=assign, fits=always>(const<i32>(0)), index25 = widen<i27b, reason=assign>(const<i4b>(6)), index26 = widen<i27b, reason=assign>(neg<i6b, overflow=ub>(const<i6b>(25))), index27 = widen<i27b, reason=assign>(neg<i6b, overflow=ub>(const<i6b>(31))), index28 = widen<i27b, reason=assign>(const<i6b>(24)), index29 = widen<i27b, reason=assign>(const<i7b>(32)), index30 = widen<i27b, reason=assign>(neg<i4b, overflow=ub>(const<i4b>(6))), index31 = widen<i27b, reason=assign>(neg<i3b, overflow=ub>(const<i3b>(2))), index32 = reinterpret<i27b, reason=assign, fits=unknown>(widen<u27b, reason=assign>(const<u2b>(2))), index33 = widen<i27b, reason=assign>(const<i10b>(320)), index34 = widen<i27b, reason=assign>(const<i9b>(192)), index35 = widen<i27b, reason=assign>(const<i4b>(7)), index36 = widen<i27b, reason=assign>(const<i6b>(30)), index37 = widen<i27b, reason=assign>(neg<i6b, overflow=ub>(const<i6b>(30))), index38 = truncate<i27b, reason=assign, fits=always>(const<i32>(1)), index39 = widen<i27b, reason=assign>(neg<i6b, overflow=ub>(const<i6b>(25))), index40 = widen<i27b, reason=assign>(const<i15b>(12345))) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @foo(%1 a: i6b, %2 b: i27b, %3 p: ptr<i6b>, %4 q: ptr<i27b>, %5 c: f32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i6b>(deref(ptr_offset<ptr<i6b>, subtract=false, element=i6b, overflow=ub>(read<ptr<i6b>>(%3), const<i32>(0))), truncate<i6b, reason=assign, fits=unknown>(read<i27b>(%2)));
// DEFAULT-NEXT:         write<i27b>(deref(ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(read<ptr<i27b>>(%4), const<i32>(0))), widen<i27b, reason=assign>(read<i6b>(%1)));
// DEFAULT-NEXT:         write<i27b>(deref(ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(read<ptr<i27b>>(%4), const<i32>(1))), reinterpret<i27b, reason=assign, fits=unknown>(widen<u27b, reason=assign>(reinterpret<u6b, reason=explicit, fits=unknown>(read<i6b>(%1)))));
// DEFAULT-NEXT:         write<i27b>(deref(ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(read<ptr<i27b>>(%4), const<i32>(2))), reinterpret<i27b, reason=assign, fits=unknown>(widen<u27b, reason=assign>(reinterpret<u9b, reason=explicit, fits=unknown>(widen<i9b, reason=explicit>(read<i6b>(%1))))));
// DEFAULT-NEXT:         write<i27b>(deref(ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(read<ptr<i27b>>(%4), const<i32>(3))), float_to_int<i27b, reason=assign, out_of_range=ub, exceptions=ignore>(read<f32>(%5)));
// DEFAULT-NEXT:         let %13: ptr<i27b> [synthetic] = ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(read<ptr<i27b>>(%4), const<i32>(4));
// DEFAULT-NEXT:         let %14: i27b [synthetic] = read<i27b>(deref(read<ptr<i27b>>(%13)));
// DEFAULT-NEXT:         let %15: i27b [synthetic] = add<i27b, overflow=ub>(read<i27b>(%14), widen<i27b, reason=usual_arith>(read<i6b>(%1)));
// DEFAULT-NEXT:         write<i27b>(deref(read<ptr<i27b>>(%13)), read<i27b>(%15));
// DEFAULT-NEXT:         write<i27b>(deref(ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(read<ptr<i27b>>(%4), const<i32>(5))), add<i27b, overflow=ub>(widen<i27b, reason=usual_arith>(read<i6b>(%1)), read<i27b>(%2)));
// DEFAULT-NEXT:         write<i27b>(deref(ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(read<ptr<i27b>>(%4), const<i32>(6))), sub<i27b, overflow=ub>(widen<i27b, reason=usual_arith>(read<i6b>(%1)), read<i27b>(%2)));
// DEFAULT-NEXT:         write<i27b>(deref(ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(read<ptr<i27b>>(%4), const<i32>(7))), mul<i27b, overflow=ub>(widen<i27b, reason=usual_arith>(read<i6b>(%1)), read<i27b>(%2)));
// DEFAULT-NEXT:         write<i27b>(deref(ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(read<ptr<i27b>>(%4), const<i32>(8))), div<i27b, by_zero=ub, min_by_neg_one=ub>(widen<i27b, reason=usual_arith>(read<i6b>(%1)), read<i27b>(%2)));
// DEFAULT-NEXT:         write<i27b>(deref(ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(read<ptr<i27b>>(%4), const<i32>(9))), rem<i27b, by_zero=ub, min_by_neg_one=ub>(widen<i27b, reason=usual_arith>(read<i6b>(%1)), read<i27b>(%2)));
// DEFAULT-NEXT:         write<i27b>(deref(ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(read<ptr<i27b>>(%4), const<i32>(10))), shl<i27b, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i27b>(%2), sub<i6b, overflow=ub>(neg<i6b, overflow=ub>(const<i6b>(20)), read<i6b>(%1))));
// DEFAULT-NEXT:         write<i27b>(deref(ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(read<ptr<i27b>>(%4), const<i32>(11))), shr<i27b, amount_out_of_range=ub, fill=sign_extend>(mul<i27b, overflow=ub>(read<i27b>(%2), widen<i27b, reason=usual_arith>(const<i9b>(131))), sub<i6b, overflow=ub>(neg<i6b, overflow=ub>(const<i6b>(20)), read<i6b>(%1))));
// DEFAULT-NEXT:         let %16: ptr<i27b> [synthetic] = ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(read<ptr<i27b>>(%4), const<i32>(12));
// DEFAULT-NEXT:         let %17: i27b [synthetic] = read<i27b>(deref(read<ptr<i27b>>(%16)));
// DEFAULT-NEXT:         let %18: i27b [synthetic] = truncate<i27b, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=usual_arith>(read<i27b>(%17)), const<i32>(1)));
// DEFAULT-NEXT:         write<i27b>(deref(read<ptr<i27b>>(%16)), read<i27b>(%18));
// DEFAULT-NEXT:         let %19: ptr<i27b> [synthetic] = ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(read<ptr<i27b>>(%4), const<i32>(13));
// DEFAULT-NEXT:         let %20: i27b [synthetic] = read<i27b>(deref(read<ptr<i27b>>(%19)));
// DEFAULT-NEXT:         let %21: i27b [synthetic] = truncate<i27b, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=usual_arith>(read<i27b>(%20)), const<i32>(1)));
// DEFAULT-NEXT:         write<i27b>(deref(read<ptr<i27b>>(%19)), read<i27b>(%21));
// DEFAULT-NEXT:         let %22: ptr<i27b> [synthetic] = ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(read<ptr<i27b>>(%4), const<i32>(14));
// DEFAULT-NEXT:         let %23: i27b [synthetic] = read<i27b>(deref(read<ptr<i27b>>(%22)));
// DEFAULT-NEXT:         let %24: i27b [synthetic] = truncate<i27b, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=usual_arith>(read<i27b>(%23)), const<i32>(1)));
// DEFAULT-NEXT:         write<i27b>(deref(read<ptr<i27b>>(%22)), read<i27b>(%24));
// DEFAULT-NEXT:         let %25: ptr<i27b> [synthetic] = ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(read<ptr<i27b>>(%4), const<i32>(15));
// DEFAULT-NEXT:         let %26: i27b [synthetic] = read<i27b>(deref(read<ptr<i27b>>(%25)));
// DEFAULT-NEXT:         let %27: i27b [synthetic] = truncate<i27b, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=usual_arith>(read<i27b>(%26)), const<i32>(1)));
// DEFAULT-NEXT:         write<i27b>(deref(read<ptr<i27b>>(%25)), read<i27b>(%27));
// DEFAULT-NEXT:         write<i27b>(deref(ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(read<ptr<i27b>>(%4), const<i32>(16))), from_bool<i27b, reason=assign>(eq<i27b>(widen<i27b, reason=usual_arith>(read<i6b>(%1)), read<i27b>(%2))));
// DEFAULT-NEXT:         write<i27b>(deref(ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(read<ptr<i27b>>(%4), const<i32>(17))), from_bool<i27b, reason=assign>(ne<i27b>(widen<i27b, reason=usual_arith>(read<i6b>(%1)), read<i27b>(%2))));
// DEFAULT-NEXT:         write<i27b>(deref(ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(read<ptr<i27b>>(%4), const<i32>(18))), from_bool<i27b, reason=assign>(gt<i27b>(widen<i27b, reason=usual_arith>(read<i6b>(%1)), read<i27b>(%2))));
// DEFAULT-NEXT:         write<i27b>(deref(ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(read<ptr<i27b>>(%4), const<i32>(19))), from_bool<i27b, reason=assign>(lt<i27b>(widen<i27b, reason=usual_arith>(read<i6b>(%1)), read<i27b>(%2))));
// DEFAULT-NEXT:         write<i27b>(deref(ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(read<ptr<i27b>>(%4), const<i32>(20))), from_bool<i27b, reason=assign>(ge<i27b>(widen<i27b, reason=usual_arith>(read<i6b>(%1)), read<i27b>(%2))));
// DEFAULT-NEXT:         write<i27b>(deref(ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(read<ptr<i27b>>(%4), const<i32>(21))), from_bool<i27b, reason=assign>(le<i27b>(widen<i27b, reason=usual_arith>(read<i6b>(%1)), read<i27b>(%2))));
// DEFAULT-NEXT:         write<i27b>(deref(ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(read<ptr<i27b>>(%4), const<i32>(22))), from_bool<i27b, reason=assign>(logical_and<bool>(ne<i6b>(read<i6b>(%1), const<i6b>(0)), ne<i27b>(read<i27b>(%2), const<i27b>(0)))));
// DEFAULT-NEXT:         write<i27b>(deref(ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(read<ptr<i27b>>(%4), const<i32>(23))), from_bool<i27b, reason=assign>(logical_or<bool>(ne<i6b>(read<i6b>(%1), const<i6b>(0)), ne<i27b>(read<i27b>(%2), const<i27b>(0)))));
// DEFAULT-NEXT:         write<i27b>(deref(ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(read<ptr<i27b>>(%4), const<i32>(24))), from_bool<i27b, reason=assign>(not<bool>(ne<i6b>(read<i6b>(%1), const<i6b>(0)))));
// DEFAULT-NEXT:         write<i27b>(deref(ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(read<ptr<i27b>>(%4), const<i32>(25))), and<i27b>(widen<i27b, reason=usual_arith>(read<i6b>(%1)), read<i27b>(%2)));
// DEFAULT-NEXT:         write<i27b>(deref(ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(read<ptr<i27b>>(%4), const<i32>(26))), or<i27b>(widen<i27b, reason=usual_arith>(read<i6b>(%1)), read<i27b>(%2)));
// DEFAULT-NEXT:         write<i27b>(deref(ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(read<ptr<i27b>>(%4), const<i32>(27))), xor<i27b>(widen<i27b, reason=usual_arith>(read<i6b>(%1)), read<i27b>(%2)));
// DEFAULT-NEXT:         write<i27b>(deref(ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(read<ptr<i27b>>(%4), const<i32>(28))), widen<i27b, reason=assign>(not<i6b>(read<i6b>(%1))));
// DEFAULT-NEXT:         let %28: ptr<i27b> [synthetic] = ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(read<ptr<i27b>>(%4), const<i32>(29));
// DEFAULT-NEXT:         let %29: i27b [synthetic] = read<i27b>(deref(read<ptr<i27b>>(%28)));
// DEFAULT-NEXT:         let %30: i27b [synthetic] = sub<i27b, overflow=ub>(read<i27b>(%29), widen<i27b, reason=usual_arith>(read<i6b>(%1)));
// DEFAULT-NEXT:         write<i27b>(deref(read<ptr<i27b>>(%28)), read<i27b>(%30));
// DEFAULT-NEXT:         let %31: ptr<i27b> [synthetic] = ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(read<ptr<i27b>>(%4), const<i32>(30));
// DEFAULT-NEXT:         let %32: i27b [synthetic] = read<i27b>(deref(read<ptr<i27b>>(%31)));
// DEFAULT-NEXT:         let %33: i27b [synthetic] = mul<i27b, overflow=ub>(read<i27b>(%32), read<i27b>(%2));
// DEFAULT-NEXT:         write<i27b>(deref(read<ptr<i27b>>(%31)), read<i27b>(%33));
// DEFAULT-NEXT:         let %34: ptr<i27b> [synthetic] = ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(read<ptr<i27b>>(%4), const<i32>(31));
// DEFAULT-NEXT:         let %35: i27b [synthetic] = read<i27b>(deref(read<ptr<i27b>>(%34)));
// DEFAULT-NEXT:         let %36: i27b [synthetic] = div<i27b, by_zero=ub, min_by_neg_one=ub>(read<i27b>(%35), read<i27b>(%2));
// DEFAULT-NEXT:         write<i27b>(deref(read<ptr<i27b>>(%34)), read<i27b>(%36));
// DEFAULT-NEXT:         let %37: ptr<i27b> [synthetic] = ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(read<ptr<i27b>>(%4), const<i32>(32));
// DEFAULT-NEXT:         let %38: i27b [synthetic] = read<i27b>(deref(read<ptr<i27b>>(%37)));
// DEFAULT-NEXT:         let %39: i27b [synthetic] = rem<i27b, by_zero=ub, min_by_neg_one=ub>(read<i27b>(%38), widen<i27b, reason=usual_arith>(read<i6b>(%1)));
// DEFAULT-NEXT:         write<i27b>(deref(read<ptr<i27b>>(%37)), read<i27b>(%39));
// DEFAULT-NEXT:         let %40: ptr<i27b> [synthetic] = ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(read<ptr<i27b>>(%4), const<i32>(33));
// DEFAULT-NEXT:         let %41: i27b [synthetic] = read<i27b>(deref(read<ptr<i27b>>(%40)));
// DEFAULT-NEXT:         let %42: i27b [synthetic] = shl<i27b, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i27b>(%41), read<i27b>(%2));
// DEFAULT-NEXT:         write<i27b>(deref(read<ptr<i27b>>(%40)), read<i27b>(%42));
// DEFAULT-NEXT:         let %43: ptr<i27b> [synthetic] = ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(read<ptr<i27b>>(%4), const<i32>(34));
// DEFAULT-NEXT:         let %44: i27b [synthetic] = read<i27b>(deref(read<ptr<i27b>>(%43)));
// DEFAULT-NEXT:         let %45: i27b [synthetic] = shr<i27b, amount_out_of_range=ub, fill=sign_extend>(read<i27b>(%44), read<i27b>(%2));
// DEFAULT-NEXT:         write<i27b>(deref(read<ptr<i27b>>(%43)), read<i27b>(%45));
// DEFAULT-NEXT:         let %46: ptr<i27b> [synthetic] = ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(read<ptr<i27b>>(%4), const<i32>(35));
// DEFAULT-NEXT:         let %47: i27b [synthetic] = read<i27b>(deref(read<ptr<i27b>>(%46)));
// DEFAULT-NEXT:         let %48: i27b [synthetic] = and<i27b>(read<i27b>(%47), widen<i27b, reason=usual_arith>(read<i6b>(%1)));
// DEFAULT-NEXT:         write<i27b>(deref(read<ptr<i27b>>(%46)), read<i27b>(%48));
// DEFAULT-NEXT:         let %49: ptr<i27b> [synthetic] = ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(read<ptr<i27b>>(%4), const<i32>(36));
// DEFAULT-NEXT:         let %50: i27b [synthetic] = read<i27b>(deref(read<ptr<i27b>>(%49)));
// DEFAULT-NEXT:         let %51: i27b [synthetic] = or<i27b>(read<i27b>(%50), read<i27b>(%2));
// DEFAULT-NEXT:         write<i27b>(deref(read<ptr<i27b>>(%49)), read<i27b>(%51));
// DEFAULT-NEXT:         let %52: ptr<i27b> [synthetic] = ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(read<ptr<i27b>>(%4), const<i32>(37));
// DEFAULT-NEXT:         let %53: i27b [synthetic] = read<i27b>(deref(read<ptr<i27b>>(%52)));
// DEFAULT-NEXT:         let %54: i27b [synthetic] = xor<i27b>(read<i27b>(%53), widen<i27b, reason=usual_arith>(read<i6b>(%1)));
// DEFAULT-NEXT:         write<i27b>(deref(read<ptr<i27b>>(%52)), read<i27b>(%54));
// DEFAULT-NEXT:         write<i27b>(deref(ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(read<ptr<i27b>>(%4), const<i32>(38))), reinterpret<i27b, reason=assign, fits=unknown>(truncate<u27b, reason=assign, fits=always>(const<u64>(1))));
// DEFAULT-NEXT:         write<i27b>(deref(ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(read<ptr<i27b>>(%4), const<i32>(39))), conditional<i27b>(ne<i27b>(read<i27b>(deref(ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(read<ptr<i27b>>(%4), const<i32>(39)))), const<i27b>(0)), widen<i27b, reason=usual_arith>(read<i6b>(%1)), read<i27b>(%2)));
// DEFAULT-NEXT:         write<i27b>(deref(ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(read<ptr<i27b>>(%4), const<i32>(40))), widen<i27b, reason=assign>(const<i15b>(12345)));
// DEFAULT-NEXT:         switch %11 read<i6b>(%1)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %11 const<i6b>(31):
// DEFAULT-NEXT:                     if ne<i27b>(read<i27b>(%2), widen<i27b, reason=usual_arith>(const<i5b>(8)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 break %11;
// DEFAULT-NEXT:                 case %11 const<i6b>(-18):
// DEFAULT-NEXT:                     if ne<i27b>(read<i27b>(%2), widen<i27b, reason=usual_arith>(const<i5b>(9)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 break %11;
// DEFAULT-NEXT:                 case %11 const<i6b>(26):
// DEFAULT-NEXT:                     if ne<i27b>(read<i27b>(%2), widen<i27b, reason=usual_arith>(const<i5b>(12)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 break %11;
// DEFAULT-NEXT:                 case %11 const<i6b>(-25):
// DEFAULT-NEXT:                     if ne<i27b>(read<i27b>(%2), widen<i27b, reason=usual_arith>(const<i4b>(6)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 break %11;
// DEFAULT-NEXT:                 case %11 const<i6b>(-19):
// DEFAULT-NEXT:                     if ne<i27b>(read<i27b>(%2), widen<i27b, reason=usual_arith>(const<i5b>(15)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 break %11;
// DEFAULT-NEXT:                 default %11:
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %7 p: i6b [storage=automatic];
// DEFAULT-NEXT:         let %8 q: array<i27b, 41> [storage=automatic];
// DEFAULT-NEXT:         write<i27b>(deref(ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(array_decay<ptr<i27b>, length=Some(41)>(%8), const<i32>(4))), widen<i27b, reason=assign>(const<i6b>(18)));
// DEFAULT-NEXT:         write<i27b>(deref(ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(array_decay<ptr<i27b>, length=Some(41)>(%8), const<i32>(12))), widen<i27b, reason=assign>(const<i4b>(7)));
// DEFAULT-NEXT:         write<i27b>(deref(ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(array_decay<ptr<i27b>, length=Some(41)>(%8), const<i32>(13))), widen<i27b, reason=assign>(const<i5b>(9)));
// DEFAULT-NEXT:         write<i27b>(deref(ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(array_decay<ptr<i27b>, length=Some(41)>(%8), const<i32>(14))), widen<i27b, reason=assign>(const<i5b>(11)));
// DEFAULT-NEXT:         write<i27b>(deref(ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(array_decay<ptr<i27b>, length=Some(41)>(%8), const<i32>(15))), widen<i27b, reason=assign>(const<i5b>(13)));
// DEFAULT-NEXT:         write<i27b>(deref(ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(array_decay<ptr<i27b>, length=Some(41)>(%8), const<i32>(29))), widen<i27b, reason=assign>(const<i4b>(7)));
// DEFAULT-NEXT:         write<i27b>(deref(ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(array_decay<ptr<i27b>, length=Some(41)>(%8), const<i32>(30))), widen<i27b, reason=assign>(neg<i2b, overflow=ub>(const<i2b>(1))));
// DEFAULT-NEXT:         write<i27b>(deref(ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(array_decay<ptr<i27b>, length=Some(41)>(%8), const<i32>(31))), widen<i27b, reason=assign>(neg<i5b, overflow=ub>(const<i5b>(13))));
// DEFAULT-NEXT:         write<i27b>(deref(ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(array_decay<ptr<i27b>, length=Some(41)>(%8), const<i32>(32))), widen<i27b, reason=assign>(const<i7b>(52)));
// DEFAULT-NEXT:         write<i27b>(deref(ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(array_decay<ptr<i27b>, length=Some(41)>(%8), const<i32>(33))), widen<i27b, reason=assign>(const<i4b>(5)));
// DEFAULT-NEXT:         write<i27b>(deref(ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(array_decay<ptr<i27b>, length=Some(41)>(%8), const<i32>(34))), widen<i27b, reason=assign>(const<i15b>(12345)));
// DEFAULT-NEXT:         write<i27b>(deref(ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(array_decay<ptr<i27b>, length=Some(41)>(%8), const<i32>(35))), widen<i27b, reason=assign>(const<i5b>(15)));
// DEFAULT-NEXT:         write<i27b>(deref(ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(array_decay<ptr<i27b>, length=Some(41)>(%8), const<i32>(36))), widen<i27b, reason=assign>(const<i6b>(28)));
// DEFAULT-NEXT:         write<i27b>(deref(ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(array_decay<ptr<i27b>, length=Some(41)>(%8), const<i32>(37))), widen<i27b, reason=assign>(const<i4b>(5)));
// DEFAULT-NEXT:         write<i27b>(deref(ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(array_decay<ptr<i27b>, length=Some(41)>(%8), const<i32>(39))), widen<i27b, reason=assign>(const<i3b>(2)));
// DEFAULT-NEXT:         call<void, signature=fn(i6b, i27b, ptr<i6b>, ptr<i27b>, f32) -> void>(%0, neg<i6b, overflow=ub>(const<i6b>(25)), widen<i27b, reason=arg>(const<i4b>(6)), addr_of<ptr<i6b>>(%7), array_decay<ptr<i27b>, length=Some(41)>(%8), const<f32>(17.0));
// DEFAULT-NEXT:         if ne<i6b>(read<i6b>(%7), widen<i6b, reason=usual_arith>(const<i4b>(6)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         let %55: ptr<i27b> [synthetic] = ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(array_decay<ptr<i27b>, length=Some(41)>(%8), const<i32>(38));
// DEFAULT-NEXT:         let %56: i27b [synthetic] = read<i27b>(deref(read<ptr<i27b>>(%55)));
// DEFAULT-NEXT:         let %57: i27b [synthetic] = reinterpret<i27b, reason=assign, fits=unknown>(truncate<u27b, reason=assign, fits=unknown>(sub<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i27b>(%56))), sub<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:         write<i27b>(deref(read<ptr<i27b>>(%55)), read<i27b>(%57));
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
// DEFAULT-NEXT:                 if ne<i27b>(read<i27b>(deref(ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(array_decay<ptr<i27b>, length=Some(41)>(%8), read<i32>(%10)))), read<i27b>(deref(ptr_offset<ptr<i27b>, subtract=false, element=i27b, overflow=ub>(array_decay<ptr<i27b>, length=Some(41)>(%9), read<i32>(%10)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
