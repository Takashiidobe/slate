/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-optimized" } */
/* __stpncpy_chk could return buf up to buf + 64, so
   the minimum object size might be far smaller than 64.  */
/* { dg-final { scan-tree-dump-not "return 64;" "optimized" } } */

typedef __SIZE_TYPE__ size_t;

size_t
foo (const char *p, size_t s, size_t t)
{
  char buf[64];
  char *q = __builtin___stpncpy_chk (buf, p, s, t);
  return __builtin_object_size (q, 2);
}

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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     fn %11 @__builtin___stpncpy_chk(%7 <unnamed>: ptr<i8>, %8 <unnamed>: ptr<const i8>, %9 <unnamed>: u64, %10 <unnamed>: u64) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %14 @__builtin_object_size(%12 <unnamed>: ptr<const void>, %13 <unnamed>: i32) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %1 @foo(%2 p: ptr<const i8>, %3 s: u64, %4 t: u64) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 buf: array<i8, 64> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %6 q: ptr<i8> [storage=automatic] = call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64, u64) -> ptr<i8>>(%11, array_decay<ptr<i8>, length=Some(64)>(%5), read<ptr<const i8>>(%2), read<u64>(%3), read<u64>(%4));
// DEFAULT-NEXT:         return call<u64, signature=fn(ptr<const void>, i32) -> u64>(%14, pointer_cast<ptr<const void>, reason=arg>(read<ptr<i8>>(%6)), const<i32>(2));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
