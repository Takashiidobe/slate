/* PR c/96549 */

long c = -1L;
long b = 0L;

int main() {
  if (3L > (short)((c ^= (b = 1L)) * 3L))
    return 0;
  __builtin_abort();
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
// DEFAULT-NEXT:     global %0 c: i64 [storage=static] = neg<i64, overflow=ub>(const<i64>(1)) [linkage=external];
// DEFAULT-NEXT:     global %1 b: i64 [storage=static] = const<i64>(0) [linkage=external];
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %3: i64 [synthetic] = read<i64>(%0);
// DEFAULT-NEXT:         write<i64>(%1, const<i64>(1));
// DEFAULT-NEXT:         let %4: i64 [synthetic] = xor<i64>(read<i64>(%3), const<i64>(1));
// DEFAULT-NEXT:         write<i64>(%0, read<i64>(%4));
// DEFAULT-NEXT:         if gt<i64>(const<i64>(3), widen<i64, reason=usual_arith>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(mul<i64, overflow=ub>(read<i64>(%4), const<i64>(3))))))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
