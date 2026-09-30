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
// IR-NEXT:     type @type[[TYPE_one_char:[0-9]+]] one_char = struct {
// IR-NEXT:         field0 a: i8;
// IR-NEXT:     } [size=1, align=1, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_three_chars:[0-9]+]] three_chars = struct {
// IR-NEXT:         field0 a: i8;
// IR-NEXT:         field1 b: i8;
// IR-NEXT:         field2 c: i8;
// IR-NEXT:     } [size=3, align=1, offsets=[0, 1, 2]];
// IR-NEXT:     type @type[[TYPE_int_pair:[0-9]+]] int_pair = struct {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:         field1 b: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type[[TYPE_int_triple:[0-9]+]] int_triple = struct {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:         field1 b: i32;
// IR-NEXT:         field2 c: i32;
// IR-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// IR-NEXT:     type @type[[TYPE_one_float:[0-9]+]] one_float = struct {
// IR-NEXT:         field0 f: f32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_one_double:[0-9]+]] one_double = struct {
// IR-NEXT:         field0 d: f64;
// IR-NEXT:     } [size=8, align=8, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_char_array:[0-9]+]] char_array = struct {
// IR-NEXT:         field0 a: array<i8, 4>;
// IR-NEXT:     } [size=4, align=1, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_odd_array:[0-9]+]] odd_array = struct {
// IR-NEXT:         field0 a: array<i8, 3>;
// IR-NEXT:         field1 b: i8;
// IR-NEXT:     } [size=4, align=1, offsets=[0, 3]];
// IR-NEXT:     type @type[[TYPE_char_short:[0-9]+]] char_short = struct {
// IR-NEXT:         field0 a: i8;
// IR-NEXT:         field1 b: i16;
// IR-NEXT:     } [size=4, align=2, offsets=[0, 2]];
// IR-NEXT:     type @type[[TYPE_nested:[0-9]+]] nested = struct {
// IR-NEXT:         field0 inner: @type[[TYPE_int_pair]];
// IR-NEXT:     } [size=8, align=4, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_pointer:[0-9]+]] pointer = struct {
// IR-NEXT:         field0 callback: ptr<fn() -> void>;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_aligned:[0-9]+]] aligned = struct {
// IR-NEXT:         field0 x: i32;
// IR-NEXT:     } [size=8, align=8, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_nested_aligned:[0-9]+]] nested_aligned = struct {
// IR-NEXT:         field0 inner: @type[[TYPE_aligned]];
// IR-NEXT:     } [size=8, align=8, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_aligned16:[0-9]+]] aligned16 = struct {
// IR-NEXT:         field0 x: i32;
// IR-NEXT:     } [size=16, align=16, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_packed:[0-9]+]] packed = struct {
// IR-NEXT:         field0 c: i8;
// IR-NEXT:         field1 i: i32;
// IR-NEXT:     } [size=5, align=1, offsets=[0, 1]];
// IR-NEXT:     type @type[[TYPE_int_or_float:[0-9]+]] int_or_float = union {
// IR-NEXT:         field0 i: i32;
// IR-NEXT:         field1 f: f32;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// IR-NEXT:     type @type[[TYPE_odd_union:[0-9]+]] odd_union = union {
// IR-NEXT:         field0 c: array<i8, 3>;
// IR-NEXT:         field1 s: i16;
// IR-NEXT:     } [size=4, align=2, offsets=[0, 0]];
// IR-NEXT:     type @type[[TYPE_v4sf:[0-9]+]] v4sf = vector<f32, 4>;
// IR-NEXT:     type @type[[TYPE_v2si:[0-9]+]] v2si = vector<i32, 2>;
// IR-NEXT:     fn %[[VALUE_return_one_char:[0-9]+]] @return_one_char() -> @type[[TYPE_one_char]] [linkage=external] [abi=x86_win32() -> native_c];
// IR-NEXT:     fn %[[VALUE_return_three_chars:[0-9]+]] @return_three_chars() -> @type[[TYPE_three_chars]] [linkage=external] [abi=x86_win32() -> native_c];
// IR-NEXT:     fn %[[VALUE_return_int_pair:[0-9]+]] @return_int_pair() -> @type[[TYPE_int_pair]] [linkage=external] [abi=x86_win32() -> native_c];
// IR-NEXT:     fn %[[VALUE_return_int_triple:[0-9]+]] @return_int_triple() -> @type[[TYPE_int_triple]] [linkage=external] [abi=x86_win32() -> native_c];
// IR-NEXT:     fn %[[VALUE_return_one_float:[0-9]+]] @return_one_float() -> @type[[TYPE_one_float]] [linkage=external] [abi=x86_win32() -> native_c];
// IR-NEXT:     fn %[[VALUE_return_one_double:[0-9]+]] @return_one_double() -> @type[[TYPE_one_double]] [linkage=external] [abi=x86_win32() -> native_c];
// IR-NEXT:     fn %[[VALUE_return_char_array:[0-9]+]] @return_char_array() -> @type[[TYPE_char_array]] [linkage=external] [abi=x86_win32() -> native_c];
// IR-NEXT:     fn %[[VALUE_return_odd_array:[0-9]+]] @return_odd_array() -> @type[[TYPE_odd_array]] [linkage=external] [abi=x86_win32() -> sret<align=1>];
// IR-NEXT:     fn %[[VALUE_return_char_short:[0-9]+]] @return_char_short() -> @type[[TYPE_char_short]] [linkage=external] [abi=x86_win32() -> native_c];
// IR-NEXT:     fn %[[VALUE_return_nested:[0-9]+]] @return_nested() -> @type[[TYPE_nested]] [linkage=external] [abi=x86_win32() -> native_c];
// IR-NEXT:     fn %[[VALUE_return_pointer:[0-9]+]] @return_pointer() -> @type[[TYPE_pointer]] [linkage=external] [abi=x86_win32() -> native_c];
// IR-NEXT:     fn %[[VALUE_return_aligned:[0-9]+]] @return_aligned() -> @type[[TYPE_aligned]] [linkage=external] [abi=x86_win32() -> native_c];
// IR-NEXT:     fn %[[VALUE_return_aligned16:[0-9]+]] @return_aligned16() -> @type[[TYPE_aligned16]] [linkage=external] [abi=x86_win32() -> native_c];
// IR-NEXT:     fn %[[VALUE_return_packed:[0-9]+]] @return_packed() -> @type[[TYPE_packed]] [linkage=external] [abi=x86_win32() -> native_c];
// IR-NEXT:     fn %[[VALUE_return_int_or_float:[0-9]+]] @return_int_or_float() -> @type[[TYPE_int_or_float]] [linkage=external] [abi=x86_win32() -> native_c];
// IR-NEXT:     fn %[[VALUE_return_odd_union:[0-9]+]] @return_odd_union() -> @type[[TYPE_odd_union]] [linkage=external] [abi=x86_win32() -> sret<align=2>];
// IR-NEXT:     fn %[[VALUE_return_complex_float:[0-9]+]] @return_complex_float() -> complex<f32> [linkage=external] [abi=x86_win32() -> native_c];
// IR-NEXT:     fn %[[VALUE_return_complex_double:[0-9]+]] @return_complex_double() -> complex<f64> [linkage=external] [abi=x86_win32() -> native_c];
// IR-NEXT:     fn %[[VALUE_return_atomic_pair:[0-9]+]] @return_atomic_pair() -> @type[[TYPE_int_pair]] [linkage=external] [abi=x86_win32() -> sret<align=8>];
// IR-NEXT:     fn %[[VALUE_pass_int_pair:[0-9]+]] @pass_int_pair(%[[VALUE0:[0-9]+]] <unnamed>: @type[[TYPE_int_pair]]) -> void [linkage=external] [abi=x86_win32(native_c) -> void];
// IR-NEXT:     fn %[[VALUE_pass_one_double:[0-9]+]] @pass_one_double(%[[VALUE1:[0-9]+]] <unnamed>: @type[[TYPE_one_double]]) -> void [linkage=external] [abi=x86_win32(native_c) -> void];
// IR-NEXT:     fn %[[VALUE_pass_aligned:[0-9]+]] @pass_aligned(%[[VALUE2:[0-9]+]] <unnamed>: @type[[TYPE_aligned]]) -> void [linkage=external] [abi=x86_win32(native_c) -> void];
// IR-NEXT:     fn %[[VALUE_pass_nested_aligned:[0-9]+]] @pass_nested_aligned(%[[VALUE3:[0-9]+]] <unnamed>: @type[[TYPE_nested_aligned]]) -> void [linkage=external] [abi=x86_win32(native_c) -> void];
// IR-NEXT:     fn %[[VALUE_pass_aligned16:[0-9]+]] @pass_aligned16(%[[VALUE4:[0-9]+]] <unnamed>: @type[[TYPE_aligned16]]) -> void [linkage=external] [abi=x86_win32(native_c) -> void];
// IR-NEXT:     fn %[[VALUE_pass_complex_double:[0-9]+]] @pass_complex_double(%[[VALUE5:[0-9]+]] <unnamed>: complex<f64>) -> void [linkage=external] [abi=x86_win32(native_c) -> void];
// IR-NEXT:     fn %[[VALUE_pass_atomic_pair:[0-9]+]] @pass_atomic_pair(%[[VALUE6:[0-9]+]] <unnamed>: atomic @type[[TYPE_int_pair]]) -> void [linkage=external] [abi=x86_win32(byval<align=4>) -> void];
// IR-NEXT:     fn %[[VALUE_pass_aligned_variadic:[0-9]+]] @pass_aligned_variadic(%[[VALUE7:[0-9]+]] <unnamed>: @type[[TYPE_aligned]], ...) -> void [linkage=external] [abi=x86_win32(native_c) -> void];
// IR-NEXT:     fn %[[VALUE_return_v2si:[0-9]+]] @return_v2si() -> vector<i32, 2> [linkage=external] [abi=x86_win32() -> direct];
// IR-NEXT:     fn %[[VALUE_return_v4sf:[0-9]+]] @return_v4sf() -> vector<f32, 4> [linkage=external] [abi=x86_win32() -> direct];
// IR-NEXT:     fn %[[VALUE_pass_vectors:[0-9]+]] @pass_vectors(%[[VALUE8:[0-9]+]] <unnamed>: vector<f32, 4>, %[[VALUE9:[0-9]+]] <unnamed>: vector<i32, 2>, %[[VALUE10:[0-9]+]] <unnamed>: vector<f32, 4>, %[[VALUE11:[0-9]+]] <unnamed>: vector<f32, 4>, %[[VALUE12:[0-9]+]] <unnamed>: vector<i32, 2>) -> void [linkage=external] [abi=x86_win32(direct, direct, direct, byref<align=16>, byref<align=8>) -> void];
// IR-NEXT: }
// SLATE-FILECHECK-END IR
