/* PR target/94567 */

volatile int   a = 1, b;
short          c, d = 4, f = 2, g;
unsigned short e = 53736;

int foo(int i, int j) { return i && j ? 0 : i + j; }

int main() {
  for (; a; a = 0) {
    unsigned short k = e;
    g                = k >> 3;
    if (foo(g < (f || c), b))
      d = 0;
  }
  if (d != 4)
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
// DEFAULT-NEXT:     global %0 a: volatile i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %1 b: volatile i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 c: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 d: i16 [storage=static] = truncate<i16, reason=assign, fits=always>(const<i32>(4)) [linkage=external];
// DEFAULT-NEXT:     global %4 f: i16 [storage=static] = truncate<i16, reason=assign, fits=always>(const<i32>(2)) [linkage=external];
// DEFAULT-NEXT:     global %5 g: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 e: u16 [storage=static] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(const<i32>(53736))) [linkage=external];
// DEFAULT-NEXT:     fn %7 @foo(%8 i: i32, %9 j: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(logical_and<bool>(ne<i32>(read<i32>(%8), const<i32>(0)), ne<i32>(read<i32>(%9), const<i32>(0))), const<i32>(0), add<i32, overflow=ub>(read<i32>(%8), read<i32>(%9)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %12
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ne<i32>(read<i32, volatile>(%0), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 write<i32, volatile>(%0, const<i32>(0));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %11 k: u16 [storage=automatic] = read<u16>(%6);
// DEFAULT-NEXT:                     write<i16>(%5, truncate<i16, reason=assign, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%11))), const<i32>(3))));
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%7, from_bool<i32, reason=arg>(lt<i32>(widen<i32, reason=promotion>(read<i16>(%5)), from_bool<i32, reason=promotion>(logical_or<bool>(ne<i16>(read<i16>(%4), const<i16>(0)), ne<i16>(read<i16>(%2), const<i16>(0)))))), read<i32, volatile>(%1)), const<i32>(0))
// DEFAULT-NEXT:                         write<i16>(%3, truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%3)), const<i32>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
