/* Test __atomic routines for existence and execution with each valid 
   memory model.  */
/* { dg-do run } */
/* { dg-require-effective-target sync_char_short } */


/* Test that __atomic_{thread,signal}_fence builtins execute.  */

int
main ()
{
  __atomic_thread_fence (__ATOMIC_RELAXED);
  __atomic_thread_fence (__ATOMIC_CONSUME);
  __atomic_thread_fence (__ATOMIC_ACQUIRE);
  __atomic_thread_fence (__ATOMIC_RELEASE);
  __atomic_thread_fence (__ATOMIC_ACQ_REL);
  __atomic_thread_fence (__ATOMIC_SEQ_CST);

  __atomic_signal_fence (__ATOMIC_RELAXED);
  __atomic_signal_fence (__ATOMIC_CONSUME);
  __atomic_signal_fence (__ATOMIC_ACQUIRE);
  __atomic_signal_fence (__ATOMIC_RELEASE);
  __atomic_signal_fence (__ATOMIC_ACQ_REL);
  __atomic_signal_fence (__ATOMIC_SEQ_CST);

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
// DEFAULT-NEXT:     fn %0 @main(unprototyped) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         fence<scope=thread, order=relaxed>;
// DEFAULT-NEXT:         fence<scope=thread, order=consume>;
// DEFAULT-NEXT:         fence<scope=thread, order=acquire>;
// DEFAULT-NEXT:         fence<scope=thread, order=release>;
// DEFAULT-NEXT:         fence<scope=thread, order=acq_rel>;
// DEFAULT-NEXT:         fence<scope=thread, order=seq_cst>;
// DEFAULT-NEXT:         fence<scope=signal, order=relaxed>;
// DEFAULT-NEXT:         fence<scope=signal, order=consume>;
// DEFAULT-NEXT:         fence<scope=signal, order=acquire>;
// DEFAULT-NEXT:         fence<scope=signal, order=release>;
// DEFAULT-NEXT:         fence<scope=signal, order=acq_rel>;
// DEFAULT-NEXT:         fence<scope=signal, order=seq_cst>;
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
