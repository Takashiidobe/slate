// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ARGS /std:c11
const char *__stdcall name_of(int x);
const char *__stdcall name_of(int x) { return x ? "one" : "zero"; }
int **__fastcall indirect(int **p) { return p; }
char *const __stdcall qualified(void);
const char *(__stdcall *selector)(int) = name_of;
_Static_assert(_Generic(&name_of, const char *(__stdcall *)(int): 1, default: 0), "name_of");
_Static_assert(_Generic(&indirect, int **(__fastcall *)(int **): 1, default: 0), "indirect");
_Static_assert(_Generic(&qualified, char *const (__stdcall *)(void): 1, default: 0), "qualified");

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "i686-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=4, align=4];
// DEFAULT-NEXT:         stack_alignment = 4;
// DEFAULT-NEXT:         long_double = f64;
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
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([111, 110, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([122, 101, 114, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_selector:[0-9]+]] selector: ptr<fn stdcall(i32) -> ptr<const i8>> [storage=static] = function_decay<ptr<fn stdcall(i32) -> ptr<const i8>>>(%[[VALUE_name_of:[0-9]+]]) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_name_of]] @name_of(%[[VALUE_x:[0-9]+]] x: i32) -> ptr<const i8> [linkage=external] [abi=x86_win32 stdcall(scalar) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return pointer_cast<ptr<const i8>, reason=return>(conditional<ptr<i8>>(ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(0)), array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str]]), array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_2]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_indirect:[0-9]+]] @indirect(%[[VALUE_p:[0-9]+]] p: ptr<ptr<i32>>) -> ptr<ptr<i32>> [linkage=external] [abi=x86_win32 fastcall(scalar) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<ptr<ptr<i32>>>(%[[VALUE_p]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_qualified:[0-9]+]] @qualified() -> ptr<i8> [linkage=external] [abi=x86_win32 stdcall() -> scalar];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
