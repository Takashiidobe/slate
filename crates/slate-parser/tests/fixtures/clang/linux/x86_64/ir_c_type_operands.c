// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ARGS --dump-ir --show-metadata
// SLATE-FILECHECK-STD DEFAULT c23

static_assert(_Generic(0L, long: 1, long long: 0));
static_assert(_Generic(0LL, long: 0, long long: 1));
static_assert(_Generic(1 < 2, int: 1, _Bool: 0));
static_assert(_Generic(!0, int: 1, _Bool: 0));
static_assert(_Generic(1 && 2, int: 1, _Bool: 0));
static_assert(_Generic((char)0, char: 1, signed char: 0, unsigned char: 0));
static_assert(_Generic(sizeof(int), unsigned long: 1, default: 0));
static_assert(_Generic(1L + 1LL, long long: 1, default: 0));
static_assert(_Generic((unsigned long)0 + (long long)0, unsigned long long: 1, default: 0));
static_assert(_Generic((unsigned _BitInt(64))0 + (long long)0, unsigned long long: 1, default: 0));

typeof(1L + 1LL) mixed_rank;
typeof(1L + 1u) signed_long;
typeof(1L < 1LL) comparison;
typeof(sizeof(int)) object_size;
const int qualified = 1;
typeof(&qualified) qualified_pointer;
struct Record { long long member; const int immutable; };

int identities(long l, long long ll, char c, signed char sc, int a, int b) {
    return _Generic(l + ll, long: 0, long long: 1)
        + _Generic(c, char: 1, signed char: 0)
        + _Generic(sc, char: 0, signed char: 1)
        + _Generic(a < b, int: 1, _Bool: 0)
        + _Generic(a && b, int: 1, _Bool: 0)
        + _Generic(!a, int: 1, _Bool: 0);
}

long long members(const struct Record *r, long l) {
    typeof(r->member + l) sum = r->member + l;
    typeof(&r->immutable) address = &r->immutable;
    return sum + *address;
}

long difference(const int *a, int *b) { return a - b; }
int truth_size(int a, int b) { return sizeof(a < b); }
int variadic(int tag, ...);
int half_argument(_Float16 value) { return variadic(0, value); }
typedef float float4 __attribute__((vector_size(16)));
typedef int int4 __attribute__((vector_size(16)));
int vector_mask(float4 a, float4 b) { return _Generic(a < b, int4: 1, default: 0); }
int array_compatibility(int (*p)[3]) { return _Generic(p, int (*)[]: 1, default: 0); }
int forwarded_truth(int a, int b) { return (a, a < b) && (b, a > b); }

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type[[TYPE_Record:[0-9]+]] Record = struct {
// DEFAULT-NEXT:         field0 member: i64;
// DEFAULT-NEXT:         field1 immutable: const i32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_float4:[0-9]+]] float4 = vector<f32, 4> [c="float __attribute__((vector_size(16)))"];
// DEFAULT-NEXT:     type @type[[TYPE_int4:[0-9]+]] int4 = vector<i32, 4> [c="int __attribute__((vector_size(16)))"];
// DEFAULT-NEXT:     global %[[VALUE_mixed_rank:[0-9]+]] mixed_rank: i64 [storage=static] [linkage=external] [c="typeof(1L + 1LL)"] [c_canon="long long"];
// DEFAULT-NEXT:     global %[[VALUE_signed_long:[0-9]+]] signed_long: i64 [storage=static] [linkage=external] [c="typeof(1L + 1u)"] [c_canon="long"];
// DEFAULT-NEXT:     global %[[VALUE_comparison:[0-9]+]] comparison: i32 [storage=static] [linkage=external] [c="typeof(1L < 1LL)"] [c_canon="int"];
// DEFAULT-NEXT:     global %[[VALUE_object_size:[0-9]+]] object_size: u64 [storage=static] [linkage=external] [c="typeof(sizeof(...))"] [c_canon="unsigned long"];
// DEFAULT-NEXT:     global %[[VALUE_qualified:[0-9]+]] qualified: i32 [storage=static] [const] = const<i32>(1) [linkage=external] [c="const int"] [c_const="true"];
// DEFAULT-NEXT:     global %[[VALUE_qualified_pointer:[0-9]+]] qualified_pointer: ptr<const i32> [storage=static] [linkage=external] [c="typeof(&qualified)"] [c_canon="const int *"];
// DEFAULT-NEXT:     fn %[[VALUE_identities:[0-9]+]] @identities(%[[VALUE_l:[0-9]+]] l: i64 [c="long"], %[[VALUE_ll:[0-9]+]] ll: i64 [c="long long"], %[[VALUE_c:[0-9]+]] c: i8 [c="char"], %[[VALUE_sc:[0-9]+]] sc: i8 [c="signed char"], %[[VALUE_a:[0-9]+]] a: i32 [c="int"], %[[VALUE_b:[0-9]+]] b: i32 [c="int"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(long, long long, char, signed char, int, int)"] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_members:[0-9]+]] @members(%[[VALUE_r:[0-9]+]] r: ptr<const @type[[TYPE_Record]]> [c="const struct Record *"], %[[VALUE_l_2:[0-9]+]] l: i64 [c="long"]) -> i64 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="long long"] [c="long long(const struct Record *, long)"] {
// DEFAULT-NEXT:         let %[[VALUE_sum:[0-9]+]] sum: i64 [storage=automatic] = add<i64, overflow=ub>(read<i64>(field0(deref(read<ptr<const @type[[TYPE_Record]]>>(%[[VALUE_r]])))), read<i64>(%[[VALUE_l_2]])) [c="typeof(r->member + l)"] [c_canon="long long"];
// DEFAULT-NEXT:         let %[[VALUE_address:[0-9]+]] address: ptr<const i32> [storage=automatic] = addr_of<ptr<const i32>>(field1(deref(read<ptr<const @type[[TYPE_Record]]>>(%[[VALUE_r]])))) [c="typeof(&r->immutable)"] [c_canon="const int *"];
// DEFAULT-NEXT:         return add<i64, overflow=ub>(read<i64>(%[[VALUE_sum]]), widen<i64, reason=usual_arith>(read<i32>(deref(read<ptr<const i32>>(%[[VALUE_address]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_difference:[0-9]+]] @difference(%[[VALUE_a_2:[0-9]+]] a: ptr<const i32> [c="const int *"], %[[VALUE_b_2:[0-9]+]] b: ptr<i32> [c="int *"]) -> i64 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="long"] [c="long(const int *, int *)"] {
// DEFAULT-NEXT:         return ptr_diff<i64, element=i32, same_array=required, overflow=ub>(read<ptr<const i32>>(%[[VALUE_a_2]]), read<ptr<i32>>(%[[VALUE_b_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_truth_size:[0-9]+]] @truth_size(%[[VALUE_a_3:[0-9]+]] a: i32 [c="int"], %[[VALUE_b_3:[0-9]+]] b: i32 [c="int"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(int, int)"] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=always>(const<u64>(4) [size_of="i32"]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_variadic:[0-9]+]] @variadic(%[[VALUE_tag:[0-9]+]] tag: i32 [c="int"], ...) -> i32 [linkage=external] [c="int(int, ...)"];
// DEFAULT-NEXT:     fn %[[VALUE_half_argument:[0-9]+]] @half_argument(%[[VALUE_value:[0-9]+]] value: f16 [c="_Float16"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(_Float16)"] {
// DEFAULT-NEXT:         return call<i32, signature=fn(i32, ...) -> i32>(%[[VALUE_variadic]], const<i32>(0), read<f16>(%[[VALUE_value]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_vector_mask:[0-9]+]] @vector_mask(%[[VALUE_a_4:[0-9]+]] a: vector<f32, 4> [c="float4"] [c_canon="float __attribute__((vector_size(16)))"] [typedef_chain="float4"], %[[VALUE_b_4:[0-9]+]] b: vector<f32, 4> [c="float4"] [c_canon="float __attribute__((vector_size(16)))"] [typedef_chain="float4"]) -> i32 [linkage=external] [abi=sysv64(direct, direct) -> scalar] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(float4, float4)"] [c_canon="int(float __attribute__((vector_size(16))), float __attribute__((vector_size(16))))"] {
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_array_compatibility:[0-9]+]] @array_compatibility(%[[VALUE_p:[0-9]+]] p: ptr<array<i32, 3>> [c="int (*)[3]"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(int (*)[3])"] {
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_forwarded_truth:[0-9]+]] @forwarded_truth(%[[VALUE_a_5:[0-9]+]] a: i32 [c="int"], %[[VALUE_b_5:[0-9]+]] b: i32 [c="int"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(int, int)"] {
// DEFAULT-NEXT:         read<i32>(%[[VALUE_a_5]]);
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%[[VALUE_a_5]]), read<i32>(%[[VALUE_b_5]]))
// DEFAULT-NEXT:             read<i32>(%[[VALUE_b_5]]);
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], gt<i32>(read<i32>(%[[VALUE_a_5]]), read<i32>(%[[VALUE_b_5]])));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], const<bool>(false));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(read<bool>(%[[VALUE0]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
