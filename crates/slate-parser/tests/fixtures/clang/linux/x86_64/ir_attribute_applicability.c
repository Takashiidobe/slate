// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir -Wno-ignored-attributes

// Applicability is a property of the pair (attribute, subject): clang applies
// a spelling on the subjects in its subject list and drops it everywhere else,
// so `used` reaches the two objects with non-local storage and not the
// automatic one. Measured against clang 22.1.8; the diagnostic the drop also
// carries lives in attribute_applicability_warnings.c.

__attribute__((used)) int used_global = 1;
__attribute__((nocommon)) int uncommon_global = 2;

int applicability(void) {
  __attribute__((used)) static int used_static = 3;
  __attribute__((used)) int used_automatic = 4;
  return used_static + used_automatic + used_global + uncommon_global;
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
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
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     global %[[VALUE_used_global:[0-9]+]] used_global: i32 [storage=static] = const<i32>(1) [linkage=external] [used];
// IR-NEXT:     global %[[VALUE_uncommon_global:[0-9]+]] uncommon_global: i32 [storage=static] = const<i32>(2) [linkage=external];
// IR-NEXT:     global %[[VALUE_used_static:[0-9]+]] used_static: i32 [storage=static] = const<i32>(3) [linkage=internal] [used];
// IR-NEXT:     fn %[[VALUE_applicability:[0-9]+]] @applicability() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_used_automatic:[0-9]+]] used_automatic: i32 [storage=automatic] = const<i32>(4);
// IR-NEXT:         return add<i32>(add<i32>(add<i32>(read<i32>(%[[VALUE_used_static]]), read<i32>(%[[VALUE_used_automatic]])), read<i32>(%[[VALUE_used_global]])), read<i32>(%[[VALUE_uncommon_global]]));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
