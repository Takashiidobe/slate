/* Test __atomic routines for existence and execution.  */
/* { dg-do run } */
/* { dg-require-effective-target sync_char_short } */

/* Test that __atomic_test_and_set and __atomic_clear builtins execute.  */

extern void   abort(void);
unsigned char a;

int main() {
  int b;

  __atomic_clear(&a, __ATOMIC_RELAXED);
  if (a != 0)
    abort();

  b = __atomic_test_and_set(&a, __ATOMIC_SEQ_CST);
  if (a != __GCC_ATOMIC_TEST_AND_SET_TRUEVAL || b != 0)
    abort();

  b = __atomic_test_and_set(&a, __ATOMIC_ACQ_REL);
  if (a != __GCC_ATOMIC_TEST_AND_SET_TRUEVAL || b != 1)
    abort();

  __atomic_clear(&a, __ATOMIC_SEQ_CST);
  if (a != 0)
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
// DEFAULT-NEXT:     global %1 a: u8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %3 b: i32 [storage=automatic];
// DEFAULT-NEXT:         write<u8, atomic=relaxed>(deref(addr_of<ptr<u8>>(%1)), const<u8>(0));
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%1))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %4: u8 [synthetic] = update<u8, result=old, atomic=seq_cst>(deref(addr_of<ptr<u8>>(%1)), const<u8>(1));
// DEFAULT-NEXT:         write<i32>(%3, from_bool<i32, reason=assign>(ne<u8>(read<u8>(%4), const<u8>(0))));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%1))), const<i32>(1)), ne<i32>(read<i32>(%3), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %5: u8 [synthetic] = update<u8, result=old, atomic=acq_rel>(deref(addr_of<ptr<u8>>(%1)), const<u8>(1));
// DEFAULT-NEXT:         write<i32>(%3, from_bool<i32, reason=assign>(ne<u8>(read<u8>(%5), const<u8>(0))));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%1))), const<i32>(1)), ne<i32>(read<i32>(%3), const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u8, atomic=seq_cst>(deref(addr_of<ptr<u8>>(%1)), const<u8>(0));
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%1))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
