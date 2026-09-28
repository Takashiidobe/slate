// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

int __attribute__((stdcall)) sc(int a);
int __attribute__((fastcall)) fc(int a);
_Static_assert(_Generic(&sc, int (*)(int): 1, default: 0), "");
_Static_assert(__builtin_types_compatible_p(__typeof__(&fc), int (*)(int)), "");
int use(void) { return sc(1) + fc(2); }

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
// IR-NEXT:     fn %1 @sc(%5 a: i32) -> i32 [linkage=external];
// IR-NEXT:     fn %3 @fc(%6 a: i32) -> i32 [linkage=external];
// IR-NEXT:     fn %4 @use() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<i32>(call<i32>(%1, const<i32>(1)), call<i32>(%3, const<i32>(2)));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
