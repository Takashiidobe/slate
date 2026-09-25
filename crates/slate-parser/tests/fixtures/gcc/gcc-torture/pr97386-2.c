/* PR rtl-optimization/97386 */

__attribute__((noipa)) unsigned foo(int x) {
  unsigned long long a =
      (0x800000000000ccccULL << x) | (0x800000000000ccccULL >> (64 - x));
  unsigned int b = a;
  return (b << 24) | (b >> 8);
}

int main() {
  if (__CHAR_BIT__ == 8 && __SIZEOF_INT__ == 4 && __SIZEOF_LONG_LONG__ == 8 &&
      foo(1) != 0x99000199U)
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
// DEFAULT-NEXT:     fn %0 @foo(%1 x: i32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %2 a: u64 [storage=automatic] = or<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(9223372036854828236), read<i32>(%1)), shr<u64, amount_out_of_range=ub, fill=zero_extend>(const<u64>(9223372036854828236), sub<i32, overflow=ub>(const<i32>(64), read<i32>(%1))));
// DEFAULT-NEXT:         let %3 b: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(read<u64>(%2));
// DEFAULT-NEXT:         return or<u32>(shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%3), const<i32>(24)), shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%3), const<i32>(8)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %5: bool [synthetic];
// DEFAULT-NEXT:         if logical_and<bool>(logical_and<bool>(eq<i32>(const<i32>(8), const<i32>(8)), eq<i32>(const<i32>(4), const<i32>(4))), eq<i32>(const<i32>(8), const<i32>(8)))
// DEFAULT-NEXT:             write<bool>(%5, ne<u32>(call<u32, signature=fn(i32) -> u32>(%0, const<i32>(1)), const<u32>(2566914457)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%5, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%5)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
