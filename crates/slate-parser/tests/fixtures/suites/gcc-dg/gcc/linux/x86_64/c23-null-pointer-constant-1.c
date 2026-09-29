/* Test zero with different types as null pointer constant: bug 112556.  */
/* { dg-do compile } */
/* { dg-options "-std=c23 -pedantic-errors -Wno-pointer-compare" } */

enum e { ZERO };
enum e2 : bool { BZERO };
enum e3 : long { LZERO };

void *p1 = 0;
void *p2 = 0LL;
void *p3 = (char) 0;
void *p4 = 0UL;
void *p5 = (bool) 0;
void *p6 = (enum e) ZERO;
void *p7 = false;
void *p8 = BZERO;
void *p9 = (enum e2) 0;
void *p10 = LZERO;
void *p11 = (enum e3) 0;
#ifdef __BITINT_MAXWIDTH__
void *p12 = 0wb;
void *p13 = 0uwb;
#endif

void f (void *);

void *
g (void)
{
  p1 = 0;
  p2 = 0LL;
  p3 = (char) 0;
  p4 = 0UL;
  p5 = (bool) 0;
  p6 = (enum e) ZERO;
  p7 = false;
  p8 = BZERO;
  p9 = (enum e2) 0;
  p10 = LZERO;
  p11 = (enum e3) 0;
#ifdef __BITINT_MAXWIDTH__
  p12 = 0wb;
  p13 = 0uwb;
#endif
  f (0);
  f (0ULL);
  f (0L);
  f ((char) 0);
  f ((bool) 0);
  f ((enum e) ZERO);
  f (false);
  f (BZERO);
  f ((enum e2) 0);
  f (LZERO);
  f ((enum e3) 0);
#ifdef __BITINT_MAXWIDTH__
  f (0wb);
  f (0uwb);
#endif
  (1 ? p1 : 0);
  (1 ? p1 : 0L);
  (1 ? p1 : 0ULL);
  (1 ? p1 : (char) 0);
  (1 ? p1 : (bool) 0);
  (1 ? p1 : (enum e) 0);
  (1 ? p1 : false);
  (1 ? p1 : BZERO);
  (1 ? p1 : (enum e2) 0);
  (1 ? p1 : LZERO);
  (1 ? p1 : (enum e3) 0);
#ifdef __BITINT_MAXWIDTH__
  (1 ? p1 : 0wb);
  (1 ? p1 : 0uwb);
#endif
  p1 == 0;
  p1 == 0LL;
  p1 == 0U;
  p1 == (char) 0;
  p1 == (bool) 0;
  p1 == (enum e) 0;
  p1 == false;
  p1 == BZERO;
  p1 == (enum e2) 0;
  p1 == LZERO;
  p1 == (enum e3) 0;
#ifdef __BITINT_MAXWIDTH__
  p1 == 0wb;
  p1 == 0uwb;
#endif
  p1 != 0;
  p1 != 0LL;
  p1 != 0U;
  p1 != (char) 0;
  p1 != (bool) 0;
  p1 != (enum e) 0;
  p1 != false;
  p1 != BZERO;
  p1 != (enum e2) 0;
  p1 != LZERO;
  p1 != (enum e3) 0;
#ifdef __BITINT_MAXWIDTH__
  p1 != 0wb;
  p1 != 0uwb;
#endif
  return 0;
  return 0UL;
  return 0LL;
  return (char) 0;
  return (bool) 0;
  return (enum e) 0;
  return false;
  return BZERO;
  return (enum e2) 0;
  return LZERO;
  return (enum e3) 0;
#ifdef __BITINT_MAXWIDTH__
  return 0wb;
  return 0uwb;
#endif
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
// DEFAULT-NEXT:     type @type0 e = enum : u32 {
// DEFAULT-NEXT:         %0 ZERO = const<i32>(0);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type1 e2 = enum : bool {
// DEFAULT-NEXT:         %0 BZERO = const<@type1>(0);
// DEFAULT-NEXT:     } [size=1, align=1];
// DEFAULT-NEXT:     type @type2 e3 = enum : i64 {
// DEFAULT-NEXT:         %0 LZERO = const<@type2>(0);
// DEFAULT-NEXT:     } [size=8, align=8];
// DEFAULT-NEXT:     global %6 p1: ptr<void> [storage=static] = null<ptr<void>> [linkage=external];
// DEFAULT-NEXT:     global %7 p2: ptr<void> [storage=static] = null<ptr<void>> [linkage=external];
// DEFAULT-NEXT:     global %8 p3: ptr<void> [storage=static] = null<ptr<void>> [linkage=external];
// DEFAULT-NEXT:     global %9 p4: ptr<void> [storage=static] = null<ptr<void>> [linkage=external];
// DEFAULT-NEXT:     global %10 p5: ptr<void> [storage=static] = null<ptr<void>> [linkage=external];
// DEFAULT-NEXT:     global %11 p6: ptr<void> [storage=static] = null<ptr<void>> [linkage=external];
// DEFAULT-NEXT:     global %12 p7: ptr<void> [storage=static] = null<ptr<void>> [linkage=external];
// DEFAULT-NEXT:     global %13 p8: ptr<void> [storage=static] = null<ptr<void>> [linkage=external];
// DEFAULT-NEXT:     global %14 p9: ptr<void> [storage=static] = null<ptr<void>> [linkage=external];
// DEFAULT-NEXT:     global %15 p10: ptr<void> [storage=static] = null<ptr<void>> [linkage=external];
// DEFAULT-NEXT:     global %16 p11: ptr<void> [storage=static] = null<ptr<void>> [linkage=external];
// DEFAULT-NEXT:     global %17 p12: ptr<void> [storage=static] = null<ptr<void>> [linkage=external];
// DEFAULT-NEXT:     global %18 p13: ptr<void> [storage=static] = null<ptr<void>> [linkage=external];
// DEFAULT-NEXT:     fn %19 @f(%21 <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %20 @g() -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<ptr<void>>(%6, null<ptr<void>>);
// DEFAULT-NEXT:         write<ptr<void>>(%7, null<ptr<void>>);
// DEFAULT-NEXT:         write<ptr<void>>(%8, null<ptr<void>>);
// DEFAULT-NEXT:         write<ptr<void>>(%9, null<ptr<void>>);
// DEFAULT-NEXT:         write<ptr<void>>(%10, null<ptr<void>>);
// DEFAULT-NEXT:         write<ptr<void>>(%11, null<ptr<void>>);
// DEFAULT-NEXT:         write<ptr<void>>(%12, null<ptr<void>>);
// DEFAULT-NEXT:         write<ptr<void>>(%13, null<ptr<void>>);
// DEFAULT-NEXT:         write<ptr<void>>(%14, null<ptr<void>>);
// DEFAULT-NEXT:         write<ptr<void>>(%15, null<ptr<void>>);
// DEFAULT-NEXT:         write<ptr<void>>(%16, null<ptr<void>>);
// DEFAULT-NEXT:         write<ptr<void>>(%17, null<ptr<void>>);
// DEFAULT-NEXT:         write<ptr<void>>(%18, null<ptr<void>>);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%19, null<ptr<void>>);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%19, null<ptr<void>>);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%19, null<ptr<void>>);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%19, null<ptr<void>>);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%19, null<ptr<void>>);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%19, null<ptr<void>>);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%19, null<ptr<void>>);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%19, null<ptr<void>>);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%19, null<ptr<void>>);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%19, null<ptr<void>>);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%19, null<ptr<void>>);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%19, null<ptr<void>>);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%19, null<ptr<void>>);
// DEFAULT-NEXT:         conditional<ptr<void>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<void>>(%6), null<ptr<void>>);
// DEFAULT-NEXT:         conditional<ptr<void>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<void>>(%6), null<ptr<void>>);
// DEFAULT-NEXT:         conditional<ptr<void>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<void>>(%6), null<ptr<void>>);
// DEFAULT-NEXT:         conditional<ptr<void>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<void>>(%6), null<ptr<void>>);
// DEFAULT-NEXT:         conditional<ptr<void>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<void>>(%6), null<ptr<void>>);
// DEFAULT-NEXT:         conditional<ptr<void>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<void>>(%6), null<ptr<void>>);
// DEFAULT-NEXT:         conditional<ptr<void>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<void>>(%6), null<ptr<void>>);
// DEFAULT-NEXT:         conditional<ptr<void>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<void>>(%6), null<ptr<void>>);
// DEFAULT-NEXT:         conditional<ptr<void>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<void>>(%6), null<ptr<void>>);
// DEFAULT-NEXT:         conditional<ptr<void>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<void>>(%6), null<ptr<void>>);
// DEFAULT-NEXT:         conditional<ptr<void>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<void>>(%6), null<ptr<void>>);
// DEFAULT-NEXT:         conditional<ptr<void>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<void>>(%6), null<ptr<void>>);
// DEFAULT-NEXT:         conditional<ptr<void>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<void>>(%6), null<ptr<void>>);
// DEFAULT-NEXT:         eq<ptr<void>>(read<ptr<void>>(%6), null<ptr<void>>);
// DEFAULT-NEXT:         eq<ptr<void>>(read<ptr<void>>(%6), null<ptr<void>>);
// DEFAULT-NEXT:         eq<ptr<void>>(read<ptr<void>>(%6), null<ptr<void>>);
// DEFAULT-NEXT:         eq<ptr<void>>(read<ptr<void>>(%6), null<ptr<void>>);
// DEFAULT-NEXT:         eq<ptr<void>>(read<ptr<void>>(%6), null<ptr<void>>);
// DEFAULT-NEXT:         eq<ptr<void>>(read<ptr<void>>(%6), null<ptr<void>>);
// DEFAULT-NEXT:         eq<ptr<void>>(read<ptr<void>>(%6), null<ptr<void>>);
// DEFAULT-NEXT:         eq<ptr<void>>(read<ptr<void>>(%6), null<ptr<void>>);
// DEFAULT-NEXT:         eq<ptr<void>>(read<ptr<void>>(%6), null<ptr<void>>);
// DEFAULT-NEXT:         eq<ptr<void>>(read<ptr<void>>(%6), null<ptr<void>>);
// DEFAULT-NEXT:         eq<ptr<void>>(read<ptr<void>>(%6), null<ptr<void>>);
// DEFAULT-NEXT:         eq<ptr<void>>(read<ptr<void>>(%6), null<ptr<void>>);
// DEFAULT-NEXT:         eq<ptr<void>>(read<ptr<void>>(%6), null<ptr<void>>);
// DEFAULT-NEXT:         ne<ptr<void>>(read<ptr<void>>(%6), null<ptr<void>>);
// DEFAULT-NEXT:         ne<ptr<void>>(read<ptr<void>>(%6), null<ptr<void>>);
// DEFAULT-NEXT:         ne<ptr<void>>(read<ptr<void>>(%6), null<ptr<void>>);
// DEFAULT-NEXT:         ne<ptr<void>>(read<ptr<void>>(%6), null<ptr<void>>);
// DEFAULT-NEXT:         ne<ptr<void>>(read<ptr<void>>(%6), null<ptr<void>>);
// DEFAULT-NEXT:         ne<ptr<void>>(read<ptr<void>>(%6), null<ptr<void>>);
// DEFAULT-NEXT:         ne<ptr<void>>(read<ptr<void>>(%6), null<ptr<void>>);
// DEFAULT-NEXT:         ne<ptr<void>>(read<ptr<void>>(%6), null<ptr<void>>);
// DEFAULT-NEXT:         ne<ptr<void>>(read<ptr<void>>(%6), null<ptr<void>>);
// DEFAULT-NEXT:         ne<ptr<void>>(read<ptr<void>>(%6), null<ptr<void>>);
// DEFAULT-NEXT:         ne<ptr<void>>(read<ptr<void>>(%6), null<ptr<void>>);
// DEFAULT-NEXT:         ne<ptr<void>>(read<ptr<void>>(%6), null<ptr<void>>);
// DEFAULT-NEXT:         ne<ptr<void>>(read<ptr<void>>(%6), null<ptr<void>>);
// DEFAULT-NEXT:         return null<ptr<void>>;
// DEFAULT-NEXT:         return null<ptr<void>>;
// DEFAULT-NEXT:         return null<ptr<void>>;
// DEFAULT-NEXT:         return null<ptr<void>>;
// DEFAULT-NEXT:         return null<ptr<void>>;
// DEFAULT-NEXT:         return null<ptr<void>>;
// DEFAULT-NEXT:         return null<ptr<void>>;
// DEFAULT-NEXT:         return null<ptr<void>>;
// DEFAULT-NEXT:         return null<ptr<void>>;
// DEFAULT-NEXT:         return null<ptr<void>>;
// DEFAULT-NEXT:         return null<ptr<void>>;
// DEFAULT-NEXT:         return null<ptr<void>>;
// DEFAULT-NEXT:         return null<ptr<void>>;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
