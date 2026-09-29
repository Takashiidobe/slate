extern void abort(void);

typedef struct foo_t {
  unsigned int blksz;
  unsigned int bf_cnt;
} foo_t;

#define _RNDUP(x, unit)   ((((x) + (unit) - 1) / (unit)) * (unit))
#define _RNDDOWN(x, unit) ((x) - ((x) % (unit)))

long long foo(foo_t *const pxp, long long offset, unsigned int extent) {
  long long    blkoffset = _RNDDOWN(offset, (long long)pxp->blksz);
  unsigned int diff      = (unsigned int)(offset - blkoffset);
  unsigned int blkextent = _RNDUP(diff + extent, pxp->blksz);

  if (pxp->blksz < blkextent)
    return -1LL;

  if (pxp->bf_cnt > pxp->blksz)
    pxp->bf_cnt = pxp->blksz;

  return blkoffset;
}

int main() {
  foo_t     x;
  long long xx;

  x.blksz  = 8192;
  x.bf_cnt = 0;
  xx       = foo(&x, 0, 4096);
  if (xx != 0LL)
    abort();
  return 0;
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
// DEFAULT-NEXT:     type @type[[TYPE_foo_t:[0-9]+]] foo_t = struct {
// DEFAULT-NEXT:         field0 blksz: u32;
// DEFAULT-NEXT:         field1 bf_cnt: u32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_foo_t_2:[0-9]+]] foo_t = @type[[TYPE_foo_t]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_pxp:[0-9]+]] pxp: ptr<@type[[TYPE_foo_t]]> [const], %[[VALUE_offset:[0-9]+]] offset: i64, %[[VALUE_extent:[0-9]+]] extent: u32) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_blkoffset:[0-9]+]] blkoffset: i64 [storage=automatic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE_offset]]), rem<i64, by_zero=ub, min_by_neg_one=ub>(read<i64>(%[[VALUE_offset]]), reinterpret<i64, reason=explicit, fits=unknown>(widen<u64, reason=explicit>(read<u32>(field0(deref(read<ptr<@type[[TYPE_foo_t]]>>(%[[VALUE_pxp]]))))))));
// DEFAULT-NEXT:         let %[[VALUE_diff:[0-9]+]] diff: u32 [storage=automatic] = reinterpret<u32, reason=explicit, fits=unknown>(truncate<i32, reason=explicit, fits=unknown>(sub<i64, overflow=ub>(read<i64>(%[[VALUE_offset]]), read<i64>(%[[VALUE_blkoffset]]))));
// DEFAULT-NEXT:         let %[[VALUE_blkextent:[0-9]+]] blkextent: u32 [storage=automatic] = mul<u32, overflow=wrap>(div<u32, by_zero=ub>(sub<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_diff]]), read<u32>(%[[VALUE_extent]])), read<u32>(field0(deref(read<ptr<@type[[TYPE_foo_t]]>>(%[[VALUE_pxp]]))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), read<u32>(field0(deref(read<ptr<@type[[TYPE_foo_t]]>>(%[[VALUE_pxp]]))))), read<u32>(field0(deref(read<ptr<@type[[TYPE_foo_t]]>>(%[[VALUE_pxp]])))));
// DEFAULT-NEXT:         if lt<u32>(read<u32>(field0(deref(read<ptr<@type[[TYPE_foo_t]]>>(%[[VALUE_pxp]])))), read<u32>(%[[VALUE_blkextent]]))
// DEFAULT-NEXT:             return neg<i64, overflow=ub>(const<i64>(1));
// DEFAULT-NEXT:         if gt<u32>(read<u32>(field1(deref(read<ptr<@type[[TYPE_foo_t]]>>(%[[VALUE_pxp]])))), read<u32>(field0(deref(read<ptr<@type[[TYPE_foo_t]]>>(%[[VALUE_pxp]])))))
// DEFAULT-NEXT:             write<u32>(field1(deref(read<ptr<@type[[TYPE_foo_t]]>>(%[[VALUE_pxp]]))), read<u32>(field0(deref(read<ptr<@type[[TYPE_foo_t]]>>(%[[VALUE_pxp]])))));
// DEFAULT-NEXT:         return read<i64>(%[[VALUE_blkoffset]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: @type[[TYPE_foo_t]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_xx:[0-9]+]] xx: i64 [storage=automatic];
// DEFAULT-NEXT:         write<u32>(field0(%[[VALUE_x]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(8192)));
// DEFAULT-NEXT:         write<u32>(field1(%[[VALUE_x]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i64>(%[[VALUE_xx]], call<i64, signature=fn(ptr<@type[[TYPE_foo_t]]>, i64, u32) -> i64>(%[[VALUE_foo]], addr_of<ptr<@type[[TYPE_foo_t]]>>(%[[VALUE_x]]), widen<i64, reason=arg>(const<i32>(0)), reinterpret<u32, reason=arg, fits=always>(const<i32>(4096))));
// DEFAULT-NEXT:         call<i64, signature=fn(ptr<@type[[TYPE_foo_t]]>, i64, u32) -> i64>(%[[VALUE_foo]], addr_of<ptr<@type[[TYPE_foo_t]]>>(%[[VALUE_x]]), widen<i64, reason=arg>(const<i32>(0)), reinterpret<u32, reason=arg, fits=always>(const<i32>(4096)));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%[[VALUE_xx]]), const<i64>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
