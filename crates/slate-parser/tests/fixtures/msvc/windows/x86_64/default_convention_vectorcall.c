// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir /std:c11 /Gv
int proto(int a);
int __stdcall explicit_std(int a);
int __fastcall explicit_fast(int a);
int var(int, ...);
int main(void);
_Static_assert(_Generic(&proto, int(__vectorcall *)(int): 1, default: 0), "proto");
_Static_assert(_Generic(&explicit_std, int(__cdecl *)(int): 1, default: 0), "std");
_Static_assert(_Generic(&explicit_fast, int(__cdecl *)(int): 1, default: 0), "fast");
_Static_assert(_Generic(&var, int(__cdecl *)(int, ...): 1, default: 0), "var");
_Static_assert(_Generic(&main, int(__cdecl *)(void): 1, default: 0), "main");

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
// IR-NEXT:     fn %[[VALUE_proto:[0-9]+]] @proto(%[[VALUE_a:[0-9]+]] a: i32) -> i32 [linkage=external] [abi=win64 vectorcall(scalar) -> scalar];
// IR-NEXT:     fn %[[VALUE_explicit_std:[0-9]+]] @explicit_std(%[[VALUE_a_2:[0-9]+]] a: i32) -> i32 [linkage=external];
// IR-NEXT:     fn %[[VALUE_explicit_fast:[0-9]+]] @explicit_fast(%[[VALUE_a_3:[0-9]+]] a: i32) -> i32 [linkage=external];
// IR-NEXT:     fn %[[VALUE_var:[0-9]+]] @var(%[[VALUE0:[0-9]+]] <unnamed>: i32, ...) -> i32 [linkage=external];
// IR-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external];
// IR-NEXT: }
// SLATE-FILECHECK-END IR
