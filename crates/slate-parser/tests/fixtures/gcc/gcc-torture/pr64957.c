/* PR rtl-optimization/64957 */

__attribute__((noinline, noclone)) int foo(int b) {
  return (((b ^ 5) | 1) ^ 5) | 1;
}

__attribute__((noinline, noclone)) int bar(int b) {
  return (((b ^ ~5) & ~1) ^ ~5) & ~1;
}

int main() {
  int i;
  for (i = 0; i < 16; i++)
    if (foo(i) != (i | 1) || bar(i) != (i & ~1))
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
// DEFAULT-NEXT:     fn %0 @foo(%1 b: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return or<i32>(xor<i32>(or<i32>(xor<i32>(read<i32>(%1), const<i32>(5)), const<i32>(1)), const<i32>(5)), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @bar(%3 b: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i32>(xor<i32>(and<i32>(xor<i32>(read<i32>(%3), not<i32>(const<i32>(5))), not<i32>(const<i32>(1))), not<i32>(const<i32>(5))), not<i32>(const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %5 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %6
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%5, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%5), const<i32>(16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %7: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:                 let %8: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%7), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%5, read<i32>(%8));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %9: bool [synthetic];
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(i32) -> i32>(%0, read<i32>(%5)), or<i32>(read<i32>(%5), const<i32>(1)))
// DEFAULT-NEXT:                     write<bool>(%9, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%9, ne<i32>(call<i32, signature=fn(i32) -> i32>(%2, read<i32>(%5)), and<i32>(read<i32>(%5), not<i32>(const<i32>(1)))));
// DEFAULT-NEXT:                 if read<bool>(%9)
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
