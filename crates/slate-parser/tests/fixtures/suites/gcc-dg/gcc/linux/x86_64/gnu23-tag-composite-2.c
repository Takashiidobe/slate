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
// DEFAULT-NEXT:     type @type[[TYPE_buf:[0-9]+]] buf = struct {
// DEFAULT-NEXT:         field0 x: i8;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_buf_2:[0-9]+]] buf = struct {
// DEFAULT-NEXT:         field0 x: i8;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_bar:[0-9]+]] bar = struct {
// DEFAULT-NEXT:         field0 y: @type[[TYPE_buf]];
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_buf_3:[0-9]+]] buf = struct {
// DEFAULT-NEXT:         field0 x: i8;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_bar_2:[0-9]+]] bar = struct {
// DEFAULT-NEXT:         field0 y: @type[[TYPE_buf_3]];
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_s:[0-9]+]] s: @type[[TYPE_buf]] [storage=static] = aggregate<@type[[TYPE_buf]], zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(0))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: @type[[TYPE_bar]] [storage=static] = aggregate<@type[[TYPE_bar]], zero_fill=false>(field0 = aggregate<@type[[TYPE_buf]], zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(0)))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_j:[0-9]+]] @j() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_t:[0-9]+]] t: @type[[TYPE_buf_2]] [storage=automatic] = aggregate<@type[[TYPE_buf_2]], zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE_u:[0-9]+]] u: @type[[TYPE_buf]] [storage=automatic] = aggregate<@type[[TYPE_buf]], zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE_v:[0-9]+]] v: @type[[TYPE_buf_2]] [storage=automatic] = aggregate<@type[[TYPE_buf_2]], zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_k:[0-9]+]] @k() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_t_2:[0-9]+]] t: @type[[TYPE_buf_3]] [storage=automatic] = aggregate<@type[[TYPE_buf_3]], zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: @type[[TYPE_bar_2]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: @type[[TYPE_bar]] [storage=automatic] = aggregate<@type[[TYPE_bar]], zero_fill=false>(field0 = aggregate<@type[[TYPE_buf]], zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
