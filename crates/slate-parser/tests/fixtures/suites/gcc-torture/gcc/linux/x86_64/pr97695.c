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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: ptr<ptr<i32>> [storage=static] = addr_of<ptr<ptr<i32>>>(%[[VALUE_a]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_g:[0-9]+]] g: i32, %[[VALUE_h:[0-9]+]] h: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(logical_or<bool>(not<bool>(ne<i32>(read<i32>(%[[VALUE_h]]), const<i32>(0))), logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_g]]), const<i32>(0)), eq<i32>(read<i32>(%[[VALUE_h]]), const<i32>(1)))), const<i32>(0), div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_g]]), read<i32>(%[[VALUE_h]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_i:[0-9]+]] @i(%[[VALUE_g_2:[0-9]+]] g: i32) -> ptr<void> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         while %[[VALUE0:[0-9]+]] lt<i32>(read<i32>(%[[VALUE_e]]), const<i32>(2))
// DEFAULT-NEXT:             if not<bool>(ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_f]], read<i32>(%[[VALUE_g_2]]), const<i32>(9)), const<i32>(0)))
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     while %[[VALUE1:[0-9]+]] ne<i32>(read<i32>(%[[VALUE_b]]), const<i32>(0))
// DEFAULT-NEXT:                         ;
// DEFAULT-NEXT:                     return null<ptr<void>>;
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return null<ptr<void>>;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_j:[0-9]+]] @j() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(i32) -> ptr<void>>(%[[VALUE_i]], const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i32>>(deref(read<ptr<ptr<i32>>>(%[[VALUE_c]])), pointer_cast<ptr<i32>, reason=assign>(call<ptr<void>, signature=fn(i32) -> ptr<void>>(%[[VALUE_i]], read<i32>(%[[VALUE_d]]))));
// DEFAULT-NEXT:         pointer_cast<ptr<i32>, reason=assign>(call<ptr<void>, signature=fn(i32) -> ptr<void>>(%[[VALUE_i]], read<i32>(%[[VALUE_d]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_j]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
