/* Test __atomic routines for existence and proper execution on 8 byte 
   values with each valid memory model.  */
/* { dg-do run } */
/* { dg-require-effective-target sync_long_long_runtime } */
/* { dg-options "" } */
/* { dg-options "-march=pentium" { target { { i?86-*-* x86_64-*-* } && ia32 } } } */

/* Test the execution of the __atomic_X builtin for a long_long.  */

extern void abort(void);

long long v, count, ret;

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
// DEFAULT-NEXT:     global %1 v: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 count: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 ret: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i64>(%1, widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         write<i64>(%2, widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %5: i64 [synthetic] = update<i64, result=old, atomic=relaxed>(deref(addr_of<ptr<i64>>(%1)), add<i64, overflow=ub>(read<i64>(%2), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%5), read<i64>(%2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %6: i64 [synthetic] = read<i64>(%2);
// DEFAULT-NEXT:         let %7: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%6), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%2, read<i64>(%7));
// DEFAULT-NEXT:         let %8: i64 [synthetic] = update<i64, result=old, atomic=acquire>(deref(addr_of<ptr<i64>>(%1)), add<i64, overflow=ub>(read<i64>(%2), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%8), read<i64>(%2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %9: i64 [synthetic] = read<i64>(%2);
// DEFAULT-NEXT:         let %10: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%9), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%2, read<i64>(%10));
// DEFAULT-NEXT:         let %11: i64 [synthetic] = update<i64, result=old, atomic=release>(deref(addr_of<ptr<i64>>(%1)), add<i64, overflow=ub>(read<i64>(%2), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%11), read<i64>(%2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %12: i64 [synthetic] = read<i64>(%2);
// DEFAULT-NEXT:         let %13: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%12), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%2, read<i64>(%13));
// DEFAULT-NEXT:         let %14: i64 [synthetic] = update<i64, result=old, atomic=acq_rel>(deref(addr_of<ptr<i64>>(%1)), add<i64, overflow=ub>(read<i64>(%2), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%14), read<i64>(%2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %15: i64 [synthetic] = read<i64>(%2);
// DEFAULT-NEXT:         let %16: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%15), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%2, read<i64>(%16));
// DEFAULT-NEXT:         let %17: i64 [synthetic] = update<i64, result=old, atomic=seq_cst>(deref(addr_of<ptr<i64>>(%1)), add<i64, overflow=ub>(read<i64>(%2), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%17), read<i64>(%2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %18: i64 [synthetic] = read<i64>(%2);
// DEFAULT-NEXT:         let %19: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%18), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%2, read<i64>(%19));
// DEFAULT-NEXT:         let %20: i64 [synthetic] = read<i64>(%2);
// DEFAULT-NEXT:         let %21: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%20), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%2, read<i64>(%21));
// DEFAULT-NEXT:         let %22: i64 [synthetic] = update<i64, result=old, atomic=relaxed>(deref(addr_of<ptr<i64>>(%1)), read<i64>(deref(addr_of<ptr<i64>>(%2))));
// DEFAULT-NEXT:         write<i64>(deref(addr_of<ptr<i64>>(%3)), read<i64>(%22));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i64>(read<i64>(%3), sub<i64, overflow=ub>(read<i64>(%2), widen<i64, reason=usual_arith>(const<i32>(1)))), ne<i64>(read<i64>(%1), read<i64>(%2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %23: i64 [synthetic] = read<i64>(%2);
// DEFAULT-NEXT:         let %24: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%23), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%2, read<i64>(%24));
// DEFAULT-NEXT:         let %25: i64 [synthetic] = update<i64, result=old, atomic=acquire>(deref(addr_of<ptr<i64>>(%1)), read<i64>(deref(addr_of<ptr<i64>>(%2))));
// DEFAULT-NEXT:         write<i64>(deref(addr_of<ptr<i64>>(%3)), read<i64>(%25));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i64>(read<i64>(%3), sub<i64, overflow=ub>(read<i64>(%2), widen<i64, reason=usual_arith>(const<i32>(1)))), ne<i64>(read<i64>(%1), read<i64>(%2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %26: i64 [synthetic] = read<i64>(%2);
// DEFAULT-NEXT:         let %27: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%26), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%2, read<i64>(%27));
// DEFAULT-NEXT:         let %28: i64 [synthetic] = update<i64, result=old, atomic=release>(deref(addr_of<ptr<i64>>(%1)), read<i64>(deref(addr_of<ptr<i64>>(%2))));
// DEFAULT-NEXT:         write<i64>(deref(addr_of<ptr<i64>>(%3)), read<i64>(%28));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i64>(read<i64>(%3), sub<i64, overflow=ub>(read<i64>(%2), widen<i64, reason=usual_arith>(const<i32>(1)))), ne<i64>(read<i64>(%1), read<i64>(%2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %29: i64 [synthetic] = read<i64>(%2);
// DEFAULT-NEXT:         let %30: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%29), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%2, read<i64>(%30));
// DEFAULT-NEXT:         let %31: i64 [synthetic] = update<i64, result=old, atomic=acq_rel>(deref(addr_of<ptr<i64>>(%1)), read<i64>(deref(addr_of<ptr<i64>>(%2))));
// DEFAULT-NEXT:         write<i64>(deref(addr_of<ptr<i64>>(%3)), read<i64>(%31));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i64>(read<i64>(%3), sub<i64, overflow=ub>(read<i64>(%2), widen<i64, reason=usual_arith>(const<i32>(1)))), ne<i64>(read<i64>(%1), read<i64>(%2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %32: i64 [synthetic] = read<i64>(%2);
// DEFAULT-NEXT:         let %33: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%32), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%2, read<i64>(%33));
// DEFAULT-NEXT:         let %34: i64 [synthetic] = update<i64, result=old, atomic=seq_cst>(deref(addr_of<ptr<i64>>(%1)), read<i64>(deref(addr_of<ptr<i64>>(%2))));
// DEFAULT-NEXT:         write<i64>(deref(addr_of<ptr<i64>>(%3)), read<i64>(%34));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i64>(read<i64>(%3), sub<i64, overflow=ub>(read<i64>(%2), widen<i64, reason=usual_arith>(const<i32>(1)))), ne<i64>(read<i64>(%1), read<i64>(%2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %35: i64 [synthetic] = read<i64>(%2);
// DEFAULT-NEXT:         let %36: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%35), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%2, read<i64>(%36));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
