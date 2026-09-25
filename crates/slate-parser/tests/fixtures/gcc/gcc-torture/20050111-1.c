/* PR middle-end/19084, rtl-optimization/19348 */

unsigned int foo(unsigned long long x) {
  unsigned int u;

  if (x == 0)
    return 0;
  u = (unsigned int)(x >> 32);
  return u;
}

unsigned long long bar(unsigned short x) { return (unsigned long long)x << 32; }

extern void abort(void);

int main(void) {
  if (sizeof(long long) != 8)
    return 0;

  if (foo(0) != 0)
    abort();
  if (foo(0xffffffffULL) != 0)
    abort();
  if (foo(0x25ff00ff00ULL) != 0x25)
    abort();
  if (bar(0) != 0)
    abort();
  if (bar(0x25) != 0x2500000000ULL)
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
// DEFAULT-NEXT:     fn %0 @foo(%1 x: u64) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %2 u: u32 [storage=automatic];
// DEFAULT-NEXT:         if eq<u64>(read<u64>(%1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             return reinterpret<u32, reason=return, fits=always>(const<i32>(0));
// DEFAULT-NEXT:         write<u32>(%2, truncate<u32, reason=explicit, fits=unknown>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%1), const<i32>(32))));
// DEFAULT-NEXT:         return read<u32>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @bar(%4 x: u16) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shl<u64, overflow=wrap, amount_out_of_range=ub>(widen<u64, reason=explicit>(read<u16>(%4)), const<i32>(32));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<u64>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u64) -> u32>(%0, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u64) -> u32>(%0, const<u64>(4294967295)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u64) -> u32>(%0, const<u64>(163192045312)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(37)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(u16) -> u64>(%3, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(0)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(u16) -> u64>(%3, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(37)))), const<u64>(158913789952))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
