/* PR middle-end/37103 */

extern void abort(void);

void foo(unsigned short x) {
  signed char y = -1;
  if (x == y)
    abort();
}

void bar(unsigned short x) {
  unsigned char y = -1;
  if (x == y)
    abort();
}

int main(void) {
  if (sizeof(int) == sizeof(short))
    return 0;
  foo(-1);
  if (sizeof(short) > 1)
    bar(-1);
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @foo(%2 x: u16) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %3 y: i8 [storage=automatic] = truncate<i8, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         if eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%2))), widen<i32, reason=promotion>(read<i8>(%3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @bar(%5 x: u16) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %6 y: u8 [storage=automatic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         if eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%5))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%6))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if eq<u64>(const<u64>(4), const<u64>(2))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         call<void, signature=fn(u16) -> void>(%1, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         if gt<u64>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn(u16) -> void>(%4, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
