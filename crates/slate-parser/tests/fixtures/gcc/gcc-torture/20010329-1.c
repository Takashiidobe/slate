// SLATE-FILECHECK-DEFINES DEFAULT

union u {
  unsigned char a;
  double b;
};

int a;

union u foo (void)
{
  union u b;

  if (a)
    b.a = 1;
  else
    b.a = 0;
  return b;
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
// DEFAULT-NEXT:     type @type0 u = union {
// DEFAULT-NEXT:         field0 a: u8;
// DEFAULT-NEXT:         field1 b: f64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     global %1 a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo() -> @type0 [linkage=external] [abi=sysv64() -> coerce<i64>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 b: @type0 [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), const<i32>(0))
// DEFAULT-NEXT:             write<u8>(field0(%3), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u8>(field0(%3), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         return copy<@type0, reason=return>(read<@type0>(%3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
