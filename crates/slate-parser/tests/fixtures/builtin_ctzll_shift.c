#include <stddef.h>
#include <stdio.h>

size_t lowest_bit(unsigned long long x) {
  return (size_t)1 << (size_t)__builtin_ctzll(x);
}

int main(void) {
  unsigned long long x = 0x1230000ULL;
  size_t n = lowest_bit(x);
  printf("%zu\n", n);
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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     global %8 .str8: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%7 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @lowest_bit(%3 x: u64) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(call<i32, signature=fn(u64) -> i32>(__builtin_ctzll, read<u64>(%3)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %5 x: u64 [storage=automatic] = const<u64>(19070976);
// DEFAULT-NEXT:         let %6 n: u64 [storage=automatic] = call<u64, signature=fn(u64) -> u64>(%2, read<u64>(%5));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%8)), read<u64>(%6));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
