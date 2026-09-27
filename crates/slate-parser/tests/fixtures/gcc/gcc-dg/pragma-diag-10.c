/* PR middle-end/98512 - #pragma GCC diagnostic ignored ineffective
   in conjunction with alias attribute
   { dg-do compile }
   { dg-options "-O2 -Wall" }
   { dg-require-alias "" } */

void *
__rawmemchr_ppc (const void *s, int c)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-overflow"
#pragma GCC diagnostic ignored "-Wstringop-overread"
  if (c != 0)
    return __builtin_memchr (s, c, (unsigned long)-1);    // { dg-bogus "specified bound \\d+ exceeds maximum object size" }
#pragma GCC diagnostic pop

  return (char *)s + __builtin_strlen (s);
}

extern __typeof (__rawmemchr_ppc) __EI___rawmemchr_ppc
  __attribute__((alias ("__rawmemchr_ppc")));

// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     fn %7 @__builtin_memchr(%4 <unnamed>: ptr<const void>, %5 <unnamed>: i32, %6 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %9 @__builtin_strlen(%8 <unnamed>: ptr<const i8>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %0 @__rawmemchr_ppc(%1 s: ptr<const void>, %2 c: i32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%2), const<i32>(0))
// DEFAULT-NEXT:             return call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%7, read<ptr<const void>>(%1), read<i32>(%2), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         return pointer_cast<ptr<void>, reason=return>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(pointer_cast<ptr<i8>, reason=explicit>(read<ptr<const void>>(%1)), call<u64, signature=fn(ptr<const i8>) -> u64>(%9, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<const void>>(%1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @__EI___rawmemchr_ppc(%10 <unnamed>: ptr<const void>, %11 <unnamed>: i32) -> ptr<void> [linkage=external] [alias="__rawmemchr_ppc"];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
