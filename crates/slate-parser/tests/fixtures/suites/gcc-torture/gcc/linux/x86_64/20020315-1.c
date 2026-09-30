// SLATE-FILECHECK-DEFINES DEFAULT

/* PR bootstrap/4128 */

extern int bar (char *, char *, int, int);
extern long baz (char *, char *, int, int);

int sgt (char *a, char *b, int c, int d)
{
  return bar (a, b, c, d) > 0;
}

long dgt (char *a, char *b, int c, int d)
{
  return baz (a, b, c, d) > 0;
}

int sne (char *a, char *b, int c, int d)
{
  return bar (a, b, c, d) != 0;
}

long dne (char *a, char *b, int c, int d)
{
  return baz (a, b, c, d) != 0;
}

int seq (char *a, char *b, int c, int d)
{
  return bar (a, b, c, d) == 0;
}

long deq (char *a, char *b, int c, int d)
{
  return baz (a, b, c, d) == 0;
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
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE0:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE2:[0-9]+]] <unnamed>: i32, %[[VALUE3:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz(%[[VALUE4:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE5:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE6:[0-9]+]] <unnamed>: i32, %[[VALUE7:[0-9]+]] <unnamed>: i32) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sgt:[0-9]+]] @sgt(%[[VALUE_a:[0-9]+]] a: ptr<i8>, %[[VALUE_b:[0-9]+]] b: ptr<i8>, %[[VALUE_c:[0-9]+]] c: i32, %[[VALUE_d:[0-9]+]] d: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(gt<i32>(call<i32, signature=fn(ptr<i8>, ptr<i8>, i32, i32) -> i32>(%[[VALUE_bar]], read<ptr<i8>>(%[[VALUE_a]]), read<ptr<i8>>(%[[VALUE_b]]), read<i32>(%[[VALUE_c]]), read<i32>(%[[VALUE_d]])), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_dgt:[0-9]+]] @dgt(%[[VALUE_a_2:[0-9]+]] a: ptr<i8>, %[[VALUE_b_2:[0-9]+]] b: ptr<i8>, %[[VALUE_c_2:[0-9]+]] c: i32, %[[VALUE_d_2:[0-9]+]] d: i32) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i64, reason=return>(gt<i64>(call<i64, signature=fn(ptr<i8>, ptr<i8>, i32, i32) -> i64>(%[[VALUE_baz]], read<ptr<i8>>(%[[VALUE_a_2]]), read<ptr<i8>>(%[[VALUE_b_2]]), read<i32>(%[[VALUE_c_2]]), read<i32>(%[[VALUE_d_2]])), widen<i64, reason=usual_arith>(const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_sne:[0-9]+]] @sne(%[[VALUE_a_3:[0-9]+]] a: ptr<i8>, %[[VALUE_b_3:[0-9]+]] b: ptr<i8>, %[[VALUE_c_3:[0-9]+]] c: i32, %[[VALUE_d_3:[0-9]+]] d: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<i32>(call<i32, signature=fn(ptr<i8>, ptr<i8>, i32, i32) -> i32>(%[[VALUE_bar]], read<ptr<i8>>(%[[VALUE_a_3]]), read<ptr<i8>>(%[[VALUE_b_3]]), read<i32>(%[[VALUE_c_3]]), read<i32>(%[[VALUE_d_3]])), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_dne:[0-9]+]] @dne(%[[VALUE_a_4:[0-9]+]] a: ptr<i8>, %[[VALUE_b_4:[0-9]+]] b: ptr<i8>, %[[VALUE_c_4:[0-9]+]] c: i32, %[[VALUE_d_4:[0-9]+]] d: i32) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i64, reason=return>(ne<i64>(call<i64, signature=fn(ptr<i8>, ptr<i8>, i32, i32) -> i64>(%[[VALUE_baz]], read<ptr<i8>>(%[[VALUE_a_4]]), read<ptr<i8>>(%[[VALUE_b_4]]), read<i32>(%[[VALUE_c_4]]), read<i32>(%[[VALUE_d_4]])), widen<i64, reason=usual_arith>(const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_seq:[0-9]+]] @seq(%[[VALUE_a_5:[0-9]+]] a: ptr<i8>, %[[VALUE_b_5:[0-9]+]] b: ptr<i8>, %[[VALUE_c_5:[0-9]+]] c: i32, %[[VALUE_d_5:[0-9]+]] d: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(call<i32, signature=fn(ptr<i8>, ptr<i8>, i32, i32) -> i32>(%[[VALUE_bar]], read<ptr<i8>>(%[[VALUE_a_5]]), read<ptr<i8>>(%[[VALUE_b_5]]), read<i32>(%[[VALUE_c_5]]), read<i32>(%[[VALUE_d_5]])), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_deq:[0-9]+]] @deq(%[[VALUE_a_6:[0-9]+]] a: ptr<i8>, %[[VALUE_b_6:[0-9]+]] b: ptr<i8>, %[[VALUE_c_6:[0-9]+]] c: i32, %[[VALUE_d_6:[0-9]+]] d: i32) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i64, reason=return>(eq<i64>(call<i64, signature=fn(ptr<i8>, ptr<i8>, i32, i32) -> i64>(%[[VALUE_baz]], read<ptr<i8>>(%[[VALUE_a_6]]), read<ptr<i8>>(%[[VALUE_b_6]]), read<i32>(%[[VALUE_c_6]]), read<i32>(%[[VALUE_d_6]])), widen<i64, reason=usual_arith>(const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
