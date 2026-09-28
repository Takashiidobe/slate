// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

struct odd3 { char a[3]; };

_Atomic struct odd3 atomic_record;
_Atomic int atomic_scalar;
volatile int volatile_scalar;
_Atomic volatile int both;
const int read_only = 1;
int *restrict restricted;

void parameters(_Atomic int a, volatile int v, const int c, int *restrict r);

void untouched(void) {
    _Atomic struct odd3 local_record;
    _Atomic int local_scalar;
    volatile int local_volatile;
    int plain;
}

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
// IR-NEXT:     type @type0 odd3 = struct {
// IR-NEXT:         field0 a: array<i8, 3>;
// IR-NEXT:     } [size=3, align=1, offsets=[0]];
// IR-NEXT:     global %1 atomic_record: atomic @type0 [storage=static] [linkage=external];
// IR-NEXT:     global %2 atomic_scalar: atomic i32 [storage=static] [linkage=external];
// IR-NEXT:     global %3 volatile_scalar: volatile i32 [storage=static] [linkage=external];
// IR-NEXT:     global %4 both: volatile atomic i32 [storage=static] [linkage=external];
// IR-NEXT:     global %5 read_only: i32 [storage=static] [const] = const<i32>(1) [linkage=external];
// IR-NEXT:     global %6 restricted: ptr<i32> [storage=static] [restrict] [linkage=external];
// IR-NEXT:     fn %7 @parameters(%13 a: atomic i32, %14 v: volatile i32, %15 c: i32 [const], %16 r: ptr<i32> [restrict]) -> void [linkage=external];
// IR-NEXT:     fn %8 @untouched() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %9 local_record: atomic @type0 [storage=automatic];
// IR-NEXT:         let %10 local_scalar: atomic i32 [storage=automatic];
// IR-NEXT:         let %11 local_volatile: volatile i32 [storage=automatic];
// IR-NEXT:         let %12 plain: i32 [storage=automatic];
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
