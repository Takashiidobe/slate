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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     global %[[VALUE_x:[0-9]+]] x: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_malloc:[0-9]+]] @malloc(%[[VALUE0:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_calloc:[0-9]+]] @calloc(%[[VALUE1:[0-9]+]] <unnamed>: u64, %[[VALUE2:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_link_error:[0-9]+]] @link_error() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test1:[0-9]+]] @test1() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ptr1:[0-9]+]] ptr1: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ptr2:[0-9]+]] ptr2: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_ptr1]], addr_of<ptr<i32>>(%[[VALUE_x]]));
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_ptr2]], pointer_cast<ptr<i32>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_malloc]], const<u64>(4))));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE_ptr1]])), const<i32>(12));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE_ptr2]])), const<i32>(8));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(read<ptr<i32>>(%[[VALUE_ptr1]]))), const<i32>(12))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2:[0-9]+]] @test2() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ptr1_2:[0-9]+]] ptr1: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ptr2_2:[0-9]+]] ptr2: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_ptr1_2]], addr_of<ptr<i32>>(%[[VALUE_x]]));
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_ptr2_2]], pointer_cast<ptr<i32>, reason=explicit>(call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_calloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), const<u64>(4))));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE_ptr1_2]])), const<i32>(12));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE_ptr2_2]])), const<i32>(8));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(read<ptr<i32>>(%[[VALUE_ptr1_2]]))), const<i32>(12))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(unprototyped) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test1]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test2]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
