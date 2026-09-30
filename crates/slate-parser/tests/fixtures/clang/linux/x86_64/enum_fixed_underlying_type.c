typedef unsigned short narrow;

enum small : unsigned char { A = 1, B };

enum : long { ANONYMOUS = 2 };

enum wide : narrow
{
    WIDE = 3,
};

typedef enum : const int { TYPEDEF_FIXED = 4 } fixed_t;

struct holder {
    enum small kind : 3;
    enum small : 2;
};

int main(void) {
    enum local : signed char { LOCAL = -1 } value = LOCAL;
    return value + A + ANONYMOUS + WIDE + TYPEDEF_FIXED;
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
// DEFAULT-NEXT:     type @type[[TYPE_narrow:[0-9]+]] narrow = u16;
// DEFAULT-NEXT:     type @type[[TYPE_small:[0-9]+]] small = enum : u8 {
// DEFAULT-NEXT:         %[[VALUE_A:[0-9]+]] A = const<@type[[TYPE_small]]>(1);
// DEFAULT-NEXT:         %[[VALUE_B:[0-9]+]] B = const<@type[[TYPE_small]]>(2);
// DEFAULT-NEXT:     } [size=1, align=1];
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = enum : i64 {
// DEFAULT-NEXT:         %[[VALUE_A]] ANONYMOUS = const<@type[[TYPE0]]>(2);
// DEFAULT-NEXT:     } [size=8, align=8];
// DEFAULT-NEXT:     type @type[[TYPE_wide:[0-9]+]] wide = enum : u16 {
// DEFAULT-NEXT:         %[[VALUE_A]] WIDE = const<@type[[TYPE_wide]]>(3);
// DEFAULT-NEXT:     } [size=2, align=2];
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = enum : i32 {
// DEFAULT-NEXT:         %[[VALUE_A]] TYPEDEF_FIXED = const<@type[[TYPE1]]>(4);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_fixed_t:[0-9]+]] fixed_t = @type[[TYPE1]];
// DEFAULT-NEXT:     type @type[[TYPE_holder:[0-9]+]] holder = struct {
// DEFAULT-NEXT:         field0 kind: @type[[TYPE_small]] : 3;
// DEFAULT-NEXT:         field1 <anonymous>: @type[[TYPE_small]] : 2;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0, 0], bit_offsets=[Some(0), Some(3)], bit_units=[(0, 1)], field_units=[Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_local:[0-9]+]] local = enum : i8 {
// DEFAULT-NEXT:         %[[VALUE_A]] LOCAL = const<@type[[TYPE_local]]>(-1);
// DEFAULT-NEXT:     } [size=1, align=1];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_value:[0-9]+]] value: @type[[TYPE_local]] [storage=automatic] = const<@type[[TYPE_local]]>(-1);
// DEFAULT-NEXT:         return truncate<i32, reason=return, fits=unknown>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(widen<i64, reason=usual_arith>(add<i32, overflow=ub>(widen<i32, reason=promotion>(enum_to_int<i8, reason=promotion>(read<@type[[TYPE_local]]>(%[[VALUE_value]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(enum_to_int<u8, reason=promotion>(const<@type[[TYPE_small]]>(1)))))), enum_to_int<i64, reason=promotion>(const<@type[[TYPE0]]>(2))), widen<i64, reason=usual_arith>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(enum_to_int<u16, reason=promotion>(const<@type[[TYPE_wide]]>(3)))))), widen<i64, reason=usual_arith>(enum_to_int<i32, reason=promotion>(const<@type[[TYPE1]]>(4)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
