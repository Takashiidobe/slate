/* PR tree-optimization/78720 */

__attribute__((noinline, noclone)) long int foo(signed char x) {
  return x < 0 ? 0x80000L : 0L;
}

__attribute__((noinline, noclone)) long int bar(signed char x) {
  return x < 0 ? 0x80L : 0L;
}

__attribute__((noinline, noclone)) long int baz(signed char x) {
  return x < 0 ? 0x20L : 0L;
}

int main() {
  if (foo(-1) != 0x80000L || bar(-1) != 0x80L || baz(-1) != 0x20L ||
      foo(0) != 0L || bar(0) != 0L || baz(0) != 0L || foo(31) != 0L ||
      bar(31) != 0L || baz(31) != 0L)
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
// DEFAULT-NEXT:     fn %0 @foo(%1 x: i8) -> i64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i64>(lt<i32>(widen<i32, reason=promotion>(read<i8>(%1)), const<i32>(0)), const<i64>(524288), const<i64>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @bar(%3 x: i8) -> i64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i64>(lt<i32>(widen<i32, reason=promotion>(read<i8>(%3)), const<i32>(0)), const<i64>(128), const<i64>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @baz(%5 x: i8) -> i64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i64>(lt<i32>(widen<i32, reason=promotion>(read<i8>(%5)), const<i32>(0)), const<i64>(32), const<i64>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %8: bool [synthetic];
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(i8) -> i64>(%0, truncate<i8, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), const<i64>(524288))
// DEFAULT-NEXT:             write<bool>(%8, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%8, ne<i64>(call<i64, signature=fn(i8) -> i64>(%2, truncate<i8, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), const<i64>(128)));
// DEFAULT-NEXT:         let %9: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%8)
// DEFAULT-NEXT:             write<bool>(%9, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%9, ne<i64>(call<i64, signature=fn(i8) -> i64>(%4, truncate<i8, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), const<i64>(32)));
// DEFAULT-NEXT:         let %10: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%9)
// DEFAULT-NEXT:             write<bool>(%10, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%10, ne<i64>(call<i64, signature=fn(i8) -> i64>(%0, truncate<i8, reason=arg, fits=always>(const<i32>(0))), const<i64>(0)));
// DEFAULT-NEXT:         let %11: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%10)
// DEFAULT-NEXT:             write<bool>(%11, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%11, ne<i64>(call<i64, signature=fn(i8) -> i64>(%2, truncate<i8, reason=arg, fits=always>(const<i32>(0))), const<i64>(0)));
// DEFAULT-NEXT:         let %12: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%11)
// DEFAULT-NEXT:             write<bool>(%12, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%12, ne<i64>(call<i64, signature=fn(i8) -> i64>(%4, truncate<i8, reason=arg, fits=always>(const<i32>(0))), const<i64>(0)));
// DEFAULT-NEXT:         let %13: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%12)
// DEFAULT-NEXT:             write<bool>(%13, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%13, ne<i64>(call<i64, signature=fn(i8) -> i64>(%0, truncate<i8, reason=arg, fits=always>(const<i32>(31))), const<i64>(0)));
// DEFAULT-NEXT:         let %14: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%13)
// DEFAULT-NEXT:             write<bool>(%14, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%14, ne<i64>(call<i64, signature=fn(i8) -> i64>(%2, truncate<i8, reason=arg, fits=always>(const<i32>(31))), const<i64>(0)));
// DEFAULT-NEXT:         let %15: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%14)
// DEFAULT-NEXT:             write<bool>(%15, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%15, ne<i64>(call<i64, signature=fn(i8) -> i64>(%4, truncate<i8, reason=arg, fits=always>(const<i32>(31))), const<i64>(0)));
// DEFAULT-NEXT:         if read<bool>(%15)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%7);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
