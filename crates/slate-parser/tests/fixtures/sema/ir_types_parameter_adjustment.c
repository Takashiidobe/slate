// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir-types -std=c23

typedef int arr_t[3];

void fixed(int a[10]);

void qualified(const int a[restrict static 4]);

void typedefed(arr_t a);

void unspecified(int a[]);

void variable(int n, int a[n]);

void star(int a[*]);

void function(int cb(int), int (*fp)(void));

void multidimensional(int a[2][3]);

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
// IR-NEXT:     type @type0 arr_t = array<i32, 3>;
// IR-NEXT:     fn %1 @fixed(%0 a: ptr<i32> [array=10]) -> void [linkage=external];
// IR-NEXT:     fn %3 @qualified(%2 a: ptr<const i32> [restrict] [array=static 4]) -> void [linkage=external];
// IR-NEXT:     fn %5 @typedefed(%4 a: ptr<i32> [array=3]) -> void [linkage=external];
// IR-NEXT:     fn %7 @unspecified(%6 a: ptr<i32>) -> void [linkage=external];
// IR-NEXT:     fn %10 @variable(%8 n: i32, %9 a: ptr<i32> [array=*]) -> void [linkage=external];
// IR-NEXT:     fn %12 @star(%11 a: ptr<i32> [array=*]) -> void [linkage=external];
// IR-NEXT:     fn %15 @function(%13 cb: ptr<fn(i32) -> i32>, %14 fp: ptr<fn() -> i32>) -> void [linkage=external];
// IR-NEXT:     fn %17 @multidimensional(%16 a: ptr<array<i32, 3>> [array=2]) -> void [linkage=external];
// IR-NEXT: }
// SLATE-FILECHECK-END IR
