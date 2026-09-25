/* PR middle-end/78622 - [7 Regression] -Wformat-overflow/-fprintf-return-value
   incorrect with overflow/wrapping
   { dg-skip-if "Requires %hhd format" { hppa*-*-hpux* } }
   { dg-require-effective-target c99_runtime }
   { dg-additional-options "-Wformat-overflow=2" } */

__attribute__((noinline, noclone)) int foo(int x) {
  if (x < 4096 + 8 || x >= 4096 + 256 + 8)
    return -1;

  char buf[5];
  int  n = __builtin_snprintf(buf, sizeof buf, "%hhd", x + 1);
  __builtin_printf("\"%hhd\" => %i\n", x + 1, n);
  return n;
}

int main(void) {
  if (__SCHAR_MAX__ != 127 || __CHAR_BIT__ != 8 || __SIZEOF_INT__ != 4)
    return 0;

  if (foo(4095 + 9) != 1 || foo(4095 + 32) != 2 || foo(4095 + 127) != 3 ||
      foo(4095 + 128) != 4 || foo(4095 + 240) != 3 || foo(4095 + 248) != 2 ||
      foo(4095 + 255) != 2 || foo(4095 + 256) != 1)
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
// DEFAULT-NEXT:     global %5 .str5: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([37, 104, 104, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %6 .str6: array<i8, 14> [storage=static] = code_units<array<i8, 14>>([34, 37, 104, 104, 100, 34, 32, 61, 62, 32, 37, 105, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @foo(%1 x: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_or<bool>(lt<i32>(read<i32>(%1), add<i32, overflow=ub>(const<i32>(4096), const<i32>(8))), ge<i32>(read<i32>(%1), add<i32, overflow=ub>(add<i32, overflow=ub>(const<i32>(4096), const<i32>(256)), const<i32>(8))))
// DEFAULT-NEXT:             return neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         let %2 buf: array<i8, 5> [storage=automatic];
// DEFAULT-NEXT:         let %3 n: i32 [storage=automatic] = call<i32, signature=fn(ptr<i8>, u64, ptr<const i8>, ...) -> i32>(__builtin_snprintf, array_decay<ptr<i8>, length=Some(5)>(%2), const<u64>(5), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%5)), add<i32, overflow=ub>(read<i32>(%1), const<i32>(1)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(14)>(%6)), add<i32, overflow=ub>(read<i32>(%1), const<i32>(1)), read<i32>(%3));
// DEFAULT-NEXT:         return read<i32>(%3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(const<i32>(127), const<i32>(127)), ne<i32>(const<i32>(8), const<i32>(8))), ne<i32>(const<i32>(4), const<i32>(4)))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         let %7: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%0, add<i32, overflow=ub>(const<i32>(4095), const<i32>(9))), const<i32>(1))
// DEFAULT-NEXT:             write<bool>(%7, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%7, ne<i32>(call<i32, signature=fn(i32) -> i32>(%0, add<i32, overflow=ub>(const<i32>(4095), const<i32>(32))), const<i32>(2)));
// DEFAULT-NEXT:         let %8: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%7)
// DEFAULT-NEXT:             write<bool>(%8, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%8, ne<i32>(call<i32, signature=fn(i32) -> i32>(%0, add<i32, overflow=ub>(const<i32>(4095), const<i32>(127))), const<i32>(3)));
// DEFAULT-NEXT:         let %9: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%8)
// DEFAULT-NEXT:             write<bool>(%9, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%9, ne<i32>(call<i32, signature=fn(i32) -> i32>(%0, add<i32, overflow=ub>(const<i32>(4095), const<i32>(128))), const<i32>(4)));
// DEFAULT-NEXT:         let %10: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%9)
// DEFAULT-NEXT:             write<bool>(%10, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%10, ne<i32>(call<i32, signature=fn(i32) -> i32>(%0, add<i32, overflow=ub>(const<i32>(4095), const<i32>(240))), const<i32>(3)));
// DEFAULT-NEXT:         let %11: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%10)
// DEFAULT-NEXT:             write<bool>(%11, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%11, ne<i32>(call<i32, signature=fn(i32) -> i32>(%0, add<i32, overflow=ub>(const<i32>(4095), const<i32>(248))), const<i32>(2)));
// DEFAULT-NEXT:         let %12: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%11)
// DEFAULT-NEXT:             write<bool>(%12, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%12, ne<i32>(call<i32, signature=fn(i32) -> i32>(%0, add<i32, overflow=ub>(const<i32>(4095), const<i32>(255))), const<i32>(2)));
// DEFAULT-NEXT:         let %13: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%12)
// DEFAULT-NEXT:             write<bool>(%13, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%13, ne<i32>(call<i32, signature=fn(i32) -> i32>(%0, add<i32, overflow=ub>(const<i32>(4095), const<i32>(256))), const<i32>(1)));
// DEFAULT-NEXT:         if read<bool>(%13)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
