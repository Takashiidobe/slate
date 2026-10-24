/* { dg-do compile } */
/* { dg-options "-O3 -fcompare-debug -fpermissive -w" } */

extern void rt_mutex_owner (void);
extern void rt_mutex_deadlock_account_lock (int);
extern void signal_pending (void);
__typeof__ (int *) a;
int b;

int
try_to_take_rt_mutex (int p1) {
  rt_mutex_owner ();
  if (b)
    return 0;
  rt_mutex_deadlock_account_lock (p1);
  return 1;
}

void
__rt_mutex_slowlock (int p1) {
  int c;
  for (;;) {
    c = ({
      asm ("" : "=r"(a));
      a;
    });
    if (try_to_take_rt_mutex (c))
      break;
    if (__builtin_expect (p1 == 0, 0))
      signal_pending ();
  }
}

// SLATE-FILECHECK-ARGS -fpermissive
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_rt_mutex_owner:[0-9]+]] @rt_mutex_owner() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_rt_mutex_deadlock_account_lock:[0-9]+]] @rt_mutex_deadlock_account_lock(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_signal_pending:[0-9]+]] @signal_pending() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_try_to_take_rt_mutex:[0-9]+]] @try_to_take_rt_mutex(%[[VALUE_p1:[0-9]+]] p1: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_rt_mutex_owner]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_b]]), const<i32>(0))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_rt_mutex_deadlock_account_lock]], read<i32>(%[[VALUE_p1]]));
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_expect:[0-9]+]] @__builtin_expect(%[[VALUE1:[0-9]+]] <unnamed>: i64, %[[VALUE2:[0-9]+]] <unnamed>: i64) -> i64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___rt_mutex_slowlock:[0-9]+]] @__rt_mutex_slowlock(%[[VALUE_p1_2:[0-9]+]] p1: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: omitted
// DEFAULT-NEXT:             increment: omitted
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE4:[0-9]+]]: ptr<i32> [synthetic];
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         asm "" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:                             lateout 0 "r" [reg] width 64 place<ptr<i32>>(%[[VALUE_a]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         write<ptr<i32>>(%[[VALUE4]], read<ptr<i32>>(%[[VALUE_a]]));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_c]], ptr_to_int<i32, reason=assign>(read<ptr<i32>>(%[[VALUE4]])));
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_try_to_take_rt_mutex]], read<i32>(%[[VALUE_c]])), const<i32>(0))
// DEFAULT-NEXT:                         break %[[VALUE3]];
// DEFAULT-NEXT:                     if ne<i64>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE___builtin_expect]], from_bool<i64, reason=arg>(eq<i32>(read<i32>(%[[VALUE_p1_2]]), const<i32>(0))), widen<i64, reason=arg>(const<i32>(0))), const<i64>(0))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_signal_pending]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
