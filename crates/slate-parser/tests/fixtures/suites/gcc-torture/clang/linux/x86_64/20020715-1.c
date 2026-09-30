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
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE0:[0-9]+]] <unnamed>: i8) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_g:[0-9]+]] @g() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_scale:[0-9]+]] @scale() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_width:[0-9]+]] width: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_bytes:[0-9]+]] bytes: i8 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_src:[0-9]+]] src: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_width]]), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i8>(%[[VALUE_bytes]], read<i8>(deref(read<ptr<i8>>(%[[VALUE_src]]))));
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_g]]);
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_width]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%[[VALUE1]]), widen<i32, reason=promotion>(read<i8>(%[[VALUE_bytes]])));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_width]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<void, signature=fn(i8) -> void>(%[[VALUE_f]], read<i8>(%[[VALUE_bytes]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
