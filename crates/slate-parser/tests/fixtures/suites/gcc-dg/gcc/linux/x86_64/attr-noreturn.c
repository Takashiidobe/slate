/* Verify that attribute noreturn on global and local function declarations
   is merged.
   { dg-do compile }
   { dg-options "-Wall -fdump-tree-optimized" } */

void foo (void);

int fnr_local_local (void)
{
  __attribute__ ((noreturn)) void fnr1 (void);

  fnr1 ();

  foo ();
}

int gnr_local_local (void)
{
  void fnr1 (void);

  fnr1 ();

  foo ();
}


int fnr_local_global (void)
{
  __attribute__ ((noreturn)) void fnr2 (void);

  fnr2 ();

  foo ();
}

void fnr2 (void);

int gnr_local_global (void)
{
  fnr2 ();

  foo ();
}


__attribute__ ((noreturn)) void fnr3 (void);

int fnr_global_local (void)
{
  fnr3 ();

  foo ();
}

int gnr_global_local (void)
{
  void fnr3 (void);

  fnr3 ();

  foo ();
}

/* { dg-final { scan-tree-dump-not "foo" "optimized" } } */

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
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fnr1:[0-9]+]] @fnr1() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_fnr_local_local:[0-9]+]] @fnr_local_local() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_fnr1]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_foo]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gnr_local_local:[0-9]+]] @gnr_local_local() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_fnr1]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_foo]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fnr2:[0-9]+]] @fnr2() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_fnr_local_global:[0-9]+]] @fnr_local_global() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_fnr2]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_foo]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gnr_local_global:[0-9]+]] @gnr_local_global() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_fnr2]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_foo]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fnr3:[0-9]+]] @fnr3() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_fnr_global_local:[0-9]+]] @fnr_global_local() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_fnr3]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_foo]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gnr_global_local:[0-9]+]] @gnr_global_local() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_fnr3]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_foo]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
