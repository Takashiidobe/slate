/* Test __atomic routines for existence and proper execution on 4 byte 
   values with each valid memory model.  */
/* { dg-do run } */
/* { dg-require-effective-target sync_int_long } */

/* Test the execution of the __atomic_compare_exchange_n builtin for an int.  */

extern void abort(void);

int v = 0;
int expected = 0;
int max = ~0;
int desired = ~0;
int zero = 0;

#define STRONG 0
#define WEAK 1

int
main ()
{

  if (!__atomic_compare_exchange_n (&v, &expected, max, STRONG , __ATOMIC_RELAXED, __ATOMIC_RELAXED)) 
    abort ();
  if (expected != 0)
    abort ();

  if (__atomic_compare_exchange_n (&v, &expected, 0, STRONG , __ATOMIC_ACQUIRE, __ATOMIC_RELAXED)) 
    abort ();
  if (expected != max)
    abort ();

  if (!__atomic_compare_exchange_n (&v, &expected, 0, STRONG , __ATOMIC_RELEASE, __ATOMIC_ACQUIRE)) 
    abort ();
  if (expected != max)
    abort ();
  if (v != 0)
    abort ();

  if (__atomic_compare_exchange_n (&v, &expected, desired, WEAK, __ATOMIC_ACQ_REL, __ATOMIC_ACQUIRE)) 
    abort ();
  if (expected != 0)
    abort ();

  if (!__atomic_compare_exchange_n (&v, &expected, desired, STRONG , __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)) 
    abort ();
  if (expected != 0)
    abort ();
  if (v != max)
    abort ();

  /* Now test the generic version.  */

  v = 0;

  if (!__atomic_compare_exchange (&v, &expected, &max, STRONG, __ATOMIC_RELAXED, __ATOMIC_RELAXED))
    abort ();
  if (expected != 0)
    abort ();

  if (__atomic_compare_exchange (&v, &expected, &zero, STRONG , __ATOMIC_ACQUIRE, __ATOMIC_RELAXED)) 
    abort ();
  if (expected != max)
    abort ();

  if (!__atomic_compare_exchange (&v, &expected, &zero, STRONG , __ATOMIC_RELEASE, __ATOMIC_ACQUIRE)) 
    abort ();
  if (expected != max)
    abort ();
  if (v != 0)
    abort ();

  if (__atomic_compare_exchange (&v, &expected, &desired, WEAK, __ATOMIC_ACQ_REL, __ATOMIC_ACQUIRE)) 
    abort ();
  if (expected != 0)
    abort ();

  if (!__atomic_compare_exchange (&v, &expected, &desired, STRONG , __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)) 
    abort ();
  if (expected != 0)
    abort ();
  if (v != max)
    abort ();

  return 0;
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
// DEFAULT-NEXT:     global %1 v: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %2 expected: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %3 max: i32 [storage=static] = not<i32>(const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %4 desired: i32 [storage=static] = not<i32>(const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %5 zero: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %6 @main(unprototyped) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7: bool [synthetic] = compare_exchange<i32, form=write_back, weak=false, success=relaxed, failure=relaxed>(deref(addr_of<ptr<i32>>(%1)), addr_of<ptr<i32>>(%2), read<i32>(%3));
// DEFAULT-NEXT:         if not<bool>(read<bool>(%7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%2), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %8: bool [synthetic] = compare_exchange<i32, form=write_back, weak=false, success=acquire, failure=relaxed>(deref(addr_of<ptr<i32>>(%1)), addr_of<ptr<i32>>(%2), const<i32>(0));
// DEFAULT-NEXT:         if read<bool>(%8)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%2), read<i32>(%3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %9: bool [synthetic] = compare_exchange<i32, form=write_back, weak=false, success=release, failure=acquire>(deref(addr_of<ptr<i32>>(%1)), addr_of<ptr<i32>>(%2), const<i32>(0));
// DEFAULT-NEXT:         if not<bool>(read<bool>(%9))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%2), read<i32>(%3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %10: bool [synthetic] = compare_exchange<i32, form=write_back, weak=true, success=acq_rel, failure=acquire>(deref(addr_of<ptr<i32>>(%1)), addr_of<ptr<i32>>(%2), read<i32>(%4));
// DEFAULT-NEXT:         if read<bool>(%10)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%2), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %11: bool [synthetic] = compare_exchange<i32, form=write_back, weak=false, success=seq_cst, failure=seq_cst>(deref(addr_of<ptr<i32>>(%1)), addr_of<ptr<i32>>(%2), read<i32>(%4));
// DEFAULT-NEXT:         if not<bool>(read<bool>(%11))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%2), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), read<i32>(%3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i32>(%1, const<i32>(0));
// DEFAULT-NEXT:         let %12: bool [synthetic] = compare_exchange<i32, form=write_back, weak=false, success=relaxed, failure=relaxed>(deref(addr_of<ptr<i32>>(%1)), addr_of<ptr<i32>>(%2), read<i32>(deref(addr_of<ptr<i32>>(%3))));
// DEFAULT-NEXT:         if not<bool>(read<bool>(%12))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%2), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %13: bool [synthetic] = compare_exchange<i32, form=write_back, weak=false, success=acquire, failure=relaxed>(deref(addr_of<ptr<i32>>(%1)), addr_of<ptr<i32>>(%2), read<i32>(deref(addr_of<ptr<i32>>(%5))));
// DEFAULT-NEXT:         if read<bool>(%13)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%2), read<i32>(%3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %14: bool [synthetic] = compare_exchange<i32, form=write_back, weak=false, success=release, failure=acquire>(deref(addr_of<ptr<i32>>(%1)), addr_of<ptr<i32>>(%2), read<i32>(deref(addr_of<ptr<i32>>(%5))));
// DEFAULT-NEXT:         if not<bool>(read<bool>(%14))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%2), read<i32>(%3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %15: bool [synthetic] = compare_exchange<i32, form=write_back, weak=true, success=acq_rel, failure=acquire>(deref(addr_of<ptr<i32>>(%1)), addr_of<ptr<i32>>(%2), read<i32>(deref(addr_of<ptr<i32>>(%4))));
// DEFAULT-NEXT:         if read<bool>(%15)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%2), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %16: bool [synthetic] = compare_exchange<i32, form=write_back, weak=false, success=seq_cst, failure=seq_cst>(deref(addr_of<ptr<i32>>(%1)), addr_of<ptr<i32>>(%2), read<i32>(deref(addr_of<ptr<i32>>(%4))));
// DEFAULT-NEXT:         if not<bool>(read<bool>(%16))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%2), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), read<i32>(%3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
