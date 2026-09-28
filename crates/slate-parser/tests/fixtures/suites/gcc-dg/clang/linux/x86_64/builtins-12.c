/* Copyright (C) 2003  Free Software Foundation.

   Verify that all the malloc-like __builtin_ allocation functions are
   recognized by the compiler.

   Written by Roger Sayle, 12th April 2003.  */

/* { dg-do compile } */
/* { dg-options "-ansi" } */
/* { dg-final { scan-assembler-not "__builtin_" } } */

typedef __SIZE_TYPE__ size_t;

void *test1(size_t n)
{
  return __builtin_malloc(n);
}

void *test2(size_t n, size_t s)
{
  return __builtin_calloc(n,s);
}

char *test3(const char *ptr)
{
  return __builtin_strdup(ptr);
}

// SLATE-FILECHECK-STD DEFAULT c89
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
// DEFAULT-NEXT:     fn %9 @__builtin_malloc(%8 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %1 @test1(%2 n: u64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u64) -> ptr<void>>(%9, read<u64>(%2));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @__builtin_calloc(%10 <unnamed>: u64, %11 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %3 @test2(%4 n: u64, %5 s: u64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%12, read<u64>(%4), read<u64>(%5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @__builtin_strdup(%13 <unnamed>: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %6 @test3(%7 ptr: ptr<const i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%14, read<ptr<const i8>>(%7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
