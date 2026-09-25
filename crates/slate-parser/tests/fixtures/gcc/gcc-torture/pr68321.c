/* PR rtl-optimization/68321 */

int  e = 1, u = 5, t2, t5, i, k;
int  a[1], b, m;
char n, t;

int fn1(int p1) {
  int g[1];
  for (;;) {
    if (p1 / 3)
      for (; t5;)
        u || n;
    t2 = p1 & 4;
    if (b + 1)
      return 0;
    u = g[0];
  }
}

int main() {
  for (; e >= 0; e--) {
    char c;
    if (!m)
      c = t;
    fn1(c);
  }

  if (a[t2] != 0)
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
// DEFAULT-NEXT:     global %0 e: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %1 u: i32 [storage=static] = const<i32>(5) [linkage=external];
// DEFAULT-NEXT:     global %2 t2: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 t5: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 i: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 k: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 a: array<i32, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %8 m: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %9 n: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %10 t: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %11 @fn1(%12 p1: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %13 g: array<i32, 1> [storage=automatic];
// DEFAULT-NEXT:         for %16
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: omitted
// DEFAULT-NEXT:             increment: omitted
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%12), const<i32>(3)), const<i32>(0))
// DEFAULT-NEXT:                         for %17
// DEFAULT-NEXT:                             init:
// DEFAULT-NEXT:                             condition: ne<i32>(read<i32>(%3), const<i32>(0))
// DEFAULT-NEXT:                             increment: omitted
// DEFAULT-NEXT:                             body:
// DEFAULT-NEXT:                                 logical_or<bool>(ne<i32>(read<i32>(%1), const<i32>(0)), ne<i8>(read<i8>(%9), const<i8>(0)));
// DEFAULT-NEXT:                     write<i32>(%2, and<i32>(read<i32>(%12), const<i32>(4)));
// DEFAULT-NEXT:                     if ne<i32>(add<i32, overflow=ub>(read<i32>(%7), const<i32>(1)), const<i32>(0))
// DEFAULT-NEXT:                         return const<i32>(0);
// DEFAULT-NEXT:                     write<i32>(%1, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%13), const<i32>(0)))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %18
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ge<i32>(read<i32>(%0), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %19: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                 let %20: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%19), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%0, read<i32>(%20));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %15 c: i8 [storage=automatic];
// DEFAULT-NEXT:                     if not<bool>(ne<i32>(read<i32>(%8), const<i32>(0)))
// DEFAULT-NEXT:                         write<i8>(%15, read<i8>(%10));
// DEFAULT-NEXT:                     call<i32, signature=fn(i32) -> i32>(%11, widen<i32, reason=arg>(read<i8>(%15)));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%6), read<i32>(%2)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
