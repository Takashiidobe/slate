/* { dg-require-effective-target longlong64 } */

long long          a = 568513516876543756;
long long          b = -754324895235774564;
unsigned long long c = 156789543257562457;

long long          expected_a[64] = {568513516876543756,
                                     1137027033753087512,
                                     2274054067506175024,
                                     4548108135012350048,
                                     9096216270024700096,
                                     -254311533660151424,
                                     -508623067320302848,
                                     -1017246134640605696,
                                     -2034492269281211392,
                                     -4068984538562422784,
                                     -8137969077124845568,
                                     2170805919459860480,
                                     4341611838919720960,
                                     8683223677839441920,
                                     -1080296718030667776,
                                     -2160593436061335552,
                                     -4321186872122671104,
                                     -8642373744245342208,
                                     1161996585218867200,
                                     2323993170437734400,
                                     4647986340875468800,
                                     -9150771391958614016,
                                     145201289792323584,
                                     290402579584647168,
                                     580805159169294336,
                                     1161610318338588672,
                                     2323220636677177344,
                                     4646441273354354688,
                                     -9153861527000842240,
                                     139021019707867136,
                                     278042039415734272,
                                     556084078831468544,
                                     1112168157662937088,
                                     2224336315325874176,
                                     4448672630651748352,
                                     8897345261303496704,
                                     -652053551102558208,
                                     -1304107102205116416,
                                     -2608214204410232832,
                                     -5216428408820465664,
                                     8013887256068620288,
                                     -2418969561572311040,
                                     -4837939123144622080,
                                     8770865827420307456,
                                     -905012418868936704,
                                     -1810024837737873408,
                                     -3620049675475746816,
                                     -7240099350951493632,
                                     3966545371806564352,
                                     7933090743613128704,
                                     -2580562586483294208,
                                     -5161125172966588416,
                                     8124493727776374784,
                                     -2197756618156802048,
                                     -4395513236313604096,
                                     -8791026472627208192,
                                     864691128455135232,
                                     1729382256910270464,
                                     3458764513820540928,
                                     6917529027641081856,
                                     -4611686018427387904,
                                     -9223372036854775808ULL,
                                     0,
                                     0};
long long          expected_b[64] = {-754324895235774564,
                                     -377162447617887282,
                                     -188581223808943641,
                                     -94290611904471821,
                                     -47145305952235911,
                                     -23572652976117956,
                                     -11786326488058978,
                                     -5893163244029489,
                                     -2946581622014745,
                                     -1473290811007373,
                                     -736645405503687,
                                     -368322702751844,
                                     -184161351375922,
                                     -92080675687961,
                                     -46040337843981,
                                     -23020168921991,
                                     -11510084460996,
                                     -5755042230498,
                                     -2877521115249,
                                     -1438760557625,
                                     -719380278813,
                                     -359690139407,
                                     -179845069704,
                                     -89922534852,
                                     -44961267426,
                                     -22480633713,
                                     -11240316857,
                                     -5620158429,
                                     -2810079215,
                                     -1405039608,
                                     -702519804,
                                     -351259902,
                                     -175629951,
                                     -87814976,
                                     -43907488,
                                     -21953744,
                                     -10976872,
                                     -5488436,
                                     -2744218,
                                     -1372109,
                                     -686055,
                                     -343028,
                                     -171514,
                                     -85757,
                                     -42879,
                                     -21440,
                                     -10720,
                                     -5360,
                                     -2680,
                                     -1340,
                                     -670,
                                     -335,
                                     -168,
                                     -84,
                                     -42,
                                     -21,
                                     -11,
                                     -6,
                                     -3,
                                     -2,
                                     -1,
                                     -1,
                                     -1,
                                     -1};
unsigned long long expected_c[64] = {156789543257562457,
                                     78394771628781228,
                                     39197385814390614,
                                     19598692907195307,
                                     9799346453597653,
                                     4899673226798826,
                                     2449836613399413,
                                     1224918306699706,
                                     612459153349853,
                                     306229576674926,
                                     153114788337463,
                                     76557394168731,
                                     38278697084365,
                                     19139348542182,
                                     9569674271091,
                                     4784837135545,
                                     2392418567772,
                                     1196209283886,
                                     598104641943,
                                     299052320971,
                                     149526160485,
                                     74763080242,
                                     37381540121,
                                     18690770060,
                                     9345385030,
                                     4672692515,
                                     2336346257,
                                     1168173128,
                                     584086564,
                                     292043282,
                                     146021641,
                                     73010820,
                                     36505410,
                                     18252705,
                                     9126352,
                                     4563176,
                                     2281588,
                                     1140794,
                                     570397,
                                     285198,
                                     142599,
                                     71299,
                                     35649,
                                     17824,
                                     8912,
                                     4456,
                                     2228,
                                     1114,
                                     557,
                                     278,
                                     139,
                                     69,
                                     34,
                                     17,
                                     8,
                                     4,
                                     2,
                                     1,
                                     0,
                                     0,
                                     0,
                                     0,
                                     0,
                                     0};

int main(void) {
  int i;

  for (i = 0; i < 64; i++) {
    if ((a << i) != expected_a[i] || (b >> i) != expected_b[i] ||
        (c >> i) != expected_c[i])
      __builtin_abort();
  }
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
// DEFAULT-NEXT:     global %0 a: i64 [storage=static] = const<i64>(568513516876543756) [linkage=external];
// DEFAULT-NEXT:     global %1 b: i64 [storage=static] = neg<i64, overflow=ub>(const<i64>(754324895235774564)) [linkage=external];
// DEFAULT-NEXT:     global %2 c: u64 [storage=static] = reinterpret<u64, reason=assign, fits=always>(const<i64>(156789543257562457)) [linkage=external];
// DEFAULT-NEXT:     global %3 expected_a: array<i64, 64> [storage=static] = aggregate<array<i64, 64>, zero_fill=false>(index0 = const<i64>(568513516876543756), index1 = const<i64>(1137027033753087512), index2 = const<i64>(2274054067506175024), index3 = const<i64>(4548108135012350048), index4 = const<i64>(9096216270024700096), index5 = neg<i64, overflow=ub>(const<i64>(254311533660151424)), index6 = neg<i64, overflow=ub>(const<i64>(508623067320302848)), index7 = neg<i64, overflow=ub>(const<i64>(1017246134640605696)), index8 = neg<i64, overflow=ub>(const<i64>(2034492269281211392)), index9 = neg<i64, overflow=ub>(const<i64>(4068984538562422784)), index10 = neg<i64, overflow=ub>(const<i64>(8137969077124845568)), index11 = const<i64>(2170805919459860480), index12 = const<i64>(4341611838919720960), index13 = const<i64>(8683223677839441920), index14 = neg<i64, overflow=ub>(const<i64>(1080296718030667776)), index15 = neg<i64, overflow=ub>(const<i64>(2160593436061335552)), index16 = neg<i64, overflow=ub>(const<i64>(4321186872122671104)), index17 = neg<i64, overflow=ub>(const<i64>(8642373744245342208)), index18 = const<i64>(1161996585218867200), index19 = const<i64>(2323993170437734400), index20 = const<i64>(4647986340875468800), index21 = neg<i64, overflow=ub>(const<i64>(9150771391958614016)), index22 = const<i64>(145201289792323584), index23 = const<i64>(290402579584647168), index24 = const<i64>(580805159169294336), index25 = const<i64>(1161610318338588672), index26 = const<i64>(2323220636677177344), index27 = const<i64>(4646441273354354688), index28 = neg<i64, overflow=ub>(const<i64>(9153861527000842240)), index29 = const<i64>(139021019707867136), index30 = const<i64>(278042039415734272), index31 = const<i64>(556084078831468544), index32 = const<i64>(1112168157662937088), index33 = const<i64>(2224336315325874176), index34 = const<i64>(4448672630651748352), index35 = const<i64>(8897345261303496704), index36 = neg<i64, overflow=ub>(const<i64>(652053551102558208)), index37 = neg<i64, overflow=ub>(const<i64>(1304107102205116416)), index38 = neg<i64, overflow=ub>(const<i64>(2608214204410232832)), index39 = neg<i64, overflow=ub>(const<i64>(5216428408820465664)), index40 = const<i64>(8013887256068620288), index41 = neg<i64, overflow=ub>(const<i64>(2418969561572311040)), index42 = neg<i64, overflow=ub>(const<i64>(4837939123144622080)), index43 = const<i64>(8770865827420307456), index44 = neg<i64, overflow=ub>(const<i64>(905012418868936704)), index45 = neg<i64, overflow=ub>(const<i64>(1810024837737873408)), index46 = neg<i64, overflow=ub>(const<i64>(3620049675475746816)), index47 = neg<i64, overflow=ub>(const<i64>(7240099350951493632)), index48 = const<i64>(3966545371806564352), index49 = const<i64>(7933090743613128704), index50 = neg<i64, overflow=ub>(const<i64>(2580562586483294208)), index51 = neg<i64, overflow=ub>(const<i64>(5161125172966588416)), index52 = const<i64>(8124493727776374784), index53 = neg<i64, overflow=ub>(const<i64>(2197756618156802048)), index54 = neg<i64, overflow=ub>(const<i64>(4395513236313604096)), index55 = neg<i64, overflow=ub>(const<i64>(8791026472627208192)), index56 = const<i64>(864691128455135232), index57 = const<i64>(1729382256910270464), index58 = const<i64>(3458764513820540928), index59 = const<i64>(6917529027641081856), index60 = neg<i64, overflow=ub>(const<i64>(4611686018427387904)), index61 = reinterpret<i64, reason=assign, fits=unknown>(neg<u64, overflow=wrap>(const<u64>(9223372036854775808))), index62 = widen<i64, reason=assign>(const<i32>(0)), index63 = widen<i64, reason=assign>(const<i32>(0))) [linkage=external];
// DEFAULT-NEXT:     global %4 expected_b: array<i64, 64> [storage=static] = aggregate<array<i64, 64>, zero_fill=false>(index0 = neg<i64, overflow=ub>(const<i64>(754324895235774564)), index1 = neg<i64, overflow=ub>(const<i64>(377162447617887282)), index2 = neg<i64, overflow=ub>(const<i64>(188581223808943641)), index3 = neg<i64, overflow=ub>(const<i64>(94290611904471821)), index4 = neg<i64, overflow=ub>(const<i64>(47145305952235911)), index5 = neg<i64, overflow=ub>(const<i64>(23572652976117956)), index6 = neg<i64, overflow=ub>(const<i64>(11786326488058978)), index7 = neg<i64, overflow=ub>(const<i64>(5893163244029489)), index8 = neg<i64, overflow=ub>(const<i64>(2946581622014745)), index9 = neg<i64, overflow=ub>(const<i64>(1473290811007373)), index10 = neg<i64, overflow=ub>(const<i64>(736645405503687)), index11 = neg<i64, overflow=ub>(const<i64>(368322702751844)), index12 = neg<i64, overflow=ub>(const<i64>(184161351375922)), index13 = neg<i64, overflow=ub>(const<i64>(92080675687961)), index14 = neg<i64, overflow=ub>(const<i64>(46040337843981)), index15 = neg<i64, overflow=ub>(const<i64>(23020168921991)), index16 = neg<i64, overflow=ub>(const<i64>(11510084460996)), index17 = neg<i64, overflow=ub>(const<i64>(5755042230498)), index18 = neg<i64, overflow=ub>(const<i64>(2877521115249)), index19 = neg<i64, overflow=ub>(const<i64>(1438760557625)), index20 = neg<i64, overflow=ub>(const<i64>(719380278813)), index21 = neg<i64, overflow=ub>(const<i64>(359690139407)), index22 = neg<i64, overflow=ub>(const<i64>(179845069704)), index23 = neg<i64, overflow=ub>(const<i64>(89922534852)), index24 = neg<i64, overflow=ub>(const<i64>(44961267426)), index25 = neg<i64, overflow=ub>(const<i64>(22480633713)), index26 = neg<i64, overflow=ub>(const<i64>(11240316857)), index27 = neg<i64, overflow=ub>(const<i64>(5620158429)), index28 = neg<i64, overflow=ub>(const<i64>(2810079215)), index29 = widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(1405039608))), index30 = widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(702519804))), index31 = widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(351259902))), index32 = widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(175629951))), index33 = widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(87814976))), index34 = widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(43907488))), index35 = widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(21953744))), index36 = widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(10976872))), index37 = widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(5488436))), index38 = widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(2744218))), index39 = widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(1372109))), index40 = widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(686055))), index41 = widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(343028))), index42 = widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(171514))), index43 = widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(85757))), index44 = widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(42879))), index45 = widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(21440))), index46 = widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(10720))), index47 = widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(5360))), index48 = widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(2680))), index49 = widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(1340))), index50 = widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(670))), index51 = widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(335))), index52 = widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(168))), index53 = widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(84))), index54 = widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(42))), index55 = widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(21))), index56 = widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(11))), index57 = widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(6))), index58 = widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(3))), index59 = widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(2))), index60 = widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(1))), index61 = widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(1))), index62 = widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(1))), index63 = widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(1)))) [linkage=external];
// DEFAULT-NEXT:     global %5 expected_c: array<u64, 64> [storage=static] = aggregate<array<u64, 64>, zero_fill=false>(index0 = reinterpret<u64, reason=assign, fits=always>(const<i64>(156789543257562457)), index1 = reinterpret<u64, reason=assign, fits=always>(const<i64>(78394771628781228)), index2 = reinterpret<u64, reason=assign, fits=always>(const<i64>(39197385814390614)), index3 = reinterpret<u64, reason=assign, fits=always>(const<i64>(19598692907195307)), index4 = reinterpret<u64, reason=assign, fits=always>(const<i64>(9799346453597653)), index5 = reinterpret<u64, reason=assign, fits=always>(const<i64>(4899673226798826)), index6 = reinterpret<u64, reason=assign, fits=always>(const<i64>(2449836613399413)), index7 = reinterpret<u64, reason=assign, fits=always>(const<i64>(1224918306699706)), index8 = reinterpret<u64, reason=assign, fits=always>(const<i64>(612459153349853)), index9 = reinterpret<u64, reason=assign, fits=always>(const<i64>(306229576674926)), index10 = reinterpret<u64, reason=assign, fits=always>(const<i64>(153114788337463)), index11 = reinterpret<u64, reason=assign, fits=always>(const<i64>(76557394168731)), index12 = reinterpret<u64, reason=assign, fits=always>(const<i64>(38278697084365)), index13 = reinterpret<u64, reason=assign, fits=always>(const<i64>(19139348542182)), index14 = reinterpret<u64, reason=assign, fits=always>(const<i64>(9569674271091)), index15 = reinterpret<u64, reason=assign, fits=always>(const<i64>(4784837135545)), index16 = reinterpret<u64, reason=assign, fits=always>(const<i64>(2392418567772)), index17 = reinterpret<u64, reason=assign, fits=always>(const<i64>(1196209283886)), index18 = reinterpret<u64, reason=assign, fits=always>(const<i64>(598104641943)), index19 = reinterpret<u64, reason=assign, fits=always>(const<i64>(299052320971)), index20 = reinterpret<u64, reason=assign, fits=always>(const<i64>(149526160485)), index21 = reinterpret<u64, reason=assign, fits=always>(const<i64>(74763080242)), index22 = reinterpret<u64, reason=assign, fits=always>(const<i64>(37381540121)), index23 = reinterpret<u64, reason=assign, fits=always>(const<i64>(18690770060)), index24 = reinterpret<u64, reason=assign, fits=always>(const<i64>(9345385030)), index25 = reinterpret<u64, reason=assign, fits=always>(const<i64>(4672692515)), index26 = reinterpret<u64, reason=assign, fits=always>(const<i64>(2336346257)), index27 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(1168173128))), index28 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(584086564))), index29 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(292043282))), index30 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(146021641))), index31 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(73010820))), index32 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(36505410))), index33 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(18252705))), index34 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(9126352))), index35 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(4563176))), index36 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(2281588))), index37 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(1140794))), index38 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(570397))), index39 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(285198))), index40 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(142599))), index41 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(71299))), index42 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(35649))), index43 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(17824))), index44 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(8912))), index45 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(4456))), index46 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(2228))), index47 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(1114))), index48 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(557))), index49 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(278))), index50 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(139))), index51 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(69))), index52 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(34))), index53 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(17))), index54 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(8))), index55 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(4))), index56 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(2))), index57 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(1))), index58 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))), index59 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))), index60 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))), index61 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))), index62 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))), index63 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)))) [linkage=external];
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %7 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %8
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%7, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%7), const<i32>(64))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %9: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                 let %10: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%9), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%7, read<i32>(%10));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if logical_or<bool>(logical_or<bool>(ne<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i64>(%0), read<i32>(%7)), read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(64)>(%3), read<i32>(%7))))), ne<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%1), read<i32>(%7)), read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(64)>(%4), read<i32>(%7)))))), ne<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%2), read<i32>(%7)), read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(64)>(%5), read<i32>(%7))))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
