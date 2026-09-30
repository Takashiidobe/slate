extern void abort(void);

char  a;
int   b, c;
short d;

int main() {
  unsigned e       = 2;
  unsigned timeout = 0;

  for (; c < 2; c++) {
    int f = ~e / 7;
    if (f)
      a = e = ~(b && d);
    while (e < 94) {
      e++;
      if (++timeout > 100)
        goto die;
    }
  }
  return 0;
die:
  abort();
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_e:[0-9]+]] e: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(2));
// DEFAULT-NEXT:         let %[[VALUE_timeout:[0-9]+]] timeout: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_c]]), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_c]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_c]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_f:[0-9]+]] f: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(div<u32, by_zero=ub>(not<u32>(read<u32>(%[[VALUE_e]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(7))));
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%[[VALUE_f]]), const<i32>(0))
// DEFAULT-NEXT:                         let %[[VALUE3:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(not<i32>(from_bool<i32, reason=promotion>(logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_b]]), const<i32>(0)), ne<i16>(read<i16>(%[[VALUE_d]]), const<i16>(0))))));
// DEFAULT-NEXT:                         write<u32>(%[[VALUE_e]], read<u32>(%[[VALUE3]]));
// DEFAULT-NEXT:                         write<i8>(%[[VALUE_a]], reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(read<u32>(%[[VALUE3]]))));
// DEFAULT-NEXT:                     while %[[VALUE4:[0-9]+]] lt<u32>(read<u32>(%[[VALUE_e]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(94)))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             let %[[VALUE5:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_e]]);
// DEFAULT-NEXT:                             let %[[VALUE6:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE5]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                             write<u32>(%[[VALUE_e]], read<u32>(%[[VALUE6]]));
// DEFAULT-NEXT:                             let %[[VALUE7:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_timeout]]);
// DEFAULT-NEXT:                             let %[[VALUE8:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE7]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                             write<u32>(%[[VALUE_timeout]], read<u32>(%[[VALUE8]]));
// DEFAULT-NEXT:                             if gt<u32>(read<u32>(%[[VALUE8]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(100)))
// DEFAULT-NEXT:                                 goto %[[VALUE_die:[0-9]+]];
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:         label %[[VALUE_die]] die:
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
