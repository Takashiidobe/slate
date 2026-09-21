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
// IR-NEXT:     target "i386-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=4, align=4];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=4];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=4];
// IR-NEXT:         storage f80 [size=12, align=4];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type0 pair = struct {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:         field1 b: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type1 one = struct {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type2 wide = struct {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:         field1 b: i32;
// IR-NEXT:         field2 c: i32;
// IR-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// IR-NEXT:     type @type3 wrapped = struct {
// IR-NEXT:         field0 bytes: array<i8, 8>;
// IR-NEXT:     } [size=8, align=1, offsets=[0]];
// IR-NEXT:     type @type4 choice = union {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:         field1 b: f32;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// IR-NEXT:     fn %5 @atomic_pair(%26 v: atomic @type0) -> void [linkage=external] [abi=x86_cdecl(byval<align=4>) -> void];
// IR-NEXT:     fn %6 @plain_pair(%27 v: @type0) -> void [linkage=external] [abi=x86_cdecl(coerce<i32, i32>) -> void];
// IR-NEXT:     fn %7 @atomic_one(%28 v: atomic @type1) -> void [linkage=external] [abi=x86_cdecl(byval<align=4>) -> void];
// IR-NEXT:     fn %8 @plain_one(%29 v: @type1) -> void [linkage=external] [abi=x86_cdecl(coerce<i32>) -> void];
// IR-NEXT:     fn %9 @atomic_wide(%30 v: atomic @type2) -> void [linkage=external] [abi=x86_cdecl(byval<align=4>) -> void];
// IR-NEXT:     fn %10 @atomic_wrapped(%31 v: atomic @type3) -> void [linkage=external] [abi=x86_cdecl(byval<align=4>) -> void];
// IR-NEXT:     fn %11 @atomic_union(%32 v: atomic @type4) -> void [linkage=external] [abi=x86_cdecl(byval<align=4>) -> void];
// IR-NEXT:     fn %12 @atomic_complex(%33 v: atomic complex<f64>) -> void [linkage=external] [abi=x86_cdecl(byval<align=4>) -> void];
// IR-NEXT:     fn %13 @atomic_scalar(%34 v: atomic i64) -> void [linkage=external];
// IR-NEXT:     fn %14 @atomic_result() -> @type0 [linkage=external] [abi=x86_cdecl() -> sret<align=8>];
// IR-NEXT:     fn %15 @plain_result() -> @type0 [linkage=external] [abi=x86_cdecl() -> sret<align=4>];
// IR-NEXT:     fn %16 @caller() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %17 a: atomic @type0 [storage=automatic];
// IR-NEXT:         let %18 b: @type0 [storage=automatic];
// IR-NEXT:         let %19 c: atomic @type1 [storage=automatic];
// IR-NEXT:         let %20 d: @type1 [storage=automatic];
// IR-NEXT:         let %21 e: atomic @type2 [storage=automatic];
// IR-NEXT:         let %22 f: atomic @type3 [storage=automatic];
// IR-NEXT:         let %23 g: atomic @type4 [storage=automatic];
// IR-NEXT:         let %24 h: atomic complex<f64> [storage=automatic];
// IR-NEXT:         let %25 i: atomic i64 [storage=automatic];
// IR-NEXT:         call<void, signature=fn(@type0) -> void, abi=x86_cdecl(byval<align=4>) -> void>(%5, copy<@type0, reason=arg>(read<@type0, atomic=seq_cst>(%17)));
// IR-NEXT:         call<void, signature=fn(@type0) -> void, abi=x86_cdecl(coerce<i32, i32>) -> void>(%6, copy<@type0, reason=arg>(read<@type0>(%18)));
// IR-NEXT:         call<void, signature=fn(@type1) -> void, abi=x86_cdecl(byval<align=4>) -> void>(%7, copy<@type1, reason=arg>(read<@type1, atomic=seq_cst>(%19)));
// IR-NEXT:         call<void, signature=fn(@type1) -> void, abi=x86_cdecl(coerce<i32>) -> void>(%8, copy<@type1, reason=arg>(read<@type1>(%20)));
// IR-NEXT:         call<void, signature=fn(@type2) -> void, abi=x86_cdecl(byval<align=4>) -> void>(%9, copy<@type2, reason=arg>(read<@type2, atomic=seq_cst>(%21)));
// IR-NEXT:         call<void, signature=fn(@type3) -> void, abi=x86_cdecl(byval<align=4>) -> void>(%10, copy<@type3, reason=arg>(read<@type3, atomic=seq_cst>(%22)));
// IR-NEXT:         call<void, signature=fn(@type4) -> void, abi=x86_cdecl(byval<align=4>) -> void>(%11, copy<@type4, reason=arg>(read<@type4, atomic=seq_cst>(%23)));
// IR-NEXT:         call<void, signature=fn(complex<f64>) -> void, abi=x86_cdecl(byval<align=4>) -> void>(%12, read<complex<f64>, atomic=seq_cst>(%24));
// IR-NEXT:         call<void, signature=fn(i64) -> void>(%13, read<i64, atomic=seq_cst>(%25));
// IR-NEXT:         call<@type0, signature=fn() -> @type0, abi=x86_cdecl() -> sret<align=8>>(%14);
// IR-NEXT:         call<@type0, signature=fn() -> @type0, abi=x86_cdecl() -> sret<align=4>>(%15);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
