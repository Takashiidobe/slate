// SLATE-FILECHECK-DEFINES AST
// SLATE-FILECHECK-STD AST gnu23

int cases(int x) {
    switch (x) {
    case 1 ? 2 : 3: return 1;
    case (0 ? 4 : 5) ... 1 ? 7 : 8: return 2;
    case sizeof(struct { int field : 3; }): return 3;
    default: return 0;
    }
}

// SLATE-FILECHECK-BEGIN AST
// AST: module {
// AST-NEXT:     target "x86_64-unknown-linux-gnu" {
// AST-NEXT:         endian = little;
// AST-NEXT:         pointer [size=8, align=8];
// AST-NEXT:         stack_alignment = 16;
// AST-NEXT:         long_double = f80;
// AST-NEXT:         storage bool [size=1, align=1];
// AST-NEXT:         storage i8, u8 [size=1, align=1];
// AST-NEXT:         storage i16, u16 [size=2, align=2];
// AST-NEXT:         storage i32, u32 [size=4, align=4];
// AST-NEXT:         storage i64, u64 [size=8, align=8];
// AST-NEXT:         storage i128, u128 [size=16, align=16];
// AST-NEXT:         storage bf16 [size=2, align=2];
// AST-NEXT:         storage f16 [size=2, align=2];
// AST-NEXT:         storage f32 [size=4, align=4];
// AST-NEXT:         storage f64 [size=8, align=8];
// AST-NEXT:         storage f80 [size=16, align=16];
// AST-NEXT:         storage f128 [size=16, align=16];
// AST-NEXT:         storage d32 [size=4, align=4];
// AST-NEXT:         storage d64 [size=8, align=8];
// AST-NEXT:         storage d128 [size=16, align=16];
// AST-NEXT:     }
// AST-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// AST-NEXT:         field0 field: i32 : 3;
// AST-NEXT:     } [size=4, align=4, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 1)], field_units=[Some(0)]];
// AST-NEXT:     fn %[[VALUE_cases:[0-9]+]] @cases(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// AST-NEXT:         switch %[[VALUE0:[0-9]+]] read<i32>(%[[VALUE_x]])
// AST-NEXT:             {
// AST-NEXT:                 case %[[VALUE0]] const<i32>(2):
// AST-NEXT:                     return const<i32>(1);
// AST-NEXT:                 case %[[VALUE0]] const<i32>(5) ... const<i32>(7):
// AST-NEXT:                     return const<i32>(2);
// AST-NEXT:                 case %[[VALUE0]] const<i32>(4):
// AST-NEXT:                     return const<i32>(3);
// AST-NEXT:                 default %[[VALUE0]]:
// AST-NEXT:                     return const<i32>(0);
// AST-NEXT:             }
// AST-NEXT:     }
// AST-NEXT: }
// SLATE-FILECHECK-END AST
