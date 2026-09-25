// SLATE-FILECHECK-DEFINES DEFAULT

float d;
int e, f;

void foo (void)
{
  struct { float u, v; } a = {0.0, 0.0};
  float b;
  int c;

  c = e;
  if (c == 0)
    c = f;
  b = d;
  if (a.v < b)
    a.v = b;
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
// DEFAULT-NEXT:     type @type0 = struct {
// DEFAULT-NEXT:         field0 u: f32;
// DEFAULT-NEXT:         field1 v: f32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     global %0 d: f32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 e: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 f: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %3 @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %5 a: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(0.0)), field1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(0.0)));
// DEFAULT-NEXT:         let %6 b: f32 [storage=automatic];
// DEFAULT-NEXT:         let %7 c: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%7, read<i32>(%1));
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%7), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(%7, read<i32>(%2));
// DEFAULT-NEXT:         write<f32>(%6, read<f32>(%0));
// DEFAULT-NEXT:         if lt<f32, exceptions=ignore>(read<f32>(field1(%5)), read<f32>(%6))
// DEFAULT-NEXT:             write<f32>(field1(%5), read<f32>(%6));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
