/* Test __atomic routines for existence and proper execution on 8 byte 
   values with each valid memory model.  */
/* { dg-do run } */
/* { dg-require-effective-target sync_long_long_runtime } */
/* { dg-options "" } */
/* { dg-options "-march=pentium" { target { { i?86-*-* x86_64-*-* } && ia32 } } } */

/* Test the execution of the __atomic_store_n builtin for a long long.  */

extern void abort(void);

long long v, count;

int
main ()
{
  v = 0;
  count = 0;

  __atomic_store_n (&v, count + 1, __ATOMIC_RELAXED);
  if (v != ++count)
    abort ();

  __atomic_store_n (&v, count + 1, __ATOMIC_RELEASE);
  if (v != ++count)
    abort ();

  __atomic_store_n (&v, count + 1, __ATOMIC_SEQ_CST);
  if (v != ++count)
    abort ();

  /* Now test the generic variant.  */
  count++;

  __atomic_store (&v, &count, __ATOMIC_RELAXED);
  if (v != count++)
    abort ();

  __atomic_store (&v, &count, __ATOMIC_RELEASE);
  if (v != count++)
    abort ();

  __atomic_store (&v, &count, __ATOMIC_SEQ_CST);
  if (v != count)
    abort ();


  return 0;
}


// SLATE-FILECHECK-FLAVOR gcc
// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:         write<i64, atomic=relaxed>(deref(addr_of<ptr<i64>>(%1)), add<i64, overflow=ub>(read<i64>(%2), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         let %4: i64 [synthetic] = read<i64>(%2);
// DEFAULT-NEXT:         let %5: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%4), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%2, read<i64>(%5));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%1), read<i64>(%5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i64, atomic=release>(deref(addr_of<ptr<i64>>(%1)), add<i64, overflow=ub>(read<i64>(%2), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         let %6: i64 [synthetic] = read<i64>(%2);
// DEFAULT-NEXT:         let %7: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%6), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%2, read<i64>(%7));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%1), read<i64>(%7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i64, atomic=seq_cst>(deref(addr_of<ptr<i64>>(%1)), add<i64, overflow=ub>(read<i64>(%2), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         let %8: i64 [synthetic] = read<i64>(%2);
// DEFAULT-NEXT:         let %9: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%8), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%2, read<i64>(%9));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%1), read<i64>(%9))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %10: i64 [synthetic] = read<i64>(%2);
// DEFAULT-NEXT:         let %11: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%10), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%2, read<i64>(%11));
// DEFAULT-NEXT:         write<i64, atomic=relaxed>(deref(addr_of<ptr<i64>>(%1)), read<i64>(deref(addr_of<ptr<i64>>(%2))));
// DEFAULT-NEXT:         let %12: i64 [synthetic] = read<i64>(%2);
// DEFAULT-NEXT:         let %13: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%12), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%2, read<i64>(%13));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%1), read<i64>(%12))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i64, atomic=release>(deref(addr_of<ptr<i64>>(%1)), read<i64>(deref(addr_of<ptr<i64>>(%2))));
// DEFAULT-NEXT:         let %14: i64 [synthetic] = read<i64>(%2);
// DEFAULT-NEXT:         let %15: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%14), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%2, read<i64>(%15));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%1), read<i64>(%14))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i64, atomic=seq_cst>(deref(addr_of<ptr<i64>>(%1)), read<i64>(deref(addr_of<ptr<i64>>(%2))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%1), read<i64>(%2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
