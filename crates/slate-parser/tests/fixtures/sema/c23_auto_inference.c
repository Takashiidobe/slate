// SLATE-FILECHECK-DEFINES VALID
// SLATE-FILECHECK-DEFINES MIXED MIXED
// SLATE-FILECHECK-DEFINES SELF SELF
// SLATE-FILECHECK-DEFINES NO_INIT NO_INIT
// SLATE-FILECHECK-DEFINES LIST LIST
// SLATE-FILECHECK-DEFINES PARAMS PARAMS
// SLATE-FILECHECK-DEFINES EXTENT EXTENT
// SLATE-FILECHECK-DEFINES TOP_ARRAY TOP_ARRAY
// SLATE-FILECHECK-DEFINES BIT_FIELD BIT_FIELD
// SLATE-FILECHECK-IR-ERROR MIXED
// SLATE-FILECHECK-IR-ERROR SELF
// SLATE-FILECHECK-IR-ERROR NO_INIT
// SLATE-FILECHECK-IR-ERROR LIST
// SLATE-FILECHECK-IR-ERROR PARAMS
// SLATE-FILECHECK-IR-ERROR EXTENT
// SLATE-FILECHECK-IR-ERROR TOP_ARRAY
// SLATE-FILECHECK-IR-ERROR BIT_FIELD
// SLATE-FILECHECK-ARGS --dump-ir -std=c23

struct S { unsigned b : 3; };
_Atomic int ai;
const int ci = 1;
int arr[3];
const int carr[2];
int g(int);
long h(long);

auto file_scope = 1.5;
static auto internal = &file_scope;
_Static_assert(_Generic(&internal, double **: 1, default: 0));

void deduce(int n, int *ip, const int *cip, struct S s) {
  int vla[n];
  auto a1 = ci;
  _Static_assert(_Generic(&a1, int *: 1, default: 0));
  auto a2 = ai;
  _Static_assert(_Generic(&a2, _Atomic int *: 1, default: 0));
  auto a3 = carr;
  _Static_assert(_Generic(&a3, const int **: 1, default: 0));
  auto a4 = g;
  _Static_assert(_Generic(&a4, int (**)(int): 1, default: 0));
  auto a5 = +s.b;
  _Static_assert(_Generic(&a5, int *: 1, default: 0));
  const auto a6 = 1;
  _Static_assert(_Generic(&a6, const int *: 1, default: 0));
  auto a7 = vla;
  auto a8 = &vla;
  auto a9 = "text";
  _Static_assert(_Generic(&a9, char **: 1, default: 0));
  static auto a10 = 2;

  const auto *p1 = ip;
  _Static_assert(_Generic(&p1, const int **: 1, default: 0));
  auto *p2 = cip;
  _Static_assert(_Generic(&p2, const int **: 1, default: 0));
  auto *const p3 = ip;
  auto **p4 = &ip;
  _Static_assert(_Generic(&p4, int ***: 1, default: 0));
  auto (*p5)(int) = g;
  auto (*p6)[3] = &arr;
  auto m1 = 1, *m2 = &m1;
  _Static_assert(_Generic(&m2, int **: 1, default: 0));
  const auto *q1 = ip, *q2 = cip;
  (void)a1, (void)a2, (void)a3, (void)a4, (void)a5, (void)a6, (void)a7;
  (void)a8, (void)a9, (void)a10, (void)p1, (void)p2, (void)p3, (void)p4;
  (void)p5, (void)p6, (void)m2, (void)q1, (void)q2;

#ifdef MIXED
  auto x = 1, y = 2.0;
#endif
#ifdef SELF
  auto z = 1 + sizeof(z);
#endif
#ifdef NO_INIT
  auto w = 1, v;
#endif
#ifdef LIST
  auto l = {1};
#endif
#ifdef PARAMS
  auto (*f)(int) = h;
#endif
#ifdef EXTENT
  auto (*e)[4] = &arr;
#endif
#ifdef TOP_ARRAY
  auto t[3] = arr;
#endif
#ifdef BIT_FIELD
  auto b = s.b;
#endif
}

// SLATE-FILECHECK-BEGIN MIXED
// MIXED: Error:   × semantic analysis failed
// MIXED: Error:
// MIXED: × invalid in this context: 'auto' deduced as different types in one
// MIXED: ╭─[tests/fixtures/sema/c23_auto_inference.c:51:3]
// MIXED: 50 │ #ifdef MIXED
// MIXED: 51 │   auto x = 1, y = 2.0;
// MIXED: ·   ────────────────────
// MIXED: 52 │ #endif
// MIXED: ╰────
// SLATE-FILECHECK-END MIXED
// SLATE-FILECHECK-BEGIN SELF
// SELF: Error:   × semantic analysis failed
// SELF: Error:
// SELF: × invalid in this context: variable declared with deduced type cannot appear
// SELF: ╭─[tests/fixtures/sema/c23_auto_inference.c:54:3]
// SELF: 53 │ #ifdef SELF
// SELF: 54 │   auto z = 1 + sizeof(z);
// SELF: ·   ───────────────────────
// SELF: 55 │ #endif
// SELF: ╰────
// SLATE-FILECHECK-END SELF
// SLATE-FILECHECK-BEGIN NO_INIT
// NO_INIT: Error:   × semantic analysis failed
// NO_INIT: Error:
// NO_INIT: × invalid in this context: declaration with deduced type requires an
// NO_INIT: ╭─[tests/fixtures/sema/c23_auto_inference.c:57:3]
// NO_INIT: 56 │ #ifdef NO_INIT
// NO_INIT: 57 │   auto w = 1, v;
// NO_INIT: ·   ──────────────
// NO_INIT: 58 │ #endif
// NO_INIT: ╰────
// SLATE-FILECHECK-END NO_INIT
// SLATE-FILECHECK-BEGIN LIST
// LIST: Error:   × semantic analysis failed
// LIST: Error:
// LIST: × invalid in this context: cannot use 'auto' with an initializer list
// LIST: ╭─[tests/fixtures/sema/c23_auto_inference.c:60:3]
// LIST: 59 │ #ifdef LIST
// LIST: 60 │   auto l = {1};
// LIST: ·   ─────────────
// LIST: 61 │ #endif
// LIST: ╰────
// SLATE-FILECHECK-END LIST
// SLATE-FILECHECK-BEGIN PARAMS
// PARAMS: Error:   × semantic analysis failed
// PARAMS: Error:
// PARAMS: × invalid in this context: initializer does not match the deduced declarator
// PARAMS: ╭─[tests/fixtures/sema/c23_auto_inference.c:63:3]
// PARAMS: 62 │ #ifdef PARAMS
// PARAMS: 63 │   auto (*f)(int) = h;
// PARAMS: ·   ───────────────────
// PARAMS: 64 │ #endif
// PARAMS: ╰────
// SLATE-FILECHECK-END PARAMS
// SLATE-FILECHECK-BEGIN EXTENT
// EXTENT: Error:   × semantic analysis failed
// EXTENT: Error:
// EXTENT: × invalid in this context: initializer does not match the deduced declarator
// EXTENT: ╭─[tests/fixtures/sema/c23_auto_inference.c:66:3]
// EXTENT: 65 │ #ifdef EXTENT
// EXTENT: 66 │   auto (*e)[4] = &arr;
// EXTENT: ·   ────────────────────
// EXTENT: 67 │ #endif
// EXTENT: ╰────
// SLATE-FILECHECK-END EXTENT
// SLATE-FILECHECK-BEGIN TOP_ARRAY
// TOP_ARRAY: Error:   × semantic analysis failed
// TOP_ARRAY: Error:
// TOP_ARRAY: × invalid in this context: initializer does not match the deduced declarator
// TOP_ARRAY: ╭─[tests/fixtures/sema/c23_auto_inference.c:69:3]
// TOP_ARRAY: 68 │ #ifdef TOP_ARRAY
// TOP_ARRAY: 69 │   auto t[3] = arr;
// TOP_ARRAY: ·   ────────────────
// TOP_ARRAY: 70 │ #endif
// TOP_ARRAY: ╰────
// SLATE-FILECHECK-END TOP_ARRAY
// SLATE-FILECHECK-BEGIN BIT_FIELD
// BIT_FIELD: Error:   × semantic analysis failed
// BIT_FIELD: Error:
// BIT_FIELD: × invalid in this context: cannot use a bit-field as a deduced-type
// BIT_FIELD: ╭─[tests/fixtures/sema/c23_auto_inference.c:72:3]
// BIT_FIELD: 71 │ #ifdef BIT_FIELD
// BIT_FIELD: 72 │   auto b = s.b;
// BIT_FIELD: ·   ─────────────
// BIT_FIELD: 73 │ #endif
// BIT_FIELD: ╰────
// SLATE-FILECHECK-END BIT_FIELD
// SLATE-FILECHECK-BEGIN VALID
// VALID: module {
// VALID-NEXT:     target "x86_64-unknown-linux-gnu" {
// VALID-NEXT:         endian = little;
// VALID-NEXT:         pointer [size=8, align=8];
// VALID-NEXT:         stack_alignment = 16;
// VALID-NEXT:         long_double = f80;
// VALID-NEXT:         storage bool [size=1, align=1];
// VALID-NEXT:         storage i8, u8 [size=1, align=1];
// VALID-NEXT:         storage i16, u16 [size=2, align=2];
// VALID-NEXT:         storage i32, u32 [size=4, align=4];
// VALID-NEXT:         storage i64, u64 [size=8, align=8];
// VALID-NEXT:         storage i128, u128 [size=16, align=16];
// VALID-NEXT:         storage bf16 [size=2, align=2];
// VALID-NEXT:         storage f16 [size=2, align=2];
// VALID-NEXT:         storage f32 [size=4, align=4];
// VALID-NEXT:         storage f64 [size=8, align=8];
// VALID-NEXT:         storage f80 [size=16, align=16];
// VALID-NEXT:         storage f128 [size=16, align=16];
// VALID-NEXT:         storage d32 [size=4, align=4];
// VALID-NEXT:         storage d64 [size=8, align=8];
// VALID-NEXT:         storage d128 [size=16, align=16];
// VALID-NEXT:     }
// VALID-NEXT:     type @type0 S = struct {
// VALID-NEXT:         field0 b: u32 : 3;
// VALID-NEXT:     } [size=4, align=4, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 1)], field_units=[Some(0)]];
// VALID-NEXT:     global %1 ai: atomic i32 [storage=static] [linkage=external];
// VALID-NEXT:     global %2 ci: i32 [storage=static] [const] = const<i32>(1) [linkage=external];
// VALID-NEXT:     global %3 arr: array<i32, 3> [storage=static] [linkage=external];
// VALID-NEXT:     global %4 carr: array<i32, 2> [storage=static] [const] [linkage=external];
// VALID-NEXT:     global %7 file_scope: f64 [storage=static] = const<f64>(1.5) [linkage=external];
// VALID-NEXT:     global %8 internal: ptr<f64> [storage=static] = addr_of<ptr<f64>>(%7) [linkage=internal];
// VALID-NEXT:     global %38 .str38: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([116, 101, 120, 116, 0]) [linkage=internal];
// VALID-NEXT:     global %24 a10: i32 [storage=static] = const<i32>(2) [linkage=internal];
// VALID-NEXT:     fn %5 @g(%35 <unnamed>: i32) -> i32 [linkage=external];
// VALID-NEXT:     fn %6 @h(%36 <unnamed>: i64) -> i64 [linkage=external];
// VALID-NEXT:     fn %9 @deduce(%10 n: i32, %11 ip: ptr<i32>, %12 cip: ptr<const i32>, %13 s: @type0) -> void [linkage=external] [abi=sysv64(scalar, scalar, scalar, coerce<i32>) -> void] [fallthrough=ret_void] {
// VALID-NEXT:         let %37: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%10)));
// VALID-NEXT:         let %14 vla: vla<i32, %37> [storage=automatic];
// VALID-NEXT:         let %15 a1: i32 [storage=automatic] = read<i32>(%2);
// VALID-NEXT:         let %16 a2: atomic i32 [storage=automatic] = read<i32, atomic=seq_cst>(%1);
// VALID-NEXT:         let %17 a3: ptr<const i32> [storage=automatic] = array_decay<ptr<const i32>, length=Some(2)>(%4);
// VALID-NEXT:         let %18 a4: ptr<fn(i32) -> i32> [storage=automatic] = function_decay<ptr<fn(i32) -> i32>>(%5);
// VALID-NEXT:         let %19 a5: i32 [storage=automatic] = reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..1, bits=0..3>(%13)));
// VALID-NEXT:         let %20 a6: i32 [storage=automatic] [const] = const<i32>(1);
// VALID-NEXT:         let %21 a7: ptr<i32> [storage=automatic] = array_decay<ptr<i32>, length=None>(%14);
// VALID-NEXT:         let %22 a8: ptr<vla<i32, %37>> [storage=automatic] = addr_of<ptr<vla<i32, %37>>>(%14);
// VALID-NEXT:         let %23 a9: ptr<i8> [storage=automatic] = array_decay<ptr<i8>, length=Some(5)>(%38);
// VALID-NEXT:         let %25 p1: ptr<const i32> [storage=automatic] = pointer_cast<ptr<const i32>, reason=assign>(read<ptr<i32>>(%11));
// VALID-NEXT:         let %26 p2: ptr<const i32> [storage=automatic] = read<ptr<const i32>>(%12);
// VALID-NEXT:         let %27 p3: ptr<i32> [storage=automatic] [const] = read<ptr<i32>>(%11);
// VALID-NEXT:         let %28 p4: ptr<ptr<i32>> [storage=automatic] = addr_of<ptr<ptr<i32>>>(%11);
// VALID-NEXT:         let %29 p5: ptr<fn(i32) -> i32> [storage=automatic] = function_decay<ptr<fn(i32) -> i32>>(%5);
// VALID-NEXT:         let %30 p6: ptr<array<i32, 3>> [storage=automatic] = addr_of<ptr<array<i32, 3>>>(%3);
// VALID-NEXT:         let %31 m1: i32 [storage=automatic] = const<i32>(1);
// VALID-NEXT:         let %32 m2: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(%31);
// VALID-NEXT:         let %33 q1: ptr<const i32> [storage=automatic] = pointer_cast<ptr<const i32>, reason=assign>(read<ptr<i32>>(%11));
// VALID-NEXT:         let %34 q2: ptr<const i32> [storage=automatic] = read<ptr<const i32>>(%12);
// VALID-NEXT:         read<i32>(%15);
// VALID-NEXT:         read<i32, atomic=seq_cst>(%16);
// VALID-NEXT:         read<ptr<const i32>>(%17);
// VALID-NEXT:         read<ptr<fn(i32) -> i32>>(%18);
// VALID-NEXT:         read<i32>(%19);
// VALID-NEXT:         read<i32>(%20);
// VALID-NEXT:         read<ptr<i32>>(%21);
// VALID-NEXT:         read<ptr<vla<i32, %37>>>(%22);
// VALID-NEXT:         read<ptr<i8>>(%23);
// VALID-NEXT:         read<i32>(%24);
// VALID-NEXT:         read<ptr<const i32>>(%25);
// VALID-NEXT:         read<ptr<const i32>>(%26);
// VALID-NEXT:         read<ptr<i32>>(%27);
// VALID-NEXT:         read<ptr<ptr<i32>>>(%28);
// VALID-NEXT:         read<ptr<fn(i32) -> i32>>(%29);
// VALID-NEXT:         read<ptr<array<i32, 3>>>(%30);
// VALID-NEXT:         read<ptr<i32>>(%32);
// VALID-NEXT:         read<ptr<const i32>>(%33);
// VALID-NEXT:         read<ptr<const i32>>(%34);
// VALID-NEXT:     }
// VALID-NEXT: }
// SLATE-FILECHECK-END VALID
