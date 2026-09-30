/* { dg-do compile } */
/* { dg-options "-std=c23" } */

// adapted from PR c/11428.

struct s { int m : 1; char (*y)[]; } s;

int
foo (void *q)
{
	struct s { int m : 1; char (*y)[1]; } t;
	typeof(1 ? &s : &t) p = q;
	return !p->m;
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
// DEFAULT-NEXT:     type @type[[TYPE_s:[0-9]+]] s = struct {
// DEFAULT-NEXT:         field0 m: i32 : 1;
// DEFAULT-NEXT:         field1 y: ptr<array<i8, incomplete>>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8], bit_offsets=[Some(0), None], bit_units=[(0, 1)], field_units=[Some(0), None]];
// DEFAULT-NEXT:     type @type[[TYPE_s_2:[0-9]+]] s = struct {
// DEFAULT-NEXT:         field0 m: i32 : 1;
// DEFAULT-NEXT:         field1 y: ptr<array<i8, 1>>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8], bit_offsets=[Some(0), None], bit_units=[(0, 1)], field_units=[Some(0), None]];
// DEFAULT-NEXT:     global %[[VALUE_s:[0-9]+]] s: @type[[TYPE_s]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_q:[0-9]+]] q: ptr<void>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_t:[0-9]+]] t: @type[[TYPE_s_2]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_s]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_s]]>, reason=assign>(read<ptr<void>>(%[[VALUE_q]]));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(not<bool>(ne<i32>(read<i32>(bitfield0<unit=0, bytes=0..1, bits=0..1>(deref(read<ptr<@type[[TYPE_s]]>>(%[[VALUE_p]])))), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
