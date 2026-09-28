#include <stdio.h>

int main(void) {
  unsigned int u  = 0x12345678u;
  unsigned int z  = 0u;
  int          s  = -12345;
  unsigned int sh = 5u;

  unsigned int rev            = __builtin_bitreverse32(u);
  unsigned int swapped        = __builtin_bswap32(u);
  int          leading        = __builtin_clz(u);
  int          trailing       = __builtin_ctz(u);
  int          first_set      = __builtin_ffs((int)u);
  int          zero_first     = __builtin_ffs((int)z);
  int          ones           = __builtin_popcount(u);
  int          odd            = __builtin_parity(u);
  int          redundant_sign = __builtin_clrsb(s);
  unsigned int left           = __builtin_rotateleft32(u, sh);
  unsigned int right          = __builtin_rotateright32(u, sh);

  printf("%u %u %d %d %d %d %d %d %d %u %u\n", rev, swapped, leading, trailing,
         first_set, zero_first, ones, odd, redundant_sign, left, right);
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
// DEFAULT-NEXT:     global %41 .str41: array<i8, 34> [storage=static] = code_units<array<i8, 34>>([37, 117, 32, 37, 117, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 117, 32, 37, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%18 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %20 @__builtin_bitreverse32(%19 <unnamed>: u32) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %22 @__builtin_bswap32(%21 <unnamed>: u32) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %24 @__builtin_clz(%23 <unnamed>: u32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %26 @__builtin_ctz(%25 <unnamed>: u32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %28 @__builtin_ffs(%27 <unnamed>: i32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %30 @__builtin_popcount(%29 <unnamed>: u32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %32 @__builtin_parity(%31 <unnamed>: u32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %34 @__builtin_clrsb(%33 <unnamed>: i32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %37 @__builtin_rotateleft32(%35 <unnamed>: u32, %36 <unnamed>: u32) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %40 @__builtin_rotateright32(%38 <unnamed>: u32, %39 <unnamed>: u32) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %3 u: u32 [storage=automatic] = const<u32>(305419896);
// DEFAULT-NEXT:         let %4 z: u32 [storage=automatic] = const<u32>(0);
// DEFAULT-NEXT:         let %5 s: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(12345));
// DEFAULT-NEXT:         let %6 sh: u32 [storage=automatic] = const<u32>(5);
// DEFAULT-NEXT:         let %7 rev: u32 [storage=automatic] = call<u32, signature=fn(u32) -> u32>(%20, read<u32>(%3));
// DEFAULT-NEXT:         let %8 swapped: u32 [storage=automatic] = call<u32, signature=fn(u32) -> u32>(%22, read<u32>(%3));
// DEFAULT-NEXT:         let %9 leading: i32 [storage=automatic] = call<i32, signature=fn(u32) -> i32>(%24, read<u32>(%3));
// DEFAULT-NEXT:         let %10 trailing: i32 [storage=automatic] = call<i32, signature=fn(u32) -> i32>(%26, read<u32>(%3));
// DEFAULT-NEXT:         let %11 first_set: i32 [storage=automatic] = call<i32, signature=fn(i32) -> i32>(%28, reinterpret<i32, reason=explicit, fits=unknown>(read<u32>(%3)));
// DEFAULT-NEXT:         let %12 zero_first: i32 [storage=automatic] = call<i32, signature=fn(i32) -> i32>(%28, reinterpret<i32, reason=explicit, fits=unknown>(read<u32>(%4)));
// DEFAULT-NEXT:         let %13 ones: i32 [storage=automatic] = call<i32, signature=fn(u32) -> i32>(%30, read<u32>(%3));
// DEFAULT-NEXT:         let %14 odd: i32 [storage=automatic] = call<i32, signature=fn(u32) -> i32>(%32, read<u32>(%3));
// DEFAULT-NEXT:         let %15 redundant_sign: i32 [storage=automatic] = call<i32, signature=fn(i32) -> i32>(%34, read<i32>(%5));
// DEFAULT-NEXT:         let %16 left: u32 [storage=automatic] = call<u32, signature=fn(u32, u32) -> u32>(%37, read<u32>(%3), read<u32>(%6));
// DEFAULT-NEXT:         let %17 right: u32 [storage=automatic] = call<u32, signature=fn(u32, u32) -> u32>(%40, read<u32>(%3), read<u32>(%6));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(34)>(%41)), read<u32>(%7), read<u32>(%8), read<i32>(%9), read<i32>(%10), read<i32>(%11), read<i32>(%12), read<i32>(%13), read<i32>(%14), read<i32>(%15), read<u32>(%16), read<u32>(%17));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
