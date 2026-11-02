// SLATE-FILECHECK-DEFINES DEFAULT

/* Test whether tree inlining works with prototyped nested functions.  */

extern void foo (char *x);
void bar (void);
void bar (void)
{
  auto void baz (void);
  void baz (void)
    {
      char tmp[2];
      foo (tmp);
    }
  baz ();
}

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
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: ptr<i8>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         nested_fn %[[VALUE_baz:[0-9]+]] @baz() -> void [captures=] {
// DEFAULT-NEXT:             let %[[VALUE_tmp:[0-9]+]] tmp: array<i8, 2> [storage=automatic];
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>) -> void>(%[[VALUE_foo]], array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_tmp]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_baz]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
