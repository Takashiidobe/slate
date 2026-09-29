/* PR tree-optimization/81913 */

typedef __UINT8_TYPE__  u8;
typedef __UINT32_TYPE__ u32;

static u32 b(u8 d, u32 e, u32 g) {
  do {
    e += g + 1;
    d--;
  } while (d >= (u8)e);

  return e;
}

int main(void) {
  u32 x = b(1, -0x378704, ~0xba64fc);
  if (x != 0xd93190d0)
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
// DEFAULT-NEXT:     type @type[[TYPE_u8:[0-9]+]] u8 = u8;
// DEFAULT-NEXT:     type @type[[TYPE_u32:[0-9]+]] u32 = u32;
// DEFAULT-NEXT:     fn %[[VALUE_b:[0-9]+]] @b(%[[VALUE_d:[0-9]+]] d: u8, %[[VALUE_e:[0-9]+]] e: u32, %[[VALUE_g:[0-9]+]] g: u32) -> u32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         do %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_e]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE1]]), add<u32, overflow=wrap>(read<u32>(%[[VALUE_g]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_e]], read<u32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: u8 [synthetic] = read<u8>(%[[VALUE_d]]);
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE3]]))), const<i32>(1))));
// DEFAULT-NEXT:                 write<u8>(%[[VALUE_d]], read<u8>(%[[VALUE4]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ge<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_d]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(read<u32>(%[[VALUE_e]])))));
// DEFAULT-NEXT:         return read<u32>(%[[VALUE_e]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: u32 [storage=automatic] = call<u32, signature=fn(u8, u32, u32) -> u32>(%[[VALUE_b]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(1))), reinterpret<u32, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(3639044))), reinterpret<u32, reason=arg, fits=unknown>(not<i32>(const<i32>(12215548))));
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%[[VALUE_x]]), const<u32>(3643904208))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
