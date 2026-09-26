#if __SIZEOF_INT__ < 4
__extension__ typedef __UINT32_TYPE__ uint32_t;
__extension__ typedef __INT32_TYPE__  int32_t;
#else
typedef unsigned uint32_t;
typedef int      int32_t;
#endif

extern void abort(void);

__attribute__((noinline, noclone)) unsigned long long f1(int32_t x) {
  return ((unsigned long long)x) << 4;
}

__attribute__((noinline, noclone)) long long f2(uint32_t x) {
  return ((long long)x) << 4;
}

__attribute__((noinline, noclone)) unsigned long long f3(uint32_t x) {
  return ((unsigned long long)x) << 4;
}

__attribute__((noinline, noclone)) long long f4(int32_t x) {
  return ((long long)x) << 4;
}

int main() {
  if (f1(0xf0000000) != 0xffffffff00000000)
    abort();
  if (f2(0xf0000000) != 0xf00000000)
    abort();
  if (f3(0xf0000000) != 0xf00000000)
    abort();
  if (f4(0xf0000000) != 0xffffffff00000000)
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
// DEFAULT-NEXT:     type @type0 uint32_t = u32;
// DEFAULT-NEXT:     type @type1 int32_t = i32;
// DEFAULT-NEXT:     fn %2 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @f1(%4 x: i32) -> u64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(read<i32>(%4))), const<i32>(4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @f2(%6 x: u32) -> i64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i64, reason=explicit, fits=unknown>(widen<u64, reason=explicit>(read<u32>(%6))), const<i32>(4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @f3(%8 x: u32) -> u64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shl<u64, overflow=wrap, amount_out_of_range=ub>(widen<u64, reason=explicit>(read<u32>(%8)), const<i32>(4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @f4(%10 x: i32) -> i64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i64, reason=explicit>(read<i32>(%10)), const<i32>(4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i32) -> u64>(%3, reinterpret<i32, reason=arg, fits=unknown>(const<u32>(4026531840))), const<u64>(18446744069414584320))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(u32) -> i64>(%5, const<u32>(4026531840)), const<i64>(64424509440))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(u32) -> u64>(%7, const<u32>(4026531840)), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(64424509440)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i32) -> i64>(%9, reinterpret<i32, reason=arg, fits=unknown>(const<u32>(4026531840)))), const<u64>(18446744069414584320))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
