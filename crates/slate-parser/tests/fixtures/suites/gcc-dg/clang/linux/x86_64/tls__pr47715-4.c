/* { dg-do compile } */
/* { dg-options "-O0" } */
/* { dg-require-effective-target tls } */

struct gomp_team_state
{
  struct gomp_team_state *prev_ts;
  unsigned team_id;
  unsigned level;
};
struct gomp_thread
{
  void *data;
  struct gomp_team_state ts;
};
extern __thread struct gomp_thread gomp_tls_data;
int
foo (int level)
{
  struct gomp_team_state *ts = &gomp_tls_data.ts;
  if (level < 0 || level > ts->level)
    return -1;
  return ts->team_id;
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
// DEFAULT-NEXT:     type @type[[TYPE_gomp_team_state:[0-9]+]] gomp_team_state = struct {
// DEFAULT-NEXT:         field0 prev_ts: ptr<@type[[TYPE_gomp_team_state]]>;
// DEFAULT-NEXT:         field1 team_id: u32;
// DEFAULT-NEXT:         field2 level: u32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8, 12]];
// DEFAULT-NEXT:     type @type[[TYPE_gomp_thread:[0-9]+]] gomp_thread = struct {
// DEFAULT-NEXT:         field0 data: ptr<void>;
// DEFAULT-NEXT:         field1 ts: @type[[TYPE_gomp_team_state]];
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     extern %[[VALUE_gomp_tls_data:[0-9]+]] gomp_tls_data: @type[[TYPE_gomp_thread]] [storage=thread] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_level:[0-9]+]] level: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_ts:[0-9]+]] ts: ptr<@type[[TYPE_gomp_team_state]]> [storage=automatic] = addr_of<ptr<@type[[TYPE_gomp_team_state]]>>(field1(%[[VALUE_gomp_tls_data]]));
// DEFAULT-NEXT:         if logical_or<bool>(lt<i32>(read<i32>(%[[VALUE_level]]), const<i32>(0)), gt<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%[[VALUE_level]])), read<u32>(field2(deref(read<ptr<@type[[TYPE_gomp_team_state]]>>(%[[VALUE_ts]]))))))
// DEFAULT-NEXT:             return neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(read<u32>(field1(deref(read<ptr<@type[[TYPE_gomp_team_state]]>>(%[[VALUE_ts]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
