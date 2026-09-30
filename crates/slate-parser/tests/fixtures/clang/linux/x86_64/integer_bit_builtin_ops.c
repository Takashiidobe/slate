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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 34> [storage=static] = code_units<array<i8, 34>>([37, 117, 32, 37, 117, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 117, 32, 37, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_bitreverse32:[0-9]+]] @__builtin_bitreverse32(%[[VALUE0:[0-9]+]] <unnamed>: u32) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_bswap32:[0-9]+]] @__builtin_bswap32(%[[VALUE1:[0-9]+]] <unnamed>: u32) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_clz:[0-9]+]] @__builtin_clz(%[[VALUE2:[0-9]+]] <unnamed>: u32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_ctz:[0-9]+]] @__builtin_ctz(%[[VALUE3:[0-9]+]] <unnamed>: u32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_ffs:[0-9]+]] @__builtin_ffs(%[[VALUE4:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_popcount:[0-9]+]] @__builtin_popcount(%[[VALUE5:[0-9]+]] <unnamed>: u32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_parity:[0-9]+]] @__builtin_parity(%[[VALUE6:[0-9]+]] <unnamed>: u32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_clrsb:[0-9]+]] @__builtin_clrsb(%[[VALUE7:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_rotateleft32:[0-9]+]] @__builtin_rotateleft32(%[[VALUE8:[0-9]+]] <unnamed>: u32, %[[VALUE9:[0-9]+]] <unnamed>: u32) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_rotateright32:[0-9]+]] @__builtin_rotateright32(%[[VALUE10:[0-9]+]] <unnamed>: u32, %[[VALUE11:[0-9]+]] <unnamed>: u32) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_u:[0-9]+]] u: u32 [storage=automatic] = const<u32>(305419896);
// DEFAULT-NEXT:         let %[[VALUE_z:[0-9]+]] z: u32 [storage=automatic] = const<u32>(0);
// DEFAULT-NEXT:         let %[[VALUE_s:[0-9]+]] s: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(12345));
// DEFAULT-NEXT:         let %[[VALUE_sh:[0-9]+]] sh: u32 [storage=automatic] = const<u32>(5);
// DEFAULT-NEXT:         let %[[VALUE_rev:[0-9]+]] rev: u32 [storage=automatic] = call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bitreverse32]], read<u32>(%[[VALUE_u]]));
// DEFAULT-NEXT:         let %[[VALUE_swapped:[0-9]+]] swapped: u32 [storage=automatic] = call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], read<u32>(%[[VALUE_u]]));
// DEFAULT-NEXT:         let %[[VALUE_leading:[0-9]+]] leading: i32 [storage=automatic] = call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_clz]], read<u32>(%[[VALUE_u]]));
// DEFAULT-NEXT:         let %[[VALUE_trailing:[0-9]+]] trailing: i32 [storage=automatic] = call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_ctz]], read<u32>(%[[VALUE_u]]));
// DEFAULT-NEXT:         let %[[VALUE_first_set:[0-9]+]] first_set: i32 [storage=automatic] = call<i32, signature=fn(i32) -> i32>(%[[VALUE___builtin_ffs]], reinterpret<i32, reason=explicit, fits=unknown>(read<u32>(%[[VALUE_u]])));
// DEFAULT-NEXT:         let %[[VALUE_zero_first:[0-9]+]] zero_first: i32 [storage=automatic] = call<i32, signature=fn(i32) -> i32>(%[[VALUE___builtin_ffs]], reinterpret<i32, reason=explicit, fits=unknown>(read<u32>(%[[VALUE_z]])));
// DEFAULT-NEXT:         let %[[VALUE_ones:[0-9]+]] ones: i32 [storage=automatic] = call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_popcount]], read<u32>(%[[VALUE_u]]));
// DEFAULT-NEXT:         let %[[VALUE_odd:[0-9]+]] odd: i32 [storage=automatic] = call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_parity]], read<u32>(%[[VALUE_u]]));
// DEFAULT-NEXT:         let %[[VALUE_redundant_sign:[0-9]+]] redundant_sign: i32 [storage=automatic] = call<i32, signature=fn(i32) -> i32>(%[[VALUE___builtin_clrsb]], read<i32>(%[[VALUE_s]]));
// DEFAULT-NEXT:         let %[[VALUE_left:[0-9]+]] left: u32 [storage=automatic] = call<u32, signature=fn(u32, u32) -> u32>(%[[VALUE___builtin_rotateleft32]], read<u32>(%[[VALUE_u]]), read<u32>(%[[VALUE_sh]]));
// DEFAULT-NEXT:         let %[[VALUE_right:[0-9]+]] right: u32 [storage=automatic] = call<u32, signature=fn(u32, u32) -> u32>(%[[VALUE___builtin_rotateright32]], read<u32>(%[[VALUE_u]]), read<u32>(%[[VALUE_sh]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(34)>(%[[VALUE_str]])), read<u32>(%[[VALUE_rev]]), read<u32>(%[[VALUE_swapped]]), read<i32>(%[[VALUE_leading]]), read<i32>(%[[VALUE_trailing]]), read<i32>(%[[VALUE_first_set]]), read<i32>(%[[VALUE_zero_first]]), read<i32>(%[[VALUE_ones]]), read<i32>(%[[VALUE_odd]]), read<i32>(%[[VALUE_redundant_sign]]), read<u32>(%[[VALUE_left]]), read<u32>(%[[VALUE_right]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
