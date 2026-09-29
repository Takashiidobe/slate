// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

typedef __builtin_va_list va_list;
struct Pair { long a; double b; };

int fixed_type(va_list ap) { return __builtin_va_arg(ap, int); }
struct Pair struct_type(va_list ap) { return __builtin_va_arg(ap, struct Pair); }
double sequenced(va_list ap) { return __builtin_va_arg(ap, double) + __builtin_va_arg(ap, int); }

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
// IR-NEXT:     type @type[[TYPE_va_list:[0-9]+]] va_list = va_list;
// IR-NEXT:     type @type[[TYPE_Pair:[0-9]+]] Pair = struct {
// IR-NEXT:         field0 a: i64;
// IR-NEXT:         field1 b: f64;
// IR-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// IR-NEXT:     fn %[[VALUE_fixed_type:[0-9]+]] @fixed_type(%[[VALUE_ap:[0-9]+]] ap: va_list) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return va_arg<i32>(%[[VALUE_ap]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_struct_type:[0-9]+]] @struct_type(%[[VALUE_ap_2:[0-9]+]] ap: va_list) -> @type[[TYPE_Pair]] [linkage=external] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type[[TYPE_Pair]], reason=return>(va_arg<@type[[TYPE_Pair]]>(%[[VALUE_ap_2]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_sequenced:[0-9]+]] @sequenced(%[[VALUE_ap_3:[0-9]+]] ap: va_list) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(va_arg<f64>(%[[VALUE_ap_3]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(va_arg<i32>(%[[VALUE_ap_3]])));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
