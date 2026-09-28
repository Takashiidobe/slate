/* PR rtl-optimization/80501 */

signed char v = 0;

static signed char foo(int x, int y) { return x << y; }

__attribute__((noinline, noclone)) int bar(void) {
  return foo(v >= 0, __CHAR_BIT__ - 1) >= 1;
}

int main() {
  if (sizeof(int) > sizeof(char) && bar() != 0)
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
// DEFAULT-NEXT:     global %0 v: i8 [storage=static] = truncate<i8, reason=assign, fits=always>(const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     fn %1 @foo(%2 x: i32, %3 y: i32) -> i8 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i8, reason=return, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i32>(%2), read<i32>(%3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @bar() -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ge<i32>(widen<i32, reason=promotion>(call<i8, signature=fn(i32, i32) -> i8>(%1, from_bool<i32, reason=arg>(ge<i32>(widen<i32, reason=promotion>(read<i8>(%0)), const<i32>(0))), sub<i32, overflow=ub>(const<i32>(8), const<i32>(1)))), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %7: bool [synthetic];
// DEFAULT-NEXT:         if gt<u64>(const<u64>(4), const<u64>(1))
// DEFAULT-NEXT:             write<bool>(%7, ne<i32>(call<i32, signature=fn() -> i32>(%4), const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%7, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%7)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
