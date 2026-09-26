/* PR middle-end/29695 */

extern void abort(void);

int           a = 128;
unsigned char b = 128;
long long     c = 0x80000000LL;
unsigned int  d = 0x80000000;

int f1(void) { return (a & 0x80) ? 0x80 : 0; }

int f2(void) { return (b & 0x80) ? 0x80 : 0; }

int f3(void) { return (b & 0x80) ? 0x380 : 0; }

int f4(void) { return (b & 0x80) ? -128 : 0; }

long long f5(void) { return (c & 0x80000000) ? 0x80000000LL : 0LL; }

long long f6(void) { return (d & 0x80000000) ? 0x80000000LL : 0LL; }

long long f7(void) { return (d & 0x80000000) ? 0x380000000LL : 0LL; }

long long f8(void) { return (d & 0x80000000) ? -2147483648LL : 0LL; }

int main(void) {
  if ((char)128 != -128 || (int)0x80000000 != -2147483648)
    return 0;
  if (f1() != 128)
    abort();
  if (f2() != 128)
    abort();
  if (f3() != 896)
    abort();
  if (f4() != -128)
    abort();
  if (f5() != 0x80000000LL)
    abort();
  if (f6() != 0x80000000LL)
    abort();
  if (f7() != 0x380000000LL)
    abort();
  if (f8() != -2147483648LL)
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
// DEFAULT-NEXT:     global %1 a: i32 [storage=static] = const<i32>(128) [linkage=external];
// DEFAULT-NEXT:     global %2 b: u8 [storage=static] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(128))) [linkage=external];
// DEFAULT-NEXT:     global %3 c: i64 [storage=static] = const<i64>(2147483648) [linkage=external];
// DEFAULT-NEXT:     global %4 d: u32 [storage=static] = const<u32>(2147483648) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %5 @f1() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(ne<i32>(and<i32>(read<i32>(%1), const<i32>(128)), const<i32>(0)), const<i32>(128), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @f2() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(ne<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%2))), const<i32>(128)), const<i32>(0)), const<i32>(128), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @f3() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(ne<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%2))), const<i32>(128)), const<i32>(0)), const<i32>(896), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @f4() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(ne<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%2))), const<i32>(128)), const<i32>(0)), neg<i32, overflow=ub>(const<i32>(128)), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @f5() -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i64>(ne<i64>(and<i64>(read<i64>(%3), reinterpret<i64, reason=usual_arith, fits=unknown>(widen<u64, reason=usual_arith>(const<u32>(2147483648)))), const<i64>(0)), const<i64>(2147483648), const<i64>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @f6() -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i64>(ne<u32>(and<u32>(read<u32>(%4), const<u32>(2147483648)), const<u32>(0)), const<i64>(2147483648), const<i64>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @f7() -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i64>(ne<u32>(and<u32>(read<u32>(%4), const<u32>(2147483648)), const<u32>(0)), const<i64>(15032385536), const<i64>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @f8() -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i64>(ne<u32>(and<u32>(read<u32>(%4), const<u32>(2147483648)), const<u32>(0)), neg<i64, overflow=ub>(const<i64>(2147483648)), const<i64>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(128))), neg<i32, overflow=ub>(const<i32>(128))), ne<i64>(widen<i64, reason=usual_arith>(reinterpret<i32, reason=explicit, fits=unknown>(const<u32>(2147483648))), neg<i64, overflow=ub>(const<i64>(2147483648))))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn() -> i32>(%5), const<i32>(128))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn() -> i32>(%6), const<i32>(128))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn() -> i32>(%7), const<i32>(896))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn() -> i32>(%8), neg<i32, overflow=ub>(const<i32>(128)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn() -> i64>(%9), const<i64>(2147483648))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn() -> i64>(%10), const<i64>(2147483648))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn() -> i64>(%11), const<i64>(15032385536))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn() -> i64>(%12), neg<i64, overflow=ub>(const<i64>(2147483648)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
