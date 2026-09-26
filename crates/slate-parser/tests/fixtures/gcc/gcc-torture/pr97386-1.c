/* PR rtl-optimization/97386 */

__attribute__((noipa)) unsigned char foo(unsigned int c) {
  return __builtin_bswap16(
      (unsigned long long)(0xccccLLU << c | 0xccccLLU >> ((-c) & 63)));
}

int main() {
  unsigned char x = foo(0);
  if (__CHAR_BIT__ == 8 && __SIZEOF_SHORT__ == 2 && x != 0xcc)
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
// DEFAULT-NEXT:     fn %5 @__builtin_bswap16(%4 <unnamed>: u16) -> u16 [linkage=external];
// DEFAULT-NEXT:     fn %0 @foo(%1 c: u32) -> u8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<u8, reason=return, fits=unknown>(call<u16, signature=fn(u16) -> u16>(%5, truncate<u16, reason=arg, fits=unknown>(or<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(52428), read<u32>(%1)), shr<u64, amount_out_of_range=ub, fill=zero_extend>(const<u64>(52428), and<u32>(neg<u32, overflow=wrap>(read<u32>(%1)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(63))))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %3 x: u8 [storage=automatic] = call<u8, signature=fn(u32) -> u8>(%0, reinterpret<u32, reason=arg, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         if logical_and<bool>(logical_and<bool>(eq<i32>(const<i32>(8), const<i32>(8)), eq<i32>(const<i32>(2), const<i32>(2))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%3))), const<i32>(204)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
