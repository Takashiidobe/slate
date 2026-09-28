// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir --show-metadata

struct S { int a, b; };
typedef int (__stdcall *PF)(int);
typedef int F(int);
int __stdcall sc(int a, long long b);
int __stdcall sc(int a, long long b) { return a; }
int __fastcall fc(int a, int b, int c) { return a; }
int __vectorcall vc(int a) { return a; }
int __thiscall tc(int a, int b) { return a; }
int __stdcall var(int a, ...) { return a; }
int __cdecl cd(int a) { return a; }
int f(int) __attribute__((stdcall));
int f(int a) { return a; }
__stdcall F g;
PF __stdcall ret_pf(void);
int (__fastcall *table[2])(int);
struct H { int (__stdcall *cb)(int); };
PF p = f;
int use(void) { return p(1) + fc(1, 2, 3) + var(1, 2) + table[0](1); }
_Static_assert(_Generic(&sc, int (__stdcall *)(int, long long): 1, default: 0), "");
_Static_assert(_Generic(&cd, int (*)(int): 1, default: 0), "");
_Static_assert(_Generic(&f, int (*)(int): 0, default: 1), "");
_Static_assert(_Generic(&var, int (*)(int, ...): 1, default: 0), "");
_Static_assert(!__builtin_types_compatible_p(PF, int (*)(int)), "");
_Static_assert(__builtin_types_compatible_p(__typeof__(&ret_pf), PF (__stdcall *)(void)), "");
_Static_assert(__builtin_types_compatible_p(__typeof__(g), int __stdcall (int)), "");

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "i686-unknown-linux-gnu" {
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
// IR-NEXT:     type @type0 S = struct {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:         field1 b: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type1 PF = ptr<fn stdcall(i32) -> i32> [c="int (*)(int) __attribute__((stdcall))"];
// IR-NEXT:     type @type2 F = fn(i32) -> i32 [c="int(int)"];
// IR-NEXT:     type @type3 H = struct {
// IR-NEXT:         field0 cb: ptr<fn stdcall(i32) -> i32>;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     global %23 table: array<ptr<fn fastcall(i32) -> i32>, 2> [storage=static] [linkage=external] [c="int (*[2])(int) __attribute__((fastcall))"];
// IR-NEXT:     global %25 p: ptr<fn stdcall(i32) -> i32> [storage=static] = function_decay<ptr<fn stdcall(i32) -> i32>>(%19) [linkage=external] [c="PF"] [c_canon="int (*)(int) __attribute__((stdcall))"] [typedef_chain="PF"];
// IR-NEXT:     fn %3 @sc(%4 a: i32 [c="int"], %5 b: i64 [c="long long"]) -> i32 [linkage=external] [abi=x86_cdecl stdcall(scalar, scalar) -> scalar] [fallthrough=ub_if_used] [c="int(int, long long) __attribute__((stdcall))"] [c_attributes="[CallingConvention(Stdcall)]"] {
// IR-NEXT:         return read<i32>(%4);
// IR-NEXT:     }
// IR-NEXT:     fn %6 @fc(%7 a: i32 [c="int"], %8 b: i32 [c="int"], %9 c: i32 [c="int"]) -> i32 [linkage=external] [abi=x86_cdecl fastcall(scalar, scalar, scalar) -> scalar] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(int, int, int) __attribute__((fastcall))"] [c_attributes="[CallingConvention(Fastcall)]"] {
// IR-NEXT:         return read<i32>(%7);
// IR-NEXT:     }
// IR-NEXT:     fn %10 @vc(%11 a: i32 [c="int"]) -> i32 [linkage=external] [abi=x86_cdecl vectorcall(scalar) -> scalar] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(int) __attribute__((vectorcall))"] [c_attributes="[CallingConvention(Vectorcall)]"] {
// IR-NEXT:         return read<i32>(%11);
// IR-NEXT:     }
// IR-NEXT:     fn %12 @tc(%13 a: i32 [c="int"], %14 b: i32 [c="int"]) -> i32 [linkage=external] [abi=x86_cdecl thiscall(scalar, scalar) -> scalar] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(int, int) __attribute__((thiscall))"] [c_attributes="[CallingConvention(Thiscall)]"] {
// IR-NEXT:         return read<i32>(%13);
// IR-NEXT:     }
// IR-NEXT:     fn %15 @var(%16 a: i32 [c="int"], ...) -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(int, ...)"] [c_attributes="[CallingConvention(Stdcall)]"] {
// IR-NEXT:         return read<i32>(%16);
// IR-NEXT:     }
// IR-NEXT:     fn %17 @cd(%18 a: i32 [c="int"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(int)"] [c_attributes="[CallingConvention(Cdecl)]"] {
// IR-NEXT:         return read<i32>(%18);
// IR-NEXT:     }
// IR-NEXT:     fn %19 @f(%20 a: i32 [c="int"]) -> i32 [linkage=external] [abi=x86_cdecl stdcall(scalar) -> scalar] [fallthrough=ub_if_used] [c="int(int) __attribute__((stdcall))"] [c_attributes="[CallingConvention(Stdcall)]"] {
// IR-NEXT:         return read<i32>(%20);
// IR-NEXT:     }
// IR-NEXT:     fn %21 @g(%30 <unnamed>: i32) -> i32 [linkage=external] [abi=x86_cdecl stdcall(scalar) -> scalar] [c="int(int) __attribute__((stdcall))"] [c_attributes="[CallingConvention(Stdcall)]"];
// IR-NEXT:     fn %22 @ret_pf() -> ptr<fn stdcall(i32) -> i32> [linkage=external] [abi=x86_cdecl stdcall() -> scalar] [c="PF(void) __attribute__((stdcall))"] [c_canon="int (*(void))(int) __attribute__((stdcall)) __attribute__((stdcall))"] [typedef_chain="PF"] [c_attributes="[CallingConvention(Stdcall)]"];
// IR-NEXT:     fn %26 @use() -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(void)"] {
// IR-NEXT:         return add<i32>(add<i32>(add<i32>(call<i32, abi=x86_cdecl stdcall(scalar) -> scalar>(read<ptr<fn stdcall(i32) -> i32>>(%25), const<i32>(1)), call<i32, abi=x86_cdecl fastcall(scalar, scalar, scalar) -> scalar>(%6, const<i32>(1), const<i32>(2), const<i32>(3))), call<i32>(%15, const<i32>(1), const<i32>(2))), call<i32, abi=x86_cdecl fastcall(scalar) -> scalar>(read<ptr<fn fastcall(i32) -> i32>>(deref(ptr_offset<ptr<ptr<fn fastcall(i32) -> i32>>, subtract=false>(array_decay<ptr<ptr<fn fastcall(i32) -> i32>>, length=Some(2)>(%23), const<i32>(0)))), const<i32>(1)));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
