/* this is another case where phiopt
   would create -signed1bit which is undefined. */
struct {
  int a : 1;
} b;
int *c = (int *)&b, d;
int  main() {
  d = c && (b.a = (d < 0) ^ 3);
  if (d != 1)
    __builtin_abort();
  return 0;
}


// SLATE-FILECHECK-DEFINES DEFAULT

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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 a: i32 : 1;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 1)], field_units=[Some(0)]];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: @type[[TYPE0]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: ptr<i32> [storage=static] = pointer_cast<ptr<i32>, reason=explicit>(addr_of<ptr<@type[[TYPE0]]>>(%[[VALUE_b]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<ptr<i32>>(read<ptr<i32>>(%[[VALUE_c]]), null<ptr<i32>>)
// DEFAULT-NEXT:             write<i32>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%[[VALUE_b]]), xor<i32>(from_bool<i32, reason=promotion>(lt<i32>(read<i32>(%[[VALUE_d]]), const<i32>(0))), const<i32>(3)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], ne<i32>(xor<i32>(from_bool<i32, reason=promotion>(lt<i32>(read<i32>(%[[VALUE_d]]), const<i32>(0))), const<i32>(3)), const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], const<bool>(false));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_d]], from_bool<i32, reason=assign>(read<bool>(%[[VALUE0]])));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_d]]), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
