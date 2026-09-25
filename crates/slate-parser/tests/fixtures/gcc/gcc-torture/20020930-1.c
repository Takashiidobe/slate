// SLATE-FILECHECK-DEFINES DEFAULT

/* PR c/8002 */

float expm1f(float x) {
     union {
         float value;
         unsigned word;
     } sf_u;
     sf_u.word = (unsigned) x * 2;
     return x + sf_u.value;
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
// DEFAULT-NEXT:     type @type0 = union {
// DEFAULT-NEXT:         field0 value: f32;
// DEFAULT-NEXT:         field1 word: u32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     fn %0 @expm1f(%1 x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 sf_u: @type0 [storage=automatic];
// DEFAULT-NEXT:         write<u32>(field1(%3), mul<u32, overflow=wrap>(float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f32>(%1)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:         return add<f32, rounding=nearest_even, exceptions=ignore>(read<f32>(%1), read<f32>(field0(%3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
