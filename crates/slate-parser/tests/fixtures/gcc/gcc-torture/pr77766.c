char  a;
short b, d = 5, h;
char  c[1];
int   e, f = 4, g, j;
int   main() {
  int i;
  for (; f; f = a) {
    g = 0;
    for (; g <= 32; ++g) {
      i = 0;
      for (; i < 3; i++)
        while (1 > d)
          if (c[b])
            break;
    L:
      if (j)
        break;
    }
  }
  e = 0;
  for (; e; e = 0) {
    d++;
    for (; h;)
      goto L;
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
// DEFAULT-NEXT:     global %0 a: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 b: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 d: i16 [storage=static] = truncate<i16, reason=assign, fits=always>(const<i32>(5)) [linkage=external];
// DEFAULT-NEXT:     global %3 h: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 c: array<i8, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 e: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 f: i32 [storage=static] = const<i32>(4) [linkage=external];
// DEFAULT-NEXT:     global %7 g: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %8 j: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %11 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %12
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ne<i32>(read<i32>(%6), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 write<i32>(%6, widen<i32, reason=assign>(read<i8>(%0)));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(%7, const<i32>(0));
// DEFAULT-NEXT:                     for %13
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                         condition: le<i32>(read<i32>(%7), const<i32>(32))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %18: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                             let %19: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%18), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%7, read<i32>(%19));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%11, const<i32>(0));
// DEFAULT-NEXT:                                 for %14
// DEFAULT-NEXT:                                     init:
// DEFAULT-NEXT:                                     condition: lt<i32>(read<i32>(%11), const<i32>(3))
// DEFAULT-NEXT:                                     increment: {
// DEFAULT-NEXT:                                         let %20: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:                                         let %21: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%20), const<i32>(1));
// DEFAULT-NEXT:                                         write<i32>(%11, read<i32>(%21));
// DEFAULT-NEXT:                                         yield void;
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                                     body:
// DEFAULT-NEXT:                                         while %15 gt<i32>(const<i32>(1), widen<i32, reason=promotion>(read<i16>(%2)))
// DEFAULT-NEXT:                                             if ne<i8>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(%4), widen<i32, reason=promotion>(read<i16>(%1))))), const<i8>(0))
// DEFAULT-NEXT:                                                 break %15;
// DEFAULT-NEXT:                                 label %10 L:
// DEFAULT-NEXT:                                     if ne<i32>(read<i32>(%8), const<i32>(0))
// DEFAULT-NEXT:                                         break %13;
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         write<i32>(%5, const<i32>(0));
// DEFAULT-NEXT:         for %16
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ne<i32>(read<i32>(%5), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 write<i32>(%5, const<i32>(0));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %22: i16 [synthetic] = read<i16>(%2);
// DEFAULT-NEXT:                     let %23: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%22)), const<i32>(1)));
// DEFAULT-NEXT:                     write<i16>(%2, read<i16>(%23));
// DEFAULT-NEXT:                     for %17
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                         condition: ne<i16>(read<i16>(%3), const<i16>(0))
// DEFAULT-NEXT:                         increment: omitted
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             goto %10;
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
