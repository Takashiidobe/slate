/* { dg-require-effective-target int32plus } */
void abort(void);
void exit(int);

int __attribute__((noinline)) foo(short x, unsigned short y) { return x * y; }

int __attribute__((noinline)) bar(unsigned short x, short y) { return x * y; }

int main() {
  if (foo(-2, 0xffff) != -131070)
    abort();
  if (foo(2, 0xffff) != 131070)
    abort();
  if (foo(-32768, 0x8000) != -1073741824)
    abort();
  if (foo(32767, 0x8000) != 1073709056)
    abort();

  if (bar(0xffff, -2) != -131070)
    abort();
  if (bar(0xffff, 2) != 131070)
    abort();
  if (bar(0x8000, -32768) != -1073741824)
    abort();
  if (bar(0x8000, 32767) != 1073709056)
    abort();

  exit(0);
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
// DEFAULT-NEXT:     fn %1 @exit(%9 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo(%3 x: i16, %4 y: u16) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%3)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @bar(%6 x: u16, %7 y: i16) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%6))), widen<i32, reason=promotion>(read<i16>(%7)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i16, u16) -> i32>(%2, truncate<i16, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(2))), reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535)))), neg<i32, overflow=ub>(const<i32>(131070)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i16, u16) -> i32>(%2, truncate<i16, reason=arg, fits=always>(const<i32>(2)), reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535)))), const<i32>(131070))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i16, u16) -> i32>(%2, truncate<i16, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(32768))), reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(32768)))), neg<i32, overflow=ub>(const<i32>(1073741824)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i16, u16) -> i32>(%2, truncate<i16, reason=arg, fits=always>(const<i32>(32767)), reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(32768)))), const<i32>(1073709056))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u16, i16) -> i32>(%5, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535))), truncate<i16, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(2)))), neg<i32, overflow=ub>(const<i32>(131070)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u16, i16) -> i32>(%5, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535))), truncate<i16, reason=arg, fits=always>(const<i32>(2))), const<i32>(131070))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u16, i16) -> i32>(%5, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(32768))), truncate<i16, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(32768)))), neg<i32, overflow=ub>(const<i32>(1073741824)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u16, i16) -> i32>(%5, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(32768))), truncate<i16, reason=arg, fits=always>(const<i32>(32767))), const<i32>(1073709056))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
