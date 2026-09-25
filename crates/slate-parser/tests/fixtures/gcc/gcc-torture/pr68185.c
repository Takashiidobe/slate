
int   a, b, d = 1, e, f, o, u, w = 1, z;
short c, q, t;

int main() {
  char g;
  for (; d; d--) {
    while (o)
      for (; e;) {
        c     = b;
        int h = o = z;
        for (; u;)
          for (; a;)
            ;
      }
    if (t < 1)
      g = w;
    f = g;
    g && (q = 1);
  }

  if (q != 1)
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
// DEFAULT-NEXT:     global %0 a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 d: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %3 e: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 f: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 o: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 u: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 w: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %8 z: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %9 c: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %10 q: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %11 t: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %13 g: i8 [storage=automatic];
// DEFAULT-NEXT:         for %15
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ne<i32>(read<i32>(%2), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %20: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:                 let %21: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%20), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%2, read<i32>(%21));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     while %16 ne<i32>(read<i32>(%5), const<i32>(0))
// DEFAULT-NEXT:                         for %17
// DEFAULT-NEXT:                             init:
// DEFAULT-NEXT:                             condition: ne<i32>(read<i32>(%3), const<i32>(0))
// DEFAULT-NEXT:                             increment: omitted
// DEFAULT-NEXT:                             body:
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     write<i16>(%9, truncate<i16, reason=assign, fits=unknown>(read<i32>(%1)));
// DEFAULT-NEXT:                                     let %14 h: i32 [storage=automatic];
// DEFAULT-NEXT:                                     write<i32>(%5, read<i32>(%8));
// DEFAULT-NEXT:                                     write<i32>(%14, read<i32>(%8));
// DEFAULT-NEXT:                                     for %18
// DEFAULT-NEXT:                                         init:
// DEFAULT-NEXT:                                         condition: ne<i32>(read<i32>(%6), const<i32>(0))
// DEFAULT-NEXT:                                         increment: omitted
// DEFAULT-NEXT:                                         body:
// DEFAULT-NEXT:                                             for %19
// DEFAULT-NEXT:                                                 init:
// DEFAULT-NEXT:                                                 condition: ne<i32>(read<i32>(%0), const<i32>(0))
// DEFAULT-NEXT:                                                 increment: omitted
// DEFAULT-NEXT:                                                 body:
// DEFAULT-NEXT:                                                     ;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                     if lt<i32>(widen<i32, reason=promotion>(read<i16>(%11)), const<i32>(1))
// DEFAULT-NEXT:                         write<i8>(%13, truncate<i8, reason=assign, fits=unknown>(read<i32>(%7)));
// DEFAULT-NEXT:                     write<i32>(%4, widen<i32, reason=assign>(read<i8>(%13)));
// DEFAULT-NEXT:                     let %22: bool [synthetic];
// DEFAULT-NEXT:                     if ne<i8>(read<i8>(%13), const<i8>(0))
// DEFAULT-NEXT:                         write<i16>(%10, truncate<i16, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                         write<bool>(%22, ne<i16>(truncate<i16, reason=assign, fits=always>(const<i32>(1)), const<i16>(0)));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<bool>(%22, const<bool>(false));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%10)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
