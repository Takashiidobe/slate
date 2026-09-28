// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

struct one_char { char a; };
struct three_chars { char a, b, c; };
struct int_pair { int a, b; };
struct int_triple { int a, b, c; };
struct one_float { float f; };
struct one_double { double d; };
struct char_array { char a[4]; };
struct odd_array { char a[3]; char b; };
struct char_short { char a; short b; };
struct nested { struct int_pair inner; };
struct pointer { void (*callback)(void); };
struct aligned { _Alignas(8) int x; };
struct nested_aligned { struct aligned inner; };
struct aligned16 { _Alignas(16) int x; };
struct __attribute__((packed)) packed { char c; int i; };
union int_or_float { int i; float f; };
union odd_union { char c[3]; short s; };

struct one_char return_one_char(void);
struct three_chars return_three_chars(void);
struct int_pair return_int_pair(void);
struct int_triple return_int_triple(void);
struct one_float return_one_float(void);
struct one_double return_one_double(void);
struct char_array return_char_array(void);
struct odd_array return_odd_array(void);
struct char_short return_char_short(void);
struct nested return_nested(void);
struct pointer return_pointer(void);
struct aligned return_aligned(void);
struct aligned16 return_aligned16(void);
struct packed return_packed(void);
union int_or_float return_int_or_float(void);
union odd_union return_odd_union(void);
float _Complex return_complex_float(void);
double _Complex return_complex_double(void);
_Atomic struct int_pair return_atomic_pair(void);

void pass_int_pair(struct int_pair);
void pass_one_double(struct one_double);
void pass_aligned(struct aligned);
void pass_nested_aligned(struct nested_aligned);
void pass_aligned16(struct aligned16);
void pass_complex_double(double _Complex);
void pass_atomic_pair(_Atomic struct int_pair);
void pass_aligned_variadic(struct aligned, ...);

typedef float v4sf __attribute__((vector_size(16)));
typedef int v2si __attribute__((vector_size(8)));

v2si return_v2si(void);
v4sf return_v4sf(void);
void pass_vectors(v4sf, v2si, v4sf, v4sf, v2si);

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "i686-pc-windows-msvc" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=4, align=4];
// IR-NEXT:         stack_alignment = 4;
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
// IR-NEXT:     type @type0 one_char = struct {
// IR-NEXT:         field0 a: i8;
// IR-NEXT:     } [size=1, align=1, offsets=[0]];
// IR-NEXT:     type @type1 three_chars = struct {
// IR-NEXT:         field0 a: i8;
// IR-NEXT:         field1 b: i8;
// IR-NEXT:         field2 c: i8;
// IR-NEXT:     } [size=3, align=1, offsets=[0, 1, 2]];
// IR-NEXT:     type @type2 int_pair = struct {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:         field1 b: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type3 int_triple = struct {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:         field1 b: i32;
// IR-NEXT:         field2 c: i32;
// IR-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// IR-NEXT:     type @type4 one_float = struct {
// IR-NEXT:         field0 f: f32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type5 one_double = struct {
// IR-NEXT:         field0 d: f64;
// IR-NEXT:     } [size=8, align=8, offsets=[0]];
// IR-NEXT:     type @type6 char_array = struct {
// IR-NEXT:         field0 a: array<i8, 4>;
// IR-NEXT:     } [size=4, align=1, offsets=[0]];
// IR-NEXT:     type @type7 odd_array = struct {
// IR-NEXT:         field0 a: array<i8, 3>;
// IR-NEXT:         field1 b: i8;
// IR-NEXT:     } [size=4, align=1, offsets=[0, 3]];
// IR-NEXT:     type @type8 char_short = struct {
// IR-NEXT:         field0 a: i8;
// IR-NEXT:         field1 b: i16;
// IR-NEXT:     } [size=4, align=2, offsets=[0, 2]];
// IR-NEXT:     type @type9 nested = struct {
// IR-NEXT:         field0 inner: @type2;
// IR-NEXT:     } [size=8, align=4, offsets=[0]];
// IR-NEXT:     type @type10 pointer = struct {
// IR-NEXT:         field0 callback: ptr<fn() -> void>;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type11 aligned = struct {
// IR-NEXT:         field0 x: i32;
// IR-NEXT:     } [size=8, align=8, offsets=[0]];
// IR-NEXT:     type @type12 nested_aligned = struct {
// IR-NEXT:         field0 inner: @type11;
// IR-NEXT:     } [size=8, align=8, offsets=[0]];
// IR-NEXT:     type @type13 aligned16 = struct {
// IR-NEXT:         field0 x: i32;
// IR-NEXT:     } [size=16, align=16, offsets=[0]];
// IR-NEXT:     type @type14 packed = struct {
// IR-NEXT:         field0 c: i8;
// IR-NEXT:         field1 i: i32;
// IR-NEXT:     } [size=5, align=1, offsets=[0, 1]];
// IR-NEXT:     type @type15 int_or_float = union {
// IR-NEXT:         field0 i: i32;
// IR-NEXT:         field1 f: f32;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// IR-NEXT:     type @type16 odd_union = union {
// IR-NEXT:         field0 c: array<i8, 3>;
// IR-NEXT:         field1 s: i16;
// IR-NEXT:     } [size=4, align=2, offsets=[0, 0]];
// IR-NEXT:     type @type17 v4sf = vector<f32, 4>;
// IR-NEXT:     type @type18 v2si = vector<i32, 2>;
// IR-NEXT:     fn %17 @return_one_char() -> @type0 [linkage=external] [abi=x86_win32() -> native_c];
// IR-NEXT:     fn %18 @return_three_chars() -> @type1 [linkage=external] [abi=x86_win32() -> native_c];
// IR-NEXT:     fn %19 @return_int_pair() -> @type2 [linkage=external] [abi=x86_win32() -> native_c];
// IR-NEXT:     fn %20 @return_int_triple() -> @type3 [linkage=external] [abi=x86_win32() -> native_c];
// IR-NEXT:     fn %21 @return_one_float() -> @type4 [linkage=external] [abi=x86_win32() -> native_c];
// IR-NEXT:     fn %22 @return_one_double() -> @type5 [linkage=external] [abi=x86_win32() -> native_c];
// IR-NEXT:     fn %23 @return_char_array() -> @type6 [linkage=external] [abi=x86_win32() -> native_c];
// IR-NEXT:     fn %24 @return_odd_array() -> @type7 [linkage=external] [abi=x86_win32() -> sret<align=1>];
// IR-NEXT:     fn %25 @return_char_short() -> @type8 [linkage=external] [abi=x86_win32() -> native_c];
// IR-NEXT:     fn %26 @return_nested() -> @type9 [linkage=external] [abi=x86_win32() -> native_c];
// IR-NEXT:     fn %27 @return_pointer() -> @type10 [linkage=external] [abi=x86_win32() -> native_c];
// IR-NEXT:     fn %28 @return_aligned() -> @type11 [linkage=external] [abi=x86_win32() -> native_c];
// IR-NEXT:     fn %29 @return_aligned16() -> @type13 [linkage=external] [abi=x86_win32() -> native_c];
// IR-NEXT:     fn %30 @return_packed() -> @type14 [linkage=external] [abi=x86_win32() -> native_c];
// IR-NEXT:     fn %31 @return_int_or_float() -> @type15 [linkage=external] [abi=x86_win32() -> native_c];
// IR-NEXT:     fn %32 @return_odd_union() -> @type16 [linkage=external] [abi=x86_win32() -> sret<align=2>];
// IR-NEXT:     fn %33 @return_complex_float() -> complex<f32> [linkage=external] [abi=x86_win32() -> native_c];
// IR-NEXT:     fn %34 @return_complex_double() -> complex<f64> [linkage=external] [abi=x86_win32() -> native_c];
// IR-NEXT:     fn %35 @return_atomic_pair() -> @type2 [linkage=external] [abi=x86_win32() -> sret<align=8>];
// IR-NEXT:     fn %36 @pass_int_pair(%49 <unnamed>: @type2) -> void [linkage=external] [abi=x86_win32(native_c) -> void];
// IR-NEXT:     fn %37 @pass_one_double(%50 <unnamed>: @type5) -> void [linkage=external] [abi=x86_win32(native_c) -> void];
// IR-NEXT:     fn %38 @pass_aligned(%51 <unnamed>: @type11) -> void [linkage=external] [abi=x86_win32(native_c) -> void];
// IR-NEXT:     fn %39 @pass_nested_aligned(%52 <unnamed>: @type12) -> void [linkage=external] [abi=x86_win32(native_c) -> void];
// IR-NEXT:     fn %40 @pass_aligned16(%53 <unnamed>: @type13) -> void [linkage=external] [abi=x86_win32(native_c) -> void];
// IR-NEXT:     fn %41 @pass_complex_double(%54 <unnamed>: complex<f64>) -> void [linkage=external] [abi=x86_win32(native_c) -> void];
// IR-NEXT:     fn %42 @pass_atomic_pair(%55 <unnamed>: atomic @type2) -> void [linkage=external] [abi=x86_win32(byval<align=4>) -> void];
// IR-NEXT:     fn %43 @pass_aligned_variadic(%56 <unnamed>: @type11, ...) -> void [linkage=external] [abi=x86_win32(native_c) -> void];
// IR-NEXT:     fn %46 @return_v2si() -> vector<i32, 2> [linkage=external] [abi=x86_win32() -> direct];
// IR-NEXT:     fn %47 @return_v4sf() -> vector<f32, 4> [linkage=external] [abi=x86_win32() -> direct];
// IR-NEXT:     fn %48 @pass_vectors(%57 <unnamed>: vector<f32, 4>, %58 <unnamed>: vector<i32, 2>, %59 <unnamed>: vector<f32, 4>, %60 <unnamed>: vector<f32, 4>, %61 <unnamed>: vector<i32, 2>) -> void [linkage=external] [abi=x86_win32(direct, direct, direct, byref<align=16>, byref<align=8>) -> void];
// IR-NEXT: }
// SLATE-FILECHECK-END IR
