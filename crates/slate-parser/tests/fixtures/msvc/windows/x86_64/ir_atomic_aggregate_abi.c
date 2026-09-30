// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

// The lock-prefixed layout is what win64 classifies, so an atomic aggregate
// can land in a register where the same record passed plainly goes indirect:
// three is 3 bytes plain and 8 with its lock. Measured against cl.exe 19.51
// /std:c17 /experimental:c11atomics.

struct three { char a, b, c; };
struct arr3 { char a[3]; };
struct one { int a; };
struct pair { int a, b; };
struct five { char a[5]; };
struct wide { long long a, b, c; };
union choice { int a; float b; };

void atomic_three(_Atomic struct three v);
void plain_three(struct three v);
void atomic_arr3(_Atomic struct arr3 v);
void plain_arr3(struct arr3 v);
void atomic_one(_Atomic struct one v);
void atomic_pair(_Atomic struct pair v);
void plain_pair(struct pair v);
void atomic_five(_Atomic struct five v);
void atomic_wide(_Atomic struct wide v);
void atomic_union(_Atomic union choice v);
void atomic_scalar(_Atomic long long v);

_Atomic struct three atomic_result(void);
_Atomic struct five atomic_result_indirect(void);

void caller(void) {
    _Atomic struct three a;
    struct three a2;
    _Atomic struct arr3 b;
    struct arr3 b2;
    _Atomic struct one c;
    _Atomic struct pair d;
    struct pair d2;
    _Atomic struct five e;
    _Atomic struct wide f;
    _Atomic union choice g;
    _Atomic long long h;
    atomic_three(a);
    plain_three(a2);
    atomic_arr3(b);
    plain_arr3(b2);
    atomic_one(c);
    atomic_pair(d);
    plain_pair(d2);
    atomic_five(e);
    atomic_wide(f);
    atomic_union(g);
    atomic_scalar(h);
}

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
// IR-NEXT:     type @type[[TYPE_three:[0-9]+]] three = struct {
// IR-NEXT:         field0 a: i8;
// IR-NEXT:         field1 b: i8;
// IR-NEXT:         field2 c: i8;
// IR-NEXT:     } [size=3, align=1, offsets=[0, 1, 2]];
// IR-NEXT:     type @type[[TYPE_arr3:[0-9]+]] arr3 = struct {
// IR-NEXT:         field0 a: array<i8, 3>;
// IR-NEXT:     } [size=3, align=1, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_one:[0-9]+]] one = struct {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_pair:[0-9]+]] pair = struct {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:         field1 b: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type[[TYPE_five:[0-9]+]] five = struct {
// IR-NEXT:         field0 a: array<i8, 5>;
// IR-NEXT:     } [size=5, align=1, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_wide:[0-9]+]] wide = struct {
// IR-NEXT:         field0 a: i64;
// IR-NEXT:         field1 b: i64;
// IR-NEXT:         field2 c: i64;
// IR-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// IR-NEXT:     type @type[[TYPE_choice:[0-9]+]] choice = union {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:         field1 b: f32;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// IR-NEXT:     fn %[[VALUE_atomic_three:[0-9]+]] @atomic_three(%[[VALUE_v:[0-9]+]] v: atomic @type[[TYPE_three]]) -> void [linkage=external] [abi=win64(native_c) -> void];
// IR-NEXT:     fn %[[VALUE_plain_three:[0-9]+]] @plain_three(%[[VALUE_v_2:[0-9]+]] v: @type[[TYPE_three]]) -> void [linkage=external] [abi=win64(native_c) -> void];
// IR-NEXT:     fn %[[VALUE_atomic_arr3:[0-9]+]] @atomic_arr3(%[[VALUE_v_3:[0-9]+]] v: atomic @type[[TYPE_arr3]]) -> void [linkage=external] [abi=win64(native_c) -> void];
// IR-NEXT:     fn %[[VALUE_plain_arr3:[0-9]+]] @plain_arr3(%[[VALUE_v_4:[0-9]+]] v: @type[[TYPE_arr3]]) -> void [linkage=external] [abi=win64(native_c) -> void];
// IR-NEXT:     fn %[[VALUE_atomic_one:[0-9]+]] @atomic_one(%[[VALUE_v_5:[0-9]+]] v: atomic @type[[TYPE_one]]) -> void [linkage=external] [abi=win64(native_c) -> void];
// IR-NEXT:     fn %[[VALUE_atomic_pair:[0-9]+]] @atomic_pair(%[[VALUE_v_6:[0-9]+]] v: atomic @type[[TYPE_pair]]) -> void [linkage=external] [abi=win64(native_c) -> void];
// IR-NEXT:     fn %[[VALUE_plain_pair:[0-9]+]] @plain_pair(%[[VALUE_v_7:[0-9]+]] v: @type[[TYPE_pair]]) -> void [linkage=external] [abi=win64(native_c) -> void];
// IR-NEXT:     fn %[[VALUE_atomic_five:[0-9]+]] @atomic_five(%[[VALUE_v_8:[0-9]+]] v: atomic @type[[TYPE_five]]) -> void [linkage=external] [abi=win64(native_c) -> void];
// IR-NEXT:     fn %[[VALUE_atomic_wide:[0-9]+]] @atomic_wide(%[[VALUE_v_9:[0-9]+]] v: atomic @type[[TYPE_wide]]) -> void [linkage=external] [abi=win64(native_c) -> void];
// IR-NEXT:     fn %[[VALUE_atomic_union:[0-9]+]] @atomic_union(%[[VALUE_v_10:[0-9]+]] v: atomic @type[[TYPE_choice]]) -> void [linkage=external] [abi=win64(native_c) -> void];
// IR-NEXT:     fn %[[VALUE_atomic_scalar:[0-9]+]] @atomic_scalar(%[[VALUE_v_11:[0-9]+]] v: atomic i64) -> void [linkage=external];
// IR-NEXT:     fn %[[VALUE_atomic_result:[0-9]+]] @atomic_result() -> @type[[TYPE_three]] [linkage=external] [abi=win64() -> native_c];
// IR-NEXT:     fn %[[VALUE_atomic_result_indirect:[0-9]+]] @atomic_result_indirect() -> @type[[TYPE_five]] [linkage=external] [abi=win64() -> native_c];
// IR-NEXT:     fn %[[VALUE_caller:[0-9]+]] @caller() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %[[VALUE_a:[0-9]+]] a: atomic @type[[TYPE_three]] [storage=automatic];
// IR-NEXT:         let %[[VALUE_a2:[0-9]+]] a2: @type[[TYPE_three]] [storage=automatic];
// IR-NEXT:         let %[[VALUE_b:[0-9]+]] b: atomic @type[[TYPE_arr3]] [storage=automatic];
// IR-NEXT:         let %[[VALUE_b2:[0-9]+]] b2: @type[[TYPE_arr3]] [storage=automatic];
// IR-NEXT:         let %[[VALUE_c:[0-9]+]] c: atomic @type[[TYPE_one]] [storage=automatic];
// IR-NEXT:         let %[[VALUE_d:[0-9]+]] d: atomic @type[[TYPE_pair]] [storage=automatic];
// IR-NEXT:         let %[[VALUE_d2:[0-9]+]] d2: @type[[TYPE_pair]] [storage=automatic];
// IR-NEXT:         let %[[VALUE_e:[0-9]+]] e: atomic @type[[TYPE_five]] [storage=automatic];
// IR-NEXT:         let %[[VALUE_f:[0-9]+]] f: atomic @type[[TYPE_wide]] [storage=automatic];
// IR-NEXT:         let %[[VALUE_g:[0-9]+]] g: atomic @type[[TYPE_choice]] [storage=automatic];
// IR-NEXT:         let %[[VALUE_h:[0-9]+]] h: atomic i64 [storage=automatic];
// IR-NEXT:         call<void, abi=win64(native_c) -> void>(%[[VALUE_atomic_three]], copy<@type[[TYPE_three]], reason=arg>(read<@type[[TYPE_three]], atomic=seq_cst>(%[[VALUE_a]])));
// IR-NEXT:         call<void, abi=win64(native_c) -> void>(%[[VALUE_plain_three]], copy<@type[[TYPE_three]], reason=arg>(read<@type[[TYPE_three]]>(%[[VALUE_a2]])));
// IR-NEXT:         call<void, abi=win64(native_c) -> void>(%[[VALUE_atomic_arr3]], copy<@type[[TYPE_arr3]], reason=arg>(read<@type[[TYPE_arr3]], atomic=seq_cst>(%[[VALUE_b]])));
// IR-NEXT:         call<void, abi=win64(native_c) -> void>(%[[VALUE_plain_arr3]], copy<@type[[TYPE_arr3]], reason=arg>(read<@type[[TYPE_arr3]]>(%[[VALUE_b2]])));
// IR-NEXT:         call<void, abi=win64(native_c) -> void>(%[[VALUE_atomic_one]], copy<@type[[TYPE_one]], reason=arg>(read<@type[[TYPE_one]], atomic=seq_cst>(%[[VALUE_c]])));
// IR-NEXT:         call<void, abi=win64(native_c) -> void>(%[[VALUE_atomic_pair]], copy<@type[[TYPE_pair]], reason=arg>(read<@type[[TYPE_pair]], atomic=seq_cst>(%[[VALUE_d]])));
// IR-NEXT:         call<void, abi=win64(native_c) -> void>(%[[VALUE_plain_pair]], copy<@type[[TYPE_pair]], reason=arg>(read<@type[[TYPE_pair]]>(%[[VALUE_d2]])));
// IR-NEXT:         call<void, abi=win64(native_c) -> void>(%[[VALUE_atomic_five]], copy<@type[[TYPE_five]], reason=arg>(read<@type[[TYPE_five]], atomic=seq_cst>(%[[VALUE_e]])));
// IR-NEXT:         call<void, abi=win64(native_c) -> void>(%[[VALUE_atomic_wide]], copy<@type[[TYPE_wide]], reason=arg>(read<@type[[TYPE_wide]], atomic=seq_cst>(%[[VALUE_f]])));
// IR-NEXT:         call<void, abi=win64(native_c) -> void>(%[[VALUE_atomic_union]], copy<@type[[TYPE_choice]], reason=arg>(read<@type[[TYPE_choice]], atomic=seq_cst>(%[[VALUE_g]])));
// IR-NEXT:         call<void>(%[[VALUE_atomic_scalar]], read<i64, atomic=seq_cst>(%[[VALUE_h]]));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
