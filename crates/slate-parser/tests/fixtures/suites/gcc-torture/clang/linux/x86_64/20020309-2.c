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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 d: f64;
// DEFAULT-NEXT:         field1 ll: i64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_A:[0-9]+]] A = @type[[TYPE0]];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: @type[[TYPE0]], %[[VALUE_y:[0-9]+]] y: ptr<ptr<@type[[TYPE0]]>>, %[[VALUE_z:[0-9]+]] z: @type[[TYPE0]]) -> void [linkage=external] [abi=sysv64(native_c, scalar, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ne<ptr<@type[[TYPE0]]>>(read<ptr<@type[[TYPE0]]>>(deref(read<ptr<ptr<@type[[TYPE0]]>>>(%[[VALUE_y]]))), null<ptr<@type[[TYPE0]]>>)
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: ptr<ptr<@type[[TYPE0]]>> [synthetic] = read<ptr<ptr<@type[[TYPE0]]>>>(%[[VALUE_y]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: ptr<ptr<@type[[TYPE0]]>> [synthetic] = ptr_offset<ptr<ptr<@type[[TYPE0]]>>, subtract=false, element=ptr<@type[[TYPE0]]>, overflow=ub>(read<ptr<ptr<@type[[TYPE0]]>>>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<ptr<@type[[TYPE0]]>>>(%[[VALUE_y]], read<ptr<ptr<@type[[TYPE0]]>>>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i64>(read<i64>(field1(%[[VALUE_x]])), widen<i64, reason=usual_arith>(const<i32>(262))), eq<f64, exceptions=ignore>(read<f64>(field0(deref(read<ptr<@type[[TYPE0]]>>(deref(read<ptr<ptr<@type[[TYPE0]]>>>(%[[VALUE_y]])))))), read<f64>(field0(%[[VALUE_z]]))))
// DEFAULT-NEXT:                     break %[[VALUE0]];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
