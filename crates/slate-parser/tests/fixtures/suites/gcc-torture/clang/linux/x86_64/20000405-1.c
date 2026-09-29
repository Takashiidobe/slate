// SLATE-FILECHECK-DEFINES DEFAULT

// Copyright (C) 2000 Free Software Foundation

// by Alexandre Oliva <oliva@lsd.ic.unicamp.br>
// simplified from libio/floatconv.c

static const double bar[] = { 0 };
int j;

double
foo ()
{
  return bar[j];
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
// DEFAULT-NEXT:     global %[[VALUE_bar:[0-9]+]] bar: array<f64, 1> [storage=static] [const] = aggregate<array<f64, 1>, zero_fill=false>(index0 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_j:[0-9]+]] j: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<f64>(deref(ptr_offset<ptr<const f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<const f64>, length=Some(1)>(%[[VALUE_bar]]), read<i32>(%[[VALUE_j]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
