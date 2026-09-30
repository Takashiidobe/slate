// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

enum State { A = 1, B = 2 };
int enum_discriminant(enum State value) {
    switch (value) {
    case B: return 1;
    default: return 0;
    }
}
int enumerator(int x) {
    switch (x) {
    case A + 2: return 1;
    default: return 0;
    }
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
// IR-NEXT:     type @type[[TYPE_State:[0-9]+]] State = enum : u32 {
// IR-NEXT:         %[[VALUE_A:[0-9]+]] A = const<i32>(1);
// IR-NEXT:         %[[VALUE_B:[0-9]+]] B = const<i32>(2);
// IR-NEXT:     } [size=4, align=4];
// IR-NEXT:     fn %[[VALUE_enum_discriminant:[0-9]+]] @enum_discriminant(%[[VALUE_value:[0-9]+]] value: @type[[TYPE_State]]) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         switch %[[VALUE0:[0-9]+]] enum_to_int<u32, reason=promotion>(read<@type[[TYPE_State]]>(%[[VALUE_value]]))
// IR-NEXT:             {
// IR-NEXT:                 case %[[VALUE0]] const<u32>(2):
// IR-NEXT:                     return const<i32>(1);
// IR-NEXT:                 default %[[VALUE0]]:
// IR-NEXT:                     return const<i32>(0);
// IR-NEXT:             }
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_enumerator:[0-9]+]] @enumerator(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         switch %[[VALUE1:[0-9]+]] read<i32>(%[[VALUE_x]])
// IR-NEXT:             {
// IR-NEXT:                 case %[[VALUE1]] const<i32>(3):
// IR-NEXT:                     return const<i32>(1);
// IR-NEXT:                 default %[[VALUE1]]:
// IR-NEXT:                     return const<i32>(0);
// IR-NEXT:             }
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
