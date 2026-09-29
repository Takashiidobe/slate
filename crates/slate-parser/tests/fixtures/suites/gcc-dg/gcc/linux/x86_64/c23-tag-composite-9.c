/* { dg-do compile } */
/* { dg-options "-std=c23 -Wc++-compat" } */

// test that DECL_BIT_FIELD_TYPE is set correctly

enum e { A, B, C };
struct s { enum e m : 3; char (*y)[]; } s = { };

void f(enum e);

void foo ()
{
	struct s { enum e m : 3; char (*y)[1]; } t = { };
	f(s.m);
	f(t.m);
	typeof(*(1 ? &s : &t)) u = { };
	f(u.m);			// should not warn
}


// SLATE-FILECHECK-STD DEFAULT c23
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
// DEFAULT-NEXT:     type @type[[TYPE_e:[0-9]+]] e = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_A:[0-9]+]] A = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_B:[0-9]+]] B = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE_C:[0-9]+]] C = const<i32>(2);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_s:[0-9]+]] s = struct {
// DEFAULT-NEXT:         field0 m: @type[[TYPE_e]] : 3;
// DEFAULT-NEXT:         field1 y: ptr<array<i8, incomplete>>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8], bit_offsets=[Some(0), None], bit_units=[(0, 1)], field_units=[Some(0), None]];
// DEFAULT-NEXT:     type @type[[TYPE_s_2:[0-9]+]] s = struct {
// DEFAULT-NEXT:         field0 m: @type[[TYPE_e]] : 3;
// DEFAULT-NEXT:         field1 y: ptr<array<i8, 1>>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8], bit_offsets=[Some(0), None], bit_units=[(0, 1)], field_units=[Some(0), None]];
// DEFAULT-NEXT:     global %[[VALUE_s:[0-9]+]] s: @type[[TYPE_s]] [storage=static] = aggregate<@type[[TYPE_s]], zero_fill=true>() [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE0:[0-9]+]] <unnamed>: @type[[TYPE_e]]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_t:[0-9]+]] t: @type[[TYPE_s_2]] [storage=automatic] = aggregate<@type[[TYPE_s_2]], zero_fill=true>();
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_e]]) -> void>(%[[VALUE_f]], int_to_enum<@type[[TYPE_e]], reason=arg>(reinterpret<u32, reason=arg, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE_e]]>(bitfield0<unit=0, bytes=0..1, bits=0..3>(%[[VALUE_s]])))))));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_e]]) -> void>(%[[VALUE_f]], int_to_enum<@type[[TYPE_e]], reason=arg>(reinterpret<u32, reason=arg, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE_e]]>(bitfield0<unit=0, bytes=0..1, bits=0..3>(%[[VALUE_t]])))))));
// DEFAULT-NEXT:         let %[[VALUE_u:[0-9]+]] u: @type[[TYPE_s]] [storage=automatic] = aggregate<@type[[TYPE_s]], zero_fill=true>();
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_e]]) -> void>(%[[VALUE_f]], int_to_enum<@type[[TYPE_e]], reason=arg>(reinterpret<u32, reason=arg, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE_e]]>(bitfield0<unit=0, bytes=0..1, bits=0..3>(%[[VALUE_u]])))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
