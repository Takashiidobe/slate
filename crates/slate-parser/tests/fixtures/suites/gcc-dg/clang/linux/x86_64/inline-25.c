/* PR c/35017 */
/* { dg-do compile } */
/* { dg-options "-std=iso9899:1999 -pedantic-errors" } */

static int a = 6;
static const int b = 6;
int c = 6;

inline int
fn1 (void)
{
  return a;		/* { dg-error "used in inline" } */
}

inline int
fn2 (void)
{
  return b;		/* { dg-error "used in inline" } */
}

inline int
fn3 (void)
{
  return c;
}

inline int
fn4 (void)
{
  static int d = 6;	/* { dg-error "declared in inline" } */
  return d;
}

inline int
fn5 (void)
{
  static const int e = 6;
  return e;
}

inline int
fn6 (void)
{
  int f = 6;
  return f;
}

inline int
fn7 (int i)
{
  static const char g[10] = "abcdefghij";
  return g[i];
}

extern inline int
fn8 (void)
{
  return a;
}

extern inline int
fn9 (void)
{
  return b;
}

extern inline int
fn10 (void)
{
  return c;
}

extern inline int
fn11 (void)
{
  static int d = 6;
  return d;
}

extern inline int
fn12 (void)
{
  static const int e = 6;
  return e;
}

extern inline int
fn13 (void)
{
  int f = 6;
  return f;
}

extern inline int
fn14 (int i)
{
  static const char g[10] = "abcdefghij";
  return g[i];
}

static inline int
fn15 (void)
{
  return a;
}

static inline int
fn16 (void)
{
  return b;
}

static inline int
fn17 (void)
{
  return c;
}

static inline int
fn18 (void)
{
  static int d = 6;
  return d;
}

static inline int
fn19 (void)
{
  static const int e = 6;
  return e;
}

static inline int
fn20 (void)
{
  int f = 6;
  return f;
}

static inline int
fn21 (int i)
{
  static const char g[10] = "abcdefghij";
  return g[i];
}

// SLATE-FILECHECK-STD DEFAULT iso9899:1999
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i32 [storage=static] = const<i32>(6) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i32 [storage=static] [const] = const<i32>(6) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i32 [storage=static] = const<i32>(6) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: i32 [storage=static] = const<i32>(6) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: i32 [storage=static] [const] = const<i32>(6) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_g:[0-9]+]] g: array<i8, 10> [storage=static] [const] = code_units<array<i8, 10>>([97, 98, 99, 100, 101, 102, 103, 104, 105, 106]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_d_2:[0-9]+]] d: i32 [storage=static] = const<i32>(6) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_e_2:[0-9]+]] e: i32 [storage=static] [const] = const<i32>(6) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_g_2:[0-9]+]] g: array<i8, 10> [storage=static] [const] = code_units<array<i8, 10>>([97, 98, 99, 100, 101, 102, 103, 104, 105, 106]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_d_3:[0-9]+]] d: i32 [storage=static] = const<i32>(6) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_e_3:[0-9]+]] e: i32 [storage=static] [const] = const<i32>(6) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_g_3:[0-9]+]] g: array<i8, 10> [storage=static] [const] = code_units<array<i8, 10>>([97, 98, 99, 100, 101, 102, 103, 104, 105, 106]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_fn1:[0-9]+]] @fn1() -> i32 [linkage=external] [inline=hint] [definition=inline_only] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_a]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2:[0-9]+]] @fn2() -> i32 [linkage=external] [inline=hint] [definition=inline_only] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_b]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3:[0-9]+]] @fn3() -> i32 [linkage=external] [inline=hint] [definition=inline_only] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_c]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn4:[0-9]+]] @fn4() -> i32 [linkage=external] [inline=hint] [definition=inline_only] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_d]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn5:[0-9]+]] @fn5() -> i32 [linkage=external] [inline=hint] [definition=inline_only] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_e]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn6:[0-9]+]] @fn6() -> i32 [linkage=external] [inline=hint] [definition=inline_only] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_f:[0-9]+]] f: i32 [storage=automatic] = const<i32>(6);
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_f]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn7:[0-9]+]] @fn7(%[[VALUE_i:[0-9]+]] i: i32) -> i32 [linkage=external] [inline=hint] [definition=inline_only] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return widen<i32, reason=return>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(10)>(%[[VALUE_g]]), read<i32>(%[[VALUE_i]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn8:[0-9]+]] @fn8() -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_a]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn9:[0-9]+]] @fn9() -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_b]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn10:[0-9]+]] @fn10() -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_c]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn11:[0-9]+]] @fn11() -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_d_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn12:[0-9]+]] @fn12() -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_e_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn13:[0-9]+]] @fn13() -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_f_2:[0-9]+]] f: i32 [storage=automatic] = const<i32>(6);
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_f_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn14:[0-9]+]] @fn14(%[[VALUE_i_2:[0-9]+]] i: i32) -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return widen<i32, reason=return>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(10)>(%[[VALUE_g_2]]), read<i32>(%[[VALUE_i_2]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn15:[0-9]+]] @fn15() -> i32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_a]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn16:[0-9]+]] @fn16() -> i32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_b]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn17:[0-9]+]] @fn17() -> i32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_c]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn18:[0-9]+]] @fn18() -> i32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_d_3]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn19:[0-9]+]] @fn19() -> i32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_e_3]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn20:[0-9]+]] @fn20() -> i32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_f_3:[0-9]+]] f: i32 [storage=automatic] = const<i32>(6);
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_f_3]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn21:[0-9]+]] @fn21(%[[VALUE_i_3:[0-9]+]] i: i32) -> i32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return widen<i32, reason=return>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(10)>(%[[VALUE_g_3]]), read<i32>(%[[VALUE_i_3]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
