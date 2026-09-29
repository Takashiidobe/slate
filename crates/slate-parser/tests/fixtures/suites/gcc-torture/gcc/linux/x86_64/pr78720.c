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
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: i8) -> i64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i64>(lt<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_x]])), const<i32>(0)), const<i64>(524288), const<i64>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_x_2:[0-9]+]] x: i8) -> i64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i64>(lt<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_x_2]])), const<i32>(0)), const<i64>(128), const<i64>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz(%[[VALUE_x_3:[0-9]+]] x: i8) -> i64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i64>(lt<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_x_3]])), const<i32>(0)), const<i64>(32), const<i64>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(i8) -> i64>(%[[VALUE_foo]], truncate<i8, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), const<i64>(524288))
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], ne<i64>(call<i64, signature=fn(i8) -> i64>(%[[VALUE_bar]], truncate<i8, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), const<i64>(128)));
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE0]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE1]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE1]], ne<i64>(call<i64, signature=fn(i8) -> i64>(%[[VALUE_baz]], truncate<i8, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), const<i64>(32)));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE1]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE2]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE2]], ne<i64>(call<i64, signature=fn(i8) -> i64>(%[[VALUE_foo]], truncate<i8, reason=arg, fits=always>(const<i32>(0))), const<i64>(0)));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE2]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE3]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE3]], ne<i64>(call<i64, signature=fn(i8) -> i64>(%[[VALUE_bar]], truncate<i8, reason=arg, fits=always>(const<i32>(0))), const<i64>(0)));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE3]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE4]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE4]], ne<i64>(call<i64, signature=fn(i8) -> i64>(%[[VALUE_baz]], truncate<i8, reason=arg, fits=always>(const<i32>(0))), const<i64>(0)));
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE4]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE5]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE5]], ne<i64>(call<i64, signature=fn(i8) -> i64>(%[[VALUE_foo]], truncate<i8, reason=arg, fits=always>(const<i32>(31))), const<i64>(0)));
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE5]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE6]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE6]], ne<i64>(call<i64, signature=fn(i8) -> i64>(%[[VALUE_bar]], truncate<i8, reason=arg, fits=always>(const<i32>(31))), const<i64>(0)));
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE6]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE7]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE7]], ne<i64>(call<i64, signature=fn(i8) -> i64>(%[[VALUE_baz]], truncate<i8, reason=arg, fits=always>(const<i32>(31))), const<i64>(0)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE7]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
