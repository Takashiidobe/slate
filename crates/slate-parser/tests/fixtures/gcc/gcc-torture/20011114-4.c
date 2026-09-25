// SLATE-FILECHECK-DEFINES DEFAULT

static inline int foo (long x)
{
  register int a = 0;
  register unsigned b;

  do
    {
      b = (x & 0x7f);
      x = (x >> 7) | ~(-1L >> 7);
      a += 1;
    }
  while ((x != 0 || (b & 0x40) != 0) && (x != -1 || (b & 0x40) == 0));
  return a;
}

static inline int bar (unsigned long x)
{
  register int a = 0;
  register unsigned b;

  do
    {
      b = (x & 0x7f);
      x >>= 7;
      a++;
    }
  while (x != 0);
  return a;
}

int
baz (unsigned long x, int y)
{
  if (y)
    return foo ((long) x);
  else
    return bar (x);
}

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
// DEFAULT-NEXT:     fn %0 @foo(%1 x: i64) -> i32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %2 a: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %3 b: u32 [storage=automatic];
// DEFAULT-NEXT:         do %11
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<u32>(%3, reinterpret<u32, reason=assign, fits=unknown>(truncate<i32, reason=assign, fits=unknown>(and<i64>(read<i64>(%1), widen<i64, reason=usual_arith>(const<i32>(127))))));
// DEFAULT-NEXT:                 write<i64>(%1, or<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%1), const<i32>(7)), not<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(neg<i64, overflow=ub>(const<i64>(1)), const<i32>(7)))));
// DEFAULT-NEXT:                 let %13: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:                 let %14: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%13), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%2, read<i32>(%14));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while logical_and<bool>(logical_or<bool>(ne<i64>(read<i64>(%1), widen<i64, reason=usual_arith>(const<i32>(0))), ne<u32>(and<u32>(read<u32>(%3), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(64))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), logical_or<bool>(ne<i64>(read<i64>(%1), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))), eq<u32>(and<u32>(read<u32>(%3), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(64))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         return read<i32>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @bar(%5 x: u64) -> i32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %6 a: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %7 b: u32 [storage=automatic];
// DEFAULT-NEXT:         do %12
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<u32>(%7, truncate<u32, reason=assign, fits=unknown>(and<u64>(read<u64>(%5), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(127))))));
// DEFAULT-NEXT:                 let %15: u64 [synthetic] = read<u64>(%5);
// DEFAULT-NEXT:                 let %16: u64 [synthetic] = shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%15), const<i32>(7));
// DEFAULT-NEXT:                 write<u64>(%5, read<u64>(%16));
// DEFAULT-NEXT:                 let %17: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                 let %18: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%17), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%6, read<i32>(%18));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<u64>(read<u64>(%5), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))));
// DEFAULT-NEXT:         return read<i32>(%6);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @baz(%9 x: u64, %10 y: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%10), const<i32>(0))
// DEFAULT-NEXT:             return call<i32, signature=fn(i64) -> i32>(%0, reinterpret<i64, reason=explicit, fits=unknown>(read<u64>(%9)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             return call<i32, signature=fn(u64) -> i32>(%4, read<u64>(%9));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
