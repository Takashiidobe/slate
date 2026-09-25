/* PR tree-optimization/109778 */

int a, b, c, d, *e = &c;

static inline unsigned foo(unsigned char x) {
  x = 1 | x << 1;
  x = x >> 4 | x << 4;
  return x;
}

static inline void bar(unsigned x) { *e = 8 > foo(x + 86) - 86; }

int main() {
  d = a && b;
  bar(d + 4);
  if (c != 1)
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
// DEFAULT-NEXT:     global %0 a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 d: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 e: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%2) [linkage=external];
// DEFAULT-NEXT:     fn %5 @foo(%6 x: u8) -> u32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<u8>(%6, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(or<i32>(const<i32>(1), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%6))), const<i32>(1))))));
// DEFAULT-NEXT:         write<u8>(%6, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(or<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%6))), const<i32>(4)), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%6))), const<i32>(4))))));
// DEFAULT-NEXT:         return widen<u32, reason=return>(read<u8>(%6));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @bar(%8 x: u32) -> void [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%4)), from_bool<i32, reason=assign>(gt<u32>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(8)), sub<u32, overflow=wrap>(call<u32, signature=fn(u8) -> u32>(%5, truncate<u8, reason=arg, fits=unknown>(add<u32, overflow=wrap>(read<u32>(%8), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(86))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(86))))));
// DEFAULT-NEXT:         from_bool<i32, reason=assign>(gt<u32>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(8)), sub<u32, overflow=wrap>(call<u32, signature=fn(u8) -> u32>(%5, truncate<u8, reason=arg, fits=unknown>(add<u32, overflow=wrap>(read<u32>(%8), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(86))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(86)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i32>(%3, from_bool<i32, reason=assign>(logical_and<bool>(ne<i32>(read<i32>(%0), const<i32>(0)), ne<i32>(read<i32>(%1), const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%7, reinterpret<u32, reason=arg, fits=unknown>(add<i32, overflow=ub>(read<i32>(%3), const<i32>(4))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%2), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
