/* { dg-do compile } */
/* { dg-options "-Og -fdump-tree-gimple" } */

struct s {
  short a : 3;
  short b : 3;
  char  c;
};

extern void g(struct s *);

void f() {
  struct s x = { 0, 0, 1 };
  g (&x);
}

/* { dg-final { scan-tree-dump-times "= {};" 1 "gimple" } } */
/* { dg-final { scan-tree-dump-not "= 0;" "gimple" } } */

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
// DEFAULT-NEXT:     type @type[[TYPE_s:[0-9]+]] s = struct {
// DEFAULT-NEXT:         field0 a: i16 : 3;
// DEFAULT-NEXT:         field1 b: i16 : 3;
// DEFAULT-NEXT:         field2 c: i8;
// DEFAULT-NEXT:     } [size=2, align=2, offsets=[0, 0, 1], bit_offsets=[Some(0), Some(3), None], bit_units=[(0, 1)], field_units=[Some(0), Some(0), None]];
// DEFAULT-NEXT:     fn %[[VALUE_g:[0-9]+]] @g(%[[VALUE0:[0-9]+]] <unnamed>: ptr<@type[[TYPE_s]]>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: @type[[TYPE_s]] [storage=automatic] = aggregate<@type[[TYPE_s]], zero_fill=false>(field0 = truncate<i16, reason=assign, fits=always>(const<i32>(0)), field1 = truncate<i16, reason=assign, fits=always>(const<i32>(0)), field2 = truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_s]]>) -> void>(%[[VALUE_g]], addr_of<ptr<@type[[TYPE_s]]>>(%[[VALUE_x]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
