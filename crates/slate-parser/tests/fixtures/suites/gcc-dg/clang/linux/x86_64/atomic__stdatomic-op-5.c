/* Test we're able use __atomic_fetch_* where possible and verify
   we generate correct code.  */
/* { dg-do run } */
/* { dg-options "-std=c11 -pedantic-errors -fdump-tree-original" } */
/* { dg-xfail-run-if "PR97444: stack atomics" { nvptx*-*-* } }*/

#include <stdatomic.h>

extern void abort (void);

static void
test_inc_dec (void)
{
  atomic_int i = ATOMIC_VAR_INIT (1);

  i++;
  if (i != 2)
    abort ();
  i--;
  if (i != 1)
    abort ();
  ++i;
  if (i != 2)
    abort ();
  --i;
  if (i != 1)
    abort ();
  if (++i != 2)
    abort ();
  if (i++ != 2)
    abort ();
  if (i != 3)
    abort ();
  if (i-- != 3)
    abort ();
  if (i != 2)
    abort ();
}

static void
test_add_sub (void)
{
  atomic_int i = ATOMIC_VAR_INIT (1);

  i += 2;
  if (i != 3)
    abort ();
  i -= 2;
  if (i != 1)
    abort ();
  if ((i += 2) != 3)
    abort ();
  if ((i -= 2) != 1)
    abort ();
}

static void
test_and (void)
{
  atomic_int i = ATOMIC_VAR_INIT (5);

  i &= 4;
  if (i != 4)
    abort ();
  if ((i &= 4) != 4)
    abort ();
}

static void
test_xor (void)
{
  atomic_int i = ATOMIC_VAR_INIT (5);

  i ^= 2;
  if (i != 7)
    abort ();
  if ((i ^= 4) != 3)
    abort ();
}

static void
test_or (void)
{
  atomic_int i = ATOMIC_VAR_INIT (5);

  i |= 2;
  if (i != 7)
    abort ();
  if ((i |= 8) != 15)
    abort ();
}

static void
test_ptr (atomic_int *p)
{
  ++*p;
  if (*p != 2)
    abort ();

  *p += 2;
  if (*p != 4)
    abort ();

  (*p)++;
  if (*p != 5)
    abort ();

  --*p;
  if (*p != 4)
    abort ();

  (*p)--;
  if (*p != 3)
    abort ();

  *p -= 2;
  if (*p != 1)
    abort ();

  atomic_int j = ATOMIC_VAR_INIT (0);
  j += *p;
  if (j != 1)
    abort ();

  j -= *p;
  if (j != 0)
    abort ();
}

int
main (void)
{
  atomic_int i = ATOMIC_VAR_INIT (1);
  test_inc_dec ();
  test_add_sub ();
  test_and ();
  test_xor ();
  test_or ();
  test_ptr (&i);
}

/* { dg-final { scan-tree-dump-not "__atomic_compare_exchange" "original" } } */

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
// DEFAULT-NEXT:     type @type0 atomic_int = i32;
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @test_inc_dec() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %3 i: atomic i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         let %17: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(%3, add<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%3), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         let %18: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(%3, sub<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%3), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         let %19: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(%3, add<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%3), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         let %20: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(%3, sub<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%3), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         let %21: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(%3, add<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%21), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         let %22: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(%3, add<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%22), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%3), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         let %23: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(%3, sub<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%23), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%3), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @test_add_sub() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %5 i: atomic i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         let %24: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(%5, add<i32, overflow=ub>(old<i32>, const<i32>(2)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%5), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         let %25: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(%5, sub<i32, overflow=ub>(old<i32>, const<i32>(2)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%5), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         let %26: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(%5, add<i32, overflow=ub>(old<i32>, const<i32>(2)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%26), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         let %27: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(%5, sub<i32, overflow=ub>(old<i32>, const<i32>(2)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%27), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @test_and() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %7 i: atomic i32 [storage=automatic] = const<i32>(5);
// DEFAULT-NEXT:         let %28: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(%7, and<i32>(old<i32>, const<i32>(4)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%7), const<i32>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         let %29: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(%7, and<i32>(old<i32>, const<i32>(4)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%29), const<i32>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @test_xor() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %9 i: atomic i32 [storage=automatic] = const<i32>(5);
// DEFAULT-NEXT:         let %30: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(%9, xor<i32>(old<i32>, const<i32>(2)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%9), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         let %31: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(%9, xor<i32>(old<i32>, const<i32>(4)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%31), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @test_or() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %11 i: atomic i32 [storage=automatic] = const<i32>(5);
// DEFAULT-NEXT:         let %32: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(%11, or<i32>(old<i32>, const<i32>(2)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%11), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         let %33: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(%11, or<i32>(old<i32>, const<i32>(8)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%33), const<i32>(15))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @test_ptr(%13 p: ptr<atomic i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %34: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(deref(read<ptr<atomic i32>>(%13)), add<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(deref(read<ptr<atomic i32>>(%13))), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         let %35: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(deref(read<ptr<atomic i32>>(%13)), add<i32, overflow=ub>(old<i32>, const<i32>(2)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(deref(read<ptr<atomic i32>>(%13))), const<i32>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         let %36: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(read<ptr<atomic i32>>(%13)), add<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(deref(read<ptr<atomic i32>>(%13))), const<i32>(5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         let %37: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(deref(read<ptr<atomic i32>>(%13)), sub<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(deref(read<ptr<atomic i32>>(%13))), const<i32>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         let %38: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(read<ptr<atomic i32>>(%13)), sub<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(deref(read<ptr<atomic i32>>(%13))), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         let %39: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(deref(read<ptr<atomic i32>>(%13)), sub<i32, overflow=ub>(old<i32>, const<i32>(2)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(deref(read<ptr<atomic i32>>(%13))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         let %14 j: atomic i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %40: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(%14, add<i32, overflow=ub>(old<i32>, read<i32, atomic=seq_cst>(deref(read<ptr<atomic i32>>(%13)))));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%14), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         let %41: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(%14, sub<i32, overflow=ub>(old<i32>, read<i32, atomic=seq_cst>(deref(read<ptr<atomic i32>>(%13)))));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%14), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %16 i: atomic i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<atomic i32>) -> void>(%12, addr_of<ptr<atomic i32>>(%16));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
