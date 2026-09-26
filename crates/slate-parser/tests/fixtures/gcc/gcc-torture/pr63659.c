/* PR rtl-optimization/63659 */

int           a, b, c, *d = &b, g, h, i;
unsigned char e;
char          f;

int main() {
  while (a) {
    for (a = 0; a; a++)
      for (; c; c++)
        ;
    if (i)
      break;
  }

  char j = c, k = -1, l;
  l = g = j >> h;
  f     = l == 0 ? k : k % l;
  e     = 0 ? 0 : f;
  *d    = e;

  if (b != 255)
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
// DEFAULT-NEXT:     global %2 c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 d: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%1) [linkage=external];
// DEFAULT-NEXT:     global %4 g: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 h: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 i: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 e: u8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %8 f: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %16 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         while %13 ne<i32>(read<i32>(%0), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 for %14
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%0, const<i32>(0));
// DEFAULT-NEXT:                     condition: ne<i32>(read<i32>(%0), const<i32>(0))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %17: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                         let %18: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%17), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%0, read<i32>(%18));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         for %15
// DEFAULT-NEXT:                             init:
// DEFAULT-NEXT:                             condition: ne<i32>(read<i32>(%2), const<i32>(0))
// DEFAULT-NEXT:                             increment: {
// DEFAULT-NEXT:                                 let %19: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:                                 let %20: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%19), const<i32>(1));
// DEFAULT-NEXT:                                 write<i32>(%2, read<i32>(%20));
// DEFAULT-NEXT:                                 yield void;
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                             body:
// DEFAULT-NEXT:                                 ;
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%6), const<i32>(0))
// DEFAULT-NEXT:                     break %13;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         let %10 j: i8 [storage=automatic] = truncate<i8, reason=assign, fits=unknown>(read<i32>(%2));
// DEFAULT-NEXT:         let %11 k: i8 [storage=automatic] = truncate<i8, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         let %12 l: i8 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%4, shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(%10)), read<i32>(%5)));
// DEFAULT-NEXT:         write<i8>(%12, truncate<i8, reason=assign, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(%10)), read<i32>(%5))));
// DEFAULT-NEXT:         write<i8>(%8, truncate<i8, reason=assign, fits=unknown>(conditional<i32>(eq<i32>(widen<i32, reason=promotion>(read<i8>(%12)), const<i32>(0)), widen<i32, reason=promotion>(read<i8>(%11)), rem<i32, by_zero=ub, min_by_neg_one=ub>(widen<i32, reason=promotion>(read<i8>(%11)), widen<i32, reason=promotion>(read<i8>(%12))))));
// DEFAULT-NEXT:         write<u8>(%7, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), const<i32>(0), widen<i32, reason=promotion>(read<i8>(%8))))));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%3)), reinterpret<i32, reason=assign, fits=unknown>(widen<u32, reason=assign>(read<u8>(%7))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), const<i32>(255))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
