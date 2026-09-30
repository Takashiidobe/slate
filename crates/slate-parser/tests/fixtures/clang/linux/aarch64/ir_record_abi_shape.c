// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

struct three { char a, b, c; };
struct arr3 { char a[3]; };
struct nest { struct three inner; };
struct arr8 { char a[8]; };
struct arr16 { char a[16]; };
struct arr20 { char a[20]; };
struct float_pair { float a, b; };
struct nested_hfa { struct float_pair inner; float c, d; };
struct float_array { float a[4]; };
struct float_five { float a[5]; };
struct float_one { float a; };
union float_union { float a, b; };
struct union_hfa { union float_union a; float b; };
struct mixed { float a; double b; };
struct atomic_wide { long long a, b; };

void pass_arr3(struct arr3 v);
void pass_nest(struct nest v);
void pass_arr8(struct arr8 v);
void pass_arr16(struct arr16 v);
void pass_arr20(struct arr20 v);
void pass_nested_hfa(struct nested_hfa v);
void pass_float_array(struct float_array v);
void pass_float_five(struct float_five v);
void pass_float_one(struct float_one v);
void pass_union_hfa(struct union_hfa v);
void pass_mixed(struct mixed v);
void pass_atomic_hfa(_Atomic struct float_pair v);
void pass_atomic_wide(_Atomic struct atomic_wide v);
void pass_atomic_complex_float(_Atomic float _Complex v);
void pass_atomic_complex_double(_Atomic double _Complex v);

struct arr16 return_arr16(void);
struct nested_hfa return_nested_hfa(void);

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "aarch64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f128;
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
// IR-NEXT:     type @type[[TYPE_three:[0-9]+]] three = struct {
// IR-NEXT:         field0 a: u8;
// IR-NEXT:         field1 b: u8;
// IR-NEXT:         field2 c: u8;
// IR-NEXT:     } [size=3, align=1, offsets=[0, 1, 2]];
// IR-NEXT:     type @type[[TYPE_arr3:[0-9]+]] arr3 = struct {
// IR-NEXT:         field0 a: array<u8, 3>;
// IR-NEXT:     } [size=3, align=1, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_nest:[0-9]+]] nest = struct {
// IR-NEXT:         field0 inner: @type[[TYPE_three]];
// IR-NEXT:     } [size=3, align=1, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_arr8:[0-9]+]] arr8 = struct {
// IR-NEXT:         field0 a: array<u8, 8>;
// IR-NEXT:     } [size=8, align=1, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_arr16:[0-9]+]] arr16 = struct {
// IR-NEXT:         field0 a: array<u8, 16>;
// IR-NEXT:     } [size=16, align=1, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_arr20:[0-9]+]] arr20 = struct {
// IR-NEXT:         field0 a: array<u8, 20>;
// IR-NEXT:     } [size=20, align=1, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_float_pair:[0-9]+]] float_pair = struct {
// IR-NEXT:         field0 a: f32;
// IR-NEXT:         field1 b: f32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type[[TYPE_nested_hfa:[0-9]+]] nested_hfa = struct {
// IR-NEXT:         field0 inner: @type[[TYPE_float_pair]];
// IR-NEXT:         field1 c: f32;
// IR-NEXT:         field2 d: f32;
// IR-NEXT:     } [size=16, align=4, offsets=[0, 8, 12]];
// IR-NEXT:     type @type[[TYPE_float_array:[0-9]+]] float_array = struct {
// IR-NEXT:         field0 a: array<f32, 4>;
// IR-NEXT:     } [size=16, align=4, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_float_five:[0-9]+]] float_five = struct {
// IR-NEXT:         field0 a: array<f32, 5>;
// IR-NEXT:     } [size=20, align=4, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_float_one:[0-9]+]] float_one = struct {
// IR-NEXT:         field0 a: f32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_float_union:[0-9]+]] float_union = union {
// IR-NEXT:         field0 a: f32;
// IR-NEXT:         field1 b: f32;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// IR-NEXT:     type @type[[TYPE_union_hfa:[0-9]+]] union_hfa = struct {
// IR-NEXT:         field0 a: @type[[TYPE_float_union]];
// IR-NEXT:         field1 b: f32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type[[TYPE_mixed:[0-9]+]] mixed = struct {
// IR-NEXT:         field0 a: f32;
// IR-NEXT:         field1 b: f64;
// IR-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// IR-NEXT:     type @type[[TYPE_atomic_wide:[0-9]+]] atomic_wide = struct {
// IR-NEXT:         field0 a: i64;
// IR-NEXT:         field1 b: i64;
// IR-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// IR-NEXT:     fn %[[VALUE_pass_arr3:[0-9]+]] @pass_arr3(%[[VALUE_v:[0-9]+]] v: @type[[TYPE_arr3]]) -> void [linkage=external] [abi=aapcs64(native_c) -> void];
// IR-NEXT:     fn %[[VALUE_pass_nest:[0-9]+]] @pass_nest(%[[VALUE_v_2:[0-9]+]] v: @type[[TYPE_nest]]) -> void [linkage=external] [abi=aapcs64(native_c) -> void];
// IR-NEXT:     fn %[[VALUE_pass_arr8:[0-9]+]] @pass_arr8(%[[VALUE_v_3:[0-9]+]] v: @type[[TYPE_arr8]]) -> void [linkage=external] [abi=aapcs64(native_c) -> void];
// IR-NEXT:     fn %[[VALUE_pass_arr16:[0-9]+]] @pass_arr16(%[[VALUE_v_4:[0-9]+]] v: @type[[TYPE_arr16]]) -> void [linkage=external] [abi=aapcs64(native_c) -> void];
// IR-NEXT:     fn %[[VALUE_pass_arr20:[0-9]+]] @pass_arr20(%[[VALUE_v_5:[0-9]+]] v: @type[[TYPE_arr20]]) -> void [linkage=external] [abi=aapcs64(native_c) -> void];
// IR-NEXT:     fn %[[VALUE_pass_nested_hfa:[0-9]+]] @pass_nested_hfa(%[[VALUE_v_6:[0-9]+]] v: @type[[TYPE_nested_hfa]]) -> void [linkage=external] [abi=aapcs64(native_c) -> void];
// IR-NEXT:     fn %[[VALUE_pass_float_array:[0-9]+]] @pass_float_array(%[[VALUE_v_7:[0-9]+]] v: @type[[TYPE_float_array]]) -> void [linkage=external] [abi=aapcs64(native_c) -> void];
// IR-NEXT:     fn %[[VALUE_pass_float_five:[0-9]+]] @pass_float_five(%[[VALUE_v_8:[0-9]+]] v: @type[[TYPE_float_five]]) -> void [linkage=external] [abi=aapcs64(native_c) -> void];
// IR-NEXT:     fn %[[VALUE_pass_float_one:[0-9]+]] @pass_float_one(%[[VALUE_v_9:[0-9]+]] v: @type[[TYPE_float_one]]) -> void [linkage=external] [abi=aapcs64(native_c) -> void];
// IR-NEXT:     fn %[[VALUE_pass_union_hfa:[0-9]+]] @pass_union_hfa(%[[VALUE_v_10:[0-9]+]] v: @type[[TYPE_union_hfa]]) -> void [linkage=external] [abi=aapcs64(native_c) -> void];
// IR-NEXT:     fn %[[VALUE_pass_mixed:[0-9]+]] @pass_mixed(%[[VALUE_v_11:[0-9]+]] v: @type[[TYPE_mixed]]) -> void [linkage=external] [abi=aapcs64(native_c) -> void];
// IR-NEXT:     fn %[[VALUE_pass_atomic_hfa:[0-9]+]] @pass_atomic_hfa(%[[VALUE_v_12:[0-9]+]] v: atomic @type[[TYPE_float_pair]]) -> void [linkage=external] [abi=aapcs64(coerce<i64>) -> void];
// IR-NEXT:     fn %[[VALUE_pass_atomic_wide:[0-9]+]] @pass_atomic_wide(%[[VALUE_v_13:[0-9]+]] v: atomic @type[[TYPE_atomic_wide]]) -> void [linkage=external] [abi=aapcs64(coerce<i128>) -> void];
// IR-NEXT:     fn %[[VALUE_pass_atomic_complex_float:[0-9]+]] @pass_atomic_complex_float(%[[VALUE_v_14:[0-9]+]] v: atomic complex<f32>) -> void [linkage=external] [abi=aapcs64(coerce<i64>) -> void];
// IR-NEXT:     fn %[[VALUE_pass_atomic_complex_double:[0-9]+]] @pass_atomic_complex_double(%[[VALUE_v_15:[0-9]+]] v: atomic complex<f64>) -> void [linkage=external] [abi=aapcs64(coerce<i128>) -> void];
// IR-NEXT:     fn %[[VALUE_return_arr16:[0-9]+]] @return_arr16() -> @type[[TYPE_arr16]] [linkage=external] [abi=aapcs64() -> native_c];
// IR-NEXT:     fn %[[VALUE_return_nested_hfa:[0-9]+]] @return_nested_hfa() -> @type[[TYPE_nested_hfa]] [linkage=external] [abi=aapcs64() -> native_c];
// IR-NEXT: }
// SLATE-FILECHECK-END IR
