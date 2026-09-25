/* PR tree-optimization/48717 */

extern void abort(void);

int v = 1, w;

unsigned short foo(unsigned short x, unsigned short y) { return x + y; }

void bar(void) { v = foo(~w, w); }

int main() {
  bar();
  if (v != (unsigned short)-1)
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
// DEFAULT-NEXT:     global %1 v: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %2 w: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @foo(%4 x: u16, %5 y: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%4))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%5))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @bar() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%1, reinterpret<i32, reason=assign, fits=unknown>(widen<u32, reason=assign>(call<u16, signature=fn(u16, u16) -> u16>(%3, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(not<i32>(read<i32>(%2)))), reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(read<i32>(%2)))))));
// DEFAULT-NEXT:         reinterpret<i32, reason=assign, fits=unknown>(widen<u32, reason=assign>(call<u16, signature=fn(u16, u16) -> u16>(%3, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(not<i32>(read<i32>(%2)))), reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(read<i32>(%2))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
