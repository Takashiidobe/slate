/* PR rtl-optimization/68249 */

int  a, b, c, g, k, l, m, n;
char h;

void fn1() {
  for (; k; k++) {
    m = b || c < 0 || c > 1 ?: c;
    g = l = n || m < 0 || (m > 1) > 1 >> m ?: 1 << m;
  }
  l = b + 1;
  for (; b < 1; b++)
    h = a + 1;
}

int main() {
  char j;
  for (; a < 1; a++) {
    fn1();
    if (h)
      j = h;
    if (j > c)
      g = 0;
  }

  if (h != 1)
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
// DEFAULT-NEXT:     global %3 g: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 k: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 l: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 m: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 n: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %8 h: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %9 @fn1() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         for %12
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ne<i32>(read<i32>(%4), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %17: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                 let %18: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%17), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%4, read<i32>(%18));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %13: bool [synthetic] = logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(%1), const<i32>(0)), lt<i32>(read<i32>(%2), const<i32>(0))), gt<i32>(read<i32>(%2), const<i32>(1)));
// DEFAULT-NEXT:                     write<i32>(%6, conditional<i32>(ne<i32>(read<bool>(%13), const<i32>(0)), read<bool>(%13), read<i32>(%2)));
// DEFAULT-NEXT:                     let %14: bool [synthetic] = logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(%7), const<i32>(0)), lt<i32>(read<i32>(%6), const<i32>(0))), gt<i32>(from_bool<i32, reason=promotion>(gt<i32>(read<i32>(%6), const<i32>(1))), shr<i32, amount_out_of_range=ub, fill=sign_extend>(const<i32>(1), read<i32>(%6))));
// DEFAULT-NEXT:                     write<i32>(%5, conditional<i32>(ne<i32>(read<bool>(%14), const<i32>(0)), read<bool>(%14), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), read<i32>(%6))));
// DEFAULT-NEXT:                     write<i32>(%3, conditional<i32>(ne<i32>(read<bool>(%14), const<i32>(0)), read<bool>(%14), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), read<i32>(%6))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         write<i32>(%5, add<i32, overflow=ub>(read<i32>(%1), const<i32>(1)));
// DEFAULT-NEXT:         for %15
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%1), const<i32>(1))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %19: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:                 let %20: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%19), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%1, read<i32>(%20));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(%8, truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%0), const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %11 j: i8 [storage=automatic];
// DEFAULT-NEXT:         for %16
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%0), const<i32>(1))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %21: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                 let %22: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%21), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%0, read<i32>(%22));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:                     if ne<i8>(read<i8>(%8), const<i8>(0))
// DEFAULT-NEXT:                         write<i8>(%11, read<i8>(%8));
// DEFAULT-NEXT:                     if gt<i32>(widen<i32, reason=promotion>(read<i8>(%11)), read<i32>(%2))
// DEFAULT-NEXT:                         write<i32>(%3, const<i32>(0));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%8)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
