// SLATE-FILECHECK-DEFINES DEFAULT

/* Test that the initializer of a compound literal is properly walked
   when tree inlining.  */
/* Origin: PR c/5105 from <aj@suse.de>.  */

typedef struct { long p; } pt;

inline pt f (pt _p)
{
  long p = _p.p;

  return (pt) { (p) };
}

static int mmap_mem (void)
{
  pt p;
  p = f (p);

  return 0;
}

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
// DEFAULT-NEXT:     type @type0 = struct {
// DEFAULT-NEXT:         field0 p: i64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type1 pt = @type0;
// DEFAULT-NEXT:     fn %2 @f(%3 _p: @type0) -> @type0 [linkage=external] [inline=hint] [definition=inline_only] [abi=sysv64(coerce<i64>) -> coerce<i64>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 p: i64 [storage=automatic] = read<i64>(field0(%3));
// DEFAULT-NEXT:         return copy<@type0, reason=return>(read<@type0>(compound_literal %7 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = read<i64>(%4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @mmap_mem() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %6 p: @type0 [storage=automatic];
// DEFAULT-NEXT:         write<@type0>(%6, copy<@type0, reason=assign>(call<@type0, signature=fn(@type0) -> @type0, abi=sysv64(coerce<i64>) -> coerce<i64>>(%2, copy<@type0, reason=arg>(read<@type0>(%6)))));
// DEFAULT-NEXT:         copy<@type0, reason=assign>(call<@type0, signature=fn(@type0) -> @type0, abi=sysv64(coerce<i64>) -> coerce<i64>>(%2, copy<@type0, reason=arg>(read<@type0>(%6))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
