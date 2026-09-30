// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

struct pair { int a; int b; };
struct one { int a; };
struct wide { long a, b, c; };
struct wrapped { char bytes[8]; };
union choice { int a; float b; };

void atomic_pair(_Atomic struct pair v);
void plain_pair(struct pair v);
void atomic_one(_Atomic struct one v);
void plain_one(struct one v);
void atomic_wide(_Atomic struct wide v);
void atomic_wrapped(_Atomic struct wrapped v);
void atomic_union(_Atomic union choice v);
void atomic_complex(_Atomic double _Complex v);
void atomic_scalar(_Atomic long long v);

_Atomic struct pair atomic_result(void);
struct pair plain_result(void);

void caller(void) {
  _Atomic struct pair a;
  struct pair b;
  _Atomic struct one c;
  struct one d;
  _Atomic struct wide e;
  _Atomic struct wrapped f;
  _Atomic union choice g;
  _Atomic double _Complex h;
  _Atomic long long i;
  atomic_pair(a);
  plain_pair(b);
  atomic_one(c);
  plain_one(d);
  atomic_wide(e);
  atomic_wrapped(f);
  atomic_union(g);
  atomic_complex(h);
  atomic_scalar(i);
  atomic_result();
  plain_result();
}

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
// IR-NEXT:     type @type[[TYPE_pair:[0-9]+]] pair = struct {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:         field1 b: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type[[TYPE_one:[0-9]+]] one = struct {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_wide:[0-9]+]] wide = struct {
// IR-NEXT:         field0 a: i64;
// IR-NEXT:         field1 b: i64;
// IR-NEXT:         field2 c: i64;
// IR-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// IR-NEXT:     type @type[[TYPE_wrapped:[0-9]+]] wrapped = struct {
// IR-NEXT:         field0 bytes: array<u8, 8>;
// IR-NEXT:     } [size=8, align=1, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_choice:[0-9]+]] choice = union {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:         field1 b: f32;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// IR-NEXT:     fn %[[VALUE_atomic_pair:[0-9]+]] @atomic_pair(%[[VALUE_v:[0-9]+]] v: atomic @type[[TYPE_pair]]) -> void [linkage=external] [abi=aapcs64(coerce<i64>) -> void];
// IR-NEXT:     fn %[[VALUE_plain_pair:[0-9]+]] @plain_pair(%[[VALUE_v_2:[0-9]+]] v: @type[[TYPE_pair]]) -> void [linkage=external] [abi=aapcs64(native_c) -> void];
// IR-NEXT:     fn %[[VALUE_atomic_one:[0-9]+]] @atomic_one(%[[VALUE_v_3:[0-9]+]] v: atomic @type[[TYPE_one]]) -> void [linkage=external] [abi=aapcs64(coerce<i32>) -> void];
// IR-NEXT:     fn %[[VALUE_plain_one:[0-9]+]] @plain_one(%[[VALUE_v_4:[0-9]+]] v: @type[[TYPE_one]]) -> void [linkage=external] [abi=aapcs64(native_c) -> void];
// IR-NEXT:     fn %[[VALUE_atomic_wide:[0-9]+]] @atomic_wide(%[[VALUE_v_5:[0-9]+]] v: atomic @type[[TYPE_wide]]) -> void [linkage=external] [abi=aapcs64(byref<align=8>) -> void];
// IR-NEXT:     fn %[[VALUE_atomic_wrapped:[0-9]+]] @atomic_wrapped(%[[VALUE_v_6:[0-9]+]] v: atomic @type[[TYPE_wrapped]]) -> void [linkage=external] [abi=aapcs64(coerce<i64>) -> void];
// IR-NEXT:     fn %[[VALUE_atomic_union:[0-9]+]] @atomic_union(%[[VALUE_v_7:[0-9]+]] v: atomic @type[[TYPE_choice]]) -> void [linkage=external] [abi=aapcs64(coerce<i32>) -> void];
// IR-NEXT:     fn %[[VALUE_atomic_complex:[0-9]+]] @atomic_complex(%[[VALUE_v_8:[0-9]+]] v: atomic complex<f64>) -> void [linkage=external] [abi=aapcs64(coerce<i128>) -> void];
// IR-NEXT:     fn %[[VALUE_atomic_scalar:[0-9]+]] @atomic_scalar(%[[VALUE_v_9:[0-9]+]] v: atomic i64) -> void [linkage=external];
// IR-NEXT:     fn %[[VALUE_atomic_result:[0-9]+]] @atomic_result() -> @type[[TYPE_pair]] [linkage=external] [abi=aapcs64() -> coerce<i64>];
// IR-NEXT:     fn %[[VALUE_plain_result:[0-9]+]] @plain_result() -> @type[[TYPE_pair]] [linkage=external] [abi=aapcs64() -> native_c];
// IR-NEXT:     fn %[[VALUE_caller:[0-9]+]] @caller() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %[[VALUE_a:[0-9]+]] a: atomic @type[[TYPE_pair]] [storage=automatic];
// IR-NEXT:         let %[[VALUE_b:[0-9]+]] b: @type[[TYPE_pair]] [storage=automatic];
// IR-NEXT:         let %[[VALUE_c:[0-9]+]] c: atomic @type[[TYPE_one]] [storage=automatic];
// IR-NEXT:         let %[[VALUE_d:[0-9]+]] d: @type[[TYPE_one]] [storage=automatic];
// IR-NEXT:         let %[[VALUE_e:[0-9]+]] e: atomic @type[[TYPE_wide]] [storage=automatic];
// IR-NEXT:         let %[[VALUE_f:[0-9]+]] f: atomic @type[[TYPE_wrapped]] [storage=automatic];
// IR-NEXT:         let %[[VALUE_g:[0-9]+]] g: atomic @type[[TYPE_choice]] [storage=automatic];
// IR-NEXT:         let %[[VALUE_h:[0-9]+]] h: atomic complex<f64> [storage=automatic];
// IR-NEXT:         let %[[VALUE_i:[0-9]+]] i: atomic i64 [storage=automatic];
// IR-NEXT:         call<void, signature=fn(@type[[TYPE_pair]]) -> void, abi=aapcs64(coerce<i64>) -> void>(%[[VALUE_atomic_pair]], copy<@type[[TYPE_pair]], reason=arg>(read<@type[[TYPE_pair]], atomic=seq_cst>(%[[VALUE_a]])));
// IR-NEXT:         call<void, signature=fn(@type[[TYPE_pair]]) -> void, abi=aapcs64(native_c) -> void>(%[[VALUE_plain_pair]], copy<@type[[TYPE_pair]], reason=arg>(read<@type[[TYPE_pair]]>(%[[VALUE_b]])));
// IR-NEXT:         call<void, signature=fn(@type[[TYPE_one]]) -> void, abi=aapcs64(coerce<i32>) -> void>(%[[VALUE_atomic_one]], copy<@type[[TYPE_one]], reason=arg>(read<@type[[TYPE_one]], atomic=seq_cst>(%[[VALUE_c]])));
// IR-NEXT:         call<void, signature=fn(@type[[TYPE_one]]) -> void, abi=aapcs64(native_c) -> void>(%[[VALUE_plain_one]], copy<@type[[TYPE_one]], reason=arg>(read<@type[[TYPE_one]]>(%[[VALUE_d]])));
// IR-NEXT:         call<void, signature=fn(@type[[TYPE_wide]]) -> void, abi=aapcs64(byref<align=8>) -> void>(%[[VALUE_atomic_wide]], copy<@type[[TYPE_wide]], reason=arg>(read<@type[[TYPE_wide]], atomic=seq_cst>(%[[VALUE_e]])));
// IR-NEXT:         call<void, signature=fn(@type[[TYPE_wrapped]]) -> void, abi=aapcs64(coerce<i64>) -> void>(%[[VALUE_atomic_wrapped]], copy<@type[[TYPE_wrapped]], reason=arg>(read<@type[[TYPE_wrapped]], atomic=seq_cst>(%[[VALUE_f]])));
// IR-NEXT:         call<void, signature=fn(@type[[TYPE_choice]]) -> void, abi=aapcs64(coerce<i32>) -> void>(%[[VALUE_atomic_union]], copy<@type[[TYPE_choice]], reason=arg>(read<@type[[TYPE_choice]], atomic=seq_cst>(%[[VALUE_g]])));
// IR-NEXT:         call<void, signature=fn(complex<f64>) -> void, abi=aapcs64(coerce<i128>) -> void>(%[[VALUE_atomic_complex]], read<complex<f64>, atomic=seq_cst>(%[[VALUE_h]]));
// IR-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_atomic_scalar]], read<i64, atomic=seq_cst>(%[[VALUE_i]]));
// IR-NEXT:         call<@type[[TYPE_pair]], signature=fn() -> @type[[TYPE_pair]], abi=aapcs64() -> coerce<i64>>(%[[VALUE_atomic_result]]);
// IR-NEXT:         call<@type[[TYPE_pair]], signature=fn() -> @type[[TYPE_pair]], abi=aapcs64() -> native_c>(%[[VALUE_plain_result]]);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
