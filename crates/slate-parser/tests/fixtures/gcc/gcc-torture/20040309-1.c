extern void abort();

int foo(unsigned short x) {
  unsigned short y;
  y = x > 32767 ? x - 32768 : 0;
  return y;
}

int main() {
  if (foo(0) != 0)
    abort();
  if (foo(32767) != 0)
    abort();
  if (foo(32768) != 0)
    abort();
  if (foo(32769) != 1)
    abort();
  if (foo(65535) != 32767)
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @foo(%2 x: u16) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 y: u16 [storage=automatic];
// DEFAULT-NEXT:         write<u16>(%3, reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(conditional<i32>(gt<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%2))), const<i32>(32767)), sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%2))), const<i32>(32768)), const<i32>(0)))));
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(widen<u32, reason=return>(read<u16>(%3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u16) -> i32>(%1, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(0)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u16) -> i32>(%1, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(32767)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u16) -> i32>(%1, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(32768)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u16) -> i32>(%1, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(32769)))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u16) -> i32>(%1, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535)))), const<i32>(32767))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
