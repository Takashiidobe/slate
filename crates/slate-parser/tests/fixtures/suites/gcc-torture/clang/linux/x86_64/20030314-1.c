// SLATE-FILECHECK-DEFINES DEFAULT

/* PR optimization/8396 */
/* Originator: <papadopo@shfj.cea.fr> */

/* Verify that the tree inliner doesn't mess up the types
   when passing the value of read-only constant arguments.  */

static inline void bar(const short int xs, const short int xe)
{
  if (xe && (xs < xe))
    ;
}
  
void f()
{
  short int xe;

  bar(0, xe);
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
// DEFAULT-NEXT:     fn %0 @bar(%1 xs: i16 [const], %2 xe: i16 [const]) -> void [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_and<bool>(ne<i16>(read<i16>(%2), const<i16>(0)), lt<i32>(widen<i32, reason=promotion>(read<i16>(%1)), widen<i32, reason=promotion>(read<i16>(%2))))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @f() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %4 xe: i16 [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(i16, i16) -> void>(%0, truncate<i16, reason=arg, fits=always>(const<i32>(0)), read<i16>(%4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
