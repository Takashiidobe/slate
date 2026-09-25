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

// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × unsupported in numeric IR lowering: ifunc attribute
// SLATE-FILECHECK-END DEFAULT
