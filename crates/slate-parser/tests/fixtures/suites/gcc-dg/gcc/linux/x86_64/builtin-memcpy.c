/* PR tree-optimization/88800 - Spurious -Werror=array-bounds for non-taken
   branch
   Verify that out-of-bounds memcpy calls are not folded when warnings are
   enabled (builtin-memcpy-2.c verifies they're not folded with warnings
   disabled).
   { dg-do compile }
   { dg-options "-O2 -Wall -fdump-tree-optimized" } */

extern void* memcpy (void*, const void*, __SIZE_TYPE__);

char a1[1], a2[2], a4[4], a8[8], a16[16], a32[32];

void f1 (const void *p)
{
  memcpy (a1, p, sizeof a1 * 2);    /* { dg-warning "\\\[-Warray-bounds" } */
}

void f2 (const void *p)
{
  memcpy (a2, p, sizeof a2 * 2);    /* { dg-warning "\\\[-Warray-bounds" } */
}

void f4 (const void *p)
{
  memcpy (a4, p, sizeof a4 * 2);    /* { dg-warning "\\\[-Warray-bounds" } */
}

void f8 (const void *p)
{
  memcpy (a8, p, sizeof a8 * 2);    /* { dg-warning "\\\[-Warray-bounds" } */
}

void f16 (const void *p)
{
  memcpy (a16, p, sizeof a16 * 2);  /* { dg-warning "\\\[-Warray-bounds" } */
}

void f32 (const void *p)
{
  memcpy (a32, p, sizeof a32 * 2);  /* { dg-warning "\\\[-Warray-bounds" } */
}

/* { dg-final { scan-tree-dump-times "memcpy" 6 "optimized" } } */

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
// DEFAULT-NEXT:     global %1 a1: array<i8, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 a2: array<i8, 2> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 a4: array<i8, 4> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 a8: array<i8, 8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 a16: array<i8, 16> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %6 a32: array<i8, 32> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     fn %0 @memcpy(%19 <unnamed>: ptr<void>, %20 <unnamed>: ptr<const void>, %21 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %7 @f1(%8 p: ptr<const void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%0, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%1)), read<ptr<const void>>(%8), mul<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @f2(%10 p: ptr<const void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%0, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%2)), read<ptr<const void>>(%10), mul<u64, overflow=wrap>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @f4(%12 p: ptr<const void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%0, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%3)), read<ptr<const void>>(%12), mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @f8(%14 p: ptr<const void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%0, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%4)), read<ptr<const void>>(%14), mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @f16(%16 p: ptr<const void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%0, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%5)), read<ptr<const void>>(%16), mul<u64, overflow=wrap>(const<u64>(16), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @f32(%18 p: ptr<const void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%0, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(32)>(%6)), read<ptr<const void>>(%18), mul<u64, overflow=wrap>(const<u64>(32), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
