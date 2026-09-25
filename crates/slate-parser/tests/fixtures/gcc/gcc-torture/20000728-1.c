// SLATE-FILECHECK-DEFINES DEFAULT

struct clock {
  long sec; long usec;
};
        
int foo(void)
{
  struct clock clock_old = {0, 0};

  for (;;) {
    long foo;

    if (foo == clock_old.sec && 0 == clock_old.usec);
  }
  return 0;
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
// DEFAULT-NEXT:     type @type0 clock = struct {
// DEFAULT-NEXT:         field0 sec: i64;
// DEFAULT-NEXT:         field1 usec: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     fn %1 @foo() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %2 clock_old: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = widen<i64, reason=assign>(const<i32>(0)), field1 = widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         for %4
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: omitted
// DEFAULT-NEXT:             increment: omitted
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %3 foo: i64 [storage=automatic];
// DEFAULT-NEXT:                     if logical_and<bool>(eq<i64>(read<i64>(%3), read<i64>(field0(%2))), eq<i64>(widen<i64, reason=usual_arith>(const<i32>(0)), read<i64>(field1(%2))))
// DEFAULT-NEXT:                         ;
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
