/* PR middle-end/37780.  */

#define VAL (8 * sizeof(int))

int __attribute__((noinline, noclone)) fooctz(int i) {
  return (i == 0) ? VAL : __builtin_ctz(i);
}

int __attribute__((noinline, noclone)) fooctz2(int i) {
  return (i != 0) ? __builtin_ctz(i) : VAL;
}

unsigned int __attribute__((noinline, noclone)) fooctz3(unsigned int i) {
  return (i > 0) ? __builtin_ctz(i) : VAL;
}

int __attribute__((noinline, noclone)) fooclz(int i) {
  return (i == 0) ? VAL : __builtin_clz(i);
}

int __attribute__((noinline, noclone)) fooclz2(int i) {
  return (i != 0) ? __builtin_clz(i) : VAL;
}

unsigned int __attribute__((noinline, noclone)) fooclz3(unsigned int i) {
  return (i > 0) ? __builtin_clz(i) : VAL;
}

int main(void) {
  if (fooctz(0) != VAL || fooctz2(0) != VAL || fooctz3(0) != VAL ||
      fooclz(0) != VAL || fooclz2(0) != VAL || fooclz3(0) != VAL)
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
// DEFAULT-NEXT:     fn %14 @__builtin_ctz(%13 <unnamed>: u32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %0 @fooctz(%1 i: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(conditional<u64>(eq<i32>(read<i32>(%1), const<i32>(0)), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(4)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(call<i32, signature=fn(u32) -> i32>(%14, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%1))))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @fooctz2(%3 i: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(conditional<u64>(ne<i32>(read<i32>(%3), const<i32>(0)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(call<i32, signature=fn(u32) -> i32>(%14, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%3))))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(4)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @fooctz3(%5 i: u32) -> u32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<u32, reason=return, fits=unknown>(conditional<u64>(gt<u32>(read<u32>(%5), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(call<i32, signature=fn(u32) -> i32>(%14, read<u32>(%5)))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @__builtin_clz(%15 <unnamed>: u32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %6 @fooclz(%7 i: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(conditional<u64>(eq<i32>(read<i32>(%7), const<i32>(0)), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(4)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(call<i32, signature=fn(u32) -> i32>(%16, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%7))))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @fooclz2(%9 i: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(conditional<u64>(ne<i32>(read<i32>(%9), const<i32>(0)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(call<i32, signature=fn(u32) -> i32>(%16, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%9))))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(4)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @fooclz3(%11 i: u32) -> u32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<u32, reason=return, fits=unknown>(conditional<u64>(gt<u32>(read<u32>(%11), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(call<i32, signature=fn(u32) -> i32>(%16, read<u32>(%11)))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %18: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(call<i32, signature=fn(i32) -> i32>(%0, const<i32>(0)))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(4)))
// DEFAULT-NEXT:             write<bool>(%18, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%18, ne<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(call<i32, signature=fn(i32) -> i32>(%2, const<i32>(0)))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(4))));
// DEFAULT-NEXT:         let %19: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%18)
// DEFAULT-NEXT:             write<bool>(%19, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%19, ne<u64>(widen<u64, reason=usual_arith>(call<u32, signature=fn(u32) -> u32>(%4, reinterpret<u32, reason=arg, fits=always>(const<i32>(0)))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(4))));
// DEFAULT-NEXT:         let %20: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%19)
// DEFAULT-NEXT:             write<bool>(%20, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%20, ne<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(call<i32, signature=fn(i32) -> i32>(%6, const<i32>(0)))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(4))));
// DEFAULT-NEXT:         let %21: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%20)
// DEFAULT-NEXT:             write<bool>(%21, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%21, ne<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(call<i32, signature=fn(i32) -> i32>(%8, const<i32>(0)))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(4))));
// DEFAULT-NEXT:         let %22: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%21)
// DEFAULT-NEXT:             write<bool>(%22, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%22, ne<u64>(widen<u64, reason=usual_arith>(call<u32, signature=fn(u32) -> u32>(%10, reinterpret<u32, reason=arg, fits=always>(const<i32>(0)))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(4))));
// DEFAULT-NEXT:         if read<bool>(%22)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%17);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
