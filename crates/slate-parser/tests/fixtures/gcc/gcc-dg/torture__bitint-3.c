/* PR c/102989 */
/* { dg-do run { target bitint } } */
/* { dg-options "-std=c23 -pedantic-errors" } */
/* { dg-skip-if "" { ! run_expensive_tests }  { "*" } { "-O0" "-O2" } } */
/* { dg-skip-if "" { ! run_expensive_tests } { "-flto" } { "" } } */

#if __BITINT_MAXWIDTH__ >= 128
__attribute__((noipa)) void foo(_BitInt(125) a, _BitInt(128) b,
                                _BitInt(125) * p, _BitInt(128) * q, float c) {
  p[0]   = b;
  q[0]   = a;
  q[1]   = (unsigned _BitInt(125))a;
  q[2]   = (unsigned _BitInt(68))a;
  q[3]   = c;
  q[4]  += a;
  q[5]   = a + b;
  q[6]   = a - b;
  q[7]   = a * b;
  q[8]   = a / b;
  q[9]   = a % b;
  q[10]  = b << (-80694244678005661015504159217709wb - a);
  q[11]  = a >> (-80694244678005661015504159217709wb - a);
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
  q[33] <<= (b - 468021wb);
  q[34] >>= (b - 468021wb);
  q[35]  &= a;
  q[36]  |= b;
  q[37]  ^= a;
  q[38]   = sizeof(a);
  q[39]   = q[39] ? a : b;
  q[40]   = 80694244678005661015504159217732wb;
  switch (a) {
  case 313298472398574896574578475487548wb:
    if (b != 813298738947385454wb)
      __builtin_abort();
    break;
  case -18198347584784758927893783748374wb:
    if (b != 9439847384738wb)
      __builtin_abort();
    break;
  case 261243875485748189278344574857484wb:
    if (b != 12549857489574wb)
      __builtin_abort();
    break;
  case -80694244678005661015504159217733wb:
    if (b != 468071wb)
      __builtin_abort();
    break;
  case -193984372895748547584754854wb:
    if (b != 15549857489574wb)
      __builtin_abort();
    break;
  default:
    __builtin_abort();
  }
}

int
main() {
  _BitInt(125) p;
  _BitInt(128) q[41];
  static _BitInt(128) qe[41] = {-80694244678005661015504159217733wb,
                                42535215170872629927260810424811808699wb,
                                211591633426360068027wb,
                                -42wb,
                                -80694244678005661015504143312912wb,
                                -80694244678005661015504158749662wb,
                                -80694244678005661015504159685804wb,
                                -37770635800678787757188047309203503043wb,
                                -172397445426026523786998466wb,
                                -238647wb,
                                7852928270336wb,
                                -4809751789450982869595538wb,
                                821095840985901334959wb,
                                13895798174897154898wb,
                                1465897921835729857453wb,
                                154987847598437549873142wb,
                                0,
                                1,
                                0,
                                1,
                                0,
                                1,
                                1,
                                1,
                                0,
                                271395wb,
                                -80694244678005661015504159021057wb,
                                -80694244678005661015504159292452wb,
                                80694244678005661015504159217732wb,
                                80694244678240648474427914115297wb,
                                -5822736520666880936123wb,
                                9818355413803wb,
                                54398547589478975845wb,
                                39390147499089156967386811811758080wb,
                                30486wb,
                                5910462358441918751905wb,
                                1342984375894755194479wb,
                                -80694244535621434450947930710749wb,
                                1,
                                -80694244678005661015504159217733wb,
                                80694244678005661015504159217732wb};
  q[4]                       = 15904821wb;
  q[12]                      = 821095840985901334958wb;
  q[13]                      = 13895798174897154897wb;
  q[14]                      = 1465897921835729857454wb;
  q[15]                      = 154987847598437549873143wb;
  q[29]                      = 234987458923754897564wb;
  q[30]                      = -12439857458947213wb;
  q[31]                      = 4595687436894573685wb;
  q[32]                      = 54398547589478975845wb;
  q[33]                      = 34985478957495847545wb;
  q[34]                      = 34324329847328473343wb;
  q[35]                      = 5984758947589437584545wb;
  q[36]                      = 1342984375894754857545wb;
  q[37]                      = 159847589475894768597656wb;
  q[39]                      = 394857584wb;
  foo(-80694244678005661015504159217733wb, 468071wb, &p, q, -42.0f);
  if (p != 468071wb)
    __builtin_abort();
  q[38] -= sizeof(p) - 1;
  for (int i = 0; i < 41; ++i)
    if (q[i] != qe[i])
      __builtin_abort();
  return 0;
}
#else
int
main() {
  return 0;
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
// DEFAULT-NEXT:     global %9 qe: array<i128b, 41> [storage=static] = aggregate<array<i128b, 41>, zero_fill=false>(index0 = widen<i128b, reason=assign>(neg<i107b, overflow=ub>(const<i107b>(80694244678005661015504159217733))), index1 = widen<i128b, reason=assign>(const<i126b>(42535215170872629927260810424811808699)), index2 = widen<i128b, reason=assign>(const<i69b>(211591633426360068027)), index3 = widen<i128b, reason=assign>(neg<i7b, overflow=ub>(const<i7b>(42))), index4 = widen<i128b, reason=assign>(neg<i107b, overflow=ub>(const<i107b>(80694244678005661015504143312912))), index5 = widen<i128b, reason=assign>(neg<i107b, overflow=ub>(const<i107b>(80694244678005661015504158749662))), index6 = widen<i128b, reason=assign>(neg<i107b, overflow=ub>(const<i107b>(80694244678005661015504159685804))), index7 = widen<i128b, reason=assign>(neg<i126b, overflow=ub>(const<i126b>(37770635800678787757188047309203503043))), index8 = widen<i128b, reason=assign>(neg<i89b, overflow=ub>(const<i89b>(172397445426026523786998466))), index9 = widen<i128b, reason=assign>(neg<i19b, overflow=ub>(const<i19b>(238647))), index10 = widen<i128b, reason=assign>(const<i44b>(7852928270336)), index11 = widen<i128b, reason=assign>(neg<i83b, overflow=ub>(const<i83b>(4809751789450982869595538))), index12 = widen<i128b, reason=assign>(const<i71b>(821095840985901334959)), index13 = widen<i128b, reason=assign>(const<i65b>(13895798174897154898)), index14 = widen<i128b, reason=assign>(const<i72b>(1465897921835729857453)), index15 = widen<i128b, reason=assign>(const<i79b>(154987847598437549873142)), index16 = widen<i128b, reason=assign>(const<i32>(0)), index17 = widen<i128b, reason=assign>(const<i32>(1)), index18 = widen<i128b, reason=assign>(const<i32>(0)), index19 = widen<i128b, reason=assign>(const<i32>(1)), index20 = widen<i128b, reason=assign>(const<i32>(0)), index21 = widen<i128b, reason=assign>(const<i32>(1)), index22 = widen<i128b, reason=assign>(const<i32>(1)), index23 = widen<i128b, reason=assign>(const<i32>(1)), index24 = widen<i128b, reason=assign>(const<i32>(0)), index25 = widen<i128b, reason=assign>(const<i20b>(271395)), index26 = widen<i128b, reason=assign>(neg<i107b, overflow=ub>(const<i107b>(80694244678005661015504159021057))), index27 = widen<i128b, reason=assign>(neg<i107b, overflow=ub>(const<i107b>(80694244678005661015504159292452))), index28 = widen<i128b, reason=assign>(const<i107b>(80694244678005661015504159217732)), index29 = widen<i128b, reason=assign>(const<i107b>(80694244678240648474427914115297)), index30 = widen<i128b, reason=assign>(neg<i74b, overflow=ub>(const<i74b>(5822736520666880936123))), index31 = widen<i128b, reason=assign>(const<i45b>(9818355413803)), index32 = widen<i128b, reason=assign>(const<i67b>(54398547589478975845)), index33 = widen<i128b, reason=assign>(const<i116b>(39390147499089156967386811811758080)), index34 = widen<i128b, reason=assign>(const<i16b>(30486)), index35 = widen<i128b, reason=assign>(const<i74b>(5910462358441918751905)), index36 = widen<i128b, reason=assign>(const<i72b>(1342984375894755194479)), index37 = widen<i128b, reason=assign>(neg<i107b, overflow=ub>(const<i107b>(80694244535621434450947930710749))), index38 = widen<i128b, reason=assign>(const<i32>(1)), index39 = widen<i128b, reason=assign>(neg<i107b, overflow=ub>(const<i107b>(80694244678005661015504159217733))), index40 = widen<i128b, reason=assign>(const<i107b>(80694244678005661015504159217732))) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @foo(%1 a: i125b, %2 b: i128b, %3 p: ptr<i125b>, %4 q: ptr<i128b>, %5 c: f32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i125b>(deref(ptr_offset<ptr<i125b>, subtract=false, element=i125b, overflow=ub>(read<ptr<i125b>>(%3), const<i32>(0))), truncate<i125b, reason=assign, fits=unknown>(read<i128b>(%2)));
// DEFAULT-NEXT:         write<i128b>(deref(ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(read<ptr<i128b>>(%4), const<i32>(0))), widen<i128b, reason=assign>(read<i125b>(%1)));
// DEFAULT-NEXT:         write<i128b>(deref(ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(read<ptr<i128b>>(%4), const<i32>(1))), reinterpret<i128b, reason=assign, fits=unknown>(widen<u128b, reason=assign>(reinterpret<u125b, reason=explicit, fits=unknown>(read<i125b>(%1)))));
// DEFAULT-NEXT:         write<i128b>(deref(ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(read<ptr<i128b>>(%4), const<i32>(2))), reinterpret<i128b, reason=assign, fits=unknown>(widen<u128b, reason=assign>(reinterpret<u68b, reason=explicit, fits=unknown>(truncate<i68b, reason=explicit, fits=unknown>(read<i125b>(%1))))));
// DEFAULT-NEXT:         write<i128b>(deref(ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(read<ptr<i128b>>(%4), const<i32>(3))), float_to_int<i128b, reason=assign, out_of_range=ub, exceptions=ignore>(read<f32>(%5)));
// DEFAULT-NEXT:         let %13: ptr<i128b> [synthetic] = ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(read<ptr<i128b>>(%4), const<i32>(4));
// DEFAULT-NEXT:         let %14: i128b [synthetic] = read<i128b>(deref(read<ptr<i128b>>(%13)));
// DEFAULT-NEXT:         let %15: i128b [synthetic] = add<i128b, overflow=ub>(read<i128b>(%14), widen<i128b, reason=usual_arith>(read<i125b>(%1)));
// DEFAULT-NEXT:         write<i128b>(deref(read<ptr<i128b>>(%13)), read<i128b>(%15));
// DEFAULT-NEXT:         write<i128b>(deref(ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(read<ptr<i128b>>(%4), const<i32>(5))), add<i128b, overflow=ub>(widen<i128b, reason=usual_arith>(read<i125b>(%1)), read<i128b>(%2)));
// DEFAULT-NEXT:         write<i128b>(deref(ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(read<ptr<i128b>>(%4), const<i32>(6))), sub<i128b, overflow=ub>(widen<i128b, reason=usual_arith>(read<i125b>(%1)), read<i128b>(%2)));
// DEFAULT-NEXT:         write<i128b>(deref(ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(read<ptr<i128b>>(%4), const<i32>(7))), mul<i128b, overflow=ub>(widen<i128b, reason=usual_arith>(read<i125b>(%1)), read<i128b>(%2)));
// DEFAULT-NEXT:         write<i128b>(deref(ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(read<ptr<i128b>>(%4), const<i32>(8))), div<i128b, by_zero=ub, min_by_neg_one=ub>(widen<i128b, reason=usual_arith>(read<i125b>(%1)), read<i128b>(%2)));
// DEFAULT-NEXT:         write<i128b>(deref(ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(read<ptr<i128b>>(%4), const<i32>(9))), rem<i128b, by_zero=ub, min_by_neg_one=ub>(widen<i128b, reason=usual_arith>(read<i125b>(%1)), read<i128b>(%2)));
// DEFAULT-NEXT:         write<i128b>(deref(ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(read<ptr<i128b>>(%4), const<i32>(10))), shl<i128b, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i128b>(%2), sub<i125b, overflow=ub>(widen<i125b, reason=usual_arith>(neg<i107b, overflow=ub>(const<i107b>(80694244678005661015504159217709))), read<i125b>(%1))));
// DEFAULT-NEXT:         write<i128b>(deref(ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(read<ptr<i128b>>(%4), const<i32>(11))), widen<i128b, reason=assign>(shr<i125b, amount_out_of_range=ub, fill=sign_extend>(read<i125b>(%1), sub<i125b, overflow=ub>(widen<i125b, reason=usual_arith>(neg<i107b, overflow=ub>(const<i107b>(80694244678005661015504159217709))), read<i125b>(%1)))));
// DEFAULT-NEXT:         let %16: ptr<i128b> [synthetic] = ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(read<ptr<i128b>>(%4), const<i32>(12));
// DEFAULT-NEXT:         let %17: i128b [synthetic] = read<i128b>(deref(read<ptr<i128b>>(%16)));
// DEFAULT-NEXT:         let %18: i128b [synthetic] = add<i128b, overflow=ub>(read<i128b>(%17), widen<i128b, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128b>(deref(read<ptr<i128b>>(%16)), read<i128b>(%18));
// DEFAULT-NEXT:         let %19: ptr<i128b> [synthetic] = ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(read<ptr<i128b>>(%4), const<i32>(13));
// DEFAULT-NEXT:         let %20: i128b [synthetic] = read<i128b>(deref(read<ptr<i128b>>(%19)));
// DEFAULT-NEXT:         let %21: i128b [synthetic] = add<i128b, overflow=ub>(read<i128b>(%20), widen<i128b, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128b>(deref(read<ptr<i128b>>(%19)), read<i128b>(%21));
// DEFAULT-NEXT:         let %22: ptr<i128b> [synthetic] = ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(read<ptr<i128b>>(%4), const<i32>(14));
// DEFAULT-NEXT:         let %23: i128b [synthetic] = read<i128b>(deref(read<ptr<i128b>>(%22)));
// DEFAULT-NEXT:         let %24: i128b [synthetic] = sub<i128b, overflow=ub>(read<i128b>(%23), widen<i128b, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128b>(deref(read<ptr<i128b>>(%22)), read<i128b>(%24));
// DEFAULT-NEXT:         let %25: ptr<i128b> [synthetic] = ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(read<ptr<i128b>>(%4), const<i32>(15));
// DEFAULT-NEXT:         let %26: i128b [synthetic] = read<i128b>(deref(read<ptr<i128b>>(%25)));
// DEFAULT-NEXT:         let %27: i128b [synthetic] = sub<i128b, overflow=ub>(read<i128b>(%26), widen<i128b, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128b>(deref(read<ptr<i128b>>(%25)), read<i128b>(%27));
// DEFAULT-NEXT:         write<i128b>(deref(ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(read<ptr<i128b>>(%4), const<i32>(16))), from_bool<i128b, reason=assign>(eq<i128b>(widen<i128b, reason=usual_arith>(read<i125b>(%1)), read<i128b>(%2))));
// DEFAULT-NEXT:         write<i128b>(deref(ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(read<ptr<i128b>>(%4), const<i32>(17))), from_bool<i128b, reason=assign>(ne<i128b>(widen<i128b, reason=usual_arith>(read<i125b>(%1)), read<i128b>(%2))));
// DEFAULT-NEXT:         write<i128b>(deref(ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(read<ptr<i128b>>(%4), const<i32>(18))), from_bool<i128b, reason=assign>(gt<i128b>(widen<i128b, reason=usual_arith>(read<i125b>(%1)), read<i128b>(%2))));
// DEFAULT-NEXT:         write<i128b>(deref(ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(read<ptr<i128b>>(%4), const<i32>(19))), from_bool<i128b, reason=assign>(lt<i128b>(widen<i128b, reason=usual_arith>(read<i125b>(%1)), read<i128b>(%2))));
// DEFAULT-NEXT:         write<i128b>(deref(ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(read<ptr<i128b>>(%4), const<i32>(20))), from_bool<i128b, reason=assign>(ge<i128b>(widen<i128b, reason=usual_arith>(read<i125b>(%1)), read<i128b>(%2))));
// DEFAULT-NEXT:         write<i128b>(deref(ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(read<ptr<i128b>>(%4), const<i32>(21))), from_bool<i128b, reason=assign>(le<i128b>(widen<i128b, reason=usual_arith>(read<i125b>(%1)), read<i128b>(%2))));
// DEFAULT-NEXT:         write<i128b>(deref(ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(read<ptr<i128b>>(%4), const<i32>(22))), from_bool<i128b, reason=assign>(logical_and<bool>(ne<i125b>(read<i125b>(%1), const<i125b>(0)), ne<i128b>(read<i128b>(%2), const<i128b>(0)))));
// DEFAULT-NEXT:         write<i128b>(deref(ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(read<ptr<i128b>>(%4), const<i32>(23))), from_bool<i128b, reason=assign>(logical_or<bool>(ne<i125b>(read<i125b>(%1), const<i125b>(0)), ne<i128b>(read<i128b>(%2), const<i128b>(0)))));
// DEFAULT-NEXT:         write<i128b>(deref(ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(read<ptr<i128b>>(%4), const<i32>(24))), from_bool<i128b, reason=assign>(not<bool>(ne<i125b>(read<i125b>(%1), const<i125b>(0)))));
// DEFAULT-NEXT:         write<i128b>(deref(ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(read<ptr<i128b>>(%4), const<i32>(25))), and<i128b>(widen<i128b, reason=usual_arith>(read<i125b>(%1)), read<i128b>(%2)));
// DEFAULT-NEXT:         write<i128b>(deref(ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(read<ptr<i128b>>(%4), const<i32>(26))), or<i128b>(widen<i128b, reason=usual_arith>(read<i125b>(%1)), read<i128b>(%2)));
// DEFAULT-NEXT:         write<i128b>(deref(ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(read<ptr<i128b>>(%4), const<i32>(27))), xor<i128b>(widen<i128b, reason=usual_arith>(read<i125b>(%1)), read<i128b>(%2)));
// DEFAULT-NEXT:         write<i128b>(deref(ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(read<ptr<i128b>>(%4), const<i32>(28))), widen<i128b, reason=assign>(not<i125b>(read<i125b>(%1))));
// DEFAULT-NEXT:         let %28: ptr<i128b> [synthetic] = ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(read<ptr<i128b>>(%4), const<i32>(29));
// DEFAULT-NEXT:         let %29: i128b [synthetic] = read<i128b>(deref(read<ptr<i128b>>(%28)));
// DEFAULT-NEXT:         let %30: i128b [synthetic] = sub<i128b, overflow=ub>(read<i128b>(%29), widen<i128b, reason=usual_arith>(read<i125b>(%1)));
// DEFAULT-NEXT:         write<i128b>(deref(read<ptr<i128b>>(%28)), read<i128b>(%30));
// DEFAULT-NEXT:         let %31: ptr<i128b> [synthetic] = ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(read<ptr<i128b>>(%4), const<i32>(30));
// DEFAULT-NEXT:         let %32: i128b [synthetic] = read<i128b>(deref(read<ptr<i128b>>(%31)));
// DEFAULT-NEXT:         let %33: i128b [synthetic] = mul<i128b, overflow=ub>(read<i128b>(%32), read<i128b>(%2));
// DEFAULT-NEXT:         write<i128b>(deref(read<ptr<i128b>>(%31)), read<i128b>(%33));
// DEFAULT-NEXT:         let %34: ptr<i128b> [synthetic] = ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(read<ptr<i128b>>(%4), const<i32>(31));
// DEFAULT-NEXT:         let %35: i128b [synthetic] = read<i128b>(deref(read<ptr<i128b>>(%34)));
// DEFAULT-NEXT:         let %36: i128b [synthetic] = div<i128b, by_zero=ub, min_by_neg_one=ub>(read<i128b>(%35), read<i128b>(%2));
// DEFAULT-NEXT:         write<i128b>(deref(read<ptr<i128b>>(%34)), read<i128b>(%36));
// DEFAULT-NEXT:         let %37: ptr<i128b> [synthetic] = ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(read<ptr<i128b>>(%4), const<i32>(32));
// DEFAULT-NEXT:         let %38: i128b [synthetic] = read<i128b>(deref(read<ptr<i128b>>(%37)));
// DEFAULT-NEXT:         let %39: i128b [synthetic] = rem<i128b, by_zero=ub, min_by_neg_one=ub>(read<i128b>(%38), widen<i128b, reason=usual_arith>(read<i125b>(%1)));
// DEFAULT-NEXT:         write<i128b>(deref(read<ptr<i128b>>(%37)), read<i128b>(%39));
// DEFAULT-NEXT:         let %40: ptr<i128b> [synthetic] = ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(read<ptr<i128b>>(%4), const<i32>(33));
// DEFAULT-NEXT:         let %41: i128b [synthetic] = read<i128b>(deref(read<ptr<i128b>>(%40)));
// DEFAULT-NEXT:         let %42: i128b [synthetic] = shl<i128b, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i128b>(%41), sub<i128b, overflow=ub>(read<i128b>(%2), widen<i128b, reason=usual_arith>(const<i20b>(468021))));
// DEFAULT-NEXT:         write<i128b>(deref(read<ptr<i128b>>(%40)), read<i128b>(%42));
// DEFAULT-NEXT:         let %43: ptr<i128b> [synthetic] = ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(read<ptr<i128b>>(%4), const<i32>(34));
// DEFAULT-NEXT:         let %44: i128b [synthetic] = read<i128b>(deref(read<ptr<i128b>>(%43)));
// DEFAULT-NEXT:         let %45: i128b [synthetic] = shr<i128b, amount_out_of_range=ub, fill=sign_extend>(read<i128b>(%44), sub<i128b, overflow=ub>(read<i128b>(%2), widen<i128b, reason=usual_arith>(const<i20b>(468021))));
// DEFAULT-NEXT:         write<i128b>(deref(read<ptr<i128b>>(%43)), read<i128b>(%45));
// DEFAULT-NEXT:         let %46: ptr<i128b> [synthetic] = ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(read<ptr<i128b>>(%4), const<i32>(35));
// DEFAULT-NEXT:         let %47: i128b [synthetic] = read<i128b>(deref(read<ptr<i128b>>(%46)));
// DEFAULT-NEXT:         let %48: i128b [synthetic] = and<i128b>(read<i128b>(%47), widen<i128b, reason=usual_arith>(read<i125b>(%1)));
// DEFAULT-NEXT:         write<i128b>(deref(read<ptr<i128b>>(%46)), read<i128b>(%48));
// DEFAULT-NEXT:         let %49: ptr<i128b> [synthetic] = ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(read<ptr<i128b>>(%4), const<i32>(36));
// DEFAULT-NEXT:         let %50: i128b [synthetic] = read<i128b>(deref(read<ptr<i128b>>(%49)));
// DEFAULT-NEXT:         let %51: i128b [synthetic] = or<i128b>(read<i128b>(%50), read<i128b>(%2));
// DEFAULT-NEXT:         write<i128b>(deref(read<ptr<i128b>>(%49)), read<i128b>(%51));
// DEFAULT-NEXT:         let %52: ptr<i128b> [synthetic] = ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(read<ptr<i128b>>(%4), const<i32>(37));
// DEFAULT-NEXT:         let %53: i128b [synthetic] = read<i128b>(deref(read<ptr<i128b>>(%52)));
// DEFAULT-NEXT:         let %54: i128b [synthetic] = xor<i128b>(read<i128b>(%53), widen<i128b, reason=usual_arith>(read<i125b>(%1)));
// DEFAULT-NEXT:         write<i128b>(deref(read<ptr<i128b>>(%52)), read<i128b>(%54));
// DEFAULT-NEXT:         write<i128b>(deref(ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(read<ptr<i128b>>(%4), const<i32>(38))), reinterpret<i128b, reason=assign, fits=unknown>(widen<u128b, reason=assign>(const<u64>(16))));
// DEFAULT-NEXT:         write<i128b>(deref(ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(read<ptr<i128b>>(%4), const<i32>(39))), conditional<i128b>(ne<i128b>(read<i128b>(deref(ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(read<ptr<i128b>>(%4), const<i32>(39)))), const<i128b>(0)), widen<i128b, reason=usual_arith>(read<i125b>(%1)), read<i128b>(%2)));
// DEFAULT-NEXT:         write<i128b>(deref(ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(read<ptr<i128b>>(%4), const<i32>(40))), widen<i128b, reason=assign>(const<i107b>(80694244678005661015504159217732)));
// DEFAULT-NEXT:         switch %11 read<i125b>(%1)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %11 const<i125b>(313298472398574896574578475487548):
// DEFAULT-NEXT:                     if ne<i128b>(read<i128b>(%2), widen<i128b, reason=usual_arith>(const<i61b>(813298738947385454)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 break %11;
// DEFAULT-NEXT:                 case %11 const<i125b>(-18198347584784758927893783748374):
// DEFAULT-NEXT:                     if ne<i128b>(read<i128b>(%2), widen<i128b, reason=usual_arith>(const<i45b>(9439847384738)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 break %11;
// DEFAULT-NEXT:                 case %11 const<i125b>(261243875485748189278344574857484):
// DEFAULT-NEXT:                     if ne<i128b>(read<i128b>(%2), widen<i128b, reason=usual_arith>(const<i45b>(12549857489574)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 break %11;
// DEFAULT-NEXT:                 case %11 const<i125b>(-80694244678005661015504159217733):
// DEFAULT-NEXT:                     if ne<i128b>(read<i128b>(%2), widen<i128b, reason=usual_arith>(const<i20b>(468071)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 break %11;
// DEFAULT-NEXT:                 case %11 const<i125b>(-193984372895748547584754854):
// DEFAULT-NEXT:                     if ne<i128b>(read<i128b>(%2), widen<i128b, reason=usual_arith>(const<i45b>(15549857489574)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 break %11;
// DEFAULT-NEXT:                 default %11:
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %7 p: i125b [storage=automatic];
// DEFAULT-NEXT:         let %8 q: array<i128b, 41> [storage=automatic];
// DEFAULT-NEXT:         write<i128b>(deref(ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(array_decay<ptr<i128b>, length=Some(41)>(%8), const<i32>(4))), widen<i128b, reason=assign>(const<i25b>(15904821)));
// DEFAULT-NEXT:         write<i128b>(deref(ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(array_decay<ptr<i128b>, length=Some(41)>(%8), const<i32>(12))), widen<i128b, reason=assign>(const<i71b>(821095840985901334958)));
// DEFAULT-NEXT:         write<i128b>(deref(ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(array_decay<ptr<i128b>, length=Some(41)>(%8), const<i32>(13))), widen<i128b, reason=assign>(const<i65b>(13895798174897154897)));
// DEFAULT-NEXT:         write<i128b>(deref(ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(array_decay<ptr<i128b>, length=Some(41)>(%8), const<i32>(14))), widen<i128b, reason=assign>(const<i72b>(1465897921835729857454)));
// DEFAULT-NEXT:         write<i128b>(deref(ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(array_decay<ptr<i128b>, length=Some(41)>(%8), const<i32>(15))), widen<i128b, reason=assign>(const<i79b>(154987847598437549873143)));
// DEFAULT-NEXT:         write<i128b>(deref(ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(array_decay<ptr<i128b>, length=Some(41)>(%8), const<i32>(29))), widen<i128b, reason=assign>(const<i69b>(234987458923754897564)));
// DEFAULT-NEXT:         write<i128b>(deref(ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(array_decay<ptr<i128b>, length=Some(41)>(%8), const<i32>(30))), widen<i128b, reason=assign>(neg<i55b, overflow=ub>(const<i55b>(12439857458947213))));
// DEFAULT-NEXT:         write<i128b>(deref(ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(array_decay<ptr<i128b>, length=Some(41)>(%8), const<i32>(31))), widen<i128b, reason=assign>(const<i63b>(4595687436894573685)));
// DEFAULT-NEXT:         write<i128b>(deref(ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(array_decay<ptr<i128b>, length=Some(41)>(%8), const<i32>(32))), widen<i128b, reason=assign>(const<i67b>(54398547589478975845)));
// DEFAULT-NEXT:         write<i128b>(deref(ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(array_decay<ptr<i128b>, length=Some(41)>(%8), const<i32>(33))), widen<i128b, reason=assign>(const<i66b>(34985478957495847545)));
// DEFAULT-NEXT:         write<i128b>(deref(ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(array_decay<ptr<i128b>, length=Some(41)>(%8), const<i32>(34))), widen<i128b, reason=assign>(const<i66b>(34324329847328473343)));
// DEFAULT-NEXT:         write<i128b>(deref(ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(array_decay<ptr<i128b>, length=Some(41)>(%8), const<i32>(35))), widen<i128b, reason=assign>(const<i74b>(5984758947589437584545)));
// DEFAULT-NEXT:         write<i128b>(deref(ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(array_decay<ptr<i128b>, length=Some(41)>(%8), const<i32>(36))), widen<i128b, reason=assign>(const<i72b>(1342984375894754857545)));
// DEFAULT-NEXT:         write<i128b>(deref(ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(array_decay<ptr<i128b>, length=Some(41)>(%8), const<i32>(37))), widen<i128b, reason=assign>(const<i79b>(159847589475894768597656)));
// DEFAULT-NEXT:         write<i128b>(deref(ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(array_decay<ptr<i128b>, length=Some(41)>(%8), const<i32>(39))), widen<i128b, reason=assign>(const<i30b>(394857584)));
// DEFAULT-NEXT:         call<void, signature=fn(i125b, i128b, ptr<i125b>, ptr<i128b>, f32) -> void>(%0, widen<i125b, reason=arg>(neg<i107b, overflow=ub>(const<i107b>(80694244678005661015504159217733))), widen<i128b, reason=arg>(const<i20b>(468071)), addr_of<ptr<i125b>>(%7), array_decay<ptr<i128b>, length=Some(41)>(%8), neg<f32>(const<f32>(42.0)));
// DEFAULT-NEXT:         if ne<i125b>(read<i125b>(%7), widen<i125b, reason=usual_arith>(const<i20b>(468071)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         let %55: ptr<i128b> [synthetic] = ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(array_decay<ptr<i128b>, length=Some(41)>(%8), const<i32>(38));
// DEFAULT-NEXT:         let %56: i128b [synthetic] = read<i128b>(deref(read<ptr<i128b>>(%55)));
// DEFAULT-NEXT:         let %57: i128b [synthetic] = sub<i128b, overflow=ub>(read<i128b>(%56), reinterpret<i128b, reason=usual_arith, fits=unknown>(widen<u128b, reason=usual_arith>(sub<u64, overflow=wrap>(const<u64>(16), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:         write<i128b>(deref(read<ptr<i128b>>(%55)), read<i128b>(%57));
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
// DEFAULT-NEXT:                 if ne<i128b>(read<i128b>(deref(ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(array_decay<ptr<i128b>, length=Some(41)>(%8), read<i32>(%10)))), read<i128b>(deref(ptr_offset<ptr<i128b>, subtract=false, element=i128b, overflow=ub>(array_decay<ptr<i128b>, length=Some(41)>(%9), read<i32>(%10)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
