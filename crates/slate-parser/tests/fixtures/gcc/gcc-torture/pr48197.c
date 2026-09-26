/* PR c/48197 */

extern void abort(void);
static int  y = 0x8000;

int main() {
  unsigned int x = (short)y;
  if (sizeof(0LL) == sizeof(0U))
    return 0;
  if (0LL > (0U ^ (short)-0x8000))
    abort();
  if (0LL > (0U ^ x))
    abort();
  if (0LL > (0U ^ (short)y))
    abort();
  if ((0U ^ (short)-0x8000) < 0LL)
    abort();
  if ((0U ^ x) < 0LL)
    abort();
  if ((0U ^ (short)y) < 0LL)
    abort();
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
// DEFAULT-NEXT:     global %1 y: i32 [storage=static] = const<i32>(32768) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %3 x: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=unknown>(widen<i32, reason=assign>(truncate<i16, reason=explicit, fits=unknown>(read<i32>(%1))));
// DEFAULT-NEXT:         if eq<u64>(const<u64>(8), const<u64>(4))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         if gt<i64>(const<i64>(0), reinterpret<i64, reason=usual_arith, fits=unknown>(widen<u64, reason=usual_arith>(xor<u32>(const<u32>(0), reinterpret<u32, reason=usual_arith, fits=unknown>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(32768)))))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if gt<i64>(const<i64>(0), reinterpret<i64, reason=usual_arith, fits=unknown>(widen<u64, reason=usual_arith>(xor<u32>(const<u32>(0), read<u32>(%3)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if gt<i64>(const<i64>(0), reinterpret<i64, reason=usual_arith, fits=unknown>(widen<u64, reason=usual_arith>(xor<u32>(const<u32>(0), reinterpret<u32, reason=usual_arith, fits=unknown>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(read<i32>(%1))))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if lt<i64>(reinterpret<i64, reason=usual_arith, fits=unknown>(widen<u64, reason=usual_arith>(xor<u32>(const<u32>(0), reinterpret<u32, reason=usual_arith, fits=unknown>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(32768)))))))), const<i64>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if lt<i64>(reinterpret<i64, reason=usual_arith, fits=unknown>(widen<u64, reason=usual_arith>(xor<u32>(const<u32>(0), read<u32>(%3)))), const<i64>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if lt<i64>(reinterpret<i64, reason=usual_arith, fits=unknown>(widen<u64, reason=usual_arith>(xor<u32>(const<u32>(0), reinterpret<u32, reason=usual_arith, fits=unknown>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(read<i32>(%1))))))), const<i64>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
