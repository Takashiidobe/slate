// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=gnu17

extern int produce(void);
void sink(void);

_Static_assert(sizeof(produce()) == 4, "sizeof of a call is its result type");

void forward(void) { return sink(); }

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_produce:[0-9]+]] @produce() -> i32 [linkage=external];
// IR-NEXT:     fn %[[VALUE_sink:[0-9]+]] @sink() -> void [linkage=external];
// IR-NEXT:     fn %[[VALUE_forward:[0-9]+]] @forward() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_sink]]);
// IR-NEXT:         return;
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
