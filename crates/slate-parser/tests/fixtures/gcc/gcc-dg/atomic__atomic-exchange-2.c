/* Test __atomic routines for existence and proper execution on 2 byte 
   values with each valid memory model.  */
/* { dg-do run } */
/* { dg-require-effective-target sync_char_short } */

/* Test the execution of the __atomic_X builtin for a short.  */

extern void abort(void);

short v, count, ret;

int main() {
  v     = 0;
  count = 0;

  if (__atomic_exchange_n(&v, count + 1, __ATOMIC_RELAXED) != count)
    abort();
  count++;

  if (__atomic_exchange_n(&v, count + 1, __ATOMIC_ACQUIRE) != count)
    abort();
  count++;

  if (__atomic_exchange_n(&v, count + 1, __ATOMIC_RELEASE) != count)
    abort();
  count++;

  if (__atomic_exchange_n(&v, count + 1, __ATOMIC_ACQ_REL) != count)
    abort();
  count++;

  if (__atomic_exchange_n(&v, count + 1, __ATOMIC_SEQ_CST) != count)
    abort();
  count++;

  /* Now test the generic version.  */

  count++;

  __atomic_exchange(&v, &count, &ret, __ATOMIC_RELAXED);
  if (ret != count - 1 || v != count)
    abort();
  count++;

  __atomic_exchange(&v, &count, &ret, __ATOMIC_ACQUIRE);
  if (ret != count - 1 || v != count)
    abort();
  count++;

  __atomic_exchange(&v, &count, &ret, __ATOMIC_RELEASE);
  if (ret != count - 1 || v != count)
    abort();
  count++;

  __atomic_exchange(&v, &count, &ret, __ATOMIC_ACQ_REL);
  if (ret != count - 1 || v != count)
    abort();
  count++;

  __atomic_exchange(&v, &count, &ret, __ATOMIC_SEQ_CST);
  if (ret != count - 1 || v != count)
    abort();
  count++;

  return 0;
}



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
// DEFAULT-NEXT:     global %1 v: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 count: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 ret: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i16>(%1, truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i16>(%2, truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %5: i16 [synthetic] = update<i16, result=old, atomic=relaxed>(deref(addr_of<ptr<i16>>(%1)), truncate<i16, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%2)), const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%5)), widen<i32, reason=promotion>(read<i16>(%2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %6: i16 [synthetic] = read<i16>(%2);
// DEFAULT-NEXT:         let %7: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%6)), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%2, read<i16>(%7));
// DEFAULT-NEXT:         let %8: i16 [synthetic] = update<i16, result=old, atomic=acquire>(deref(addr_of<ptr<i16>>(%1)), truncate<i16, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%2)), const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%8)), widen<i32, reason=promotion>(read<i16>(%2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %9: i16 [synthetic] = read<i16>(%2);
// DEFAULT-NEXT:         let %10: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%9)), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%2, read<i16>(%10));
// DEFAULT-NEXT:         let %11: i16 [synthetic] = update<i16, result=old, atomic=release>(deref(addr_of<ptr<i16>>(%1)), truncate<i16, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%2)), const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%11)), widen<i32, reason=promotion>(read<i16>(%2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %12: i16 [synthetic] = read<i16>(%2);
// DEFAULT-NEXT:         let %13: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%12)), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%2, read<i16>(%13));
// DEFAULT-NEXT:         let %14: i16 [synthetic] = update<i16, result=old, atomic=acq_rel>(deref(addr_of<ptr<i16>>(%1)), truncate<i16, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%2)), const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%14)), widen<i32, reason=promotion>(read<i16>(%2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %15: i16 [synthetic] = read<i16>(%2);
// DEFAULT-NEXT:         let %16: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%15)), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%2, read<i16>(%16));
// DEFAULT-NEXT:         let %17: i16 [synthetic] = update<i16, result=old, atomic=seq_cst>(deref(addr_of<ptr<i16>>(%1)), truncate<i16, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%2)), const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%17)), widen<i32, reason=promotion>(read<i16>(%2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %18: i16 [synthetic] = read<i16>(%2);
// DEFAULT-NEXT:         let %19: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%18)), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%2, read<i16>(%19));
// DEFAULT-NEXT:         let %20: i16 [synthetic] = read<i16>(%2);
// DEFAULT-NEXT:         let %21: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%20)), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%2, read<i16>(%21));
// DEFAULT-NEXT:         let %22: i16 [synthetic] = update<i16, result=old, atomic=relaxed>(deref(addr_of<ptr<i16>>(%1)), read<i16>(deref(addr_of<ptr<i16>>(%2))));
// DEFAULT-NEXT:         write<i16>(deref(addr_of<ptr<i16>>(%3)), read<i16>(%22));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i16>(%3)), sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%2)), const<i32>(1))), ne<i32>(widen<i32, reason=promotion>(read<i16>(%1)), widen<i32, reason=promotion>(read<i16>(%2))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %23: i16 [synthetic] = read<i16>(%2);
// DEFAULT-NEXT:         let %24: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%23)), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%2, read<i16>(%24));
// DEFAULT-NEXT:         let %25: i16 [synthetic] = update<i16, result=old, atomic=acquire>(deref(addr_of<ptr<i16>>(%1)), read<i16>(deref(addr_of<ptr<i16>>(%2))));
// DEFAULT-NEXT:         write<i16>(deref(addr_of<ptr<i16>>(%3)), read<i16>(%25));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i16>(%3)), sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%2)), const<i32>(1))), ne<i32>(widen<i32, reason=promotion>(read<i16>(%1)), widen<i32, reason=promotion>(read<i16>(%2))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %26: i16 [synthetic] = read<i16>(%2);
// DEFAULT-NEXT:         let %27: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%26)), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%2, read<i16>(%27));
// DEFAULT-NEXT:         let %28: i16 [synthetic] = update<i16, result=old, atomic=release>(deref(addr_of<ptr<i16>>(%1)), read<i16>(deref(addr_of<ptr<i16>>(%2))));
// DEFAULT-NEXT:         write<i16>(deref(addr_of<ptr<i16>>(%3)), read<i16>(%28));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i16>(%3)), sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%2)), const<i32>(1))), ne<i32>(widen<i32, reason=promotion>(read<i16>(%1)), widen<i32, reason=promotion>(read<i16>(%2))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %29: i16 [synthetic] = read<i16>(%2);
// DEFAULT-NEXT:         let %30: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%29)), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%2, read<i16>(%30));
// DEFAULT-NEXT:         let %31: i16 [synthetic] = update<i16, result=old, atomic=acq_rel>(deref(addr_of<ptr<i16>>(%1)), read<i16>(deref(addr_of<ptr<i16>>(%2))));
// DEFAULT-NEXT:         write<i16>(deref(addr_of<ptr<i16>>(%3)), read<i16>(%31));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i16>(%3)), sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%2)), const<i32>(1))), ne<i32>(widen<i32, reason=promotion>(read<i16>(%1)), widen<i32, reason=promotion>(read<i16>(%2))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %32: i16 [synthetic] = read<i16>(%2);
// DEFAULT-NEXT:         let %33: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%32)), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%2, read<i16>(%33));
// DEFAULT-NEXT:         let %34: i16 [synthetic] = update<i16, result=old, atomic=seq_cst>(deref(addr_of<ptr<i16>>(%1)), read<i16>(deref(addr_of<ptr<i16>>(%2))));
// DEFAULT-NEXT:         write<i16>(deref(addr_of<ptr<i16>>(%3)), read<i16>(%34));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i16>(%3)), sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%2)), const<i32>(1))), ne<i32>(widen<i32, reason=promotion>(read<i16>(%1)), widen<i32, reason=promotion>(read<i16>(%2))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %35: i16 [synthetic] = read<i16>(%2);
// DEFAULT-NEXT:         let %36: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%35)), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%2, read<i16>(%36));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
