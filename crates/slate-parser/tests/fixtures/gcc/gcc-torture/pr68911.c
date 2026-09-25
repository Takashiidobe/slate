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
// DEFAULT-NEXT:     global %1 a: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 d: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %7 e: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(2));
// DEFAULT-NEXT:         let %8 timeout: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:         for %10
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%3), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %12: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                 let %13: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%12), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%3, read<i32>(%13));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %9 f: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(div<u32, by_zero=ub>(not<u32>(read<u32>(%7)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(7))));
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%9), const<i32>(0))
// DEFAULT-NEXT:                         write<u32>(%7, reinterpret<u32, reason=assign, fits=unknown>(not<i32>(from_bool<i32, reason=promotion>(logical_and<bool>(ne<i32>(read<i32>(%2), const<i32>(0)), ne<i16>(read<i16>(%4), const<i16>(0)))))));
// DEFAULT-NEXT:                         write<i8>(%1, reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(reinterpret<u32, reason=assign, fits=unknown>(not<i32>(from_bool<i32, reason=promotion>(logical_and<bool>(ne<i32>(read<i32>(%2), const<i32>(0)), ne<i16>(read<i16>(%4), const<i16>(0)))))))));
// DEFAULT-NEXT:                     while %11 lt<u32>(read<u32>(%7), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(94)))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             let %14: u32 [synthetic] = read<u32>(%7);
// DEFAULT-NEXT:                             let %15: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%14), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                             write<u32>(%7, read<u32>(%15));
// DEFAULT-NEXT:                             let %16: u32 [synthetic] = read<u32>(%8);
// DEFAULT-NEXT:                             let %17: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%16), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                             write<u32>(%8, read<u32>(%17));
// DEFAULT-NEXT:                             if gt<u32>(read<u32>(%17), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(100)))
// DEFAULT-NEXT:                                 goto %6;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:         label %6 die:
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
