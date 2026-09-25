// SLATE-FILECHECK-DEFINES DEFAULT

void f1 (double);
void f2 (int);

void
foo (int type, double xx)
{
  if (type)
    f1 (xx);
  else
    f2 (type);
}

void
bar (int type)
{
  foo (type, 1.0);
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
// DEFAULT-NEXT:     fn %0 @f1(%7 <unnamed>: f64) -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @f2(%8 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo(%3 type: i32, %4 xx: f64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%3), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn(f64) -> void>(%0, read<f64>(%4));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%1, read<i32>(%3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @bar(%6 type: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, f64) -> void>(%2, read<i32>(%6), const<f64>(1.0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
