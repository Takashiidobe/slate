// SLATE-FILECHECK-DEFINES DEFAULT

long sys_reboot(int magic1, int magic2, int cmd, void * arg)
{
  switch (cmd) {
  case 0x89ABCDEF:
    return 1;

  case 0x00000000:
    return 2;

  case 0xCDEF0123:
    return 3;

  case 0x4321FEDC:
    return 4;

  case 0xA1B2C3D4:
    return 5;

  default:
    return 0;
  };
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
// DEFAULT-NEXT:     fn %0 @sys_reboot(%1 magic1: i32, %2 magic2: i32, %3 cmd: i32, %4 arg: ptr<void>) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         switch %5 read<i32>(%3)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %5 const<i32>(-1985229329):
// DEFAULT-NEXT:                     return widen<i64, reason=return>(const<i32>(1));
// DEFAULT-NEXT:                 case %5 const<i32>(0):
// DEFAULT-NEXT:                     return widen<i64, reason=return>(const<i32>(2));
// DEFAULT-NEXT:                 case %5 const<i32>(-839974621):
// DEFAULT-NEXT:                     return widen<i64, reason=return>(const<i32>(3));
// DEFAULT-NEXT:                 case %5 const<i32>(1126301404):
// DEFAULT-NEXT:                     return widen<i64, reason=return>(const<i32>(4));
// DEFAULT-NEXT:                 case %5 const<i32>(-1582119980):
// DEFAULT-NEXT:                     return widen<i64, reason=return>(const<i32>(5));
// DEFAULT-NEXT:                 default %5:
// DEFAULT-NEXT:                     return widen<i64, reason=return>(const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
