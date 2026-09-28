/* { dg-do compile } */
/* { dg-require-effective-target stdint_types } */
/* { dg-options "" } */
/* { dg-final { scan-assembler-not "__builtin_" } } */

#include <stdint.h>

uint16_t foo16 (uint16_t a)
{
  uint16_t b;

  b = __builtin_bswap16 (a);

  return b;
}

uint32_t foo32 (uint32_t a)
{
  uint32_t b;

  b = __builtin_bswap32 (a);

  return b;
}

uint64_t foo64 (uint64_t a)
{
  uint64_t b;

  b = __builtin_bswap64 (a);

  return b;
}

// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     type @type0 __uint16_t = u16;
// DEFAULT-NEXT:     type @type1 __uint32_t = u32;
// DEFAULT-NEXT:     type @type2 __uint64_t = u64;
// DEFAULT-NEXT:     type @type3 uint16_t = u16;
// DEFAULT-NEXT:     type @type4 uint32_t = u32;
// DEFAULT-NEXT:     type @type5 uint64_t = u64;
// DEFAULT-NEXT:     fn %16 @__builtin_bswap16(%15 <unnamed>: u16) -> u16 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %6 @foo16(%7 a: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8 b: u16 [storage=automatic];
// DEFAULT-NEXT:         write<u16>(%8, call<u16, signature=fn(u16) -> u16>(%16, read<u16>(%7)));
// DEFAULT-NEXT:         return read<u16>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @__builtin_bswap32(%17 <unnamed>: u32) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %9 @foo32(%10 a: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %11 b: u32 [storage=automatic];
// DEFAULT-NEXT:         write<u32>(%11, call<u32, signature=fn(u32) -> u32>(%18, read<u32>(%10)));
// DEFAULT-NEXT:         return read<u32>(%11);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @__builtin_bswap64(%19 <unnamed>: u64) -> u64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %12 @foo64(%13 a: u64) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %14 b: u64 [storage=automatic];
// DEFAULT-NEXT:         write<u64>(%14, call<u64, signature=fn(u64) -> u64>(%20, read<u64>(%13)));
// DEFAULT-NEXT:         return read<u64>(%14);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
