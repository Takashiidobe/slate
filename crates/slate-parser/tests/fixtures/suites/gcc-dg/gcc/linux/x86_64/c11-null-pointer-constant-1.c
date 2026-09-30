/* Test zero with different types as null pointer constant: bug 112556.  */
/* { dg-do compile } */
/* { dg-options "-std=c11 -pedantic-errors -Wno-pointer-compare" } */

enum e { ZERO };

void *p1 = 0;
void *p2 = 0LL;
void *p3 = (char) 0;
void *p4 = 0UL;
void *p5 = (_Bool) 0;
void *p6 = (enum e) ZERO;

void f (void *);

void *
g (void)
{
  p1 = 0;
  p2 = 0LL;
  p3 = (char) 0;
  p4 = 0UL;
  p5 = (_Bool) 0;
  p6 = (enum e) ZERO;
  f (0);
  f (0ULL);
  f (0L);
  f ((char) 0);
  f ((_Bool) 0);
  f ((enum e) ZERO);
  (1 ? p1 : 0);
  (1 ? p1 : 0L);
  (1 ? p1 : 0ULL);
  (1 ? p1 : (char) 0);
  (1 ? p1 : (_Bool) 0);
  (1 ? p1 : (enum e) 0);
  p1 == 0;
  p1 == 0LL;
  p1 == 0U;
  p1 == (char) 0;
  p1 == (_Bool) 0;
  p1 == (enum e) 0;
  p1 != 0;
  p1 != 0LL;
  p1 != 0U;
  p1 != (char) 0;
  p1 != (_Bool) 0;
  p1 != (enum e) 0;
  return 0;
  return 0UL;
  return 0LL;
  return (char) 0;
  return (_Bool) 0;
  return (enum e) 0;
}

// SLATE-FILECHECK-STD DEFAULT c11
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
// DEFAULT-NEXT:     type @type[[TYPE_e:[0-9]+]] e = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_ZERO:[0-9]+]] ZERO = const<i32>(0);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     global %[[VALUE_p1:[0-9]+]] p1: ptr<void> [storage=static] = null<ptr<void>> [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_p2:[0-9]+]] p2: ptr<void> [storage=static] = null<ptr<void>> [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_p3:[0-9]+]] p3: ptr<void> [storage=static] = null<ptr<void>> [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_p4:[0-9]+]] p4: ptr<void> [storage=static] = null<ptr<void>> [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_p5:[0-9]+]] p5: ptr<void> [storage=static] = null<ptr<void>> [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_p6:[0-9]+]] p6: ptr<void> [storage=static] = null<ptr<void>> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_g:[0-9]+]] @g() -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_p1]], null<ptr<void>>);
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_p2]], null<ptr<void>>);
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_p3]], null<ptr<void>>);
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_p4]], null<ptr<void>>);
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_p5]], null<ptr<void>>);
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_p6]], null<ptr<void>>);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_f]], null<ptr<void>>);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_f]], null<ptr<void>>);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_f]], null<ptr<void>>);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_f]], null<ptr<void>>);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_f]], null<ptr<void>>);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_f]], null<ptr<void>>);
// DEFAULT-NEXT:         conditional<ptr<void>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<void>>(%[[VALUE_p1]]), null<ptr<void>>);
// DEFAULT-NEXT:         conditional<ptr<void>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<void>>(%[[VALUE_p1]]), null<ptr<void>>);
// DEFAULT-NEXT:         conditional<ptr<void>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<void>>(%[[VALUE_p1]]), null<ptr<void>>);
// DEFAULT-NEXT:         conditional<ptr<void>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<void>>(%[[VALUE_p1]]), null<ptr<void>>);
// DEFAULT-NEXT:         conditional<ptr<void>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<void>>(%[[VALUE_p1]]), null<ptr<void>>);
// DEFAULT-NEXT:         conditional<ptr<void>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<void>>(%[[VALUE_p1]]), null<ptr<void>>);
// DEFAULT-NEXT:         eq<ptr<void>>(read<ptr<void>>(%[[VALUE_p1]]), null<ptr<void>>);
// DEFAULT-NEXT:         eq<ptr<void>>(read<ptr<void>>(%[[VALUE_p1]]), null<ptr<void>>);
// DEFAULT-NEXT:         eq<ptr<void>>(read<ptr<void>>(%[[VALUE_p1]]), null<ptr<void>>);
// DEFAULT-NEXT:         eq<ptr<void>>(read<ptr<void>>(%[[VALUE_p1]]), null<ptr<void>>);
// DEFAULT-NEXT:         eq<ptr<void>>(read<ptr<void>>(%[[VALUE_p1]]), null<ptr<void>>);
// DEFAULT-NEXT:         eq<ptr<void>>(read<ptr<void>>(%[[VALUE_p1]]), null<ptr<void>>);
// DEFAULT-NEXT:         ne<ptr<void>>(read<ptr<void>>(%[[VALUE_p1]]), null<ptr<void>>);
// DEFAULT-NEXT:         ne<ptr<void>>(read<ptr<void>>(%[[VALUE_p1]]), null<ptr<void>>);
// DEFAULT-NEXT:         ne<ptr<void>>(read<ptr<void>>(%[[VALUE_p1]]), null<ptr<void>>);
// DEFAULT-NEXT:         ne<ptr<void>>(read<ptr<void>>(%[[VALUE_p1]]), null<ptr<void>>);
// DEFAULT-NEXT:         ne<ptr<void>>(read<ptr<void>>(%[[VALUE_p1]]), null<ptr<void>>);
// DEFAULT-NEXT:         ne<ptr<void>>(read<ptr<void>>(%[[VALUE_p1]]), null<ptr<void>>);
// DEFAULT-NEXT:         return null<ptr<void>>;
// DEFAULT-NEXT:         return null<ptr<void>>;
// DEFAULT-NEXT:         return null<ptr<void>>;
// DEFAULT-NEXT:         return null<ptr<void>>;
// DEFAULT-NEXT:         return null<ptr<void>>;
// DEFAULT-NEXT:         return null<ptr<void>>;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
