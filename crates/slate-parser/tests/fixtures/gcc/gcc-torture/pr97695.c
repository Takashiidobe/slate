int *a, b, **c = &a, d, e;

int f(int g, int h) { return !h || (g && h == 1) ? 0 : g / h; }

static void *i(int g) {
  while (e < 2)
    if (!f(g, 9)) {
      while (b)
        ;
      return 0;
    }
  return 0;
}

void j() {
  i(1);
  *c = i(d);
}

int main() {
  j();
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
// DEFAULT-NEXT:     global %0 a: ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 c: ptr<ptr<i32>> [storage=static] = addr_of<ptr<ptr<i32>>>(%0) [linkage=external];
// DEFAULT-NEXT:     global %3 d: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 e: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %5 @f(%6 g: i32, %7 h: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(logical_or<bool>(not<bool>(ne<i32>(read<i32>(%7), const<i32>(0))), logical_and<bool>(ne<i32>(read<i32>(%6), const<i32>(0)), eq<i32>(read<i32>(%7), const<i32>(1)))), const<i32>(0), div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%6), read<i32>(%7)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @i(%9 g: i32) -> ptr<void> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         while %12 lt<i32>(read<i32>(%4), const<i32>(2))
// DEFAULT-NEXT:             if not<bool>(ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%5, read<i32>(%9), const<i32>(9)), const<i32>(0)))
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     while %13 ne<i32>(read<i32>(%1), const<i32>(0))
// DEFAULT-NEXT:                         ;
// DEFAULT-NEXT:                     return null<ptr<void>>;
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return null<ptr<void>>;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @j() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(i32) -> ptr<void>>(%8, const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i32>>(deref(read<ptr<ptr<i32>>>(%2)), pointer_cast<ptr<i32>, reason=assign>(call<ptr<void>, signature=fn(i32) -> ptr<void>>(%8, read<i32>(%3))));
// DEFAULT-NEXT:         pointer_cast<ptr<i32>, reason=assign>(call<ptr<void>, signature=fn(i32) -> ptr<void>>(%8, read<i32>(%3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
