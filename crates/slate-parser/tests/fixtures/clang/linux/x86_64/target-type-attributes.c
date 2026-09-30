__attribute__((cpu_dispatch(generic, haswell))) int dispatched(void);
__attribute__((cpu_specific(haswell))) int specific(void);
__attribute__((target_clones("default", "arch=x86-64-v2"))) int cloned(void);
__attribute__((ifunc("resolver"))) int indirect(void);
__attribute__((dllimport)) int imported;
__attribute__((weak_import)) extern int weak_platform;
__attribute__((stdcall, nomips16)) int calling_convention(void);
__attribute__((availability(macos, introduced=12.0))) int platform_api;
typedef int vector_type __attribute__((ext_vector_type(2)));
__attribute__((scalar_storage_order("big-endian"))) int ordered;
union union_value {
  int value;
} __attribute__((transparent_union));
struct __attribute__((ms_struct)) ms_platform_struct {
  int value;
};
struct __attribute__((gcc_struct)) gcc_platform_struct {
  int value;
};
__attribute__((format(printf, 1, 2))) int formatted(char *format, ...);
__attribute__((format_arg(1))) char *format_argument(char *value);
__attribute__((common, nocommon)) int common_value;

// SLATE-FILECHECK-DEFINES DEFAULT

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
// DEFAULT-NEXT:     type @type[[TYPE_vector_type:[0-9]+]] vector_type = vector<i32, 2>;
// DEFAULT-NEXT:     type @type[[TYPE_union_value:[0-9]+]] union_value = union {
// DEFAULT-NEXT:         field0 value: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_ms_platform_struct:[0-9]+]] ms_platform_struct = struct {
// DEFAULT-NEXT:         field0 value: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_gcc_platform_struct:[0-9]+]] gcc_platform_struct = struct {
// DEFAULT-NEXT:         field0 value: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_imported:[0-9]+]] imported: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_weak_platform:[0-9]+]] weak_platform: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_platform_api:[0-9]+]] platform_api: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ordered:[0-9]+]] ordered: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_common_value:[0-9]+]] common_value: i32 [storage=static] [linkage=external] [common];
// DEFAULT-NEXT:     fn %[[VALUE_dispatched:[0-9]+]] @dispatched() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_specific:[0-9]+]] @specific() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_cloned:[0-9]+]] @cloned() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_indirect:[0-9]+]] @indirect() -> i32 [linkage=external] [ifunc="resolver"];
// DEFAULT-NEXT:     fn %[[VALUE_calling_convention:[0-9]+]] @calling_convention() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_formatted:[0-9]+]] @formatted(%[[VALUE_format:[0-9]+]] format: ptr<i8>, ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_format_argument:[0-9]+]] @format_argument(%[[VALUE_value:[0-9]+]] value: ptr<i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
