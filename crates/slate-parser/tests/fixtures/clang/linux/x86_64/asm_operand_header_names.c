// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-ISYSTEM tests/fixtures/inputs
#include "asm_operand_header_names.h"

long operands(void) {
  long out;
  asm volatile("" : "=r"(out) : "r"(asm_header_global), "i"(asm_header_constant),
                    "r"((asm_header_word)sizeof(asm_header_word)));
  return out;
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type0 asm_header_word = i64;
// IR-NEXT:     type @type1 = enum : u32 {
// IR-NEXT:         %0 asm_header_constant = const<i32>(7);
// IR-NEXT:     } [size=4, align=4];
// IR-NEXT:     extern %0 asm_header_global: i32 [storage=static] [linkage=external];
// IR-NEXT:     fn %4 @operands() -> i64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %5 out: i64 [storage=automatic];
// IR-NEXT:         asm volatile "" [dialect=att] [options=nostack] {
// IR-NEXT:             lateout 0 "r" [reg] width 64 place<i64>(%5);
// IR-NEXT:             in 1 "r" [reg] width 32 read<i32>(%0);
// IR-NEXT:             in 2 "i" [imm | sym] -> imm width 32 const<i32>(7);
// IR-NEXT:             in 3 "r" [reg] width 64 reinterpret<i64, reason=explicit, fits=always>(const<u64>(8));
// IR-NEXT:         }
// IR-NEXT:         return read<i64>(%5);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
