/* Verify that attribute returns_nonnull on global and local function
   declarations is merged.
   { dg-do compile }
   { dg-options "-Wall -fdump-tree-optimized -fdelete-null-pointer-checks" }
   { dg-skip-if "" keeps_null_pointer_checks } */

void foo (void);


void frnn_local_local (void)
{
  __attribute__ ((returns_nonnull)) void* frnn1 (void);

  if (!frnn1 ())
    foo ();
}

void gnr_local_local (void)
{
  void* frnn1 (void);

  if (!frnn1 ())
    foo ();
}

void frnn_local_global (void)
{
  __attribute__ ((returns_nonnull)) void* frnn2 (void);

  if (!frnn2 ())
    foo ();
}

void* frnn2 (void);

void gnr_local_global (void)
{
  if (!frnn2 ())
    foo ();
}

__attribute__ ((returns_nonnull)) void* frnn3 (void);

void frnn_global_local (void)
{
  if (!frnn3 ())
    foo ();
}

void gnr_global_local (void)
{
  void* frnn3 (void);

  if (!frnn3 ())
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
// DEFAULT-NEXT:     fn %[[VALUE_frnn1:[0-9]+]] @frnn1() -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_frnn_local_local:[0-9]+]] @frnn_local_local() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if not<bool>(ne<ptr<void>>(call<ptr<void>, signature=fn() -> ptr<void>>(%[[VALUE_frnn1]]), null<ptr<void>>))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_foo]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gnr_local_local:[0-9]+]] @gnr_local_local() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if not<bool>(ne<ptr<void>>(call<ptr<void>, signature=fn() -> ptr<void>>(%[[VALUE_frnn1]]), null<ptr<void>>))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_foo]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_frnn2:[0-9]+]] @frnn2() -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_frnn_local_global:[0-9]+]] @frnn_local_global() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if not<bool>(ne<ptr<void>>(call<ptr<void>, signature=fn() -> ptr<void>>(%[[VALUE_frnn2]]), null<ptr<void>>))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_foo]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gnr_local_global:[0-9]+]] @gnr_local_global() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if not<bool>(ne<ptr<void>>(call<ptr<void>, signature=fn() -> ptr<void>>(%[[VALUE_frnn2]]), null<ptr<void>>))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_foo]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_frnn3:[0-9]+]] @frnn3() -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_frnn_global_local:[0-9]+]] @frnn_global_local() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if not<bool>(ne<ptr<void>>(call<ptr<void>, signature=fn() -> ptr<void>>(%[[VALUE_frnn3]]), null<ptr<void>>))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_foo]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gnr_global_local:[0-9]+]] @gnr_global_local() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if not<bool>(ne<ptr<void>>(call<ptr<void>, signature=fn() -> ptr<void>>(%[[VALUE_frnn3]]), null<ptr<void>>))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_foo]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
