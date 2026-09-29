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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 p: i64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_pt:[0-9]+]] pt = @type[[TYPE0]];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE__p:[0-9]+]] _p: @type[[TYPE0]]) -> @type[[TYPE0]] [linkage=external] [inline=hint] [definition=inline_only] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: i64 [storage=automatic] = read<i64>(field0(%[[VALUE__p]]));
// DEFAULT-NEXT:         return copy<@type[[TYPE0]], reason=return>(read<@type[[TYPE0]]>(compound_literal %[[VALUE0:[0-9]+]] [storage=automatic] = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = read<i64>(%[[VALUE_p]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_mmap_mem:[0-9]+]] @mmap_mem() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_p_2:[0-9]+]] p: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:         write<@type[[TYPE0]]>(%[[VALUE_p_2]], copy<@type[[TYPE0]], reason=assign>(call<@type[[TYPE0]], signature=fn(@type[[TYPE0]]) -> @type[[TYPE0]], abi=sysv64(native_c) -> native_c>(%[[VALUE_f]], copy<@type[[TYPE0]], reason=arg>(read<@type[[TYPE0]]>(%[[VALUE_p_2]])))));
// DEFAULT-NEXT:         copy<@type[[TYPE0]], reason=assign>(call<@type[[TYPE0]], signature=fn(@type[[TYPE0]]) -> @type[[TYPE0]], abi=sysv64(native_c) -> native_c>(%[[VALUE_f]], copy<@type[[TYPE0]], reason=arg>(read<@type[[TYPE0]]>(%[[VALUE_p_2]]))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
