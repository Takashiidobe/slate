enum Small { S0, S1, };
enum Wide { W0 = -1, W1 = 0x80000000 };
enum Fixed : unsigned char { F0 = 1, F1, };

long wide_sum(void) { return W1 + 1; }
int small_sum(void) { return S1 + 1; }
int fixed_value(enum Fixed f) { return f + F1; }

// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=c23

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
// IR-NEXT:     type @type[[TYPE_Small:[0-9]+]] Small = enum : u32 {
// IR-NEXT:         %[[VALUE_S0:[0-9]+]] S0 = const<i32>(0);
// IR-NEXT:         %[[VALUE_S1:[0-9]+]] S1 = const<i32>(1);
// IR-NEXT:     } [size=4, align=4];
// IR-NEXT:     type @type[[TYPE_Wide:[0-9]+]] Wide = enum : i64 {
// IR-NEXT:         %[[VALUE_S0]] W0 = const<@type[[TYPE_Wide]]>(-1);
// IR-NEXT:         %[[VALUE_S1]] W1 = const<@type[[TYPE_Wide]]>(2147483648);
// IR-NEXT:     } [size=8, align=8];
// IR-NEXT:     type @type[[TYPE_Fixed:[0-9]+]] Fixed = enum : u8 {
// IR-NEXT:         %[[VALUE_S0]] F0 = const<@type[[TYPE_Fixed]]>(1);
// IR-NEXT:         %[[VALUE_S1]] F1 = const<@type[[TYPE_Fixed]]>(2);
// IR-NEXT:     } [size=1, align=1];
// IR-NEXT:     fn %[[VALUE_wide_sum:[0-9]+]] @wide_sum() -> i64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<i64, overflow=ub>(enum_to_int<i64, reason=promotion>(const<@type[[TYPE_Wide]]>(2147483648)), widen<i64, reason=usual_arith>(const<i32>(1)));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_small_sum:[0-9]+]] @small_sum() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<i32, overflow=ub>(const<i32>(1), const<i32>(1));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_fixed_value:[0-9]+]] @fixed_value(%[[VALUE_f:[0-9]+]] f: @type[[TYPE_Fixed]]) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(enum_to_int<u8, reason=promotion>(read<@type[[TYPE_Fixed]]>(%[[VALUE_f]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(enum_to_int<u8, reason=promotion>(const<@type[[TYPE_Fixed]]>(2)))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
