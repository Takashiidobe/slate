// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=gnu23

int f(int a) { return a; }
_Static_assert(__builtin_types_compatible_p(typeof(&f), int (*)(int)), "");
enum { N = 3 };
int shadow(void) {
    int N = 1;
    _Static_assert(sizeof(N) == sizeof(int), "");
    return N;
}
int sized(void) {
    static int buf[N];
    _Static_assert(sizeof(buf) == 3 * sizeof(int), "");
    return buf[0];
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
// IR-NEXT:     type @type0 = enum : u32 {
// IR-NEXT:         %0 N = const<i32>(3);
// IR-NEXT:     } [size=4, align=4];
// IR-NEXT:     global %7 buf: array<i32, 3> [storage=static] [linkage=internal];
// IR-NEXT:     fn %0 @f(%1 a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<i32>(%1);
// IR-NEXT:     }
// IR-NEXT:     fn %4 @shadow() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %5 N: i32 [storage=automatic] = const<i32>(1);
// IR-NEXT:         return read<i32>(%5);
// IR-NEXT:     }
// IR-NEXT:     fn %6 @sized() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%7), const<i32>(0))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
