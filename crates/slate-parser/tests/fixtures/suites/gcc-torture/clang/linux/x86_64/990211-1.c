/* Copyright (C) 1999 Free Software Foundation, Inc.
  Contributed by Nathan Sidwell 20 Jan 1999 <nathan@acm.org> */

/* check range combining boolean operations work */

extern void abort();

#define N 77

void func(int i) {
  /* fold-const does some clever things with range tests. Make sure
     we get (some of) them right */

  /* these must fail, regardless of the value of i */
  if ((i < 0) && (i >= 0))
    abort();
  if ((i > 0) && (i <= 0))
    abort();
  if ((i >= 0) && (i < 0))
    abort();
  if ((i <= 0) && (i > 0))
    abort();

  if ((i < N) && (i >= N))
    abort();
  if ((i > N) && (i <= N))
    abort();
  if ((i >= N) && (i < N))
    abort();
  if ((i <= N) && (i > N))
    abort();

  /* these must pass, regardless of the value of i */
  if (!((i < 0) || (i >= 0)))
    abort();
  if (!((i > 0) || (i <= 0)))
    abort();
  if (!((i >= 0) || (i < 0)))
    abort();
  if (!((i <= 0) || (i > 0)))
    abort();

  if (!((i < N) || (i >= N)))
    abort();
  if (!((i > N) || (i <= N)))
    abort();
  if (!((i >= N) || (i < N)))
    abort();
  if (!((i <= N) || (i > N)))
    abort();

  return;
}

int main() {
  func(0);
  func(1);
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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_func:[0-9]+]] @func(%[[VALUE_i:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_and<bool>(lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0)), ge<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_and<bool>(gt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0)), le<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_and<bool>(ge<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0)), lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0)), gt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_and<bool>(lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(77)), ge<i32>(read<i32>(%[[VALUE_i]]), const<i32>(77)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_and<bool>(gt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(77)), le<i32>(read<i32>(%[[VALUE_i]]), const<i32>(77)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_and<bool>(ge<i32>(read<i32>(%[[VALUE_i]]), const<i32>(77)), lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(77)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(read<i32>(%[[VALUE_i]]), const<i32>(77)), gt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(77)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(logical_or<bool>(lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0)), ge<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(logical_or<bool>(gt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0)), le<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(logical_or<bool>(ge<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0)), lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(logical_or<bool>(le<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0)), gt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(logical_or<bool>(lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(77)), ge<i32>(read<i32>(%[[VALUE_i]]), const<i32>(77))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(logical_or<bool>(gt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(77)), le<i32>(read<i32>(%[[VALUE_i]]), const<i32>(77))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(logical_or<bool>(ge<i32>(read<i32>(%[[VALUE_i]]), const<i32>(77)), lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(77))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(logical_or<bool>(le<i32>(read<i32>(%[[VALUE_i]]), const<i32>(77)), gt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(77))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_func]], const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_func]], const<i32>(1));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
