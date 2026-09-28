/* PR middle-end/20297 */
/* The memcpy FUNCTION_DECL built in the middle-end for block moves got
   hidden visibility from the first push, so the call didn't use the PLT.  */

/* { dg-do compile { target i?86-*-* x86_64-*-* } } */
/* { dg-skip-if "" { *-*-darwin* } } */
/* { dg-require-visibility "" } */
/* { dg-require-effective-target fpic } */
/* { dg-options "-Os -fpic -mstringop-strategy=libcall" } */
/* { dg-final { scan-assembler "memcpy@PLT" } } */

#pragma GCC visibility push(hidden)
#pragma GCC visibility push(default)
extern void* memcpy (void *, const void *, __SIZE_TYPE__);
#pragma GCC visibility pop

struct a { int a[4096]; };

extern void *bar (struct a *, struct a *, int);

void *
foo (struct a *a, struct a *b, int c)
{
  struct a cc = *b;
  return bar (a, &cc, 4 * c);
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
// DEFAULT-NEXT:     type @type0 a = struct {
// DEFAULT-NEXT:         field0 a: array<i32, 4096>;
// DEFAULT-NEXT:     } [size=16384, align=4, offsets=[0]];
// DEFAULT-NEXT:     fn %0 @memcpy(%8 <unnamed>: ptr<void>, %9 <unnamed>: ptr<const void>, %10 <unnamed>: u64) -> ptr<void> [linkage=external] [visibility=hidden];
// DEFAULT-NEXT:     fn %2 @bar(%11 <unnamed>: ptr<@type0>, %12 <unnamed>: ptr<@type0>, %13 <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %3 @foo(%4 a: ptr<@type0>, %5 b: ptr<@type0>, %6 c: i32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7 cc: @type0 [storage=automatic] = copy<@type0, reason=assign>(read<@type0>(deref(read<ptr<@type0>>(%5))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(ptr<@type0>, ptr<@type0>, i32) -> ptr<void>>(%2, read<ptr<@type0>>(%4), addr_of<ptr<@type0>>(%7), mul<i32, overflow=ub>(const<i32>(4), read<i32>(%6)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
