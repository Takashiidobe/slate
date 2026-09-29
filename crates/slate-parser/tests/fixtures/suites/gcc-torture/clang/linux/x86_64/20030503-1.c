// SLATE-FILECHECK-DEFINES DEFAULT

void bar (void);

void foo ()
{
  if (1)
    goto foo;
  else
    for (;;)
      {
      foo:
	bar ();
	return;
      }
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
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(const<i32>(1), const<i32>(0))
// DEFAULT-NEXT:             goto %[[VALUE_foo_2:[0-9]+]];
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:                 init:
// DEFAULT-NEXT:                 condition: omitted
// DEFAULT-NEXT:                 increment: omitted
// DEFAULT-NEXT:                 body:
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         label %[[VALUE_foo_2]] foo:
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_bar]]);
// DEFAULT-NEXT:                         return;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
