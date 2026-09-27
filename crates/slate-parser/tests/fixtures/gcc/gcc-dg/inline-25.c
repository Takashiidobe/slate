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

// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     global %0 a: i32 [storage=static] = const<i32>(6) [linkage=internal];
// DEFAULT-NEXT:     global %1 b: i32 [storage=static] [const] = const<i32>(6) [linkage=internal];
// DEFAULT-NEXT:     global %2 c: i32 [storage=static] = const<i32>(6) [linkage=external];
// DEFAULT-NEXT:     global %7 d: i32 [storage=static] = const<i32>(6) [linkage=internal];
// DEFAULT-NEXT:     global %9 e: i32 [storage=static] [const] = const<i32>(6) [linkage=internal];
// DEFAULT-NEXT:     global %14 g: array<i8, 10> [storage=static] [const] = code_units<array<i8, 10>>([97, 98, 99, 100, 101, 102, 103, 104, 105, 106]) [linkage=internal];
// DEFAULT-NEXT:     global %19 d: i32 [storage=static] = const<i32>(6) [linkage=internal];
// DEFAULT-NEXT:     global %21 e: i32 [storage=static] [const] = const<i32>(6) [linkage=internal];
// DEFAULT-NEXT:     global %26 g: array<i8, 10> [storage=static] [const] = code_units<array<i8, 10>>([97, 98, 99, 100, 101, 102, 103, 104, 105, 106]) [linkage=internal];
// DEFAULT-NEXT:     global %31 d: i32 [storage=static] = const<i32>(6) [linkage=internal];
// DEFAULT-NEXT:     global %33 e: i32 [storage=static] [const] = const<i32>(6) [linkage=internal];
// DEFAULT-NEXT:     global %38 g: array<i8, 10> [storage=static] [const] = code_units<array<i8, 10>>([97, 98, 99, 100, 101, 102, 103, 104, 105, 106]) [linkage=internal];
// DEFAULT-NEXT:     fn %3 @fn1() -> i32 [linkage=external] [inline=hint] [definition=inline_only] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @fn2() -> i32 [linkage=external] [inline=hint] [definition=inline_only] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @fn3() -> i32 [linkage=external] [inline=hint] [definition=inline_only] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @fn4() -> i32 [linkage=external] [inline=hint] [definition=inline_only] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%7);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @fn5() -> i32 [linkage=external] [inline=hint] [definition=inline_only] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%9);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @fn6() -> i32 [linkage=external] [inline=hint] [definition=inline_only] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %11 f: i32 [storage=automatic] = const<i32>(6);
// DEFAULT-NEXT:         return read<i32>(%11);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @fn7(%13 i: i32) -> i32 [linkage=external] [inline=hint] [definition=inline_only] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return widen<i32, reason=return>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(10)>(%14), read<i32>(%13)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @fn8() -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @fn9() -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @fn10() -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @fn11() -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%19);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @fn12() -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%21);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @fn13() -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %23 f: i32 [storage=automatic] = const<i32>(6);
// DEFAULT-NEXT:         return read<i32>(%23);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @fn14(%25 i: i32) -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return widen<i32, reason=return>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(10)>(%26), read<i32>(%25)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %27 @fn15() -> i32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %28 @fn16() -> i32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %29 @fn17() -> i32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %30 @fn18() -> i32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%31);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %32 @fn19() -> i32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%33);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %34 @fn20() -> i32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %35 f: i32 [storage=automatic] = const<i32>(6);
// DEFAULT-NEXT:         return read<i32>(%35);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %36 @fn21(%37 i: i32) -> i32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return widen<i32, reason=return>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(10)>(%38), read<i32>(%37)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
