long long __attribute__((noinline, noclone)) foo(long long ival) {
  if (ival <= 0)
    return -0x7fffffffffffffffL - 1;

  return 0x7fffffffffffffffL;
}

int main(void) {
  if (foo(-1) != (-0x7fffffffffffffffL - 1))
    __builtin_abort();

  if (foo(1) != 0x7fffffffffffffffL)
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
// DEFAULT-NEXT:     fn %0 @foo(%1 ival: i64) -> i64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if le<i64>(read<i64>(%1), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             return sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         return const<i64>(9223372036854775807);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(i64) -> i64>(%0, widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1)))), sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(i64) -> i64>(%0, widen<i64, reason=arg>(const<i32>(1))), const<i64>(9223372036854775807))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
