// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir --show-metadata /Zc:preprocessor
#include <intrin.h>

unsigned __int64 __rdtscp(unsigned int *);

void nop(void) { __nop(); }
unsigned __int64 counter(unsigned int *aux) { return __rdtscp(aux); }

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "aarch64-pc-windows-msvc" {
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
// IR-NEXT:     fn %[[VALUE___nop:[0-9]+]] @__nop() -> void [linkage=external] [c="void(void)"] [c_builtin="__nop"] [c_builtin_kind="builtin"] [c_builtin_header="intrin.h"];
// IR-NEXT:     fn %[[VALUE___rdtscp:[0-9]+]] @__rdtscp(%[[VALUE0:[0-9]+]] <unnamed>: ptr<u32> [c="unsigned int *"]) -> u64 [linkage=external] [c="unsigned long long(unsigned int *)"];
// IR-NEXT:     fn %[[VALUE_nop:[0-9]+]] @nop() -> void [linkage=external] [fallthrough=ret_void] [c_storage="none"] [c_return="void"] [c="void(void)"] {
// IR-NEXT:         call<void>(%[[VALUE___nop]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_counter:[0-9]+]] @counter(%[[VALUE_aux:[0-9]+]] aux: ptr<u32> [c="unsigned int *"]) -> u64 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="unsigned long long"] [c="unsigned long long(unsigned int *)"] {
// IR-NEXT:         return call<u64>(%[[VALUE___rdtscp]], read<ptr<u32>>(%[[VALUE_aux]]));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
