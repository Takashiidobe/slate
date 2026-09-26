/* Test __atomic routines for existence and proper execution on 16 byte 
   values with each valid memory model.  */
/* { dg-do run } */
/* { dg-require-effective-target sync_int_128_runtime } */
/* { dg-options "-mcx16" { target { i?86-*-* x86_64-*-* } } } */
/* { dg-options "-mlsx -mscq" { target { loongarch64-*-* } } } */

/* Test the execution of __atomic_compare_exchange_n builtin for an int_128.  */

extern void abort(void);

__int128_t v        = 0;
__int128_t expected = 0;
__int128_t max      = ~0;
__int128_t desired  = ~0;
__int128_t zero     = 0;

#define STRONG 0
#define WEAK   1

int main() {

  if (!__atomic_compare_exchange_n(&v, &expected, max, STRONG, __ATOMIC_RELAXED,
                                   __ATOMIC_RELAXED))
    abort();
  if (expected != 0)
    abort();

  if (__atomic_compare_exchange_n(&v, &expected, 0, STRONG, __ATOMIC_ACQUIRE,
                                  __ATOMIC_RELAXED))
    abort();
  if (expected != max)
    abort();

  if (!__atomic_compare_exchange_n(&v, &expected, 0, STRONG, __ATOMIC_RELEASE,
                                   __ATOMIC_ACQUIRE))
    abort();
  if (expected != max)
    abort();
  if (v != 0)
    abort();

  if (__atomic_compare_exchange_n(&v, &expected, desired, WEAK,
                                  __ATOMIC_ACQ_REL, __ATOMIC_ACQUIRE))
    abort();
  if (expected != 0)
    abort();

  if (!__atomic_compare_exchange_n(&v, &expected, desired, STRONG,
                                   __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST))
    abort();
  if (expected != 0)
    abort();
  if (v != max)
    abort();

  /* Now test the generic version.  */

  v = 0;

  if (!__atomic_compare_exchange(&v, &expected, &max, STRONG, __ATOMIC_RELAXED,
                                 __ATOMIC_RELAXED))
    abort();
  if (expected != 0)
    abort();

  if (__atomic_compare_exchange(&v, &expected, &zero, STRONG, __ATOMIC_ACQUIRE,
                                __ATOMIC_RELAXED))
    abort();
  if (expected != max)
    abort();

  if (!__atomic_compare_exchange(&v, &expected, &zero, STRONG, __ATOMIC_RELEASE,
                                 __ATOMIC_ACQUIRE))
    abort();
  if (expected != max)
    abort();
  if (v != 0)
    abort();

  if (__atomic_compare_exchange(&v, &expected, &desired, WEAK, __ATOMIC_ACQ_REL,
                                __ATOMIC_ACQUIRE))
    abort();
  if (expected != 0)
    abort();

  if (!__atomic_compare_exchange(&v, &expected, &desired, STRONG,
                                 __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST))
    abort();
  if (expected != 0)
    abort();
  if (v != max)
    abort();

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
// DEFAULT-NEXT:     global %1 v: i128 [storage=static] = widen<i128, reason=assign>(const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %2 expected: i128 [storage=static] = widen<i128, reason=assign>(const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %3 max: i128 [storage=static] = widen<i128, reason=assign>(not<i32>(const<i32>(0))) [linkage=external];
// DEFAULT-NEXT:     global %4 desired: i128 [storage=static] = widen<i128, reason=assign>(not<i32>(const<i32>(0))) [linkage=external];
// DEFAULT-NEXT:     global %5 zero: i128 [storage=static] = widen<i128, reason=assign>(const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %7: bool [synthetic] = compare_exchange<i128, form=write_back, weak=false, success=relaxed, failure=relaxed>(deref(addr_of<ptr<i128>>(%1)), addr_of<ptr<i128>>(%2), read<i128>(%3));
// DEFAULT-NEXT:         if not<bool>(read<bool>(%7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%2), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %8: bool [synthetic] = compare_exchange<i128, form=write_back, weak=false, success=acquire, failure=relaxed>(deref(addr_of<ptr<i128>>(%1)), addr_of<ptr<i128>>(%2), widen<i128, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%8)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%2), read<i128>(%3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %9: bool [synthetic] = compare_exchange<i128, form=write_back, weak=false, success=release, failure=acquire>(deref(addr_of<ptr<i128>>(%1)), addr_of<ptr<i128>>(%2), widen<i128, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         if not<bool>(read<bool>(%9))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%2), read<i128>(%3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%1), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %10: bool [synthetic] = compare_exchange<i128, form=write_back, weak=true, success=acq_rel, failure=acquire>(deref(addr_of<ptr<i128>>(%1)), addr_of<ptr<i128>>(%2), read<i128>(%4));
// DEFAULT-NEXT:         if read<bool>(%10)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%2), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %11: bool [synthetic] = compare_exchange<i128, form=write_back, weak=false, success=seq_cst, failure=seq_cst>(deref(addr_of<ptr<i128>>(%1)), addr_of<ptr<i128>>(%2), read<i128>(%4));
// DEFAULT-NEXT:         if not<bool>(read<bool>(%11))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%2), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%1), read<i128>(%3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<i128>(%1, widen<i128, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %12: bool [synthetic] = compare_exchange<i128, form=write_back, weak=false, success=relaxed, failure=relaxed>(deref(addr_of<ptr<i128>>(%1)), addr_of<ptr<i128>>(%2), read<i128>(deref(addr_of<ptr<i128>>(%3))));
// DEFAULT-NEXT:         if not<bool>(read<bool>(%12))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%2), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %13: bool [synthetic] = compare_exchange<i128, form=write_back, weak=false, success=acquire, failure=relaxed>(deref(addr_of<ptr<i128>>(%1)), addr_of<ptr<i128>>(%2), read<i128>(deref(addr_of<ptr<i128>>(%5))));
// DEFAULT-NEXT:         if read<bool>(%13)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%2), read<i128>(%3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %14: bool [synthetic] = compare_exchange<i128, form=write_back, weak=false, success=release, failure=acquire>(deref(addr_of<ptr<i128>>(%1)), addr_of<ptr<i128>>(%2), read<i128>(deref(addr_of<ptr<i128>>(%5))));
// DEFAULT-NEXT:         if not<bool>(read<bool>(%14))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%2), read<i128>(%3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%1), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %15: bool [synthetic] = compare_exchange<i128, form=write_back, weak=true, success=acq_rel, failure=acquire>(deref(addr_of<ptr<i128>>(%1)), addr_of<ptr<i128>>(%2), read<i128>(deref(addr_of<ptr<i128>>(%4))));
// DEFAULT-NEXT:         if read<bool>(%15)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%2), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %16: bool [synthetic] = compare_exchange<i128, form=write_back, weak=false, success=seq_cst, failure=seq_cst>(deref(addr_of<ptr<i128>>(%1)), addr_of<ptr<i128>>(%2), read<i128>(deref(addr_of<ptr<i128>>(%4))));
// DEFAULT-NEXT:         if not<bool>(read<bool>(%16))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%2), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%1), read<i128>(%3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
