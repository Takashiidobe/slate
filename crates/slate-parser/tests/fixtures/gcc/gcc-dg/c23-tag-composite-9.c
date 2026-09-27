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


// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     type @type0 e = enum : u32 {
// DEFAULT-NEXT:         %0 A = const<i32>(0);
// DEFAULT-NEXT:         %1 B = const<i32>(1);
// DEFAULT-NEXT:         %2 C = const<i32>(2);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type1 s = struct {
// DEFAULT-NEXT:         field0 m: @type0 : 3;
// DEFAULT-NEXT:         field1 y: ptr<array<i8, incomplete>>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8], bit_offsets=[Some(0), None], bit_units=[(0, 1)], field_units=[Some(0), None]];
// DEFAULT-NEXT:     type @type2 s = struct {
// DEFAULT-NEXT:         field0 m: @type0 : 3;
// DEFAULT-NEXT:         field1 y: ptr<array<i8, 1>>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8], bit_offsets=[Some(0), None], bit_units=[(0, 1)], field_units=[Some(0), None]];
// DEFAULT-NEXT:     global %5 s: @type1 [storage=static] = aggregate<@type1, zero_fill=true>() [linkage=external];
// DEFAULT-NEXT:     fn %6 @f(%11 <unnamed>: @type0) -> void [linkage=external];
// DEFAULT-NEXT:     fn %7 @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %9 t: @type2 [storage=automatic] = aggregate<@type2, zero_fill=true>();
// DEFAULT-NEXT:         call<void, signature=fn(@type0) -> void>(%6, int_to_enum<@type0, reason=arg>(reinterpret<u32, reason=arg, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type0>(bitfield0<unit=0, bytes=0..1, bits=0..3>(%5)))))));
// DEFAULT-NEXT:         call<void, signature=fn(@type0) -> void>(%6, int_to_enum<@type0, reason=arg>(reinterpret<u32, reason=arg, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type0>(bitfield0<unit=0, bytes=0..1, bits=0..3>(%9)))))));
// DEFAULT-NEXT:         let %10 u: @type1 [storage=automatic] = aggregate<@type1, zero_fill=true>();
// DEFAULT-NEXT:         call<void, signature=fn(@type0) -> void>(%6, int_to_enum<@type0, reason=arg>(reinterpret<u32, reason=arg, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type0>(bitfield0<unit=0, bytes=0..1, bits=0..3>(%10)))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
