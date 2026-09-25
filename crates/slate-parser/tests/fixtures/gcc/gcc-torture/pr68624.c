int b, c, d, e = 1, f, g, h, j;

static int fn1() {
  int a = c;
  if (h)
    return 9;
  g = (c || b) % e;
  if ((g || f) && b)
    return 9;
  e = d;
  for (c = 0; c > -4; c--)
    ;
  if (d)
    c--;
  j = c;
  return d;
}

int main() {
  fn1();

  if (c != -4)
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
// DEFAULT-NEXT:     global %0 b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 d: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 e: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %4 f: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 g: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 h: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 j: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %8 @fn1() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %9 a: i32 [storage=automatic] = read<i32>(%1);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%6), const<i32>(0))
// DEFAULT-NEXT:             return const<i32>(9);
// DEFAULT-NEXT:         write<i32>(%5, rem<i32, by_zero=ub, min_by_neg_one=ub>(from_bool<i32, reason=promotion>(logical_or<bool>(ne<i32>(read<i32>(%1), const<i32>(0)), ne<i32>(read<i32>(%0), const<i32>(0)))), read<i32>(%3)));
// DEFAULT-NEXT:         if logical_and<bool>(logical_or<bool>(ne<i32>(read<i32>(%5), const<i32>(0)), ne<i32>(read<i32>(%4), const<i32>(0))), ne<i32>(read<i32>(%0), const<i32>(0)))
// DEFAULT-NEXT:             return const<i32>(9);
// DEFAULT-NEXT:         write<i32>(%3, read<i32>(%2));
// DEFAULT-NEXT:         for %11
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%1, const<i32>(0));
// DEFAULT-NEXT:             condition: gt<i32>(read<i32>(%1), neg<i32, overflow=ub>(const<i32>(4)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %12: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:                 let %13: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%12), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%1, read<i32>(%13));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%2), const<i32>(0))
// DEFAULT-NEXT:             let %14: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:             let %15: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%14), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%1, read<i32>(%15));
// DEFAULT-NEXT:         write<i32>(%7, read<i32>(%1));
// DEFAULT-NEXT:         return read<i32>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%8);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), neg<i32, overflow=ub>(const<i32>(4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
