// SLATE-FILECHECK-DEFINES DEFAULT

/* PR optimization/7153 */
/* Verify that GCC doesn't promote a register when its
   lifetime is not limited to one basic block. */

void f(char);
void g(void);

void scale(void)
{
  int width;
  char bytes;
  char *src;

  if (width)
  {
    bytes = *src;
    g();
    width *= bytes;
  }

  f(bytes);
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
// DEFAULT-NEXT:     fn %0 @f(%6 <unnamed>: i8) -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @g() -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @scale() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %3 width: i32 [storage=automatic];
// DEFAULT-NEXT:         let %4 bytes: i8 [storage=automatic];
// DEFAULT-NEXT:         let %5 src: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%3), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i8>(%4, read<i8>(deref(read<ptr<i8>>(%5))));
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 let %7: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                 let %8: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%7), widen<i32, reason=promotion>(read<i8>(%4)));
// DEFAULT-NEXT:                 write<i32>(%3, read<i32>(%8));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<void, signature=fn(i8) -> void>(%0, read<i8>(%4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
