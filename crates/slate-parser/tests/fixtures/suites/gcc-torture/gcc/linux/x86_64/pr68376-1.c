/* PR rtl-optimization/68376 */

int         a, b, c = 1;
signed char d;

int main() {
  for (; a < 1; a++)
    for (; b < 1; b++) {
      signed char e = ~d;
      if (d < 1)
        e = d;
      d = e;
      if (!c)
        __builtin_abort();
    }

  if (d != 0)
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
// DEFAULT-NEXT:     global %2 c: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %3 d: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %8 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %6
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%0), const<i32>(1))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %9: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                 let %10: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%9), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%0, read<i32>(%10));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %7
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%1), const<i32>(1))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %11: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:                         let %12: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%11), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%1, read<i32>(%12));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             let %5 e: i8 [storage=automatic] = truncate<i8, reason=assign, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i8>(%3))));
// DEFAULT-NEXT:                             if lt<i32>(widen<i32, reason=promotion>(read<i8>(%3)), const<i32>(1))
// DEFAULT-NEXT:                                 write<i8>(%5, read<i8>(%3));
// DEFAULT-NEXT:                             write<i8>(%3, read<i8>(%5));
// DEFAULT-NEXT:                             if not<bool>(ne<i32>(read<i32>(%2), const<i32>(0)))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%3)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
