typedef unsigned int u32;

static const u32 deadfish = 0xdeadf155;

static const u32 cfb_tab8_be[] = {
    0x00000000, 0x000000ff, 0x0000ff00, 0x0000ffff, 0x00ff0000, 0x00ff00ff,
    0x00ffff00, 0x00ffffff, 0xff000000, 0xff0000ff, 0xff00ff00, 0xff00ffff,
    0xffff0000, 0xffff00ff, 0xffffff00, 0xffffffff};

static const u32 cfb_tab8_le[] = {
    0x00000000, 0xff000000, 0x00ff0000, 0xffff0000, 0x0000ff00, 0xff00ff00,
    0x00ffff00, 0xffffff00, 0x000000ff, 0xff0000ff, 0x00ff00ff, 0xffff00ff,
    0x0000ffff, 0xff00ffff, 0x00ffffff, 0xffffffff};

static const u32 cfb_tab16_be[] = {0x00000000, 0x0000ffff, 0xffff0000,
                                   0xffffffff};

static const u32 cfb_tab16_le[] = {0x00000000, 0xffff0000, 0x0000ffff,
                                   0xffffffff};

static const u32 cfb_tab32[] = {0x00000000, 0xffffffff};

const u32 *xxx(int bpp) {
  const u32 *tab;

  if (0)
    return &deadfish;

  switch (bpp) {
  case 8:
    tab = cfb_tab8_be;
    break;
  case 16:
    tab = cfb_tab16_be;
    break;
  case 32:
  default:
    tab = cfb_tab32;
    break;
  }

  return tab;
}

int main(void) {
  const u32 *a = xxx(8);
  int        b = a[0];
  if (b != cfb_tab8_be[0])
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
// DEFAULT-NEXT:     type @type[[TYPE_u32:[0-9]+]] u32 = u32;
// DEFAULT-NEXT:     global %[[VALUE_deadfish:[0-9]+]] deadfish: u32 [storage=static] [const] = reinterpret<u32, reason=assign>(const<u32>(3735941461)) [c="u32"] [linkage=internal] [c="const u32"];
// DEFAULT-NEXT:     global %[[VALUE_cfb_tab8_be:[0-9]+]] cfb_tab8_be: array<u32, 16> [storage=static] [const] [align=16] = aggregate<array<u32, 16>, zero_fill=false>(index0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(0)) [c="u32"], index1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(255)) [c="u32"], index2 = reinterpret<u32, reason=assign, fits=always>(const<i32>(65280)) [c="u32"], index3 = reinterpret<u32, reason=assign, fits=always>(const<i32>(65535)) [c="u32"], index4 = reinterpret<u32, reason=assign, fits=always>(const<i32>(16711680)) [c="u32"], index5 = reinterpret<u32, reason=assign, fits=always>(const<i32>(16711935)) [c="u32"], index6 = reinterpret<u32, reason=assign, fits=always>(const<i32>(16776960)) [c="u32"], index7 = reinterpret<u32, reason=assign, fits=always>(const<i32>(16777215)) [c="u32"], index8 = reinterpret<u32, reason=assign>(const<u32>(4278190080)) [c="u32"], index9 = reinterpret<u32, reason=assign>(const<u32>(4278190335)) [c="u32"], index10 = reinterpret<u32, reason=assign>(const<u32>(4278255360)) [c="u32"], index11 = reinterpret<u32, reason=assign>(const<u32>(4278255615)) [c="u32"], index12 = reinterpret<u32, reason=assign>(const<u32>(4294901760)) [c="u32"], index13 = reinterpret<u32, reason=assign>(const<u32>(4294902015)) [c="u32"], index14 = reinterpret<u32, reason=assign>(const<u32>(4294967040)) [c="u32"], index15 = reinterpret<u32, reason=assign>(const<u32>(4294967295)) [c="u32"]) [linkage=internal] [c="const u32[]"];
// DEFAULT-NEXT:     global %[[VALUE_cfb_tab8_le:[0-9]+]] cfb_tab8_le: array<u32, 16> [storage=static] [const] [align=16] = aggregate<array<u32, 16>, zero_fill=false>(index0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(0)) [c="u32"], index1 = reinterpret<u32, reason=assign>(const<u32>(4278190080)) [c="u32"], index2 = reinterpret<u32, reason=assign, fits=always>(const<i32>(16711680)) [c="u32"], index3 = reinterpret<u32, reason=assign>(const<u32>(4294901760)) [c="u32"], index4 = reinterpret<u32, reason=assign, fits=always>(const<i32>(65280)) [c="u32"], index5 = reinterpret<u32, reason=assign>(const<u32>(4278255360)) [c="u32"], index6 = reinterpret<u32, reason=assign, fits=always>(const<i32>(16776960)) [c="u32"], index7 = reinterpret<u32, reason=assign>(const<u32>(4294967040)) [c="u32"], index8 = reinterpret<u32, reason=assign, fits=always>(const<i32>(255)) [c="u32"], index9 = reinterpret<u32, reason=assign>(const<u32>(4278190335)) [c="u32"], index10 = reinterpret<u32, reason=assign, fits=always>(const<i32>(16711935)) [c="u32"], index11 = reinterpret<u32, reason=assign>(const<u32>(4294902015)) [c="u32"], index12 = reinterpret<u32, reason=assign, fits=always>(const<i32>(65535)) [c="u32"], index13 = reinterpret<u32, reason=assign>(const<u32>(4278255615)) [c="u32"], index14 = reinterpret<u32, reason=assign, fits=always>(const<i32>(16777215)) [c="u32"], index15 = reinterpret<u32, reason=assign>(const<u32>(4294967295)) [c="u32"]) [linkage=internal] [c="const u32[]"];
// DEFAULT-NEXT:     global %[[VALUE_cfb_tab16_be:[0-9]+]] cfb_tab16_be: array<u32, 4> [storage=static] [const] [align=16] = aggregate<array<u32, 4>, zero_fill=false>(index0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(0)) [c="u32"], index1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(65535)) [c="u32"], index2 = reinterpret<u32, reason=assign>(const<u32>(4294901760)) [c="u32"], index3 = reinterpret<u32, reason=assign>(const<u32>(4294967295)) [c="u32"]) [linkage=internal] [c="const u32[]"];
// DEFAULT-NEXT:     global %[[VALUE_cfb_tab16_le:[0-9]+]] cfb_tab16_le: array<u32, 4> [storage=static] [const] [align=16] = aggregate<array<u32, 4>, zero_fill=false>(index0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(0)) [c="u32"], index1 = reinterpret<u32, reason=assign>(const<u32>(4294901760)) [c="u32"], index2 = reinterpret<u32, reason=assign, fits=always>(const<i32>(65535)) [c="u32"], index3 = reinterpret<u32, reason=assign>(const<u32>(4294967295)) [c="u32"]) [linkage=internal] [c="const u32[]"];
// DEFAULT-NEXT:     global %[[VALUE_cfb_tab32:[0-9]+]] cfb_tab32: array<u32, 2> [storage=static] [const] = aggregate<array<u32, 2>, zero_fill=false>(index0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(0)) [c="u32"], index1 = reinterpret<u32, reason=assign>(const<u32>(4294967295)) [c="u32"]) [linkage=internal] [c="const u32[]"];
// DEFAULT-NEXT:     fn %[[VALUE_xxx:[0-9]+]] @xxx(%[[VALUE_bpp:[0-9]+]] bpp: i32) -> ptr<const u32> [linkage=external] [fallthrough=ub_if_used] [c="const u32 *(int)"] {
// DEFAULT-NEXT:         let %[[VALUE_tab:[0-9]+]] tab: ptr<const u32> [storage=automatic] [c="const u32 *"];
// DEFAULT-NEXT:         if ne<i32>(const<i32>(0), const<i32>(0))
// DEFAULT-NEXT:             return addr_of<ptr<const u32>>(%[[VALUE_deadfish]]) [c="const u32 *"];
// DEFAULT-NEXT:         switch %[[VALUE0:[0-9]+]] read<i32>(%[[VALUE_bpp]])
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(8):
// DEFAULT-NEXT:                     write<ptr<const u32>>(%[[VALUE_tab]], array_decay<ptr<const u32>, length=Some(16)>(%[[VALUE_cfb_tab8_be]]) [c="const u32 *"]) [c="const u32 *"];
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(16):
// DEFAULT-NEXT:                     write<ptr<const u32>>(%[[VALUE_tab]], array_decay<ptr<const u32>, length=Some(4)>(%[[VALUE_cfb_tab16_be]]) [c="const u32 *"]) [c="const u32 *"];
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(32):
// DEFAULT-NEXT:                     default %[[VALUE0]]:
// DEFAULT-NEXT:                         write<ptr<const u32>>(%[[VALUE_tab]], array_decay<ptr<const u32>, length=Some(2)>(%[[VALUE_cfb_tab32]]) [c="const u32 *"]) [c="const u32 *"];
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<ptr<const u32>>(%[[VALUE_tab]]) [c="const u32 *"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: ptr<const u32> [storage=automatic] = call<ptr<const u32>, signature=fn(i32) -> ptr<const u32>>(%[[VALUE_xxx]], const<i32>(8)) [c="const u32 *"] [c="const u32 *"];
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(read<u32>(deref(ptr_offset<ptr<const u32>, subtract=false, element=u32, overflow=ub>(read<ptr<const u32>>(%[[VALUE_a]]), const<i32>(0)))) [c="u32"]);
// DEFAULT-NEXT:         if ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%[[VALUE_b]])) [c="u32"], read<u32>(deref(ptr_offset<ptr<const u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<const u32>, length=Some(16)>(%[[VALUE_cfb_tab8_be]]), const<i32>(0)))) [c="u32"])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
