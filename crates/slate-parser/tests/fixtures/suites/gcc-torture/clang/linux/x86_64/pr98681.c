/* PR target/98681 */

__attribute__((noipa)) int foo(int x) {
  if (x > 32)
    return (x << -64) & 255;
  else
    return x;
}

int main() {
  if (foo(32) != 32 || foo(-150) != -150)
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
// DEFAULT-NEXT:     fn %0 @foo(%1 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if gt<i32>(read<i32>(%1), const<i32>(32))
// DEFAULT-NEXT:             return and<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i32>(%1), neg<i32, overflow=ub>(const<i32>(64))), const<i32>(255));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             return read<i32>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %4: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%0, const<i32>(32)), const<i32>(32))
// DEFAULT-NEXT:             write<bool>(%4, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%4, ne<i32>(call<i32, signature=fn(i32) -> i32>(%0, neg<i32, overflow=ub>(const<i32>(150))), neg<i32, overflow=ub>(const<i32>(150))));
// DEFAULT-NEXT:         if read<bool>(%4)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
