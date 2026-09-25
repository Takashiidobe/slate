
int a, b = 2, c = 2;
int main() {
  b = ~(1 % (a ^ (b - (1 && c) || c & b)));
  if (b < -1)
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
// DEFAULT-NEXT:     global %1 b: i32 [storage=static] = const<i32>(2) [linkage=external];
// DEFAULT-NEXT:     global %2 c: i32 [storage=static] = const<i32>(2) [linkage=external];
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i32>(%1, not<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(1), xor<i32>(read<i32>(%0), from_bool<i32, reason=promotion>(logical_or<bool>(ne<i32>(sub<i32, overflow=ub>(read<i32>(%1), from_bool<i32, reason=promotion>(logical_and<bool>(ne<i32>(const<i32>(1), const<i32>(0)), ne<i32>(read<i32>(%2), const<i32>(0))))), const<i32>(0)), ne<i32>(and<i32>(read<i32>(%2), read<i32>(%1)), const<i32>(0))))))));
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%1), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
