// SLATE-FILECHECK-DEFINES DEFAULT

/* This testcase ICEd on IA-32 at -O2, because loop was calling convert_modes
   between a MODE_FLOAT and MODE_INT class modes.  */

typedef union
{
  double d;
  long long ll;
} A;

void
foo (A x, A **y, A z)
{
  for (; *y; y++)
    if (x.ll == 262 && (*y)->d == z.d)
      break;
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
// DEFAULT-NEXT:         field0 d: f64;
// DEFAULT-NEXT:         field1 ll: i64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type1 A = @type0;
// DEFAULT-NEXT:     fn %2 @foo(%3 x: @type0, %4 y: ptr<ptr<@type0>>, %5 z: @type0) -> void [linkage=external] [abi=sysv64(coerce<i64>, scalar, coerce<i64>) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         for %6
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ne<ptr<@type0>>(read<ptr<@type0>>(deref(read<ptr<ptr<@type0>>>(%4))), null<ptr<@type0>>)
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %7: ptr<ptr<@type0>> [synthetic] = read<ptr<ptr<@type0>>>(%4);
// DEFAULT-NEXT:                 let %8: ptr<ptr<@type0>> [synthetic] = ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(read<ptr<ptr<@type0>>>(%7), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<ptr<@type0>>>(%4, read<ptr<ptr<@type0>>>(%8));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i64>(read<i64>(field1(%3)), widen<i64, reason=usual_arith>(const<i32>(262))), eq<f64, exceptions=observable>(read<f64>(field0(deref(read<ptr<@type0>>(deref(read<ptr<ptr<@type0>>>(%4)))))), read<f64>(field0(%5))))
// DEFAULT-NEXT:                     break %6;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
