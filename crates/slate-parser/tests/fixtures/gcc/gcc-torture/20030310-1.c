// SLATE-FILECHECK-DEFINES DEFAULT

static inline void
foo (char accept)
{
  char s;
  while (s == accept) ;
}

static void
bar (void)
{
  char ch;
  foo (ch);
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
// DEFAULT-NEXT:     fn %0 @foo(%1 accept: i8) -> void [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %2 s: i8 [storage=automatic];
// DEFAULT-NEXT:         while %5 eq<i32>(widen<i32, reason=promotion>(read<i8>(%2)), widen<i32, reason=promotion>(read<i8>(%1)))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @bar() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %4 ch: i8 [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(i8) -> void>(%0, read<i8>(%4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
