// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES SVE
typedef __SVInt32_t vector;
svint32_t identity(svint32_t value) { return value; }
svint32x2_t pair(svint32x2_t value) { return value; }
svbool_t copy(svbool_t predicate) {
  svbool_t other = predicate;
  return other;
}
vector alias(vector value) { return value; }

// SLATE-FILECHECK-BEGIN SVE
// SVE: module {
// SVE-NEXT:     target "aarch64-unknown-linux-gnu" {
// SVE-NEXT:         endian = little;
// SVE-NEXT:         pointer [size=8, align=8];
// SVE-NEXT:         stack_alignment = 16;
// SVE-NEXT:         long_double = f128;
// SVE-NEXT:         storage bool [size=1, align=1];
// SVE-NEXT:         storage i8, u8 [size=1, align=1];
// SVE-NEXT:         storage i16, u16 [size=2, align=2];
// SVE-NEXT:         storage i32, u32 [size=4, align=4];
// SVE-NEXT:         storage i64, u64 [size=8, align=8];
// SVE-NEXT:         storage i128, u128 [size=16, align=16];
// SVE-NEXT:         storage bf16 [size=2, align=2];
// SVE-NEXT:         storage f16 [size=2, align=2];
// SVE-NEXT:         storage f32 [size=4, align=4];
// SVE-NEXT:         storage f64 [size=8, align=8];
// SVE-NEXT:         storage f128 [size=16, align=16];
// SVE-NEXT:         storage d32 [size=4, align=4];
// SVE-NEXT:         storage d64 [size=8, align=8];
// SVE-NEXT:         storage d128 [size=16, align=16];
// SVE-NEXT:     }
// SVE-NEXT:     type @type[[TYPE_vector:[0-9]+]] vector = sizeless<svint32_t>;
// SVE-NEXT:     fn %[[VALUE_identity:[0-9]+]] @identity(%[[VALUE_value:[0-9]+]] value: sizeless<svint32_t>) -> sizeless<svint32_t> [linkage=external] [fallthrough=ub_if_used] {
// SVE-NEXT:         return read<sizeless<svint32_t>>(%[[VALUE_value]]);
// SVE-NEXT:     }
// SVE-NEXT:     fn %[[VALUE_pair:[0-9]+]] @pair(%[[VALUE_value_2:[0-9]+]] value: sizeless<svint32x2_t>) -> sizeless<svint32x2_t> [linkage=external] [fallthrough=ub_if_used] {
// SVE-NEXT:         return read<sizeless<svint32x2_t>>(%[[VALUE_value_2]]);
// SVE-NEXT:     }
// SVE-NEXT:     fn %[[VALUE_copy:[0-9]+]] @copy(%[[VALUE_predicate:[0-9]+]] predicate: sizeless<svbool_t>) -> sizeless<svbool_t> [linkage=external] [fallthrough=ub_if_used] {
// SVE-NEXT:         let %[[VALUE_other:[0-9]+]] other: sizeless<svbool_t> [storage=automatic] = read<sizeless<svbool_t>>(%[[VALUE_predicate]]);
// SVE-NEXT:         return read<sizeless<svbool_t>>(%[[VALUE_other]]);
// SVE-NEXT:     }
// SVE-NEXT:     fn %[[VALUE_alias:[0-9]+]] @alias(%[[VALUE_value_3:[0-9]+]] value: sizeless<svint32_t> [c="vector"]) -> sizeless<svint32_t> [linkage=external] [fallthrough=ub_if_used] [c="vector(vector)"] {
// SVE-NEXT:         return read<sizeless<svint32_t>>(%[[VALUE_value_3]]) [c="vector"];
// SVE-NEXT:     }
// SVE-NEXT: }
// SLATE-FILECHECK-END SVE
