// SLATE-FILECHECK-DEFINES DEFAULT

/* PR optimization/6822 */

extern unsigned char foo1 (void);
extern unsigned short foo2 (void);

int bar1 (void)
{
  unsigned char q = foo1 ();
  return (q < 0x80) ? 64 : 0;
}

int bar2 (void)
{
  unsigned short h = foo2 ();
  return (h < 0x8000) ? 64 : 0;
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
// DEFAULT-NEXT:     fn %0 @foo1() -> u8 [linkage=external];
// DEFAULT-NEXT:     fn %1 @foo2() -> u16 [linkage=external];
// DEFAULT-NEXT:     fn %2 @bar1() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 q: u8 [storage=automatic] = call<u8, signature=fn() -> u8>(%0);
// DEFAULT-NEXT:         return conditional<i32>(lt<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%3))), const<i32>(128)), const<i32>(64), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @bar2() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 h: u16 [storage=automatic] = call<u16, signature=fn() -> u16>(%1);
// DEFAULT-NEXT:         return conditional<i32>(lt<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%5))), const<i32>(32768)), const<i32>(64), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
