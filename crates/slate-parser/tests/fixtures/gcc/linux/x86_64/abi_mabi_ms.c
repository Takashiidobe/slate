#include <stdarg.h>
struct big { long a, b, c; };
int logf_(const char *format, ...);
int vlog(const char *format, va_list ap);
long take(struct big b, double d) { return b.a + (long)d; }
long __attribute__((sysv_abi)) explicit_sysv(long x) { return x; }
long __attribute__((cdecl)) explicit_cdecl(long a, long b) { return a - b; }
int va_list_size = sizeof(va_list);
int main(int argc, char **argv) { return logf_("%d", argc) + (argv != 0); }

// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES MS
// SLATE-FILECHECK-PREFIX-ARGS MS -mabi=ms -msseregparm
// SLATE-FILECHECK-DEFINES SYSV
// SLATE-FILECHECK-PREFIX-ARGS SYSV -mabi=ms -mabi=sysv

// SLATE-FILECHECK-BEGIN MS
// MS: module {
// MS-NEXT:     target "x86_64-unknown-linux-gnu" {
// MS-NEXT:         endian = little;
// MS-NEXT:         pointer [size=8, align=8];
// MS-NEXT:         stack_alignment = 16;
// MS-NEXT:         long_double = f80;
// MS-NEXT:         storage bool [size=1, align=1];
// MS-NEXT:         storage i8, u8 [size=1, align=1];
// MS-NEXT:         storage i16, u16 [size=2, align=2];
// MS-NEXT:         storage i32, u32 [size=4, align=4];
// MS-NEXT:         storage i64, u64 [size=8, align=8];
// MS-NEXT:         storage i128, u128 [size=16, align=16];
// MS-NEXT:         storage bf16 [size=2, align=2];
// MS-NEXT:         storage f16 [size=2, align=2];
// MS-NEXT:         storage f32 [size=4, align=4];
// MS-NEXT:         storage f64 [size=8, align=8];
// MS-NEXT:         storage f80 [size=16, align=16];
// MS-NEXT:         storage f128 [size=16, align=16];
// MS-NEXT:         storage d32 [size=4, align=4];
// MS-NEXT:         storage d64 [size=8, align=8];
// MS-NEXT:         storage d128 [size=16, align=16];
// MS-NEXT:     }
// MS-NEXT:     type @type[[TYPE___gnuc_va_list:[0-9]+]] __gnuc_va_list = ptr<i8>;
// MS-NEXT:     type @type[[TYPE_va_list:[0-9]+]] va_list = ptr<i8> [c="__gnuc_va_list"];
// MS-NEXT:     type @type[[TYPE_big:[0-9]+]] big = struct {
// MS-NEXT:         field0 a: i64;
// MS-NEXT:         field1 b: i64;
// MS-NEXT:         field2 c: i64;
// MS-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// MS-NEXT:     global %[[VALUE_va_list_size:[0-9]+]] va_list_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(size_of<u64, ptr<i8>>(8) [c="__size_t"])) [linkage=external];
// MS-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 100, 0]) [linkage=internal];
// MS-NEXT:     fn %[[VALUE_logf_:[0-9]+]] @logf_(%[[VALUE_format:[0-9]+]] format: ptr<const i8>, ...) -> i32 [linkage=external] [abi=win64 ms_abi(scalar) -> scalar];
// MS-NEXT:     fn %[[VALUE_vlog:[0-9]+]] @vlog(%[[VALUE_format_2:[0-9]+]] format: ptr<const i8>, %[[VALUE_ap:[0-9]+]] ap: ptr<i8> [c="va_list"]) -> i32 [linkage=external] [abi=win64 ms_abi(scalar, scalar) -> scalar] [c="int(const char *, va_list) __attribute__((ms_abi))"];
// MS-NEXT:     fn %[[VALUE_take:[0-9]+]] @take(%[[VALUE_b:[0-9]+]] b: @type[[TYPE_big]], %[[VALUE_d:[0-9]+]] d: f64) -> i64 [linkage=external] [abi=win64 ms_abi(native_c, scalar) -> scalar] [fallthrough=ub_if_used] {
// MS-NEXT:         return add<i64>(read<i64>(field0(%[[VALUE_b]])), float_to_int<i64>(read<f64>(%[[VALUE_d]])));
// MS-NEXT:     }
// MS-NEXT:     fn %[[VALUE_explicit_sysv:[0-9]+]] @explicit_sysv(%[[VALUE_x:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// MS-NEXT:         return read<i64>(%[[VALUE_x]]);
// MS-NEXT:     }
// MS-NEXT:     fn %[[VALUE_explicit_cdecl:[0-9]+]] @explicit_cdecl(%[[VALUE_a:[0-9]+]] a: i64, %[[VALUE_b_2:[0-9]+]] b: i64) -> i64 [linkage=external] [abi=win64 ms_abi(scalar, scalar) -> scalar] [fallthrough=ub_if_used] {
// MS-NEXT:         return sub<i64>(read<i64>(%[[VALUE_a]]), read<i64>(%[[VALUE_b_2]]));
// MS-NEXT:     }
// MS-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(%[[VALUE_argc:[0-9]+]] argc: i32, %[[VALUE_argv:[0-9]+]] argv: ptr<ptr<i8>>) -> i32 [linkage=external] [abi=win64 ms_abi(scalar, scalar) -> scalar] [fallthrough=ret_zero] {
// MS-NEXT:         return add<i32>(call<i32, abi=win64 ms_abi(scalar, scalar) -> scalar>(%[[VALUE_logf_]], pointer_cast<ptr<const i8>>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str]])), read<i32>(%[[VALUE_argc]])), from_bool<i32>(ne<ptr<ptr<i8>>>(read<ptr<ptr<i8>>>(%[[VALUE_argv]]), null<ptr<ptr<i8>>>)));
// MS-NEXT:     }
// MS-NEXT: }
// SLATE-FILECHECK-END MS
// SLATE-FILECHECK-BEGIN SYSV
// SYSV: module {
// SYSV-NEXT:     target "x86_64-unknown-linux-gnu" {
// SYSV-NEXT:         endian = little;
// SYSV-NEXT:         pointer [size=8, align=8];
// SYSV-NEXT:         stack_alignment = 16;
// SYSV-NEXT:         long_double = f80;
// SYSV-NEXT:         storage bool [size=1, align=1];
// SYSV-NEXT:         storage i8, u8 [size=1, align=1];
// SYSV-NEXT:         storage i16, u16 [size=2, align=2];
// SYSV-NEXT:         storage i32, u32 [size=4, align=4];
// SYSV-NEXT:         storage i64, u64 [size=8, align=8];
// SYSV-NEXT:         storage i128, u128 [size=16, align=16];
// SYSV-NEXT:         storage bf16 [size=2, align=2];
// SYSV-NEXT:         storage f16 [size=2, align=2];
// SYSV-NEXT:         storage f32 [size=4, align=4];
// SYSV-NEXT:         storage f64 [size=8, align=8];
// SYSV-NEXT:         storage f80 [size=16, align=16];
// SYSV-NEXT:         storage f128 [size=16, align=16];
// SYSV-NEXT:         storage d32 [size=4, align=4];
// SYSV-NEXT:         storage d64 [size=8, align=8];
// SYSV-NEXT:         storage d128 [size=16, align=16];
// SYSV-NEXT:     }
// SYSV-NEXT:     type @type[[TYPE___gnuc_va_list:[0-9]+]] __gnuc_va_list = va_list;
// SYSV-NEXT:     type @type[[TYPE_va_list:[0-9]+]] va_list = va_list [c="__gnuc_va_list"];
// SYSV-NEXT:     type @type[[TYPE_big:[0-9]+]] big = struct {
// SYSV-NEXT:         field0 a: i64;
// SYSV-NEXT:         field1 b: i64;
// SYSV-NEXT:         field2 c: i64;
// SYSV-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// SYSV-NEXT:     global %[[VALUE_va_list_size:[0-9]+]] va_list_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(size_of<u64, va_list>(24) [c="__size_t"])) [linkage=external];
// SYSV-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 100, 0]) [linkage=internal];
// SYSV-NEXT:     fn %[[VALUE_logf_:[0-9]+]] @logf_(%[[VALUE_format:[0-9]+]] format: ptr<const i8>, ...) -> i32 [linkage=external];
// SYSV-NEXT:     fn %[[VALUE_vlog:[0-9]+]] @vlog(%[[VALUE_format_2:[0-9]+]] format: ptr<const i8>, %[[VALUE_ap:[0-9]+]] ap: va_list [c="va_list"]) -> i32 [linkage=external] [c="int(const char *, va_list)"];
// SYSV-NEXT:     fn %[[VALUE_take:[0-9]+]] @take(%[[VALUE_b:[0-9]+]] b: @type[[TYPE_big]], %[[VALUE_d:[0-9]+]] d: f64) -> i64 [linkage=external] [abi=sysv64(native_c, scalar) -> scalar] [fallthrough=ub_if_used] {
// SYSV-NEXT:         return add<i64>(read<i64>(field0(%[[VALUE_b]])), float_to_int<i64>(read<f64>(%[[VALUE_d]])));
// SYSV-NEXT:     }
// SYSV-NEXT:     fn %[[VALUE_explicit_sysv:[0-9]+]] @explicit_sysv(%[[VALUE_x:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// SYSV-NEXT:         return read<i64>(%[[VALUE_x]]);
// SYSV-NEXT:     }
// SYSV-NEXT:     fn %[[VALUE_explicit_cdecl:[0-9]+]] @explicit_cdecl(%[[VALUE_a:[0-9]+]] a: i64, %[[VALUE_b_2:[0-9]+]] b: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// SYSV-NEXT:         return sub<i64>(read<i64>(%[[VALUE_a]]), read<i64>(%[[VALUE_b_2]]));
// SYSV-NEXT:     }
// SYSV-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(%[[VALUE_argc:[0-9]+]] argc: i32, %[[VALUE_argv:[0-9]+]] argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// SYSV-NEXT:         return add<i32>(call<i32>(%[[VALUE_logf_]], pointer_cast<ptr<const i8>>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str]])), read<i32>(%[[VALUE_argc]])), from_bool<i32>(ne<ptr<ptr<i8>>>(read<ptr<ptr<i8>>>(%[[VALUE_argv]]), null<ptr<ptr<i8>>>)));
// SYSV-NEXT:     }
// SYSV-NEXT: }
// SLATE-FILECHECK-END SYSV
