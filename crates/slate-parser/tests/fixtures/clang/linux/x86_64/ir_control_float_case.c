// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

int float_case(int x) {
    switch (x) {
    case (int)1.0: return 1;
    case (int)-2.75: return 2;
    case (int)3.75f: return 3;
    case (int)4.75L: return 4;
    case (int)5.75f16: return 5;
    case (int)-6.75Q: return 6;
    case (int)2147483647.75: return 7;
    case (int)-2147483648.75: return 8;
    case (unsigned)-0.75: return 9;
    case (int)7.9 ... (int)9.9: return 10;
    default: return (int)1.0 + 2;
    }
}

int unsigned_float_case(unsigned long long x) {
    switch (x) {
    case (unsigned long long)0xffffffffffffffff.0p0L: return 1;
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
// IR-NEXT:     fn %[[VALUE_float_case:[0-9]+]] @float_case(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         switch %[[VALUE0:[0-9]+]] read<i32>(%[[VALUE_x]])
// IR-NEXT:             {
// IR-NEXT:                 case %[[VALUE0]] const<i32>(1):
// IR-NEXT:                     return const<i32>(1);
// IR-NEXT:                 case %[[VALUE0]] const<i32>(-2):
// IR-NEXT:                     return const<i32>(2);
// IR-NEXT:                 case %[[VALUE0]] const<i32>(3):
// IR-NEXT:                     return const<i32>(3);
// IR-NEXT:                 case %[[VALUE0]] const<i32>(4):
// IR-NEXT:                     return const<i32>(4);
// IR-NEXT:                 case %[[VALUE0]] const<i32>(5):
// IR-NEXT:                     return const<i32>(5);
// IR-NEXT:                 case %[[VALUE0]] const<i32>(-6):
// IR-NEXT:                     return const<i32>(6);
// IR-NEXT:                 case %[[VALUE0]] const<i32>(2147483647):
// IR-NEXT:                     return const<i32>(7);
// IR-NEXT:                 case %[[VALUE0]] const<i32>(-2147483648):
// IR-NEXT:                     return const<i32>(8);
// IR-NEXT:                 case %[[VALUE0]] const<i32>(0):
// IR-NEXT:                     return const<i32>(9);
// IR-NEXT:                 case %[[VALUE0]] const<i32>(7) ... const<i32>(9):
// IR-NEXT:                     return const<i32>(10);
// IR-NEXT:                 default %[[VALUE0]]:
// IR-NEXT:                     return add<i32, overflow=ub>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.0)), const<i32>(2));
// IR-NEXT:             }
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_unsigned_float_case:[0-9]+]] @unsigned_float_case(%[[VALUE_x_2:[0-9]+]] x: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         switch %[[VALUE1:[0-9]+]] read<u64>(%[[VALUE_x_2]])
// IR-NEXT:             {
// IR-NEXT:                 case %[[VALUE1]] const<u64>(18446744073709551615):
// IR-NEXT:                     return const<i32>(1);
// IR-NEXT:                 default %[[VALUE1]]:
// IR-NEXT:                     return const<i32>(0);
// IR-NEXT:             }
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
