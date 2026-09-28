// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

// win64 classifies a record on its size alone -- one register at 1, 2, 4 or 8
// bytes and indirect otherwise -- so a record whose fields are not all scalars
// still gets a verified answer rather than native_c. These pin the shapes
// against clang -target x86_64-pc-windows-msvc.

struct three { char a, b, c; };
struct arr3 { char a[3]; };
struct nest { struct three inner; };
struct arr8 { char a[8]; };
struct arr2f { float a[2]; };
struct wrapped_union { union { int a; float b; } inner; };
struct float_pair { float a, b; };
struct one_double { double a; };
struct two_doubles { double a, b; };
struct arr20 { char a[20]; };

void by_reference_three(struct three v);
void by_reference_arr3(struct arr3 v);
void by_reference_nest(struct nest v);
void by_reference_two_doubles(struct two_doubles v);
void by_reference_arr20(struct arr20 v);

void in_register_arr8(struct arr8 v);
void in_register_arr2f(struct arr2f v);
void in_register_wrapped_union(struct wrapped_union v);
void in_register_float_pair(struct float_pair v);
void in_register_one_double(struct one_double v);

struct arr8 returns_in_register(void);
struct arr3 returns_indirect(void);
struct arr20 returns_indirect_wide(void);

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-pc-windows-msvc" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f64;
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
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type0 three = struct {
// IR-NEXT:         field0 a: i8;
// IR-NEXT:         field1 b: i8;
// IR-NEXT:         field2 c: i8;
// IR-NEXT:     } [size=3, align=1, offsets=[0, 1, 2]];
// IR-NEXT:     type @type1 arr3 = struct {
// IR-NEXT:         field0 a: array<i8, 3>;
// IR-NEXT:     } [size=3, align=1, offsets=[0]];
// IR-NEXT:     type @type2 nest = struct {
// IR-NEXT:         field0 inner: @type0;
// IR-NEXT:     } [size=3, align=1, offsets=[0]];
// IR-NEXT:     type @type3 arr8 = struct {
// IR-NEXT:         field0 a: array<i8, 8>;
// IR-NEXT:     } [size=8, align=1, offsets=[0]];
// IR-NEXT:     type @type4 arr2f = struct {
// IR-NEXT:         field0 a: array<f32, 2>;
// IR-NEXT:     } [size=8, align=4, offsets=[0]];
// IR-NEXT:     type @type5 wrapped_union = struct {
// IR-NEXT:         field0 inner: @type6;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type6 = union {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:         field1 b: f32;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// IR-NEXT:     type @type7 float_pair = struct {
// IR-NEXT:         field0 a: f32;
// IR-NEXT:         field1 b: f32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type8 one_double = struct {
// IR-NEXT:         field0 a: f64;
// IR-NEXT:     } [size=8, align=8, offsets=[0]];
// IR-NEXT:     type @type9 two_doubles = struct {
// IR-NEXT:         field0 a: f64;
// IR-NEXT:         field1 b: f64;
// IR-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// IR-NEXT:     type @type10 arr20 = struct {
// IR-NEXT:         field0 a: array<i8, 20>;
// IR-NEXT:     } [size=20, align=1, offsets=[0]];
// IR-NEXT:     fn %11 @by_reference_three(%24 v: @type0) -> void [linkage=external] [abi=win64(native_c) -> void];
// IR-NEXT:     fn %12 @by_reference_arr3(%25 v: @type1) -> void [linkage=external] [abi=win64(native_c) -> void];
// IR-NEXT:     fn %13 @by_reference_nest(%26 v: @type2) -> void [linkage=external] [abi=win64(native_c) -> void];
// IR-NEXT:     fn %14 @by_reference_two_doubles(%27 v: @type9) -> void [linkage=external] [abi=win64(native_c) -> void];
// IR-NEXT:     fn %15 @by_reference_arr20(%28 v: @type10) -> void [linkage=external] [abi=win64(native_c) -> void];
// IR-NEXT:     fn %16 @in_register_arr8(%29 v: @type3) -> void [linkage=external] [abi=win64(native_c) -> void];
// IR-NEXT:     fn %17 @in_register_arr2f(%30 v: @type4) -> void [linkage=external] [abi=win64(native_c) -> void];
// IR-NEXT:     fn %18 @in_register_wrapped_union(%31 v: @type5) -> void [linkage=external] [abi=win64(native_c) -> void];
// IR-NEXT:     fn %19 @in_register_float_pair(%32 v: @type7) -> void [linkage=external] [abi=win64(native_c) -> void];
// IR-NEXT:     fn %20 @in_register_one_double(%33 v: @type8) -> void [linkage=external] [abi=win64(native_c) -> void];
// IR-NEXT:     fn %21 @returns_in_register() -> @type3 [linkage=external] [abi=win64() -> native_c];
// IR-NEXT:     fn %22 @returns_indirect() -> @type1 [linkage=external] [abi=win64() -> native_c];
// IR-NEXT:     fn %23 @returns_indirect_wide() -> @type10 [linkage=external] [abi=win64() -> native_c];
// IR-NEXT: }
// SLATE-FILECHECK-END IR
