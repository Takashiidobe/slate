/* Test __atomic routines for existence and proper execution on 8 byte 
   values with each valid memory model.  */
/* { dg-do run } */
/* { dg-require-effective-target sync_long_long_runtime } */
/* { dg-options "" } */
/* { dg-options "-march=pentium" { target { { i?86-*-* x86_64-*-* } && ia32 } } } */

extern void abort(void);

long long v, count;

int main() {
  v     = 0;
  count = 0;

  if (__atomic_load_n(&v, __ATOMIC_RELAXED) != count++)
    abort();
  else
    v++;

  if (__atomic_load_n(&v, __ATOMIC_ACQUIRE) != count++)
    abort();
  else
    v++;

  if (__atomic_load_n(&v, __ATOMIC_CONSUME) != count++)
    abort();
  else
    v++;

  if (__atomic_load_n(&v, __ATOMIC_SEQ_CST) != count++)
    abort();
  else
    v++;

  /* Now test the generic variants.  */

  __atomic_load(&v, &count, __ATOMIC_RELAXED);
  if (count != v)
    abort();
  else
    v++;

  __atomic_load(&v, &count, __ATOMIC_ACQUIRE);
  if (count != v)
    abort();
  else
    v++;

  __atomic_load(&v, &count, __ATOMIC_CONSUME);
  if (count != v)
    abort();
  else
    v++;

  __atomic_load(&v, &count, __ATOMIC_SEQ_CST);
  if (count != v)
    abort();
  else
    v++;

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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i64>(%1, widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         write<i64>(%2, widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %4: i64 [synthetic] = read<i64>(%2);
// DEFAULT-NEXT:         let %5: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%4), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%2, read<i64>(%5));
// DEFAULT-NEXT:         if ne<i64>(read<i64, atomic=relaxed>(deref(addr_of<ptr<i64>>(%1))), read<i64>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %6: i64 [synthetic] = read<i64>(%1);
// DEFAULT-NEXT:             let %7: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%6), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:             write<i64>(%1, read<i64>(%7));
// DEFAULT-NEXT:         let %8: i64 [synthetic] = read<i64>(%2);
// DEFAULT-NEXT:         let %9: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%8), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%2, read<i64>(%9));
// DEFAULT-NEXT:         if ne<i64>(read<i64, atomic=acquire>(deref(addr_of<ptr<i64>>(%1))), read<i64>(%8))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %10: i64 [synthetic] = read<i64>(%1);
// DEFAULT-NEXT:             let %11: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%10), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:             write<i64>(%1, read<i64>(%11));
// DEFAULT-NEXT:         let %12: i64 [synthetic] = read<i64>(%2);
// DEFAULT-NEXT:         let %13: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%12), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%2, read<i64>(%13));
// DEFAULT-NEXT:         if ne<i64>(read<i64, atomic=consume>(deref(addr_of<ptr<i64>>(%1))), read<i64>(%12))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %14: i64 [synthetic] = read<i64>(%1);
// DEFAULT-NEXT:             let %15: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%14), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:             write<i64>(%1, read<i64>(%15));
// DEFAULT-NEXT:         let %16: i64 [synthetic] = read<i64>(%2);
// DEFAULT-NEXT:         let %17: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%16), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%2, read<i64>(%17));
// DEFAULT-NEXT:         if ne<i64>(read<i64, atomic=seq_cst>(deref(addr_of<ptr<i64>>(%1))), read<i64>(%16))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %18: i64 [synthetic] = read<i64>(%1);
// DEFAULT-NEXT:             let %19: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%18), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:             write<i64>(%1, read<i64>(%19));
// DEFAULT-NEXT:         write<i64>(deref(addr_of<ptr<i64>>(%2)), read<i64, atomic=relaxed>(deref(addr_of<ptr<i64>>(%1))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%2), read<i64>(%1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %20: i64 [synthetic] = read<i64>(%1);
// DEFAULT-NEXT:             let %21: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%20), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:             write<i64>(%1, read<i64>(%21));
// DEFAULT-NEXT:         write<i64>(deref(addr_of<ptr<i64>>(%2)), read<i64, atomic=acquire>(deref(addr_of<ptr<i64>>(%1))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%2), read<i64>(%1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %22: i64 [synthetic] = read<i64>(%1);
// DEFAULT-NEXT:             let %23: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%22), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:             write<i64>(%1, read<i64>(%23));
// DEFAULT-NEXT:         write<i64>(deref(addr_of<ptr<i64>>(%2)), read<i64, atomic=consume>(deref(addr_of<ptr<i64>>(%1))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%2), read<i64>(%1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %24: i64 [synthetic] = read<i64>(%1);
// DEFAULT-NEXT:             let %25: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%24), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:             write<i64>(%1, read<i64>(%25));
// DEFAULT-NEXT:         write<i64>(deref(addr_of<ptr<i64>>(%2)), read<i64, atomic=seq_cst>(deref(addr_of<ptr<i64>>(%1))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%2), read<i64>(%1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %26: i64 [synthetic] = read<i64>(%1);
// DEFAULT-NEXT:             let %27: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%26), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:             write<i64>(%1, read<i64>(%27));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
