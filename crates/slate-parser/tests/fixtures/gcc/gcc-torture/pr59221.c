

int   a = 1, b, d;
short e;

int main() {
  for (; b; b++)
    ;
  short f = a;
  int   g = 15;
  e       = f ? f : 1 << g;
  int h   = e;
  d       = h == 83647 ? 0 : h;
  if (d != 1)
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
// DEFAULT-NEXT:     global %0 a: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %1 b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 d: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 e: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %8
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ne<i32>(read<i32>(%1), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %9: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:                 let %10: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%9), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%1, read<i32>(%10));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:         let %5 f: i16 [storage=automatic] = truncate<i16, reason=assign, fits=unknown>(read<i32>(%0));
// DEFAULT-NEXT:         let %6 g: i32 [storage=automatic] = const<i32>(15);
// DEFAULT-NEXT:         write<i16>(%3, truncate<i16, reason=assign, fits=unknown>(conditional<i32>(ne<i16>(read<i16>(%5), const<i16>(0)), widen<i32, reason=promotion>(read<i16>(%5)), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), read<i32>(%6)))));
// DEFAULT-NEXT:         let %7 h: i32 [storage=automatic] = widen<i32, reason=assign>(read<i16>(%3));
// DEFAULT-NEXT:         write<i32>(%2, conditional<i32>(eq<i32>(read<i32>(%7), const<i32>(83647)), const<i32>(0), read<i32>(%7)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%2), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
