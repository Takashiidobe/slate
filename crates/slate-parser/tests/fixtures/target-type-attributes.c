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
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × unsupported in numeric IR lowering: ifunc attribute
// DEFAULT: ╭─[tests/fixtures/target-type-attributes.c:4:1]
// DEFAULT: 3 │ __attribute__((target_clones("default", "arch=x86-64-v2"))) int cloned(void);
// DEFAULT: 4 │ __attribute__((ifunc("resolver"))) int indirect(void);
// DEFAULT: · ──────────────────────────────────────────────────────
// DEFAULT: 5 │ __attribute__((dllimport)) int imported;
// DEFAULT: ╰────
// DEFAULT: ⚠ unknown attribute 'dllimport' ignored
// DEFAULT: ╭─[tests/fixtures/target-type-attributes.c:5:16]
// DEFAULT: 4 │ __attribute__((ifunc("resolver"))) int indirect(void);
// DEFAULT: 5 │ __attribute__((dllimport)) int imported;
// DEFAULT: ·                ─────────
// DEFAULT: 6 │ __attribute__((weak_import)) extern int weak_platform;
// DEFAULT: ╰────
// DEFAULT: ⚠ unknown attribute 'nomips16' ignored
// DEFAULT: ╭─[tests/fixtures/target-type-attributes.c:7:25]
// DEFAULT: 6 │ __attribute__((weak_import)) extern int weak_platform;
// DEFAULT: 7 │ __attribute__((stdcall, nomips16)) int calling_convention(void);
// DEFAULT: ·                         ────────
// DEFAULT: 8 │ __attribute__((availability(macos, introduced=12.0))) int platform_api;
// DEFAULT: ╰────
// DEFAULT: ⚠ unknown attribute 'scalar_storage_order' ignored
// DEFAULT: ╭─[tests/fixtures/target-type-attributes.c:10:16]
// DEFAULT: 9 │ typedef int vector_type __attribute__((ext_vector_type(2)));
// DEFAULT: 10 │ __attribute__((scalar_storage_order("big-endian"))) int ordered;
// DEFAULT: ·                ────────────────────
// DEFAULT: 11 │ union union_value {
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
