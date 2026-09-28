/* { dg-do compile }
 * { dg-options "-std=c23" } 
 */

// attributes

struct [[gnu::designated_init]] buf { char x; };

struct buf s = { 0 };				/* { dg-warning "positional" } */

void j()
{
	struct buf { char x; } t = { 0 };
	typeof(*(1 ? &s : &t)) u = { 0 };	/* { dg-warning "positional" } */
	typeof(*(1 ? &t : &s)) v = { 0 };	/* { dg-warning "positional" } */
}


struct bar { struct buf y; };
extern struct bar a;
struct bar a = { { 0 } };			/* { dg-warning "positional" } */

void k()
{
	struct buf { char x; } t = { 0 };
	struct bar { struct buf y; } b;
	extern typeof(*(1 ? &a : &b)) a;
	typeof(*(1 ? &a : &b)) c = { { 0 } };	/* { dg-warning "positional" } */
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
// DEFAULT-NEXT:     type @type0 buf = struct {
// DEFAULT-NEXT:         field0 x: i8;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type1 buf = struct {
// DEFAULT-NEXT:         field0 x: i8;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type2 bar = struct {
// DEFAULT-NEXT:         field0 y: @type0;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type3 buf = struct {
// DEFAULT-NEXT:         field0 x: i8;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type4 bar = struct {
// DEFAULT-NEXT:         field0 y: @type3;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     global %1 s: @type0 [storage=static] = aggregate<@type0, zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(0))) [linkage=external];
// DEFAULT-NEXT:     global %8 a: @type2 [storage=static] = aggregate<@type2, zero_fill=false>(field0 = aggregate<@type0, zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(0)))) [linkage=external];
// DEFAULT-NEXT:     fn %2 @j() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %4 t: @type1 [storage=automatic] = aggregate<@type1, zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %5 u: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %6 v: @type1 [storage=automatic] = aggregate<@type1, zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @k() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %11 t: @type3 [storage=automatic] = aggregate<@type3, zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %13 b: @type4 [storage=automatic];
// DEFAULT-NEXT:         let %14 c: @type2 [storage=automatic] = aggregate<@type2, zero_fill=false>(field0 = aggregate<@type0, zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
