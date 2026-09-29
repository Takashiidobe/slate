/*
 * { dg-do compile }
 * { dg-options "-std=gnu23" }
 */



void test2(void)
{
  enum ee *a;
  enum ee { F = 2 } *b;
  b = a;
}



enum A { B = 7 } y;

void g(void)
{
	// incomplete during construction
	// this is a GNU extension because enum A is used
	// before the type is completed.

	enum A { B = _Generic(&y, enum A*: 1, default: 7) };
	_Static_assert(7 == B, "");
}



// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     type @type[[TYPE_ee:[0-9]+]] ee = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_F:[0-9]+]] F = const<i32>(2);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_A:[0-9]+]] A = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_F]] B = const<i32>(7);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_A_2:[0-9]+]] A = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_F]] B = const<i32>(7);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     global %[[VALUE_y:[0-9]+]] y: @type[[TYPE_A]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_F]] @test2() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: ptr<@type[[TYPE_ee]]> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: ptr<@type[[TYPE_ee]]> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_ee]]>>(%[[VALUE_b]], read<ptr<@type[[TYPE_ee]]>>(%[[VALUE_a]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_g:[0-9]+]] @g() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
