// SLATE-FILECHECK-DEFINES DEFAULT

unsigned char a, b;

void baz (void)
{
  if (b & 0x08)
    {
      int g = 0;
      int c = (b & 0x01);
      int d = a - g - c;
      int e = (a & 0x0f) - (g & 0x0f);
      int f = (a & 0xf0) - (g & 0xf0);
      int h = (a & 0x0f) - (g & 0x0f);

      if ((a ^ g) & (a ^ d) & 0x80) b |= 0x40;
      if ((d & 0xff00) == 0) b |= 0x01;
      if (!((a - h - c) & 0xff)) b |= 0x02;
      if ((a - g - c) & 0x80) b |= 0x80;
      a = (e & 0x0f) | (f & 0xf0);
    }
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
// DEFAULT-NEXT:     global %0 a: u8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 b: u8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %2 @baz() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%1))), const<i32>(8)), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %3 g: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                 let %4 c: i32 [storage=automatic] = and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%1))), const<i32>(1));
// DEFAULT-NEXT:                 let %5 d: i32 [storage=automatic] = sub<i32, overflow=ub>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%0))), read<i32>(%3)), read<i32>(%4));
// DEFAULT-NEXT:                 let %6 e: i32 [storage=automatic] = sub<i32, overflow=ub>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%0))), const<i32>(15)), and<i32>(read<i32>(%3), const<i32>(15)));
// DEFAULT-NEXT:                 let %7 f: i32 [storage=automatic] = sub<i32, overflow=ub>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%0))), const<i32>(240)), and<i32>(read<i32>(%3), const<i32>(240)));
// DEFAULT-NEXT:                 let %8 h: i32 [storage=automatic] = sub<i32, overflow=ub>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%0))), const<i32>(15)), and<i32>(read<i32>(%3), const<i32>(15)));
// DEFAULT-NEXT:                 if ne<i32>(and<i32>(and<i32>(xor<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%0))), read<i32>(%3)), xor<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%0))), read<i32>(%5))), const<i32>(128)), const<i32>(0))
// DEFAULT-NEXT:                     let %9: u8 [synthetic] = read<u8>(%1);
// DEFAULT-NEXT:                     let %10: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%9))), const<i32>(64))));
// DEFAULT-NEXT:                     write<u8>(%1, read<u8>(%10));
// DEFAULT-NEXT:                 if eq<i32>(and<i32>(read<i32>(%5), const<i32>(65280)), const<i32>(0))
// DEFAULT-NEXT:                     let %11: u8 [synthetic] = read<u8>(%1);
// DEFAULT-NEXT:                     let %12: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%11))), const<i32>(1))));
// DEFAULT-NEXT:                     write<u8>(%1, read<u8>(%12));
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(and<i32>(sub<i32, overflow=ub>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%0))), read<i32>(%8)), read<i32>(%4)), const<i32>(255)), const<i32>(0)))
// DEFAULT-NEXT:                     let %13: u8 [synthetic] = read<u8>(%1);
// DEFAULT-NEXT:                     let %14: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%13))), const<i32>(2))));
// DEFAULT-NEXT:                     write<u8>(%1, read<u8>(%14));
// DEFAULT-NEXT:                 if ne<i32>(and<i32>(sub<i32, overflow=ub>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%0))), read<i32>(%3)), read<i32>(%4)), const<i32>(128)), const<i32>(0))
// DEFAULT-NEXT:                     let %15: u8 [synthetic] = read<u8>(%1);
// DEFAULT-NEXT:                     let %16: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%15))), const<i32>(128))));
// DEFAULT-NEXT:                     write<u8>(%1, read<u8>(%16));
// DEFAULT-NEXT:                 write<u8>(%0, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(or<i32>(and<i32>(read<i32>(%6), const<i32>(15)), and<i32>(read<i32>(%7), const<i32>(240))))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
