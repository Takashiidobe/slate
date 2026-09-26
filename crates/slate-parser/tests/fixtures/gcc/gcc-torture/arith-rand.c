void abort(void);
void exit(int);

long simple_rand() {
  static unsigned long seed = 47114711;
  unsigned long this        = seed * 1103515245 + 12345;
  seed                      = this;
  return this >> 8;
}

unsigned long int random_bitstring() {
  unsigned long int x;
  int               n_bits;
  long              ran;
  int               tot_bits = 0;

  x = 0;
  for (;;) {
    ran       = simple_rand();
    n_bits    = (ran >> 1) % 16;
    tot_bits += n_bits;

    if (n_bits == 0)
      return x;
    else {
      x <<= n_bits;
      if (ran & 1)
        x |= (1 << n_bits) - 1;

      if (tot_bits > 8 * sizeof(long) + 6)
        return x;
    }
  }
}

#define ABS(x) ((x) >= 0 ? (x) : -(x))

int main(void) {
  long int i;

  for (i = 0; i < 1000; i++) {
    unsigned long x, y;
    x = random_bitstring();
    y = random_bitstring();

    if (sizeof(int) == sizeof(long))
      goto save_time;

    {
      unsigned long xx = x, yy = y, r1, r2;
      if (yy == 0)
        continue;
      r1 = xx / yy;
      r2 = xx % yy;
      if (r2 >= yy || r1 * yy + r2 != xx)
        abort();
    }
    {
      signed long xx = x, yy = y, r1, r2;
      if ((unsigned long)xx << 1 == 0 && yy == -1)
        continue;
      r1 = xx / yy;
      r2 = xx % yy;
      if (ABS(r2) >= (unsigned long)ABS(yy) ||
          (signed long)(r1 * yy + r2) != xx)
        abort();
    }
  save_time: {
    unsigned int xx = x, yy = y, r1, r2;
    if (yy == 0)
      continue;
    r1 = xx / yy;
    r2 = xx % yy;
    if (r2 >= yy || r1 * yy + r2 != xx)
      abort();
  }
    {
      signed int xx = x, yy = y, r1, r2;
      if ((unsigned int)xx << 1 == 0 && yy == -1)
        continue;
      r1 = xx / yy;
      r2 = xx % yy;
      if (ABS(r2) >= (unsigned int)ABS(yy) || (signed int)(r1 * yy + r2) != xx)
        abort();
    }
    {
      unsigned short xx = x, yy = y, r1, r2;
      if (yy == 0)
        continue;
      r1 = xx / yy;
      r2 = xx % yy;
      if (r2 >= yy || r1 * yy + r2 != xx)
        abort();
    }
    {
      signed short xx = x, yy = y, r1, r2;
      r1 = xx / yy;
      r2 = xx % yy;
      if (ABS(r2) >= (unsigned short)ABS(yy) ||
          (signed short)(r1 * yy + r2) != xx)
        abort();
    }
    {
      unsigned char xx = x, yy = y, r1, r2;
      if (yy == 0)
        continue;
      r1 = xx / yy;
      r2 = xx % yy;
      if (r2 >= yy || r1 * yy + r2 != xx)
        abort();
    }
    {
      signed char xx = x, yy = y, r1, r2;
      r1 = xx / yy;
      r2 = xx % yy;
      if (ABS(r2) >= (unsigned char)ABS(yy) ||
          (signed char)(r1 * yy + r2) != xx)
        abort();
    }
  }

  exit(0);
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
// DEFAULT-NEXT:     global %3 seed: u64 [storage=static] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(47114711))) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%47 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @simple_rand() -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 this: u64 [storage=automatic] = add<u64, overflow=wrap>(mul<u64, overflow=wrap>(read<u64>(%3), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1103515245)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(12345))));
// DEFAULT-NEXT:         write<u64>(%3, read<u64>(%4));
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%4), const<i32>(8)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @random_bitstring() -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %6 x: u64 [storage=automatic];
// DEFAULT-NEXT:         let %7 n_bits: i32 [storage=automatic];
// DEFAULT-NEXT:         let %8 ran: i64 [storage=automatic];
// DEFAULT-NEXT:         let %9 tot_bits: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         write<u64>(%6, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:         for %48
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: omitted
// DEFAULT-NEXT:             increment: omitted
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i64>(%8, call<i64, signature=fn() -> i64>(%2));
// DEFAULT-NEXT:                     call<i64, signature=fn() -> i64>(%2);
// DEFAULT-NEXT:                     write<i32>(%7, truncate<i32, reason=assign, fits=unknown>(rem<i64, by_zero=ub, min_by_neg_one=ub>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(1)), widen<i64, reason=usual_arith>(const<i32>(16)))));
// DEFAULT-NEXT:                     let %50: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:                     let %51: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%50), read<i32>(%7));
// DEFAULT-NEXT:                     write<i32>(%9, read<i32>(%51));
// DEFAULT-NEXT:                     if eq<i32>(read<i32>(%7), const<i32>(0))
// DEFAULT-NEXT:                         return read<u64>(%6);
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             let %52: u64 [synthetic] = read<u64>(%6);
// DEFAULT-NEXT:                             let %53: u64 [synthetic] = shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%52), read<i32>(%7));
// DEFAULT-NEXT:                             write<u64>(%6, read<u64>(%53));
// DEFAULT-NEXT:                             if ne<i64>(and<i64>(read<i64>(%8), widen<i64, reason=usual_arith>(const<i32>(1))), const<i64>(0))
// DEFAULT-NEXT:                                 let %54: u64 [synthetic] = read<u64>(%6);
// DEFAULT-NEXT:                                 let %55: u64 [synthetic] = or<u64>(read<u64>(%54), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), read<i32>(%7)), const<i32>(1)))));
// DEFAULT-NEXT:                                 write<u64>(%6, read<u64>(%55));
// DEFAULT-NEXT:                             if gt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%9))), add<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(6)))))
// DEFAULT-NEXT:                                 return read<u64>(%6);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %12 i: i64 [storage=automatic];
// DEFAULT-NEXT:         for %49
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i64>(%12, widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:             condition: lt<i64>(read<i64>(%12), widen<i64, reason=usual_arith>(const<i32>(1000)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %56: i64 [synthetic] = read<i64>(%12);
// DEFAULT-NEXT:                 let %57: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%56), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(%12, read<i64>(%57));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %13 x: u64 [storage=automatic];
// DEFAULT-NEXT:                     let %14 y: u64 [storage=automatic];
// DEFAULT-NEXT:                     write<u64>(%13, call<u64, signature=fn() -> u64>(%5));
// DEFAULT-NEXT:                     call<u64, signature=fn() -> u64>(%5);
// DEFAULT-NEXT:                     write<u64>(%14, call<u64, signature=fn() -> u64>(%5));
// DEFAULT-NEXT:                     call<u64, signature=fn() -> u64>(%5);
// DEFAULT-NEXT:                     if eq<u64>(const<u64>(4), const<u64>(8))
// DEFAULT-NEXT:                         goto %11;
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %15 xx: u64 [storage=automatic] = read<u64>(%13);
// DEFAULT-NEXT:                         let %16 yy: u64 [storage=automatic] = read<u64>(%14);
// DEFAULT-NEXT:                         let %17 r1: u64 [storage=automatic];
// DEFAULT-NEXT:                         let %18 r2: u64 [storage=automatic];
// DEFAULT-NEXT:                         if eq<u64>(read<u64>(%16), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                             continue %49;
// DEFAULT-NEXT:                         write<u64>(%17, div<u64, by_zero=ub>(read<u64>(%15), read<u64>(%16)));
// DEFAULT-NEXT:                         write<u64>(%18, rem<u64, by_zero=ub>(read<u64>(%15), read<u64>(%16)));
// DEFAULT-NEXT:                         if logical_or<bool>(ge<u64>(read<u64>(%18), read<u64>(%16)), ne<u64>(add<u64, overflow=wrap>(mul<u64, overflow=wrap>(read<u64>(%17), read<u64>(%16)), read<u64>(%18)), read<u64>(%15)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %19 xx: i64 [storage=automatic] = reinterpret<i64, reason=assign, fits=unknown>(read<u64>(%13));
// DEFAULT-NEXT:                         let %20 yy: i64 [storage=automatic] = reinterpret<i64, reason=assign, fits=unknown>(read<u64>(%14));
// DEFAULT-NEXT:                         let %21 r1: i64 [storage=automatic];
// DEFAULT-NEXT:                         let %22 r2: i64 [storage=automatic];
// DEFAULT-NEXT:                         if logical_and<bool>(eq<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(read<i64>(%19)), const<i32>(1)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), eq<i64>(read<i64>(%20), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             continue %49;
// DEFAULT-NEXT:                         write<i64>(%21, div<i64, by_zero=ub, min_by_neg_one=ub>(read<i64>(%19), read<i64>(%20)));
// DEFAULT-NEXT:                         write<i64>(%22, rem<i64, by_zero=ub, min_by_neg_one=ub>(read<i64>(%19), read<i64>(%20)));
// DEFAULT-NEXT:                         if logical_or<bool>(ge<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(conditional<i64>(ge<i64>(read<i64>(%22), widen<i64, reason=usual_arith>(const<i32>(0))), read<i64>(%22), neg<i64, overflow=ub>(read<i64>(%22)))), reinterpret<u64, reason=explicit, fits=unknown>(conditional<i64>(ge<i64>(read<i64>(%20), widen<i64, reason=usual_arith>(const<i32>(0))), read<i64>(%20), neg<i64, overflow=ub>(read<i64>(%20))))), ne<i64>(add<i64, overflow=ub>(mul<i64, overflow=ub>(read<i64>(%21), read<i64>(%20)), read<i64>(%22)), read<i64>(%19)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     label %11 save_time:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             let %23 xx: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(read<u64>(%13));
// DEFAULT-NEXT:                             let %24 yy: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(read<u64>(%14));
// DEFAULT-NEXT:                             let %25 r1: u32 [storage=automatic];
// DEFAULT-NEXT:                             let %26 r2: u32 [storage=automatic];
// DEFAULT-NEXT:                             if eq<u32>(read<u32>(%24), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:                                 continue %49;
// DEFAULT-NEXT:                             write<u32>(%25, div<u32, by_zero=ub>(read<u32>(%23), read<u32>(%24)));
// DEFAULT-NEXT:                             write<u32>(%26, rem<u32, by_zero=ub>(read<u32>(%23), read<u32>(%24)));
// DEFAULT-NEXT:                             if logical_or<bool>(ge<u32>(read<u32>(%26), read<u32>(%24)), ne<u32>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(read<u32>(%25), read<u32>(%24)), read<u32>(%26)), read<u32>(%23)))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %27 xx: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(read<u64>(%13)));
// DEFAULT-NEXT:                         let %28 yy: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(read<u64>(%14)));
// DEFAULT-NEXT:                         let %29 r1: i32 [storage=automatic];
// DEFAULT-NEXT:                         let %30 r2: i32 [storage=automatic];
// DEFAULT-NEXT:                         if logical_and<bool>(eq<u32>(shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit, fits=unknown>(read<i32>(%27)), const<i32>(1)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), eq<i32>(read<i32>(%28), neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             continue %49;
// DEFAULT-NEXT:                         write<i32>(%29, div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%27), read<i32>(%28)));
// DEFAULT-NEXT:                         write<i32>(%30, rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%27), read<i32>(%28)));
// DEFAULT-NEXT:                         if logical_or<bool>(ge<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(conditional<i32>(ge<i32>(read<i32>(%30), const<i32>(0)), read<i32>(%30), neg<i32, overflow=ub>(read<i32>(%30)))), reinterpret<u32, reason=explicit, fits=unknown>(conditional<i32>(ge<i32>(read<i32>(%28), const<i32>(0)), read<i32>(%28), neg<i32, overflow=ub>(read<i32>(%28))))), ne<i32>(add<i32, overflow=ub>(mul<i32, overflow=ub>(read<i32>(%29), read<i32>(%28)), read<i32>(%30)), read<i32>(%27)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %31 xx: u16 [storage=automatic] = truncate<u16, reason=assign, fits=unknown>(read<u64>(%13));
// DEFAULT-NEXT:                         let %32 yy: u16 [storage=automatic] = truncate<u16, reason=assign, fits=unknown>(read<u64>(%14));
// DEFAULT-NEXT:                         let %33 r1: u16 [storage=automatic];
// DEFAULT-NEXT:                         let %34 r2: u16 [storage=automatic];
// DEFAULT-NEXT:                         if eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%32))), const<i32>(0))
// DEFAULT-NEXT:                             continue %49;
// DEFAULT-NEXT:                         write<u16>(%33, reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(div<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%31))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%32)))))));
// DEFAULT-NEXT:                         write<u16>(%34, reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%31))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%32)))))));
// DEFAULT-NEXT:                         if logical_or<bool>(ge<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%34))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%32)))), ne<i32>(add<i32, overflow=ub>(mul<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%33))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%32)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%34)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%31)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %35 xx: i16 [storage=automatic] = reinterpret<i16, reason=assign, fits=unknown>(truncate<u16, reason=assign, fits=unknown>(read<u64>(%13)));
// DEFAULT-NEXT:                         let %36 yy: i16 [storage=automatic] = reinterpret<i16, reason=assign, fits=unknown>(truncate<u16, reason=assign, fits=unknown>(read<u64>(%14)));
// DEFAULT-NEXT:                         let %37 r1: i16 [storage=automatic];
// DEFAULT-NEXT:                         let %38 r2: i16 [storage=automatic];
// DEFAULT-NEXT:                         write<i16>(%37, truncate<i16, reason=assign, fits=unknown>(div<i32, by_zero=ub, min_by_neg_one=ub>(widen<i32, reason=promotion>(read<i16>(%35)), widen<i32, reason=promotion>(read<i16>(%36)))));
// DEFAULT-NEXT:                         write<i16>(%38, truncate<i16, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(widen<i32, reason=promotion>(read<i16>(%35)), widen<i32, reason=promotion>(read<i16>(%36)))));
// DEFAULT-NEXT:                         if logical_or<bool>(ge<i32>(conditional<i32>(ge<i32>(widen<i32, reason=promotion>(read<i16>(%38)), const<i32>(0)), widen<i32, reason=promotion>(read<i16>(%38)), neg<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%38)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(conditional<i32>(ge<i32>(widen<i32, reason=promotion>(read<i16>(%36)), const<i32>(0)), widen<i32, reason=promotion>(read<i16>(%36)), neg<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%36))))))))), ne<i32>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%37)), widen<i32, reason=promotion>(read<i16>(%36))), widen<i32, reason=promotion>(read<i16>(%38))))), widen<i32, reason=promotion>(read<i16>(%35))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %39 xx: u8 [storage=automatic] = truncate<u8, reason=assign, fits=unknown>(read<u64>(%13));
// DEFAULT-NEXT:                         let %40 yy: u8 [storage=automatic] = truncate<u8, reason=assign, fits=unknown>(read<u64>(%14));
// DEFAULT-NEXT:                         let %41 r1: u8 [storage=automatic];
// DEFAULT-NEXT:                         let %42 r2: u8 [storage=automatic];
// DEFAULT-NEXT:                         if eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%40))), const<i32>(0))
// DEFAULT-NEXT:                             continue %49;
// DEFAULT-NEXT:                         write<u8>(%41, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(div<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%39))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%40)))))));
// DEFAULT-NEXT:                         write<u8>(%42, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%39))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%40)))))));
// DEFAULT-NEXT:                         if logical_or<bool>(ge<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%42))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%40)))), ne<i32>(add<i32, overflow=ub>(mul<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%41))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%40)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%42)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%39)))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %43 xx: i8 [storage=automatic] = reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(read<u64>(%13)));
// DEFAULT-NEXT:                         let %44 yy: i8 [storage=automatic] = reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(read<u64>(%14)));
// DEFAULT-NEXT:                         let %45 r1: i8 [storage=automatic];
// DEFAULT-NEXT:                         let %46 r2: i8 [storage=automatic];
// DEFAULT-NEXT:                         write<i8>(%45, truncate<i8, reason=assign, fits=unknown>(div<i32, by_zero=ub, min_by_neg_one=ub>(widen<i32, reason=promotion>(read<i8>(%43)), widen<i32, reason=promotion>(read<i8>(%44)))));
// DEFAULT-NEXT:                         write<i8>(%46, truncate<i8, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(widen<i32, reason=promotion>(read<i8>(%43)), widen<i32, reason=promotion>(read<i8>(%44)))));
// DEFAULT-NEXT:                         if logical_or<bool>(ge<i32>(conditional<i32>(ge<i32>(widen<i32, reason=promotion>(read<i8>(%46)), const<i32>(0)), widen<i32, reason=promotion>(read<i8>(%46)), neg<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%46)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(conditional<i32>(ge<i32>(widen<i32, reason=promotion>(read<i8>(%44)), const<i32>(0)), widen<i32, reason=promotion>(read<i8>(%44)), neg<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%44))))))))), ne<i32>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%45)), widen<i32, reason=promotion>(read<i8>(%44))), widen<i32, reason=promotion>(read<i8>(%46))))), widen<i32, reason=promotion>(read<i8>(%43))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
