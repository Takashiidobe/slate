/* Test __atomic routines for existence and proper execution on 4 byte 
   values with each valid memory model.  */
/* { dg-do run } */
/* { dg-require-effective-target sync_int_long } */

/* Test the execution of the __atomic_X builtin for an int.  */

extern void abort(void);

int v, count, ret;

int
main ()
{
  v = 0;
  count = 0;

  if (__atomic_exchange_n (&v, count + 1, __ATOMIC_RELAXED) != count)
    abort ();
  count++;

  if (__atomic_exchange_n (&v, count + 1, __ATOMIC_ACQUIRE) != count)
    abort ();
  count++;

  if (__atomic_exchange_n (&v, count + 1, __ATOMIC_RELEASE) != count)
    abort ();
  count++;

  if (__atomic_exchange_n (&v, count + 1, __ATOMIC_ACQ_REL) != count)
    abort ();
  count++;

  if (__atomic_exchange_n (&v, count + 1, __ATOMIC_SEQ_CST) != count)
    abort ();
  count++;

  /* Now test the generic version.  */

  count++;

  __atomic_exchange (&v, &count, &ret, __ATOMIC_RELAXED);
  if (ret != count - 1 || v != count)
    abort ();
  count++;

  __atomic_exchange (&v, &count, &ret, __ATOMIC_ACQUIRE);
  if (ret != count - 1 || v != count)
    abort ();
  count++;

  __atomic_exchange (&v, &count, &ret, __ATOMIC_RELEASE);
  if (ret != count - 1 || v != count)
    abort ();
  count++;

  __atomic_exchange (&v, &count, &ret, __ATOMIC_ACQ_REL);
  if (ret != count - 1 || v != count)
    abort ();
  count++;

  __atomic_exchange (&v, &count, &ret, __ATOMIC_SEQ_CST);
  if (ret != count - 1 || v != count)
    abort ();
  count++;

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
// DEFAULT-NEXT:     global %1 v: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 count: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 ret: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @main(unprototyped) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(%1, const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%2, const<i32>(0));
// DEFAULT-NEXT:         let %5: i32 [synthetic] = update<i32, result=old, atomic=relaxed>(deref(addr_of<ptr<i32>>(%1)), add<i32, overflow=ub>(read<i32>(%2), const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%5), read<i32>(%2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %6: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %7: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%6), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%7));
// DEFAULT-NEXT:         let %8: i32 [synthetic] = update<i32, result=old, atomic=acquire>(deref(addr_of<ptr<i32>>(%1)), add<i32, overflow=ub>(read<i32>(%2), const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%8), read<i32>(%2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %9: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %10: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%9), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%10));
// DEFAULT-NEXT:         let %11: i32 [synthetic] = update<i32, result=old, atomic=release>(deref(addr_of<ptr<i32>>(%1)), add<i32, overflow=ub>(read<i32>(%2), const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%11), read<i32>(%2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %12: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %13: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%12), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%13));
// DEFAULT-NEXT:         let %14: i32 [synthetic] = update<i32, result=old, atomic=acq_rel>(deref(addr_of<ptr<i32>>(%1)), add<i32, overflow=ub>(read<i32>(%2), const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%14), read<i32>(%2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %15: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %16: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%15), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%16));
// DEFAULT-NEXT:         let %17: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%1)), add<i32, overflow=ub>(read<i32>(%2), const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%17), read<i32>(%2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %18: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %19: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%18), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%19));
// DEFAULT-NEXT:         let %20: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %21: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%20), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%21));
// DEFAULT-NEXT:         let %22: i32 [synthetic] = update<i32, result=old, atomic=relaxed>(deref(addr_of<ptr<i32>>(%1)), read<i32>(deref(addr_of<ptr<i32>>(%2))));
// DEFAULT-NEXT:         write<i32>(deref(addr_of<ptr<i32>>(%3)), read<i32>(%22));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(%3), sub<i32, overflow=ub>(read<i32>(%2), const<i32>(1))), ne<i32>(read<i32>(%1), read<i32>(%2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %23: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %24: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%23), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%24));
// DEFAULT-NEXT:         let %25: i32 [synthetic] = update<i32, result=old, atomic=acquire>(deref(addr_of<ptr<i32>>(%1)), read<i32>(deref(addr_of<ptr<i32>>(%2))));
// DEFAULT-NEXT:         write<i32>(deref(addr_of<ptr<i32>>(%3)), read<i32>(%25));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(%3), sub<i32, overflow=ub>(read<i32>(%2), const<i32>(1))), ne<i32>(read<i32>(%1), read<i32>(%2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %26: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %27: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%26), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%27));
// DEFAULT-NEXT:         let %28: i32 [synthetic] = update<i32, result=old, atomic=release>(deref(addr_of<ptr<i32>>(%1)), read<i32>(deref(addr_of<ptr<i32>>(%2))));
// DEFAULT-NEXT:         write<i32>(deref(addr_of<ptr<i32>>(%3)), read<i32>(%28));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(%3), sub<i32, overflow=ub>(read<i32>(%2), const<i32>(1))), ne<i32>(read<i32>(%1), read<i32>(%2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %29: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %30: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%29), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%30));
// DEFAULT-NEXT:         let %31: i32 [synthetic] = update<i32, result=old, atomic=acq_rel>(deref(addr_of<ptr<i32>>(%1)), read<i32>(deref(addr_of<ptr<i32>>(%2))));
// DEFAULT-NEXT:         write<i32>(deref(addr_of<ptr<i32>>(%3)), read<i32>(%31));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(%3), sub<i32, overflow=ub>(read<i32>(%2), const<i32>(1))), ne<i32>(read<i32>(%1), read<i32>(%2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %32: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %33: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%32), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%33));
// DEFAULT-NEXT:         let %34: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%1)), read<i32>(deref(addr_of<ptr<i32>>(%2))));
// DEFAULT-NEXT:         write<i32>(deref(addr_of<ptr<i32>>(%3)), read<i32>(%34));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(%3), sub<i32, overflow=ub>(read<i32>(%2), const<i32>(1))), ne<i32>(read<i32>(%1), read<i32>(%2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %35: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %36: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%35), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%36));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
