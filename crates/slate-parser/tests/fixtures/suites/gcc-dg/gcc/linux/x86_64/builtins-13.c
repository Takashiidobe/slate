/* Copyright (C) 2003  Free Software Foundation.

   Verify that the malloc-like __builtin_ allocation functions are
   correctly aliased by the compiler.

   Written by Roger Sayle, 12th April 2003.  */

/* { dg-do link } */
/* { dg-options "-ansi" } */

typedef __SIZE_TYPE__ size_t;

extern void abort (void);
extern void *malloc (size_t);
extern void *calloc (size_t,size_t);

extern void link_error (void);

static int x;

void test1(void)
{
  int *ptr1, *ptr2;

  ptr1 = &x;
  ptr2 = (int*) malloc (sizeof (int));

  *ptr1 = 12;
  *ptr2 = 8;

  if (*ptr1 != 12)
    link_error();
}

void test2(void)
{
  int *ptr1, *ptr2;

  ptr1 = &x;
  ptr2 = (int*) calloc (1, sizeof (int));

  *ptr1 = 12;
  *ptr2 = 8;

  if (*ptr1 != 12)
    link_error ();
}

int main()
{
  test1 ();
  test2 ();
  return 0;
}

#ifndef __OPTIMIZE__
void link_error (void)
{
  abort ();
}
#endif


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
// DEFAULT-NEXT:     global %5 x: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @malloc(%13 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %3 @calloc(%14 <unnamed>: u64, %15 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %4 @link_error() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @test1() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %7 ptr1: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         let %8 ptr2: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i32>>(%7, addr_of<ptr<i32>>(%5));
// DEFAULT-NEXT:         write<ptr<i32>>(%8, pointer_cast<ptr<i32>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%2, const<u64>(4))));
// DEFAULT-NEXT:         pointer_cast<ptr<i32>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%2, const<u64>(4)));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%7)), const<i32>(12));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%8)), const<i32>(8));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(read<ptr<i32>>(%7))), const<i32>(12))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @test2() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %10 ptr1: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         let %11 ptr2: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i32>>(%10, addr_of<ptr<i32>>(%5));
// DEFAULT-NEXT:         write<ptr<i32>>(%11, pointer_cast<ptr<i32>, reason=explicit>(call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%3, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), const<u64>(4))));
// DEFAULT-NEXT:         pointer_cast<ptr<i32>, reason=explicit>(call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%3, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), const<u64>(4)));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%10)), const<i32>(12));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%11)), const<i32>(8));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(read<ptr<i32>>(%10))), const<i32>(12))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @main(unprototyped) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
