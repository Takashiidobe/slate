// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir /std:c11 /Gz
typedef int fn_t(int);
int proto(int a);
int knr();
int kdef(a) int a; { return a; }
int var(int a, ...);
int __cdecl explicit_cdecl(int a);
int __fastcall explicit_fast(int a);
fn_t typedefd;
int main(void);
_Static_assert(_Generic(&proto, int(__stdcall *)(int): 1, default: 0), "proto");
_Static_assert(_Generic(&knr, int(__stdcall *)(): 1, default: 0), "knr");
_Static_assert(_Generic(&var, int(__cdecl *)(int, ...): 1, default: 0), "var");
_Static_assert(_Generic(&explicit_cdecl, int(__cdecl *)(int): 1, default: 0), "cdecl");
_Static_assert(_Generic(&explicit_fast, int(__fastcall *)(int): 1, default: 0), "fast");
_Static_assert(_Generic(&typedefd, int(__stdcall *)(int): 1, default: 0), "typedef");
_Static_assert(_Generic(&main, int(__cdecl *)(void): 1, default: 0), "main");
int (*fp)(int);
_Static_assert(_Generic(fp, int(__stdcall *)(int): 1, default: 0), "fp");
int use(void) { return implicit(1); }
int main(void) { return 0; }

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
// IR-NEXT:     type @type[[TYPE_fn_t:[0-9]+]] fn_t = fn stdcall(i32) -> i32;
// IR-NEXT:     global %[[VALUE_fp:[0-9]+]] fp: ptr<fn stdcall(i32) -> i32> [storage=static] [linkage=external];
// IR-NEXT:     fn %[[VALUE_proto:[0-9]+]] @proto(%[[VALUE_a:[0-9]+]] a: i32) -> i32 [linkage=external] [abi=x86_win32 stdcall(scalar) -> scalar];
// IR-NEXT:     fn %[[VALUE_knr:[0-9]+]] @knr(unprototyped) -> i32 [linkage=external] [abi=x86_win32 stdcall() -> scalar];
// IR-NEXT:     fn %[[VALUE_kdef:[0-9]+]] @kdef(%[[VALUE_a_2:[0-9]+]] a: i32) -> i32 [linkage=external] [abi=x86_win32 stdcall(scalar) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<i32>(%[[VALUE_a_2]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_var:[0-9]+]] @var(%[[VALUE_a_3:[0-9]+]] a: i32, ...) -> i32 [linkage=external];
// IR-NEXT:     fn %[[VALUE_explicit_cdecl:[0-9]+]] @explicit_cdecl(%[[VALUE_a_4:[0-9]+]] a: i32) -> i32 [linkage=external];
// IR-NEXT:     fn %[[VALUE_explicit_fast:[0-9]+]] @explicit_fast(%[[VALUE_a_5:[0-9]+]] a: i32) -> i32 [linkage=external] [abi=x86_win32 fastcall(scalar) -> scalar];
// IR-NEXT:     fn %[[VALUE_typedefd:[0-9]+]] @typedefd(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external] [abi=x86_win32 stdcall(scalar) -> scalar];
// IR-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// IR-NEXT:         return const<i32>(0);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_implicit:[0-9]+]] @implicit(unprototyped) -> i32 [linkage=external] [abi=x86_win32 stdcall() -> scalar];
// IR-NEXT:     fn %[[VALUE_use:[0-9]+]] @use() -> i32 [linkage=external] [abi=x86_win32 stdcall() -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, abi=x86_win32 stdcall(scalar) -> scalar>(%[[VALUE_implicit]], const<i32>(1));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
