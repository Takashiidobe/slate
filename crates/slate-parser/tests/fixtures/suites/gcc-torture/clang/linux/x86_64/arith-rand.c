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
// DEFAULT-NEXT:     global %[[VALUE_seed:[0-9]+]] seed: u64 [storage=static] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(47114711))) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_simple_rand:[0-9]+]] @simple_rand() -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_this:[0-9]+]] this: u64 [storage=automatic] = add<u64, overflow=wrap>(mul<u64, overflow=wrap>(read<u64>(%[[VALUE_seed]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1103515245)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(12345))));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_seed]], read<u64>(%[[VALUE_this]]));
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%[[VALUE_this]]), const<i32>(8)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_random_bitstring:[0-9]+]] @random_bitstring() -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: u64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_n_bits:[0-9]+]] n_bits: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ran:[0-9]+]] ran: i64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_tot_bits:[0-9]+]] tot_bits: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         write<u64>(%[[VALUE_x]], reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:         for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: omitted
// DEFAULT-NEXT:             increment: omitted
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i64>(%[[VALUE_ran]], call<i64, signature=fn() -> i64>(%[[VALUE_simple_rand]]));
// DEFAULT-NEXT:                     call<i64, signature=fn() -> i64>(%[[VALUE_simple_rand]]);
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_n_bits]], truncate<i32, reason=assign, fits=unknown>(rem<i64, by_zero=ub, min_by_neg_one=ub>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%[[VALUE_ran]]), const<i32>(1)), widen<i64, reason=usual_arith>(const<i32>(16)))));
// DEFAULT-NEXT:                     let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_tot_bits]]);
// DEFAULT-NEXT:                     let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), read<i32>(%[[VALUE_n_bits]]));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_tot_bits]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:                     if eq<i32>(read<i32>(%[[VALUE_n_bits]]), const<i32>(0))
// DEFAULT-NEXT:                         return read<u64>(%[[VALUE_x]]);
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             let %[[VALUE4:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_x]]);
// DEFAULT-NEXT:                             let %[[VALUE5:[0-9]+]]: u64 [synthetic] = shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE4]]), read<i32>(%[[VALUE_n_bits]]));
// DEFAULT-NEXT:                             write<u64>(%[[VALUE_x]], read<u64>(%[[VALUE5]]));
// DEFAULT-NEXT:                             if ne<i64>(and<i64>(read<i64>(%[[VALUE_ran]]), widen<i64, reason=usual_arith>(const<i32>(1))), const<i64>(0))
// DEFAULT-NEXT:                                 let %[[VALUE6:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_x]]);
// DEFAULT-NEXT:                                 let %[[VALUE7:[0-9]+]]: u64 [synthetic] = or<u64>(read<u64>(%[[VALUE6]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), read<i32>(%[[VALUE_n_bits]])), const<i32>(1)))));
// DEFAULT-NEXT:                                 write<u64>(%[[VALUE_x]], read<u64>(%[[VALUE7]]));
// DEFAULT-NEXT:                             if gt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_tot_bits]]))), add<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(6)))))
// DEFAULT-NEXT:                                 return read<u64>(%[[VALUE_x]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i64 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE8:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i64>(%[[VALUE_i]], widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:             condition: lt<i64>(read<i64>(%[[VALUE_i]]), widen<i64, reason=usual_arith>(const<i32>(1000)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE9:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE10:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE9]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(%[[VALUE_i]], read<i64>(%[[VALUE10]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_x_2:[0-9]+]] x: u64 [storage=automatic];
// DEFAULT-NEXT:                     let %[[VALUE_y:[0-9]+]] y: u64 [storage=automatic];
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], call<u64, signature=fn() -> u64>(%[[VALUE_random_bitstring]]));
// DEFAULT-NEXT:                     call<u64, signature=fn() -> u64>(%[[VALUE_random_bitstring]]);
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_y]], call<u64, signature=fn() -> u64>(%[[VALUE_random_bitstring]]));
// DEFAULT-NEXT:                     call<u64, signature=fn() -> u64>(%[[VALUE_random_bitstring]]);
// DEFAULT-NEXT:                     if eq<u64>(const<u64>(4), const<u64>(8))
// DEFAULT-NEXT:                         goto %[[VALUE_save_time:[0-9]+]];
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_xx:[0-9]+]] xx: u64 [storage=automatic] = read<u64>(%[[VALUE_x_2]]);
// DEFAULT-NEXT:                         let %[[VALUE_yy:[0-9]+]] yy: u64 [storage=automatic] = read<u64>(%[[VALUE_y]]);
// DEFAULT-NEXT:                         let %[[VALUE_r1:[0-9]+]] r1: u64 [storage=automatic];
// DEFAULT-NEXT:                         let %[[VALUE_r2:[0-9]+]] r2: u64 [storage=automatic];
// DEFAULT-NEXT:                         if eq<u64>(read<u64>(%[[VALUE_yy]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                             continue %[[VALUE8]];
// DEFAULT-NEXT:                         write<u64>(%[[VALUE_r1]], div<u64, by_zero=ub>(read<u64>(%[[VALUE_xx]]), read<u64>(%[[VALUE_yy]])));
// DEFAULT-NEXT:                         write<u64>(%[[VALUE_r2]], rem<u64, by_zero=ub>(read<u64>(%[[VALUE_xx]]), read<u64>(%[[VALUE_yy]])));
// DEFAULT-NEXT:                         if logical_or<bool>(ge<u64>(read<u64>(%[[VALUE_r2]]), read<u64>(%[[VALUE_yy]])), ne<u64>(add<u64, overflow=wrap>(mul<u64, overflow=wrap>(read<u64>(%[[VALUE_r1]]), read<u64>(%[[VALUE_yy]])), read<u64>(%[[VALUE_r2]])), read<u64>(%[[VALUE_xx]])))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_xx_2:[0-9]+]] xx: i64 [storage=automatic] = reinterpret<i64, reason=assign, fits=unknown>(read<u64>(%[[VALUE_x_2]]));
// DEFAULT-NEXT:                         let %[[VALUE_yy_2:[0-9]+]] yy: i64 [storage=automatic] = reinterpret<i64, reason=assign, fits=unknown>(read<u64>(%[[VALUE_y]]));
// DEFAULT-NEXT:                         let %[[VALUE_r1_2:[0-9]+]] r1: i64 [storage=automatic];
// DEFAULT-NEXT:                         let %[[VALUE_r2_2:[0-9]+]] r2: i64 [storage=automatic];
// DEFAULT-NEXT:                         if logical_and<bool>(eq<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(read<i64>(%[[VALUE_xx_2]])), const<i32>(1)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), eq<i64>(read<i64>(%[[VALUE_yy_2]]), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                             continue %[[VALUE8]];
// DEFAULT-NEXT:                         write<i64>(%[[VALUE_r1_2]], div<i64, by_zero=ub, min_by_neg_one=ub>(read<i64>(%[[VALUE_xx_2]]), read<i64>(%[[VALUE_yy_2]])));
// DEFAULT-NEXT:                         write<i64>(%[[VALUE_r2_2]], rem<i64, by_zero=ub, min_by_neg_one=ub>(read<i64>(%[[VALUE_xx_2]]), read<i64>(%[[VALUE_yy_2]])));
// DEFAULT-NEXT:                         if logical_or<bool>(ge<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(conditional<i64>(ge<i64>(read<i64>(%[[VALUE_r2_2]]), widen<i64, reason=usual_arith>(const<i32>(0))), read<i64>(%[[VALUE_r2_2]]), neg<i64, overflow=ub>(read<i64>(%[[VALUE_r2_2]])))), reinterpret<u64, reason=explicit, fits=unknown>(conditional<i64>(ge<i64>(read<i64>(%[[VALUE_yy_2]]), widen<i64, reason=usual_arith>(const<i32>(0))), read<i64>(%[[VALUE_yy_2]]), neg<i64, overflow=ub>(read<i64>(%[[VALUE_yy_2]]))))), ne<i64>(add<i64, overflow=ub>(mul<i64, overflow=ub>(read<i64>(%[[VALUE_r1_2]]), read<i64>(%[[VALUE_yy_2]])), read<i64>(%[[VALUE_r2_2]])), read<i64>(%[[VALUE_xx_2]])))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     label %[[VALUE_save_time]] save_time:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             let %[[VALUE_xx_3:[0-9]+]] xx: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(read<u64>(%[[VALUE_x_2]]));
// DEFAULT-NEXT:                             let %[[VALUE_yy_3:[0-9]+]] yy: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(read<u64>(%[[VALUE_y]]));
// DEFAULT-NEXT:                             let %[[VALUE_r1_3:[0-9]+]] r1: u32 [storage=automatic];
// DEFAULT-NEXT:                             let %[[VALUE_r2_3:[0-9]+]] r2: u32 [storage=automatic];
// DEFAULT-NEXT:                             if eq<u32>(read<u32>(%[[VALUE_yy_3]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:                                 continue %[[VALUE8]];
// DEFAULT-NEXT:                             write<u32>(%[[VALUE_r1_3]], div<u32, by_zero=ub>(read<u32>(%[[VALUE_xx_3]]), read<u32>(%[[VALUE_yy_3]])));
// DEFAULT-NEXT:                             write<u32>(%[[VALUE_r2_3]], rem<u32, by_zero=ub>(read<u32>(%[[VALUE_xx_3]]), read<u32>(%[[VALUE_yy_3]])));
// DEFAULT-NEXT:                             if logical_or<bool>(ge<u32>(read<u32>(%[[VALUE_r2_3]]), read<u32>(%[[VALUE_yy_3]])), ne<u32>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(read<u32>(%[[VALUE_r1_3]]), read<u32>(%[[VALUE_yy_3]])), read<u32>(%[[VALUE_r2_3]])), read<u32>(%[[VALUE_xx_3]])))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_xx_4:[0-9]+]] xx: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(read<u64>(%[[VALUE_x_2]])));
// DEFAULT-NEXT:                         let %[[VALUE_yy_4:[0-9]+]] yy: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(read<u64>(%[[VALUE_y]])));
// DEFAULT-NEXT:                         let %[[VALUE_r1_4:[0-9]+]] r1: i32 [storage=automatic];
// DEFAULT-NEXT:                         let %[[VALUE_r2_4:[0-9]+]] r2: i32 [storage=automatic];
// DEFAULT-NEXT:                         if logical_and<bool>(eq<u32>(shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit, fits=unknown>(read<i32>(%[[VALUE_xx_4]])), const<i32>(1)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), eq<i32>(read<i32>(%[[VALUE_yy_4]]), neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             continue %[[VALUE8]];
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_r1_4]], div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_xx_4]]), read<i32>(%[[VALUE_yy_4]])));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_r2_4]], rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_xx_4]]), read<i32>(%[[VALUE_yy_4]])));
// DEFAULT-NEXT:                         if logical_or<bool>(ge<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(conditional<i32>(ge<i32>(read<i32>(%[[VALUE_r2_4]]), const<i32>(0)), read<i32>(%[[VALUE_r2_4]]), neg<i32, overflow=ub>(read<i32>(%[[VALUE_r2_4]])))), reinterpret<u32, reason=explicit, fits=unknown>(conditional<i32>(ge<i32>(read<i32>(%[[VALUE_yy_4]]), const<i32>(0)), read<i32>(%[[VALUE_yy_4]]), neg<i32, overflow=ub>(read<i32>(%[[VALUE_yy_4]]))))), ne<i32>(add<i32, overflow=ub>(mul<i32, overflow=ub>(read<i32>(%[[VALUE_r1_4]]), read<i32>(%[[VALUE_yy_4]])), read<i32>(%[[VALUE_r2_4]])), read<i32>(%[[VALUE_xx_4]])))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_xx_5:[0-9]+]] xx: u16 [storage=automatic] = truncate<u16, reason=assign, fits=unknown>(read<u64>(%[[VALUE_x_2]]));
// DEFAULT-NEXT:                         let %[[VALUE_yy_5:[0-9]+]] yy: u16 [storage=automatic] = truncate<u16, reason=assign, fits=unknown>(read<u64>(%[[VALUE_y]]));
// DEFAULT-NEXT:                         let %[[VALUE_r1_5:[0-9]+]] r1: u16 [storage=automatic];
// DEFAULT-NEXT:                         let %[[VALUE_r2_5:[0-9]+]] r2: u16 [storage=automatic];
// DEFAULT-NEXT:                         if eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_yy_5]]))), const<i32>(0))
// DEFAULT-NEXT:                             continue %[[VALUE8]];
// DEFAULT-NEXT:                         write<u16>(%[[VALUE_r1_5]], reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(div<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_xx_5]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_yy_5]])))))));
// DEFAULT-NEXT:                         write<u16>(%[[VALUE_r2_5]], reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_xx_5]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_yy_5]])))))));
// DEFAULT-NEXT:                         if logical_or<bool>(ge<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_r2_5]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_yy_5]])))), ne<i32>(add<i32, overflow=ub>(mul<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_r1_5]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_yy_5]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_r2_5]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_xx_5]])))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_xx_6:[0-9]+]] xx: i16 [storage=automatic] = reinterpret<i16, reason=assign, fits=unknown>(truncate<u16, reason=assign, fits=unknown>(read<u64>(%[[VALUE_x_2]])));
// DEFAULT-NEXT:                         let %[[VALUE_yy_6:[0-9]+]] yy: i16 [storage=automatic] = reinterpret<i16, reason=assign, fits=unknown>(truncate<u16, reason=assign, fits=unknown>(read<u64>(%[[VALUE_y]])));
// DEFAULT-NEXT:                         let %[[VALUE_r1_6:[0-9]+]] r1: i16 [storage=automatic];
// DEFAULT-NEXT:                         let %[[VALUE_r2_6:[0-9]+]] r2: i16 [storage=automatic];
// DEFAULT-NEXT:                         write<i16>(%[[VALUE_r1_6]], truncate<i16, reason=assign, fits=unknown>(div<i32, by_zero=ub, min_by_neg_one=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_xx_6]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE_yy_6]])))));
// DEFAULT-NEXT:                         write<i16>(%[[VALUE_r2_6]], truncate<i16, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_xx_6]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE_yy_6]])))));
// DEFAULT-NEXT:                         if logical_or<bool>(ge<i32>(conditional<i32>(ge<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_r2_6]])), const<i32>(0)), widen<i32, reason=promotion>(read<i16>(%[[VALUE_r2_6]])), neg<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_r2_6]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(conditional<i32>(ge<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_yy_6]])), const<i32>(0)), widen<i32, reason=promotion>(read<i16>(%[[VALUE_yy_6]])), neg<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_yy_6]]))))))))), ne<i32>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_r1_6]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE_yy_6]]))), widen<i32, reason=promotion>(read<i16>(%[[VALUE_r2_6]]))))), widen<i32, reason=promotion>(read<i16>(%[[VALUE_xx_6]]))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_xx_7:[0-9]+]] xx: u8 [storage=automatic] = truncate<u8, reason=assign, fits=unknown>(read<u64>(%[[VALUE_x_2]]));
// DEFAULT-NEXT:                         let %[[VALUE_yy_7:[0-9]+]] yy: u8 [storage=automatic] = truncate<u8, reason=assign, fits=unknown>(read<u64>(%[[VALUE_y]]));
// DEFAULT-NEXT:                         let %[[VALUE_r1_7:[0-9]+]] r1: u8 [storage=automatic];
// DEFAULT-NEXT:                         let %[[VALUE_r2_7:[0-9]+]] r2: u8 [storage=automatic];
// DEFAULT-NEXT:                         if eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_yy_7]]))), const<i32>(0))
// DEFAULT-NEXT:                             continue %[[VALUE8]];
// DEFAULT-NEXT:                         write<u8>(%[[VALUE_r1_7]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(div<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_xx_7]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_yy_7]])))))));
// DEFAULT-NEXT:                         write<u8>(%[[VALUE_r2_7]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_xx_7]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_yy_7]])))))));
// DEFAULT-NEXT:                         if logical_or<bool>(ge<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_r2_7]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_yy_7]])))), ne<i32>(add<i32, overflow=ub>(mul<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_r1_7]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_yy_7]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_r2_7]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_xx_7]])))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_xx_8:[0-9]+]] xx: i8 [storage=automatic] = reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(read<u64>(%[[VALUE_x_2]])));
// DEFAULT-NEXT:                         let %[[VALUE_yy_8:[0-9]+]] yy: i8 [storage=automatic] = reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(read<u64>(%[[VALUE_y]])));
// DEFAULT-NEXT:                         let %[[VALUE_r1_8:[0-9]+]] r1: i8 [storage=automatic];
// DEFAULT-NEXT:                         let %[[VALUE_r2_8:[0-9]+]] r2: i8 [storage=automatic];
// DEFAULT-NEXT:                         write<i8>(%[[VALUE_r1_8]], truncate<i8, reason=assign, fits=unknown>(div<i32, by_zero=ub, min_by_neg_one=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_xx_8]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_yy_8]])))));
// DEFAULT-NEXT:                         write<i8>(%[[VALUE_r2_8]], truncate<i8, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_xx_8]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_yy_8]])))));
// DEFAULT-NEXT:                         if logical_or<bool>(ge<i32>(conditional<i32>(ge<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_r2_8]])), const<i32>(0)), widen<i32, reason=promotion>(read<i8>(%[[VALUE_r2_8]])), neg<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_r2_8]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(conditional<i32>(ge<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_yy_8]])), const<i32>(0)), widen<i32, reason=promotion>(read<i8>(%[[VALUE_yy_8]])), neg<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_yy_8]]))))))))), ne<i32>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_r1_8]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_yy_8]]))), widen<i32, reason=promotion>(read<i8>(%[[VALUE_r2_8]]))))), widen<i32, reason=promotion>(read<i8>(%[[VALUE_xx_8]]))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
