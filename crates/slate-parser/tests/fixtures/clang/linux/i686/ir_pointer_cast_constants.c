// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

enum {
  WRAPPED = (unsigned long long)(int *)-1 == 0xffffffffull,
  SIGNED = (long long)(int *)-2 == 0xfffffffell,
  HIGH_BITS_DROPPED = (unsigned long long)(int *)0x100000004ull,
};

int values(void) {
  switch (1) {
  case (unsigned long long)(char *)-1 - 0xfffffffeull:
    return WRAPPED + SIGNED + HIGH_BITS_DROPPED;
  }
  return 0;
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "i686-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=4, align=4];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=4];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=4];
// IR-NEXT:         storage f80 [size=12, align=4];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type[[TYPE0:[0-9]+]] = enum : u32 {
// IR-NEXT:         %[[VALUE_WRAPPED:[0-9]+]] WRAPPED = const<i32>(1);
// IR-NEXT:         %[[VALUE_SIGNED:[0-9]+]] SIGNED = const<i32>(1);
// IR-NEXT:         %[[VALUE_HIGH_BITS_DROPPED:[0-9]+]] HIGH_BITS_DROPPED = const<i32>(4);
// IR-NEXT:     } [size=4, align=4];
// IR-NEXT:     fn %[[VALUE_values:[0-9]+]] @values() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         switch %[[VALUE0:[0-9]+]] const<i32>(1)
// IR-NEXT:             {
// IR-NEXT:                 case %[[VALUE0]] const<i32>(1):
// IR-NEXT:                     return add<i32>(add<i32>(const<i32>(1), const<i32>(1)), const<i32>(4));
// IR-NEXT:             }
// IR-NEXT:         return const<i32>(0);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
