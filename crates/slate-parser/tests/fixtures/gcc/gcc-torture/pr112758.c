/* PR rtl-optimization/112758 */

int a = -__INT_MAX__ - 1;

int main() {
  if (-__INT_MAX__ - 1U == 0x80000000ULL) {
    unsigned long long b = 0xffff00ffffffffffULL;
    if ((b & a) != 0xffff00ff80000000ULL)
      __builtin_abort();
  }
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
// DEFAULT-NEXT:     global %0 a: i32 [storage=static] = sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     fn %1 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if eq<u64>(widen<u64, reason=usual_arith>(sub<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(2147483647))), const<u32>(1))), const<u64>(2147483648))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %2 b: u64 [storage=automatic] = const<u64>(18446463698244468735);
// DEFAULT-NEXT:                 if ne<u64>(and<u64>(read<u64>(%2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%0)))), const<u64>(18446463696096985088))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
