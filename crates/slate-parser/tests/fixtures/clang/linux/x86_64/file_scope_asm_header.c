// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-ISYSTEM tests/fixtures/inputs
#include "file_scope_asm_header.h"

int header_target(int x) { return x + 1; }

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
// IR-NEXT:     asm ".pushsection .static_call.text, \"ax\"\n.globl __SCT__header_call\n__SCT__header_call:\n.byte 0xe9; .long header_target - (. + 4)\n.popsection\n" [dialect=att];
// IR-NEXT:     fn %[[VALUE_header_target:[0-9]+]] @header_target(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<i32, overflow=ub>(read<i32>(%[[VALUE_x]]), const<i32>(1));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
