/* PR c++/55137 */

extern void abort(void);

int foo(unsigned int x) { return ((int)(x + 1U) + 1) < (int)x; }

int bar(unsigned int x) { return (int)(x + 1U) + 1; }

int baz(unsigned int x) { return x + 1U; }

int main() {
  if (foo(__INT_MAX__) != (bar(__INT_MAX__) < __INT_MAX__) ||
      foo(__INT_MAX__) != ((int)baz(__INT_MAX__) + 1 < __INT_MAX__))
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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(lt<i32>(add<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_x]]), const<u32>(1))), const<i32>(1)), reinterpret<i32, reason=explicit, fits=unknown>(read<u32>(%[[VALUE_x]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_x_2:[0-9]+]] x: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_x_2]]), const<u32>(1))), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz(%[[VALUE_x_3:[0-9]+]] x: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_x_3]]), const<u32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_foo]], reinterpret<u32, reason=arg, fits=always>(const<i32>(2147483647))), from_bool<i32, reason=promotion>(lt<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_bar]], reinterpret<u32, reason=arg, fits=always>(const<i32>(2147483647))), const<i32>(2147483647))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_foo]], reinterpret<u32, reason=arg, fits=always>(const<i32>(2147483647))), from_bool<i32, reason=promotion>(lt<i32>(add<i32, overflow=ub>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_baz]], reinterpret<u32, reason=arg, fits=always>(const<i32>(2147483647))), const<i32>(1)), const<i32>(2147483647)))));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE0]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
