unsigned p;

long __attribute__((noinline, noclone)) test(unsigned a) {
  return (long)(p + a) - (long)p;
}

int main() {
  p = (unsigned)-2;
  if (test(0) != 0)
    __builtin_abort();
  if (test(1) != 1)
    __builtin_abort();
  if (test(2) != -(long)(unsigned)-2)
    __builtin_abort();
  p = (unsigned)-1;
  if (test(0) != 0)
    __builtin_abort();
  if (test(1) != -(long)(unsigned)-1)
    __builtin_abort();
  if (test(2) != -(long)(unsigned)-2)
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
// DEFAULT-NEXT:     global %0 p: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %1 @test(%2 a: u32) -> i64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return sub<i64, overflow=ub>(reinterpret<i64, reason=explicit, fits=unknown>(widen<u64, reason=explicit>(add<u32, overflow=wrap>(read<u32>(%0), read<u32>(%2)))), reinterpret<i64, reason=explicit, fits=unknown>(widen<u64, reason=explicit>(read<u32>(%0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<u32>(%0, reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(2))));
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(u32) -> i64>(%1, reinterpret<u32, reason=arg, fits=always>(const<i32>(0))), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(u32) -> i64>(%1, reinterpret<u32, reason=arg, fits=always>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(u32) -> i64>(%1, reinterpret<u32, reason=arg, fits=always>(const<i32>(2))), neg<i64, overflow=ub>(reinterpret<i64, reason=explicit, fits=unknown>(widen<u64, reason=explicit>(reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(2)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         write<u32>(%0, reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(u32) -> i64>(%1, reinterpret<u32, reason=arg, fits=always>(const<i32>(0))), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(u32) -> i64>(%1, reinterpret<u32, reason=arg, fits=always>(const<i32>(1))), neg<i64, overflow=ub>(reinterpret<i64, reason=explicit, fits=unknown>(widen<u64, reason=explicit>(reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(u32) -> i64>(%1, reinterpret<u32, reason=arg, fits=always>(const<i32>(2))), neg<i64, overflow=ub>(reinterpret<i64, reason=explicit, fits=unknown>(widen<u64, reason=explicit>(reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(2)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
