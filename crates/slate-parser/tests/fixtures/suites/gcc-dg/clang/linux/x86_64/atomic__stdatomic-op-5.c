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
// DEFAULT-NEXT:     type @type[[TYPE_atomic_int:[0-9]+]] atomic_int = i32;
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_test_inc_dec:[0-9]+]] @test_inc_dec() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: atomic i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(%[[VALUE_i]], add<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%[[VALUE_i]]), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(%[[VALUE_i]], sub<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%[[VALUE_i]]), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(%[[VALUE_i]], add<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%[[VALUE_i]]), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(%[[VALUE_i]], sub<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%[[VALUE_i]]), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(%[[VALUE_i]], add<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE4]]), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(%[[VALUE_i]], add<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE5]]), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%[[VALUE_i]]), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(%[[VALUE_i]], sub<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE6]]), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%[[VALUE_i]]), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_add_sub:[0-9]+]] @test_add_sub() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_2:[0-9]+]] i: atomic i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(%[[VALUE_i_2]], add<i32, overflow=ub>(old<i32>, const<i32>(2)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%[[VALUE_i_2]]), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(%[[VALUE_i_2]], sub<i32, overflow=ub>(old<i32>, const<i32>(2)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%[[VALUE_i_2]]), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(%[[VALUE_i_2]], add<i32, overflow=ub>(old<i32>, const<i32>(2)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE9]]), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(%[[VALUE_i_2]], sub<i32, overflow=ub>(old<i32>, const<i32>(2)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE10]]), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_and:[0-9]+]] @test_and() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_3:[0-9]+]] i: atomic i32 [storage=automatic] = const<i32>(5);
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(%[[VALUE_i_3]], and<i32>(old<i32>, const<i32>(4)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%[[VALUE_i_3]]), const<i32>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(%[[VALUE_i_3]], and<i32>(old<i32>, const<i32>(4)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE12]]), const<i32>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_xor:[0-9]+]] @test_xor() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_4:[0-9]+]] i: atomic i32 [storage=automatic] = const<i32>(5);
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(%[[VALUE_i_4]], xor<i32>(old<i32>, const<i32>(2)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%[[VALUE_i_4]]), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(%[[VALUE_i_4]], xor<i32>(old<i32>, const<i32>(4)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE14]]), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_or:[0-9]+]] @test_or() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_5:[0-9]+]] i: atomic i32 [storage=automatic] = const<i32>(5);
// DEFAULT-NEXT:         let %[[VALUE15:[0-9]+]]: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(%[[VALUE_i_5]], or<i32>(old<i32>, const<i32>(2)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%[[VALUE_i_5]]), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE16:[0-9]+]]: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(%[[VALUE_i_5]], or<i32>(old<i32>, const<i32>(8)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE16]]), const<i32>(15))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_ptr:[0-9]+]] @test_ptr(%[[VALUE_p:[0-9]+]] p: ptr<atomic i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE17:[0-9]+]]: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(deref(read<ptr<atomic i32>>(%[[VALUE_p]])), add<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(deref(read<ptr<atomic i32>>(%[[VALUE_p]]))), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE18:[0-9]+]]: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(deref(read<ptr<atomic i32>>(%[[VALUE_p]])), add<i32, overflow=ub>(old<i32>, const<i32>(2)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(deref(read<ptr<atomic i32>>(%[[VALUE_p]]))), const<i32>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE19:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(read<ptr<atomic i32>>(%[[VALUE_p]])), add<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(deref(read<ptr<atomic i32>>(%[[VALUE_p]]))), const<i32>(5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE20:[0-9]+]]: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(deref(read<ptr<atomic i32>>(%[[VALUE_p]])), sub<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(deref(read<ptr<atomic i32>>(%[[VALUE_p]]))), const<i32>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE21:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(read<ptr<atomic i32>>(%[[VALUE_p]])), sub<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(deref(read<ptr<atomic i32>>(%[[VALUE_p]]))), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE22:[0-9]+]]: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(deref(read<ptr<atomic i32>>(%[[VALUE_p]])), sub<i32, overflow=ub>(old<i32>, const<i32>(2)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(deref(read<ptr<atomic i32>>(%[[VALUE_p]]))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: atomic i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE23:[0-9]+]]: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(%[[VALUE_j]], add<i32, overflow=ub>(old<i32>, read<i32, atomic=seq_cst>(deref(read<ptr<atomic i32>>(%[[VALUE_p]])))));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%[[VALUE_j]]), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE24:[0-9]+]]: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(%[[VALUE_j]], sub<i32, overflow=ub>(old<i32>, read<i32, atomic=seq_cst>(deref(read<ptr<atomic i32>>(%[[VALUE_p]])))));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%[[VALUE_j]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_i_6:[0-9]+]] i: atomic i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_inc_dec]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_add_sub]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_and]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_xor]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_or]]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<atomic i32>) -> void>(%[[VALUE_test_ptr]], addr_of<ptr<atomic i32>>(%[[VALUE_i_6]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
