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
// IR-NEXT:     type @type0 three = struct {
// IR-NEXT:         field0 a: i8;
// IR-NEXT:         field1 b: i8;
// IR-NEXT:         field2 c: i8;
// IR-NEXT:     } [size=3, align=1, offsets=[0, 1, 2]];
// IR-NEXT:     type @type1 arr3 = struct {
// IR-NEXT:         field0 a: array<i8, 3>;
// IR-NEXT:     } [size=3, align=1, offsets=[0]];
// IR-NEXT:     type @type2 one = struct {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type3 pair = struct {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:         field1 b: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type4 five = struct {
// IR-NEXT:         field0 a: array<i8, 5>;
// IR-NEXT:     } [size=5, align=1, offsets=[0]];
// IR-NEXT:     type @type5 wide = struct {
// IR-NEXT:         field0 a: i64;
// IR-NEXT:         field1 b: i64;
// IR-NEXT:         field2 c: i64;
// IR-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// IR-NEXT:     type @type6 choice = union {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:         field1 b: f32;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// IR-NEXT:     fn %8 @atomic_three(%43 v: atomic @type0) -> void [linkage=external] [abi=win64(native_c) -> void];
// IR-NEXT:     fn %10 @plain_three(%44 v: @type0) -> void [linkage=external] [abi=win64(native_c) -> void];
// IR-NEXT:     fn %12 @atomic_arr3(%45 v: atomic @type1) -> void [linkage=external] [abi=win64(native_c) -> void];
// IR-NEXT:     fn %14 @plain_arr3(%46 v: @type1) -> void [linkage=external] [abi=win64(native_c) -> void];
// IR-NEXT:     fn %16 @atomic_one(%47 v: atomic @type2) -> void [linkage=external] [abi=win64(native_c) -> void];
// IR-NEXT:     fn %18 @atomic_pair(%48 v: atomic @type3) -> void [linkage=external] [abi=win64(native_c) -> void];
// IR-NEXT:     fn %20 @plain_pair(%49 v: @type3) -> void [linkage=external] [abi=win64(native_c) -> void];
// IR-NEXT:     fn %22 @atomic_five(%50 v: atomic @type4) -> void [linkage=external] [abi=win64(native_c) -> void];
// IR-NEXT:     fn %24 @atomic_wide(%51 v: atomic @type5) -> void [linkage=external] [abi=win64(native_c) -> void];
// IR-NEXT:     fn %26 @atomic_union(%52 v: atomic @type6) -> void [linkage=external] [abi=win64(native_c) -> void];
// IR-NEXT:     fn %28 @atomic_scalar(%53 v: atomic i64) -> void [linkage=external];
// IR-NEXT:     fn %29 @atomic_result() -> @type0 [linkage=external] [abi=win64() -> native_c];
// IR-NEXT:     fn %30 @atomic_result_indirect() -> @type4 [linkage=external] [abi=win64() -> native_c];
// IR-NEXT:     fn %31 @caller() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %32 a: atomic @type0 [storage=automatic];
// IR-NEXT:         let %33 a2: @type0 [storage=automatic];
// IR-NEXT:         let %34 b: atomic @type1 [storage=automatic];
// IR-NEXT:         let %35 b2: @type1 [storage=automatic];
// IR-NEXT:         let %36 c: atomic @type2 [storage=automatic];
// IR-NEXT:         let %37 d: atomic @type3 [storage=automatic];
// IR-NEXT:         let %38 d2: @type3 [storage=automatic];
// IR-NEXT:         let %39 e: atomic @type4 [storage=automatic];
// IR-NEXT:         let %40 f: atomic @type5 [storage=automatic];
// IR-NEXT:         let %41 g: atomic @type6 [storage=automatic];
// IR-NEXT:         let %42 h: atomic i64 [storage=automatic];
// IR-NEXT:         call<void, abi=win64(native_c) -> void>(%8, copy<@type0, reason=arg>(read<@type0, atomic=seq_cst>(%32)));
// IR-NEXT:         call<void, abi=win64(native_c) -> void>(%10, copy<@type0, reason=arg>(read<@type0>(%33)));
// IR-NEXT:         call<void, abi=win64(native_c) -> void>(%12, copy<@type1, reason=arg>(read<@type1, atomic=seq_cst>(%34)));
// IR-NEXT:         call<void, abi=win64(native_c) -> void>(%14, copy<@type1, reason=arg>(read<@type1>(%35)));
// IR-NEXT:         call<void, abi=win64(native_c) -> void>(%16, copy<@type2, reason=arg>(read<@type2, atomic=seq_cst>(%36)));
// IR-NEXT:         call<void, abi=win64(native_c) -> void>(%18, copy<@type3, reason=arg>(read<@type3, atomic=seq_cst>(%37)));
// IR-NEXT:         call<void, abi=win64(native_c) -> void>(%20, copy<@type3, reason=arg>(read<@type3>(%38)));
// IR-NEXT:         call<void, abi=win64(native_c) -> void>(%22, copy<@type4, reason=arg>(read<@type4, atomic=seq_cst>(%39)));
// IR-NEXT:         call<void, abi=win64(native_c) -> void>(%24, copy<@type5, reason=arg>(read<@type5, atomic=seq_cst>(%40)));
// IR-NEXT:         call<void, abi=win64(native_c) -> void>(%26, copy<@type6, reason=arg>(read<@type6, atomic=seq_cst>(%41)));
// IR-NEXT:         call<void>(%28, read<i64, atomic=seq_cst>(%42));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
