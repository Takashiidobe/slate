/* PR target/39240 */

extern void abort(void);

__attribute__((noinline)) static int foo1(int x) { return x; }

__attribute__((noinline)) unsigned int bar1(int x) { return foo1(x + 6); }

volatile unsigned long l1 = (unsigned int)-4;

__attribute__((noinline)) static short int foo2(int x) { return x; }

__attribute__((noinline)) unsigned short int bar2(int x) { return foo2(x + 6); }

volatile unsigned long l2 = (unsigned short int)-4;

__attribute__((noinline)) static signed char foo3(int x) { return x; }

__attribute__((noinline)) unsigned char bar3(int x) { return foo3(x + 6); }

volatile unsigned long l3 = (unsigned char)-4;

__attribute__((noinline)) static unsigned int foo4(int x) { return x; }

__attribute__((noinline)) int bar4(int x) { return foo4(x + 6); }

volatile unsigned long l4 = (int)-4;

__attribute__((noinline)) static unsigned short int foo5(int x) { return x; }

__attribute__((noinline)) short int bar5(int x) { return foo5(x + 6); }

volatile unsigned long l5 = (short int)-4;

__attribute__((noinline)) static unsigned char foo6(int x) { return x; }

__attribute__((noinline)) signed char bar6(int x) { return foo6(x + 6); }

volatile unsigned long l6 = (signed char)-4;

int main(void) {
  if (bar1(-10) != l1)
    abort();
  if (bar2(-10) != l2)
    abort();
  if (bar3(-10) != l3)
    abort();
  if (bar4(-10) != l4)
    abort();
  if (bar5(-10) != l5)
    abort();
  if (bar6(-10) != l6)
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
// DEFAULT-NEXT:     global %5 l1: volatile u64 [storage=static] = widen<u64, reason=assign>(reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(4)))) [linkage=external];
// DEFAULT-NEXT:     global %10 l2: volatile u64 [storage=static] = widen<u64, reason=assign>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(4))))) [linkage=external];
// DEFAULT-NEXT:     global %15 l3: volatile u64 [storage=static] = widen<u64, reason=assign>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(4))))) [linkage=external];
// DEFAULT-NEXT:     global %20 l4: volatile u64 [storage=static] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(4)))) [linkage=external];
// DEFAULT-NEXT:     global %25 l5: volatile u64 [storage=static] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(4))))) [linkage=external];
// DEFAULT-NEXT:     global %30 l6: volatile u64 [storage=static] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(4))))) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @foo1(%2 x: i32) -> i32 [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @bar1(%4 x: i32) -> u32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(call<i32, signature=fn(i32) -> i32>(%1, add<i32, overflow=ub>(read<i32>(%4), const<i32>(6))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @foo2(%7 x: i32) -> i16 [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(read<i32>(%7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @bar2(%9 x: i32) -> u16 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(call<i16, signature=fn(i32) -> i16>(%6, add<i32, overflow=ub>(read<i32>(%9), const<i32>(6))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @foo3(%12 x: i32) -> i8 [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i8, reason=return, fits=unknown>(read<i32>(%12));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @bar3(%14 x: i32) -> u8 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u8, reason=return, fits=unknown>(call<i8, signature=fn(i32) -> i8>(%11, add<i32, overflow=ub>(read<i32>(%14), const<i32>(6))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @foo4(%17 x: i32) -> u32 [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(read<i32>(%17));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @bar4(%19 x: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(call<u32, signature=fn(i32) -> u32>(%16, add<i32, overflow=ub>(read<i32>(%19), const<i32>(6))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @foo5(%22 x: i32) -> u16 [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(read<i32>(%22)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @bar5(%24 x: i32) -> i16 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i16, reason=return, fits=unknown>(call<u16, signature=fn(i32) -> u16>(%21, add<i32, overflow=ub>(read<i32>(%24), const<i32>(6))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %26 @foo6(%27 x: i32) -> u8 [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u8, reason=return, fits=unknown>(truncate<i8, reason=return, fits=unknown>(read<i32>(%27)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %28 @bar6(%29 x: i32) -> i8 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i8, reason=return, fits=unknown>(call<u8, signature=fn(i32) -> u8>(%26, add<i32, overflow=ub>(read<i32>(%29), const<i32>(6))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %31 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<u64>(widen<u64, reason=usual_arith>(call<u32, signature=fn(i32) -> u32>(%3, neg<i32, overflow=ub>(const<i32>(10)))), read<u64, volatile>(%5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(i32) -> u16>(%8, neg<i32, overflow=ub>(const<i32>(10))))))), read<u64, volatile>(%10))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i32) -> u8>(%13, neg<i32, overflow=ub>(const<i32>(10))))))), read<u64, volatile>(%15))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(call<i32, signature=fn(i32) -> i32>(%18, neg<i32, overflow=ub>(const<i32>(10))))), read<u64, volatile>(%20))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(widen<i32, reason=promotion>(call<i16, signature=fn(i32) -> i16>(%23, neg<i32, overflow=ub>(const<i32>(10)))))), read<u64, volatile>(%25))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(widen<i32, reason=promotion>(call<i8, signature=fn(i32) -> i8>(%28, neg<i32, overflow=ub>(const<i32>(10)))))), read<u64, volatile>(%30))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
