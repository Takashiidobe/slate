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
// DEFAULT-NEXT:     type @type0 u32 = u32;
// DEFAULT-NEXT:     global %1 deadfish: u32 [storage=static] [const] = const<u32>(3735941461) [linkage=internal];
// DEFAULT-NEXT:     global %2 cfb_tab8_be: array<u32, 16> [storage=static] [const] [align=16] = aggregate<array<u32, 16>, zero_fill=false>(index0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(0)), index1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(255)), index2 = reinterpret<u32, reason=assign, fits=always>(const<i32>(65280)), index3 = reinterpret<u32, reason=assign, fits=always>(const<i32>(65535)), index4 = reinterpret<u32, reason=assign, fits=always>(const<i32>(16711680)), index5 = reinterpret<u32, reason=assign, fits=always>(const<i32>(16711935)), index6 = reinterpret<u32, reason=assign, fits=always>(const<i32>(16776960)), index7 = reinterpret<u32, reason=assign, fits=always>(const<i32>(16777215)), index8 = const<u32>(4278190080), index9 = const<u32>(4278190335), index10 = const<u32>(4278255360), index11 = const<u32>(4278255615), index12 = const<u32>(4294901760), index13 = const<u32>(4294902015), index14 = const<u32>(4294967040), index15 = const<u32>(4294967295)) [linkage=internal];
// DEFAULT-NEXT:     global %3 cfb_tab8_le: array<u32, 16> [storage=static] [const] [align=16] = aggregate<array<u32, 16>, zero_fill=false>(index0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(0)), index1 = const<u32>(4278190080), index2 = reinterpret<u32, reason=assign, fits=always>(const<i32>(16711680)), index3 = const<u32>(4294901760), index4 = reinterpret<u32, reason=assign, fits=always>(const<i32>(65280)), index5 = const<u32>(4278255360), index6 = reinterpret<u32, reason=assign, fits=always>(const<i32>(16776960)), index7 = const<u32>(4294967040), index8 = reinterpret<u32, reason=assign, fits=always>(const<i32>(255)), index9 = const<u32>(4278190335), index10 = reinterpret<u32, reason=assign, fits=always>(const<i32>(16711935)), index11 = const<u32>(4294902015), index12 = reinterpret<u32, reason=assign, fits=always>(const<i32>(65535)), index13 = const<u32>(4278255615), index14 = reinterpret<u32, reason=assign, fits=always>(const<i32>(16777215)), index15 = const<u32>(4294967295)) [linkage=internal];
// DEFAULT-NEXT:     global %4 cfb_tab16_be: array<u32, 4> [storage=static] [const] [align=16] = aggregate<array<u32, 4>, zero_fill=false>(index0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(0)), index1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(65535)), index2 = const<u32>(4294901760), index3 = const<u32>(4294967295)) [linkage=internal];
// DEFAULT-NEXT:     global %5 cfb_tab16_le: array<u32, 4> [storage=static] [const] [align=16] = aggregate<array<u32, 4>, zero_fill=false>(index0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(0)), index1 = const<u32>(4294901760), index2 = reinterpret<u32, reason=assign, fits=always>(const<i32>(65535)), index3 = const<u32>(4294967295)) [linkage=internal];
// DEFAULT-NEXT:     global %6 cfb_tab32: array<u32, 2> [storage=static] [const] = aggregate<array<u32, 2>, zero_fill=false>(index0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(0)), index1 = const<u32>(4294967295)) [linkage=internal];
// DEFAULT-NEXT:     fn %7 @xxx(%8 bpp: i32) -> ptr<const u32> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %9 tab: ptr<const u32> [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(const<i32>(0), const<i32>(0))
// DEFAULT-NEXT:             return addr_of<ptr<const u32>>(%1);
// DEFAULT-NEXT:         switch %13 read<i32>(%8)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %13 const<i32>(8):
// DEFAULT-NEXT:                     write<ptr<const u32>>(%9, array_decay<ptr<const u32>, length=Some(16)>(%2));
// DEFAULT-NEXT:                 break %13;
// DEFAULT-NEXT:                 case %13 const<i32>(16):
// DEFAULT-NEXT:                     write<ptr<const u32>>(%9, array_decay<ptr<const u32>, length=Some(4)>(%4));
// DEFAULT-NEXT:                 break %13;
// DEFAULT-NEXT:                 case %13 const<i32>(32):
// DEFAULT-NEXT:                     default %13:
// DEFAULT-NEXT:                         write<ptr<const u32>>(%9, array_decay<ptr<const u32>, length=Some(2)>(%6));
// DEFAULT-NEXT:                 break %13;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<ptr<const u32>>(%9);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %11 a: ptr<const u32> [storage=automatic] = call<ptr<const u32>, signature=fn(i32) -> ptr<const u32>>(%7, const<i32>(8));
// DEFAULT-NEXT:         let %12 b: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(read<u32>(deref(ptr_offset<ptr<const u32>, subtract=false, element=u32, overflow=ub>(read<ptr<const u32>>(%11), const<i32>(0)))));
// DEFAULT-NEXT:         if ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%12)), read<u32>(deref(ptr_offset<ptr<const u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<const u32>, length=Some(16)>(%2), const<i32>(0)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%14);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
