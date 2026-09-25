/* PR rtl-optimization/68250 */

signed char a, b, h, k, l, m, o;
short       c, d, n;
int         e, f, g, j, q;

void fn1(void) {
  int p = b || a;
  n     = o > 0 || d > 1 >> o ? d : d << o;
  for (; j; j++)
    m = c < 0 || m || c << p;
  l = f + 1;
  for (; f < 1; f = 1)
    k = h + 1;
}

__attribute__((noinline, noclone)) void fn2(int k) {
  if (k != 1)
    __builtin_abort();
}

int main() {
  signed char i;
  for (; e < 1; e++) {
    fn1();
    if (k)
      i = k;
    if (i > q)
      g = 0;
  }
  fn2(k);
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
// DEFAULT-NEXT:     global %1 b: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 h: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 k: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 l: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 m: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 o: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 c: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %8 d: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %9 n: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %10 e: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %11 f: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %12 g: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %13 j: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %14 q: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %15 @fn1() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %16 p: i32 [storage=automatic] = from_bool<i32, reason=assign>(logical_or<bool>(ne<i8>(read<i8>(%1), const<i8>(0)), ne<i8>(read<i8>(%0), const<i8>(0))));
// DEFAULT-NEXT:         write<i16>(%9, truncate<i16, reason=assign, fits=unknown>(conditional<i32>(logical_or<bool>(gt<i32>(widen<i32, reason=promotion>(read<i8>(%6)), const<i32>(0)), gt<i32>(widen<i32, reason=promotion>(read<i16>(%8)), shr<i32, amount_out_of_range=ub, fill=sign_extend>(const<i32>(1), widen<i32, reason=promotion>(read<i8>(%6))))), widen<i32, reason=promotion>(read<i16>(%8)), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i32, reason=promotion>(read<i16>(%8)), widen<i32, reason=promotion>(read<i8>(%6))))));
// DEFAULT-NEXT:         for %21
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ne<i32>(read<i32>(%13), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %24: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:                 let %25: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%24), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%13, read<i32>(%25));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(%5, from_bool<i8, reason=assign>(logical_or<bool>(logical_or<bool>(lt<i32>(widen<i32, reason=promotion>(read<i16>(%7)), const<i32>(0)), ne<i8>(read<i8>(%5), const<i8>(0))), ne<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i32, reason=promotion>(read<i16>(%7)), read<i32>(%16)), const<i32>(0)))));
// DEFAULT-NEXT:         write<i8>(%4, truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%11), const<i32>(1))));
// DEFAULT-NEXT:         for %22
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%11), const<i32>(1))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 write<i32>(%11, const<i32>(1));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(%3, truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%2)), const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @fn2(%18 k: i32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%18), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %20 i: i8 [storage=automatic];
// DEFAULT-NEXT:         for %23
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%10), const<i32>(1))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %26: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                 let %27: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%26), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%10, read<i32>(%27));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%15);
// DEFAULT-NEXT:                     if ne<i8>(read<i8>(%3), const<i8>(0))
// DEFAULT-NEXT:                         write<i8>(%20, read<i8>(%3));
// DEFAULT-NEXT:                     if gt<i32>(widen<i32, reason=promotion>(read<i8>(%20)), read<i32>(%14))
// DEFAULT-NEXT:                         write<i32>(%12, const<i32>(0));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%17, widen<i32, reason=arg>(read<i8>(%3)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
