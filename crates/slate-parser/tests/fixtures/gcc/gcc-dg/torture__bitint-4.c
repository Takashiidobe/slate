/* PR c/102989 */
/* { dg-do run { target bitint } } */
/* { dg-options "-std=c23 -pedantic-errors" } */
/* { dg-skip-if "" { ! run_expensive_tests }  { "*" } { "-O0" "-O2" } } */
/* { dg-skip-if "" { ! run_expensive_tests } { "-flto" } { "" } } */

#if __BITINT_MAXWIDTH__ >= 128
__attribute__((noipa)) void foo(unsigned _BitInt(125) a,
                                unsigned _BitInt(127) b,
                                unsigned _BitInt(125) * p,
                                unsigned _BitInt(127) * q, float c) {
  p[0]   = b;
  q[0]   = a;
  q[1]   = (signed _BitInt(125))a;
  q[2]   = (_BitInt(73))a;
  q[3]   = c;
  q[4]  += a;
  q[5]   = a + b;
  q[6]   = a - b;
  q[7]   = a * b;
  q[8]   = a / b;
  q[9]   = a % b;
  q[10]  = b << (26105549790521884176764218952781428833uwb - a);
  q[11]  = (a * 131uwb) >> (26105549790521884176764218952781428833uwb - a);
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
  q[33] <<= (12589712345465422uwb - b);
  q[34] >>= (12589712345465417uwb - b);
  q[35]  &= a;
  q[36]  |= b;
  q[37]  ^= a;
  q[38]   = sizeof(a);
  q[39]   = q[39] ? a : b;
  q[40]   = 26105549790654675897348954738956342847uwb;
  switch (a) {
  case 26105549790521884176764218952781428772uwb:
    if (b != 1244154958745894754wb)
      __builtin_abort();
    break;
  case 11821400154985748973289734545487uwb:
    if (b != 133445145984759847584574uwb)
      __builtin_abort();
    break;
  case 12145uwb:
    if (b != 12121243985745894732uwb)
      __builtin_abort();
    break;
  case 26105549790521884176764218952781428771uwb:
    if (b != 12589712345465342uwb)
      __builtin_abort();
    break;
  case 71284121548547895457123873874452345uwb:
    if (b != 150123439857459847uwb)
      __builtin_abort();
    break;
  default:
    __builtin_abort();
  }
}

int
main() {
  unsigned _BitInt(125) p;
  unsigned _BitInt(127) q[41];
  static unsigned _BitInt(127)
      qe[41] = {26105549790521884176764218952781428771uwb,
                153711437385873807975529696739694508067uwb,
                2816339038065666848803uwb,
                97uwb,
                26105549790521884178598517527767103757uwb,
                26105549790521884176776808665126894113uwb,
                26105549790521884176751629240435963429uwb,
                49552990805035300718174502183957321146uwb,
                2073562054015060870989uwb,
                8076837748665533uwb,
                58059800399605194176279512422023168uwb,
                3687014528101034113uwb,
                7439587439856743895438uwb,
                95435435436uwb,
                112349587439856746858975446545uwb,
                13145398574895748967847348972322uwb,
                0,
                1,
                1,
                0,
                1,
                0,
                1,
                1,
                0,
                3439290896351266uwb,
                26105549790521884176773369374230542847uwb,
                26105549790521884176769930083334191581uwb,
                16429746074595423756157606976189597660uwb,
                144035633670022787123399980532060335632uwb,
                63332113050644322882197117954233460326uwb,
                27442uwb,
                3245984754897548957498574895745uwb,
                14236875428959659760604435112230125568uwb,
                326775104184uwb,
                1170981383577634uwb,
                2843243404090270511102uwb,
                26313477358277918295486482038303331024uwb,
                1,
                26105549790521884176764218952781428771uwb,
                26105549790654675897348954738956342847uwb};
  q[4]       = 1834298574985674986uwb;
  q[12]      = 7439587439856743895437uwb;
  q[13]      = 95435435435uwb;
  q[14]      = 112349587439856746858975446546uwb;
  q[15]      = 13145398574895748967847348972323uwb;
  q[29]      = 75439568476895768957658675uwb;
  q[30]      = 455984375894754983574895745485uwb;
  q[31]      = 345495847589475847548uwb;
  q[32]      = 3245984754897548957498574895745uwb;
  q[33]      = 32594875648957489754854664897464uwb;
  q[34]      = 12345214395483754897548574857485748uwb;
  q[35]      = 1523143544545454uwb;
  q[36]      = 2843243245456456576876uwb;
  q[37]      = 542359486759867589675986576895765235uwb;
  q[39]      = 5498657685976587653uwb;
  foo(26105549790521884176764218952781428771UWB, 12589712345465342uwb, &p, q,
      97.0f);
  if (p != 12589712345465342uwb)
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
// DEFAULT-NEXT:     global %9 qe: array<u127b, 41> [storage=static] = aggregate<array<u127b, 41>, zero_fill=false>(index0 = widen<u127b, reason=assign>(const<u125b>(26105549790521884176764218952781428771)), index1 = const<u127b>(153711437385873807975529696739694508067), index2 = widen<u127b, reason=assign>(const<u72b>(2816339038065666848803)), index3 = widen<u127b, reason=assign>(const<u7b>(97)), index4 = widen<u127b, reason=assign>(const<u125b>(26105549790521884178598517527767103757)), index5 = widen<u127b, reason=assign>(const<u125b>(26105549790521884176776808665126894113)), index6 = widen<u127b, reason=assign>(const<u125b>(26105549790521884176751629240435963429)), index7 = widen<u127b, reason=assign>(const<u126b>(49552990805035300718174502183957321146)), index8 = widen<u127b, reason=assign>(const<u71b>(2073562054015060870989)), index9 = widen<u127b, reason=assign>(const<u53b>(8076837748665533)), index10 = widen<u127b, reason=assign>(const<u116b>(58059800399605194176279512422023168)), index11 = widen<u127b, reason=assign>(const<u62b>(3687014528101034113)), index12 = widen<u127b, reason=assign>(const<u73b>(7439587439856743895438)), index13 = widen<u127b, reason=assign>(const<u37b>(95435435436)), index14 = widen<u127b, reason=assign>(const<u97b>(112349587439856746858975446545)), index15 = widen<u127b, reason=assign>(const<u104b>(13145398574895748967847348972322)), index16 = reinterpret<u127b, reason=assign, fits=unknown>(widen<i127b, reason=assign>(const<i32>(0))), index17 = reinterpret<u127b, reason=assign, fits=unknown>(widen<i127b, reason=assign>(const<i32>(1))), index18 = reinterpret<u127b, reason=assign, fits=unknown>(widen<i127b, reason=assign>(const<i32>(1))), index19 = reinterpret<u127b, reason=assign, fits=unknown>(widen<i127b, reason=assign>(const<i32>(0))), index20 = reinterpret<u127b, reason=assign, fits=unknown>(widen<i127b, reason=assign>(const<i32>(1))), index21 = reinterpret<u127b, reason=assign, fits=unknown>(widen<i127b, reason=assign>(const<i32>(0))), index22 = reinterpret<u127b, reason=assign, fits=unknown>(widen<i127b, reason=assign>(const<i32>(1))), index23 = reinterpret<u127b, reason=assign, fits=unknown>(widen<i127b, reason=assign>(const<i32>(1))), index24 = reinterpret<u127b, reason=assign, fits=unknown>(widen<i127b, reason=assign>(const<i32>(0))), index25 = widen<u127b, reason=assign>(const<u52b>(3439290896351266)), index26 = widen<u127b, reason=assign>(const<u125b>(26105549790521884176773369374230542847)), index27 = widen<u127b, reason=assign>(const<u125b>(26105549790521884176769930083334191581)), index28 = widen<u127b, reason=assign>(const<u124b>(16429746074595423756157606976189597660)), index29 = const<u127b>(144035633670022787123399980532060335632), index30 = widen<u127b, reason=assign>(const<u126b>(63332113050644322882197117954233460326)), index31 = widen<u127b, reason=assign>(const<u15b>(27442)), index32 = widen<u127b, reason=assign>(const<u102b>(3245984754897548957498574895745)), index33 = widen<u127b, reason=assign>(const<u124b>(14236875428959659760604435112230125568)), index34 = widen<u127b, reason=assign>(const<u39b>(326775104184)), index35 = widen<u127b, reason=assign>(const<u51b>(1170981383577634)), index36 = widen<u127b, reason=assign>(const<u72b>(2843243404090270511102)), index37 = widen<u127b, reason=assign>(const<u125b>(26313477358277918295486482038303331024)), index38 = reinterpret<u127b, reason=assign, fits=unknown>(widen<i127b, reason=assign>(const<i32>(1))), index39 = widen<u127b, reason=assign>(const<u125b>(26105549790521884176764218952781428771)), index40 = widen<u127b, reason=assign>(const<u125b>(26105549790654675897348954738956342847))) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @foo(%1 a: u125b, %2 b: u127b, %3 p: ptr<u125b>, %4 q: ptr<u127b>, %5 c: f32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<u125b>(deref(ptr_offset<ptr<u125b>, subtract=false, element=u125b, overflow=ub>(read<ptr<u125b>>(%3), const<i32>(0))), truncate<u125b, reason=assign, fits=unknown>(read<u127b>(%2)));
// DEFAULT-NEXT:         write<u127b>(deref(ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(read<ptr<u127b>>(%4), const<i32>(0))), widen<u127b, reason=assign>(read<u125b>(%1)));
// DEFAULT-NEXT:         write<u127b>(deref(ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(read<ptr<u127b>>(%4), const<i32>(1))), reinterpret<u127b, reason=assign, fits=unknown>(widen<i127b, reason=assign>(reinterpret<i125b, reason=explicit, fits=unknown>(read<u125b>(%1)))));
// DEFAULT-NEXT:         write<u127b>(deref(ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(read<ptr<u127b>>(%4), const<i32>(2))), reinterpret<u127b, reason=assign, fits=unknown>(widen<i127b, reason=assign>(reinterpret<i73b, reason=explicit, fits=unknown>(truncate<u73b, reason=explicit, fits=unknown>(read<u125b>(%1))))));
// DEFAULT-NEXT:         write<u127b>(deref(ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(read<ptr<u127b>>(%4), const<i32>(3))), float_to_int<u127b, reason=assign, out_of_range=ub, exceptions=ignore>(read<f32>(%5)));
// DEFAULT-NEXT:         let %13: ptr<u127b> [synthetic] = ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(read<ptr<u127b>>(%4), const<i32>(4));
// DEFAULT-NEXT:         let %14: u127b [synthetic] = read<u127b>(deref(read<ptr<u127b>>(%13)));
// DEFAULT-NEXT:         let %15: u127b [synthetic] = add<u127b, overflow=wrap>(read<u127b>(%14), widen<u127b, reason=usual_arith>(read<u125b>(%1)));
// DEFAULT-NEXT:         write<u127b>(deref(read<ptr<u127b>>(%13)), read<u127b>(%15));
// DEFAULT-NEXT:         write<u127b>(deref(ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(read<ptr<u127b>>(%4), const<i32>(5))), add<u127b, overflow=wrap>(widen<u127b, reason=usual_arith>(read<u125b>(%1)), read<u127b>(%2)));
// DEFAULT-NEXT:         write<u127b>(deref(ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(read<ptr<u127b>>(%4), const<i32>(6))), sub<u127b, overflow=wrap>(widen<u127b, reason=usual_arith>(read<u125b>(%1)), read<u127b>(%2)));
// DEFAULT-NEXT:         write<u127b>(deref(ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(read<ptr<u127b>>(%4), const<i32>(7))), mul<u127b, overflow=wrap>(widen<u127b, reason=usual_arith>(read<u125b>(%1)), read<u127b>(%2)));
// DEFAULT-NEXT:         write<u127b>(deref(ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(read<ptr<u127b>>(%4), const<i32>(8))), div<u127b, by_zero=ub>(widen<u127b, reason=usual_arith>(read<u125b>(%1)), read<u127b>(%2)));
// DEFAULT-NEXT:         write<u127b>(deref(ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(read<ptr<u127b>>(%4), const<i32>(9))), rem<u127b, by_zero=ub>(widen<u127b, reason=usual_arith>(read<u125b>(%1)), read<u127b>(%2)));
// DEFAULT-NEXT:         write<u127b>(deref(ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(read<ptr<u127b>>(%4), const<i32>(10))), shl<u127b, overflow=wrap, amount_out_of_range=ub>(read<u127b>(%2), sub<u125b, overflow=wrap>(const<u125b>(26105549790521884176764218952781428833), read<u125b>(%1))));
// DEFAULT-NEXT:         write<u127b>(deref(ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(read<ptr<u127b>>(%4), const<i32>(11))), widen<u127b, reason=assign>(shr<u125b, amount_out_of_range=ub, fill=zero_extend>(mul<u125b, overflow=wrap>(read<u125b>(%1), widen<u125b, reason=usual_arith>(const<u8b>(131))), sub<u125b, overflow=wrap>(const<u125b>(26105549790521884176764218952781428833), read<u125b>(%1)))));
// DEFAULT-NEXT:         let %16: ptr<u127b> [synthetic] = ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(read<ptr<u127b>>(%4), const<i32>(12));
// DEFAULT-NEXT:         let %17: u127b [synthetic] = read<u127b>(deref(read<ptr<u127b>>(%16)));
// DEFAULT-NEXT:         let %18: u127b [synthetic] = add<u127b, overflow=wrap>(read<u127b>(%17), reinterpret<u127b, reason=usual_arith, fits=unknown>(widen<i127b, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         write<u127b>(deref(read<ptr<u127b>>(%16)), read<u127b>(%18));
// DEFAULT-NEXT:         let %19: ptr<u127b> [synthetic] = ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(read<ptr<u127b>>(%4), const<i32>(13));
// DEFAULT-NEXT:         let %20: u127b [synthetic] = read<u127b>(deref(read<ptr<u127b>>(%19)));
// DEFAULT-NEXT:         let %21: u127b [synthetic] = add<u127b, overflow=wrap>(read<u127b>(%20), reinterpret<u127b, reason=usual_arith, fits=unknown>(widen<i127b, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         write<u127b>(deref(read<ptr<u127b>>(%19)), read<u127b>(%21));
// DEFAULT-NEXT:         let %22: ptr<u127b> [synthetic] = ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(read<ptr<u127b>>(%4), const<i32>(14));
// DEFAULT-NEXT:         let %23: u127b [synthetic] = read<u127b>(deref(read<ptr<u127b>>(%22)));
// DEFAULT-NEXT:         let %24: u127b [synthetic] = sub<u127b, overflow=wrap>(read<u127b>(%23), reinterpret<u127b, reason=usual_arith, fits=unknown>(widen<i127b, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         write<u127b>(deref(read<ptr<u127b>>(%22)), read<u127b>(%24));
// DEFAULT-NEXT:         let %25: ptr<u127b> [synthetic] = ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(read<ptr<u127b>>(%4), const<i32>(15));
// DEFAULT-NEXT:         let %26: u127b [synthetic] = read<u127b>(deref(read<ptr<u127b>>(%25)));
// DEFAULT-NEXT:         let %27: u127b [synthetic] = sub<u127b, overflow=wrap>(read<u127b>(%26), reinterpret<u127b, reason=usual_arith, fits=unknown>(widen<i127b, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         write<u127b>(deref(read<ptr<u127b>>(%25)), read<u127b>(%27));
// DEFAULT-NEXT:         write<u127b>(deref(ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(read<ptr<u127b>>(%4), const<i32>(16))), from_bool<u127b, reason=assign>(eq<u127b>(widen<u127b, reason=usual_arith>(read<u125b>(%1)), read<u127b>(%2))));
// DEFAULT-NEXT:         write<u127b>(deref(ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(read<ptr<u127b>>(%4), const<i32>(17))), from_bool<u127b, reason=assign>(ne<u127b>(widen<u127b, reason=usual_arith>(read<u125b>(%1)), read<u127b>(%2))));
// DEFAULT-NEXT:         write<u127b>(deref(ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(read<ptr<u127b>>(%4), const<i32>(18))), from_bool<u127b, reason=assign>(gt<u127b>(widen<u127b, reason=usual_arith>(read<u125b>(%1)), read<u127b>(%2))));
// DEFAULT-NEXT:         write<u127b>(deref(ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(read<ptr<u127b>>(%4), const<i32>(19))), from_bool<u127b, reason=assign>(lt<u127b>(widen<u127b, reason=usual_arith>(read<u125b>(%1)), read<u127b>(%2))));
// DEFAULT-NEXT:         write<u127b>(deref(ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(read<ptr<u127b>>(%4), const<i32>(20))), from_bool<u127b, reason=assign>(ge<u127b>(widen<u127b, reason=usual_arith>(read<u125b>(%1)), read<u127b>(%2))));
// DEFAULT-NEXT:         write<u127b>(deref(ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(read<ptr<u127b>>(%4), const<i32>(21))), from_bool<u127b, reason=assign>(le<u127b>(widen<u127b, reason=usual_arith>(read<u125b>(%1)), read<u127b>(%2))));
// DEFAULT-NEXT:         write<u127b>(deref(ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(read<ptr<u127b>>(%4), const<i32>(22))), from_bool<u127b, reason=assign>(logical_and<bool>(ne<u125b>(read<u125b>(%1), const<u125b>(0)), ne<u127b>(read<u127b>(%2), const<u127b>(0)))));
// DEFAULT-NEXT:         write<u127b>(deref(ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(read<ptr<u127b>>(%4), const<i32>(23))), from_bool<u127b, reason=assign>(logical_or<bool>(ne<u125b>(read<u125b>(%1), const<u125b>(0)), ne<u127b>(read<u127b>(%2), const<u127b>(0)))));
// DEFAULT-NEXT:         write<u127b>(deref(ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(read<ptr<u127b>>(%4), const<i32>(24))), from_bool<u127b, reason=assign>(not<bool>(ne<u125b>(read<u125b>(%1), const<u125b>(0)))));
// DEFAULT-NEXT:         write<u127b>(deref(ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(read<ptr<u127b>>(%4), const<i32>(25))), and<u127b>(widen<u127b, reason=usual_arith>(read<u125b>(%1)), read<u127b>(%2)));
// DEFAULT-NEXT:         write<u127b>(deref(ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(read<ptr<u127b>>(%4), const<i32>(26))), or<u127b>(widen<u127b, reason=usual_arith>(read<u125b>(%1)), read<u127b>(%2)));
// DEFAULT-NEXT:         write<u127b>(deref(ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(read<ptr<u127b>>(%4), const<i32>(27))), xor<u127b>(widen<u127b, reason=usual_arith>(read<u125b>(%1)), read<u127b>(%2)));
// DEFAULT-NEXT:         write<u127b>(deref(ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(read<ptr<u127b>>(%4), const<i32>(28))), widen<u127b, reason=assign>(not<u125b>(read<u125b>(%1))));
// DEFAULT-NEXT:         let %28: ptr<u127b> [synthetic] = ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(read<ptr<u127b>>(%4), const<i32>(29));
// DEFAULT-NEXT:         let %29: u127b [synthetic] = read<u127b>(deref(read<ptr<u127b>>(%28)));
// DEFAULT-NEXT:         let %30: u127b [synthetic] = sub<u127b, overflow=wrap>(read<u127b>(%29), widen<u127b, reason=usual_arith>(read<u125b>(%1)));
// DEFAULT-NEXT:         write<u127b>(deref(read<ptr<u127b>>(%28)), read<u127b>(%30));
// DEFAULT-NEXT:         let %31: ptr<u127b> [synthetic] = ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(read<ptr<u127b>>(%4), const<i32>(30));
// DEFAULT-NEXT:         let %32: u127b [synthetic] = read<u127b>(deref(read<ptr<u127b>>(%31)));
// DEFAULT-NEXT:         let %33: u127b [synthetic] = mul<u127b, overflow=wrap>(read<u127b>(%32), read<u127b>(%2));
// DEFAULT-NEXT:         write<u127b>(deref(read<ptr<u127b>>(%31)), read<u127b>(%33));
// DEFAULT-NEXT:         let %34: ptr<u127b> [synthetic] = ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(read<ptr<u127b>>(%4), const<i32>(31));
// DEFAULT-NEXT:         let %35: u127b [synthetic] = read<u127b>(deref(read<ptr<u127b>>(%34)));
// DEFAULT-NEXT:         let %36: u127b [synthetic] = div<u127b, by_zero=ub>(read<u127b>(%35), read<u127b>(%2));
// DEFAULT-NEXT:         write<u127b>(deref(read<ptr<u127b>>(%34)), read<u127b>(%36));
// DEFAULT-NEXT:         let %37: ptr<u127b> [synthetic] = ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(read<ptr<u127b>>(%4), const<i32>(32));
// DEFAULT-NEXT:         let %38: u127b [synthetic] = read<u127b>(deref(read<ptr<u127b>>(%37)));
// DEFAULT-NEXT:         let %39: u127b [synthetic] = rem<u127b, by_zero=ub>(read<u127b>(%38), widen<u127b, reason=usual_arith>(read<u125b>(%1)));
// DEFAULT-NEXT:         write<u127b>(deref(read<ptr<u127b>>(%37)), read<u127b>(%39));
// DEFAULT-NEXT:         let %40: ptr<u127b> [synthetic] = ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(read<ptr<u127b>>(%4), const<i32>(33));
// DEFAULT-NEXT:         let %41: u127b [synthetic] = read<u127b>(deref(read<ptr<u127b>>(%40)));
// DEFAULT-NEXT:         let %42: u127b [synthetic] = shl<u127b, overflow=wrap, amount_out_of_range=ub>(read<u127b>(%41), sub<u127b, overflow=wrap>(widen<u127b, reason=usual_arith>(const<u54b>(12589712345465422)), read<u127b>(%2)));
// DEFAULT-NEXT:         write<u127b>(deref(read<ptr<u127b>>(%40)), read<u127b>(%42));
// DEFAULT-NEXT:         let %43: ptr<u127b> [synthetic] = ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(read<ptr<u127b>>(%4), const<i32>(34));
// DEFAULT-NEXT:         let %44: u127b [synthetic] = read<u127b>(deref(read<ptr<u127b>>(%43)));
// DEFAULT-NEXT:         let %45: u127b [synthetic] = shr<u127b, amount_out_of_range=ub, fill=zero_extend>(read<u127b>(%44), sub<u127b, overflow=wrap>(widen<u127b, reason=usual_arith>(const<u54b>(12589712345465417)), read<u127b>(%2)));
// DEFAULT-NEXT:         write<u127b>(deref(read<ptr<u127b>>(%43)), read<u127b>(%45));
// DEFAULT-NEXT:         let %46: ptr<u127b> [synthetic] = ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(read<ptr<u127b>>(%4), const<i32>(35));
// DEFAULT-NEXT:         let %47: u127b [synthetic] = read<u127b>(deref(read<ptr<u127b>>(%46)));
// DEFAULT-NEXT:         let %48: u127b [synthetic] = and<u127b>(read<u127b>(%47), widen<u127b, reason=usual_arith>(read<u125b>(%1)));
// DEFAULT-NEXT:         write<u127b>(deref(read<ptr<u127b>>(%46)), read<u127b>(%48));
// DEFAULT-NEXT:         let %49: ptr<u127b> [synthetic] = ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(read<ptr<u127b>>(%4), const<i32>(36));
// DEFAULT-NEXT:         let %50: u127b [synthetic] = read<u127b>(deref(read<ptr<u127b>>(%49)));
// DEFAULT-NEXT:         let %51: u127b [synthetic] = or<u127b>(read<u127b>(%50), read<u127b>(%2));
// DEFAULT-NEXT:         write<u127b>(deref(read<ptr<u127b>>(%49)), read<u127b>(%51));
// DEFAULT-NEXT:         let %52: ptr<u127b> [synthetic] = ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(read<ptr<u127b>>(%4), const<i32>(37));
// DEFAULT-NEXT:         let %53: u127b [synthetic] = read<u127b>(deref(read<ptr<u127b>>(%52)));
// DEFAULT-NEXT:         let %54: u127b [synthetic] = xor<u127b>(read<u127b>(%53), widen<u127b, reason=usual_arith>(read<u125b>(%1)));
// DEFAULT-NEXT:         write<u127b>(deref(read<ptr<u127b>>(%52)), read<u127b>(%54));
// DEFAULT-NEXT:         write<u127b>(deref(ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(read<ptr<u127b>>(%4), const<i32>(38))), widen<u127b, reason=assign>(const<u64>(16)));
// DEFAULT-NEXT:         write<u127b>(deref(ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(read<ptr<u127b>>(%4), const<i32>(39))), conditional<u127b>(ne<u127b>(read<u127b>(deref(ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(read<ptr<u127b>>(%4), const<i32>(39)))), const<u127b>(0)), widen<u127b, reason=usual_arith>(read<u125b>(%1)), read<u127b>(%2)));
// DEFAULT-NEXT:         write<u127b>(deref(ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(read<ptr<u127b>>(%4), const<i32>(40))), widen<u127b, reason=assign>(const<u125b>(26105549790654675897348954738956342847)));
// DEFAULT-NEXT:         switch %11 read<u125b>(%1)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %11 const<u125b>(26105549790521884176764218952781428772):
// DEFAULT-NEXT:                     if ne<u127b>(read<u127b>(%2), reinterpret<u127b, reason=usual_arith, fits=unknown>(widen<i127b, reason=usual_arith>(const<i62b>(1244154958745894754))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 break %11;
// DEFAULT-NEXT:                 case %11 const<u125b>(11821400154985748973289734545487):
// DEFAULT-NEXT:                     if ne<u127b>(read<u127b>(%2), widen<u127b, reason=usual_arith>(const<u77b>(133445145984759847584574)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 break %11;
// DEFAULT-NEXT:                 case %11 const<u125b>(12145):
// DEFAULT-NEXT:                     if ne<u127b>(read<u127b>(%2), widen<u127b, reason=usual_arith>(const<u64b>(12121243985745894732)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 break %11;
// DEFAULT-NEXT:                 case %11 const<u125b>(26105549790521884176764218952781428771):
// DEFAULT-NEXT:                     if ne<u127b>(read<u127b>(%2), widen<u127b, reason=usual_arith>(const<u54b>(12589712345465342)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 break %11;
// DEFAULT-NEXT:                 case %11 const<u125b>(71284121548547895457123873874452345):
// DEFAULT-NEXT:                     if ne<u127b>(read<u127b>(%2), widen<u127b, reason=usual_arith>(const<u58b>(150123439857459847)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 break %11;
// DEFAULT-NEXT:                 default %11:
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %7 p: u125b [storage=automatic];
// DEFAULT-NEXT:         let %8 q: array<u127b, 41> [storage=automatic];
// DEFAULT-NEXT:         write<u127b>(deref(ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(array_decay<ptr<u127b>, length=Some(41)>(%8), const<i32>(4))), widen<u127b, reason=assign>(const<u61b>(1834298574985674986)));
// DEFAULT-NEXT:         write<u127b>(deref(ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(array_decay<ptr<u127b>, length=Some(41)>(%8), const<i32>(12))), widen<u127b, reason=assign>(const<u73b>(7439587439856743895437)));
// DEFAULT-NEXT:         write<u127b>(deref(ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(array_decay<ptr<u127b>, length=Some(41)>(%8), const<i32>(13))), widen<u127b, reason=assign>(const<u37b>(95435435435)));
// DEFAULT-NEXT:         write<u127b>(deref(ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(array_decay<ptr<u127b>, length=Some(41)>(%8), const<i32>(14))), widen<u127b, reason=assign>(const<u97b>(112349587439856746858975446546)));
// DEFAULT-NEXT:         write<u127b>(deref(ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(array_decay<ptr<u127b>, length=Some(41)>(%8), const<i32>(15))), widen<u127b, reason=assign>(const<u104b>(13145398574895748967847348972323)));
// DEFAULT-NEXT:         write<u127b>(deref(ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(array_decay<ptr<u127b>, length=Some(41)>(%8), const<i32>(29))), widen<u127b, reason=assign>(const<u86b>(75439568476895768957658675)));
// DEFAULT-NEXT:         write<u127b>(deref(ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(array_decay<ptr<u127b>, length=Some(41)>(%8), const<i32>(30))), widen<u127b, reason=assign>(const<u99b>(455984375894754983574895745485)));
// DEFAULT-NEXT:         write<u127b>(deref(ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(array_decay<ptr<u127b>, length=Some(41)>(%8), const<i32>(31))), widen<u127b, reason=assign>(const<u69b>(345495847589475847548)));
// DEFAULT-NEXT:         write<u127b>(deref(ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(array_decay<ptr<u127b>, length=Some(41)>(%8), const<i32>(32))), widen<u127b, reason=assign>(const<u102b>(3245984754897548957498574895745)));
// DEFAULT-NEXT:         write<u127b>(deref(ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(array_decay<ptr<u127b>, length=Some(41)>(%8), const<i32>(33))), widen<u127b, reason=assign>(const<u105b>(32594875648957489754854664897464)));
// DEFAULT-NEXT:         write<u127b>(deref(ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(array_decay<ptr<u127b>, length=Some(41)>(%8), const<i32>(34))), widen<u127b, reason=assign>(const<u114b>(12345214395483754897548574857485748)));
// DEFAULT-NEXT:         write<u127b>(deref(ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(array_decay<ptr<u127b>, length=Some(41)>(%8), const<i32>(35))), widen<u127b, reason=assign>(const<u51b>(1523143544545454)));
// DEFAULT-NEXT:         write<u127b>(deref(ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(array_decay<ptr<u127b>, length=Some(41)>(%8), const<i32>(36))), widen<u127b, reason=assign>(const<u72b>(2843243245456456576876)));
// DEFAULT-NEXT:         write<u127b>(deref(ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(array_decay<ptr<u127b>, length=Some(41)>(%8), const<i32>(37))), widen<u127b, reason=assign>(const<u119b>(542359486759867589675986576895765235)));
// DEFAULT-NEXT:         write<u127b>(deref(ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(array_decay<ptr<u127b>, length=Some(41)>(%8), const<i32>(39))), widen<u127b, reason=assign>(const<u63b>(5498657685976587653)));
// DEFAULT-NEXT:         call<void, signature=fn(u125b, u127b, ptr<u125b>, ptr<u127b>, f32) -> void>(%0, const<u125b>(26105549790521884176764218952781428771), widen<u127b, reason=arg>(const<u54b>(12589712345465342)), addr_of<ptr<u125b>>(%7), array_decay<ptr<u127b>, length=Some(41)>(%8), const<f32>(97.0));
// DEFAULT-NEXT:         if ne<u125b>(read<u125b>(%7), widen<u125b, reason=usual_arith>(const<u54b>(12589712345465342)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         let %55: ptr<u127b> [synthetic] = ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(array_decay<ptr<u127b>, length=Some(41)>(%8), const<i32>(38));
// DEFAULT-NEXT:         let %56: u127b [synthetic] = read<u127b>(deref(read<ptr<u127b>>(%55)));
// DEFAULT-NEXT:         let %57: u127b [synthetic] = sub<u127b, overflow=wrap>(read<u127b>(%56), widen<u127b, reason=usual_arith>(sub<u64, overflow=wrap>(const<u64>(16), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))));
// DEFAULT-NEXT:         write<u127b>(deref(read<ptr<u127b>>(%55)), read<u127b>(%57));
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
// DEFAULT-NEXT:                 if ne<u127b>(read<u127b>(deref(ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(array_decay<ptr<u127b>, length=Some(41)>(%8), read<i32>(%10)))), read<u127b>(deref(ptr_offset<ptr<u127b>, subtract=false, element=u127b, overflow=ub>(array_decay<ptr<u127b>, length=Some(41)>(%9), read<i32>(%10)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
