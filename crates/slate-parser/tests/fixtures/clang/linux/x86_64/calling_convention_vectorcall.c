// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

int __vectorcall vc(int a, double b);
int __attribute__((vectorcall)) vg(int a);
int __stdcall sc(int a);
_Static_assert(!_Generic(&vc, int (*)(int, double): 1, default: 0), "");
_Static_assert(_Generic(&vc, int (__vectorcall *)(int, double): 1, default: 0), "");
_Static_assert(!__builtin_types_compatible_p(__typeof__(&vg), int (*)(int)), "");
_Static_assert(__builtin_types_compatible_p(__typeof__(&sc), int (*)(int)), "");
int vg(int a);
int (__vectorcall *pointer)(int, double) = vc;
int use(void) { return vc(1, 2.0) + vg(2) + sc(3) + pointer(4, 5.0); }

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
// IR-NEXT:     global %8 pointer: ptr<fn vectorcall(i32, f64) -> i32> [storage=static] = function_decay<ptr<fn vectorcall(i32, f64) -> i32>>(%2) [linkage=external];
// IR-NEXT:     fn %2 @vc(%10 a: i32, %11 b: f64) -> i32 [linkage=external] [abi=sysv64 vectorcall(scalar, scalar) -> scalar];
// IR-NEXT:     fn %4 @vg(%12 a: i32) -> i32 [linkage=external] [abi=sysv64 vectorcall(scalar) -> scalar];
// IR-NEXT:     fn %6 @sc(%13 a: i32) -> i32 [linkage=external];
// IR-NEXT:     fn %9 @use() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<i32>(add<i32>(add<i32>(call<i32, abi=sysv64 vectorcall(scalar, scalar) -> scalar>(%2, const<i32>(1), const<f64>(2.0)), call<i32, abi=sysv64 vectorcall(scalar) -> scalar>(%4, const<i32>(2))), call<i32>(%6, const<i32>(3))), call<i32, abi=sysv64 vectorcall(scalar, scalar) -> scalar>(read<ptr<fn vectorcall(i32, f64) -> i32>>(%8), const<i32>(4), const<f64>(5.0)));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
