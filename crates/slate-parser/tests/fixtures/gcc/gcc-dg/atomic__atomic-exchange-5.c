/* Test __atomic routines for existence and proper execution on 16 byte 
   values with each valid memory model.  */
/* { dg-do run } */
/* { dg-require-effective-target sync_int_128_runtime } */
/* { dg-options "-mcx16" { target { i?86-*-* x86_64-*-* } } } */
/* { dg-options "-mlsx -mscq" { target { loongarch64-*-* } } } */

/* Test the execution of the __atomic_X builtin for a 16 byte value.  */

extern void abort(void);

__int128_t v, count, ret;

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
// DEFAULT-NEXT:     global %1 v: i128 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 count: i128 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 ret: i128 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i128>(%1, widen<i128, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         write<i128>(%2, widen<i128, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %5: i128 [synthetic] = update<i128, result=old, atomic=relaxed>(deref(addr_of<ptr<i128>>(%1)), add<i128, overflow=ub>(read<i128>(%2), widen<i128, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%5), read<i128>(%2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %6: i128 [synthetic] = read<i128>(%2);
// DEFAULT-NEXT:         let %7: i128 [synthetic] = add<i128, overflow=ub>(read<i128>(%6), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%2, read<i128>(%7));
// DEFAULT-NEXT:         let %8: i128 [synthetic] = update<i128, result=old, atomic=acquire>(deref(addr_of<ptr<i128>>(%1)), add<i128, overflow=ub>(read<i128>(%2), widen<i128, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%8), read<i128>(%2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %9: i128 [synthetic] = read<i128>(%2);
// DEFAULT-NEXT:         let %10: i128 [synthetic] = add<i128, overflow=ub>(read<i128>(%9), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%2, read<i128>(%10));
// DEFAULT-NEXT:         let %11: i128 [synthetic] = update<i128, result=old, atomic=release>(deref(addr_of<ptr<i128>>(%1)), add<i128, overflow=ub>(read<i128>(%2), widen<i128, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%11), read<i128>(%2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %12: i128 [synthetic] = read<i128>(%2);
// DEFAULT-NEXT:         let %13: i128 [synthetic] = add<i128, overflow=ub>(read<i128>(%12), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%2, read<i128>(%13));
// DEFAULT-NEXT:         let %14: i128 [synthetic] = update<i128, result=old, atomic=acq_rel>(deref(addr_of<ptr<i128>>(%1)), add<i128, overflow=ub>(read<i128>(%2), widen<i128, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%14), read<i128>(%2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %15: i128 [synthetic] = read<i128>(%2);
// DEFAULT-NEXT:         let %16: i128 [synthetic] = add<i128, overflow=ub>(read<i128>(%15), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%2, read<i128>(%16));
// DEFAULT-NEXT:         let %17: i128 [synthetic] = update<i128, result=old, atomic=seq_cst>(deref(addr_of<ptr<i128>>(%1)), add<i128, overflow=ub>(read<i128>(%2), widen<i128, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%17), read<i128>(%2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %18: i128 [synthetic] = read<i128>(%2);
// DEFAULT-NEXT:         let %19: i128 [synthetic] = add<i128, overflow=ub>(read<i128>(%18), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%2, read<i128>(%19));
// DEFAULT-NEXT:         let %20: i128 [synthetic] = read<i128>(%2);
// DEFAULT-NEXT:         let %21: i128 [synthetic] = add<i128, overflow=ub>(read<i128>(%20), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%2, read<i128>(%21));
// DEFAULT-NEXT:         let %22: i128 [synthetic] = update<i128, result=old, atomic=relaxed>(deref(addr_of<ptr<i128>>(%1)), read<i128>(deref(addr_of<ptr<i128>>(%2))));
// DEFAULT-NEXT:         write<i128>(deref(addr_of<ptr<i128>>(%3)), read<i128>(%22));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i128>(read<i128>(%3), sub<i128, overflow=ub>(read<i128>(%2), widen<i128, reason=usual_arith>(const<i32>(1)))), ne<i128>(read<i128>(%1), read<i128>(%2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %23: i128 [synthetic] = read<i128>(%2);
// DEFAULT-NEXT:         let %24: i128 [synthetic] = add<i128, overflow=ub>(read<i128>(%23), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%2, read<i128>(%24));
// DEFAULT-NEXT:         let %25: i128 [synthetic] = update<i128, result=old, atomic=acquire>(deref(addr_of<ptr<i128>>(%1)), read<i128>(deref(addr_of<ptr<i128>>(%2))));
// DEFAULT-NEXT:         write<i128>(deref(addr_of<ptr<i128>>(%3)), read<i128>(%25));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i128>(read<i128>(%3), sub<i128, overflow=ub>(read<i128>(%2), widen<i128, reason=usual_arith>(const<i32>(1)))), ne<i128>(read<i128>(%1), read<i128>(%2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %26: i128 [synthetic] = read<i128>(%2);
// DEFAULT-NEXT:         let %27: i128 [synthetic] = add<i128, overflow=ub>(read<i128>(%26), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%2, read<i128>(%27));
// DEFAULT-NEXT:         let %28: i128 [synthetic] = update<i128, result=old, atomic=release>(deref(addr_of<ptr<i128>>(%1)), read<i128>(deref(addr_of<ptr<i128>>(%2))));
// DEFAULT-NEXT:         write<i128>(deref(addr_of<ptr<i128>>(%3)), read<i128>(%28));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i128>(read<i128>(%3), sub<i128, overflow=ub>(read<i128>(%2), widen<i128, reason=usual_arith>(const<i32>(1)))), ne<i128>(read<i128>(%1), read<i128>(%2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %29: i128 [synthetic] = read<i128>(%2);
// DEFAULT-NEXT:         let %30: i128 [synthetic] = add<i128, overflow=ub>(read<i128>(%29), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%2, read<i128>(%30));
// DEFAULT-NEXT:         let %31: i128 [synthetic] = update<i128, result=old, atomic=acq_rel>(deref(addr_of<ptr<i128>>(%1)), read<i128>(deref(addr_of<ptr<i128>>(%2))));
// DEFAULT-NEXT:         write<i128>(deref(addr_of<ptr<i128>>(%3)), read<i128>(%31));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i128>(read<i128>(%3), sub<i128, overflow=ub>(read<i128>(%2), widen<i128, reason=usual_arith>(const<i32>(1)))), ne<i128>(read<i128>(%1), read<i128>(%2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %32: i128 [synthetic] = read<i128>(%2);
// DEFAULT-NEXT:         let %33: i128 [synthetic] = add<i128, overflow=ub>(read<i128>(%32), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%2, read<i128>(%33));
// DEFAULT-NEXT:         let %34: i128 [synthetic] = update<i128, result=old, atomic=seq_cst>(deref(addr_of<ptr<i128>>(%1)), read<i128>(deref(addr_of<ptr<i128>>(%2))));
// DEFAULT-NEXT:         write<i128>(deref(addr_of<ptr<i128>>(%3)), read<i128>(%34));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i128>(read<i128>(%3), sub<i128, overflow=ub>(read<i128>(%2), widen<i128, reason=usual_arith>(const<i32>(1)))), ne<i128>(read<i128>(%1), read<i128>(%2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %35: i128 [synthetic] = read<i128>(%2);
// DEFAULT-NEXT:         let %36: i128 [synthetic] = add<i128, overflow=ub>(read<i128>(%35), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%2, read<i128>(%36));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
