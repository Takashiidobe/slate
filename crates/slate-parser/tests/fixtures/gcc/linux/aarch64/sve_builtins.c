// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES SVE
svint32_t add(svbool_t predicate, svint32_t left, svint32_t right) {
  return svadd_s32_x(predicate, left, right);
}
svbool_t first_four(void) { return svptrue_pat_b8(SV_VL4); }
void prefetch(svbool_t predicate, const int *base) { svprfb(predicate, base, SV_PLDL1KEEP); }
void clear(void) { svzero_za(); }

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
// SVE-NEXT:     type @type[[TYPE_svpattern:[0-9]+]] svpattern = enum : u32 {
// SVE-NEXT:         %[[VALUE_SV_POW2:[0-9]+]] SV_POW2 = const<i32>(0);
// SVE-NEXT:         %[[VALUE_SV_VL1:[0-9]+]] SV_VL1 = const<i32>(1);
// SVE-NEXT:         %[[VALUE_SV_VL2:[0-9]+]] SV_VL2 = const<i32>(2);
// SVE-NEXT:         %[[VALUE_SV_VL3:[0-9]+]] SV_VL3 = const<i32>(3);
// SVE-NEXT:         %[[VALUE_SV_VL4:[0-9]+]] SV_VL4 = const<i32>(4);
// SVE-NEXT:         %[[VALUE_SV_VL5:[0-9]+]] SV_VL5 = const<i32>(5);
// SVE-NEXT:         %[[VALUE_SV_VL6:[0-9]+]] SV_VL6 = const<i32>(6);
// SVE-NEXT:         %[[VALUE_SV_VL7:[0-9]+]] SV_VL7 = const<i32>(7);
// SVE-NEXT:         %[[VALUE_SV_VL8:[0-9]+]] SV_VL8 = const<i32>(8);
// SVE-NEXT:         %[[VALUE_SV_VL16:[0-9]+]] SV_VL16 = const<i32>(9);
// SVE-NEXT:         %[[VALUE_SV_VL32:[0-9]+]] SV_VL32 = const<i32>(10);
// SVE-NEXT:         %[[VALUE_SV_VL64:[0-9]+]] SV_VL64 = const<i32>(11);
// SVE-NEXT:         %[[VALUE_SV_VL128:[0-9]+]] SV_VL128 = const<i32>(12);
// SVE-NEXT:         %[[VALUE_SV_VL256:[0-9]+]] SV_VL256 = const<i32>(13);
// SVE-NEXT:         %[[VALUE_SV_MUL4:[0-9]+]] SV_MUL4 = const<i32>(29);
// SVE-NEXT:         %[[VALUE_SV_MUL3:[0-9]+]] SV_MUL3 = const<i32>(30);
// SVE-NEXT:         %[[VALUE_SV_ALL:[0-9]+]] SV_ALL = const<i32>(31);
// SVE-NEXT:     } [size=4, align=4];
// SVE-NEXT:     type @type[[TYPE_svprfop:[0-9]+]] svprfop = enum : u32 {
// SVE-NEXT:         %[[VALUE_SV_POW2]] SV_PLDL1KEEP = const<i32>(0);
// SVE-NEXT:         %[[VALUE_SV_VL1]] SV_PLDL1STRM = const<i32>(1);
// SVE-NEXT:         %[[VALUE_SV_VL2]] SV_PLDL2KEEP = const<i32>(2);
// SVE-NEXT:         %[[VALUE_SV_VL3]] SV_PLDL2STRM = const<i32>(3);
// SVE-NEXT:         %[[VALUE_SV_VL4]] SV_PLDL3KEEP = const<i32>(4);
// SVE-NEXT:         %[[VALUE_SV_VL5]] SV_PLDL3STRM = const<i32>(5);
// SVE-NEXT:         %[[VALUE_SV_VL6]] SV_PSTL1KEEP = const<i32>(8);
// SVE-NEXT:         %[[VALUE_SV_VL7]] SV_PSTL1STRM = const<i32>(9);
// SVE-NEXT:         %[[VALUE_SV_VL8]] SV_PSTL2KEEP = const<i32>(10);
// SVE-NEXT:         %[[VALUE_SV_VL16]] SV_PSTL2STRM = const<i32>(11);
// SVE-NEXT:         %[[VALUE_SV_VL32]] SV_PSTL3KEEP = const<i32>(12);
// SVE-NEXT:         %[[VALUE_SV_VL64]] SV_PSTL3STRM = const<i32>(13);
// SVE-NEXT:     } [size=4, align=4];
// SVE-NEXT:     fn %[[VALUE_svadd_s32_x:[0-9]+]] @svadd_s32_x(%[[VALUE0:[0-9]+]] <unnamed>: sizeless<svbool_t>, %[[VALUE1:[0-9]+]] <unnamed>: sizeless<svint32_t>, %[[VALUE2:[0-9]+]] <unnamed>: sizeless<svint32_t>) -> sizeless<svint32_t> [linkage=external] [memory=none];
// SVE-NEXT:     fn %[[VALUE_add:[0-9]+]] @add(%[[VALUE_predicate:[0-9]+]] predicate: sizeless<svbool_t>, %[[VALUE_left:[0-9]+]] left: sizeless<svint32_t>, %[[VALUE_right:[0-9]+]] right: sizeless<svint32_t>) -> sizeless<svint32_t> [linkage=external] [fallthrough=ub_if_used] {
// SVE-NEXT:         return call<sizeless<svint32_t>>(%[[VALUE_svadd_s32_x]], read<sizeless<svbool_t>>(%[[VALUE_predicate]]), read<sizeless<svint32_t>>(%[[VALUE_left]]), read<sizeless<svint32_t>>(%[[VALUE_right]]));
// SVE-NEXT:     }
// SVE-NEXT:     fn %[[VALUE_svptrue_pat_b8:[0-9]+]] @svptrue_pat_b8(%[[VALUE3:[0-9]+]] <unnamed>: u32) -> sizeless<svbool_t> [linkage=external] [memory=none];
// SVE-NEXT:     fn %[[VALUE_first_four:[0-9]+]] @first_four() -> sizeless<svbool_t> [linkage=external] [fallthrough=ub_if_used] {
// SVE-NEXT:         return call<sizeless<svbool_t>>(%[[VALUE_svptrue_pat_b8]], reinterpret<u32>(const<i32>(4)));
// SVE-NEXT:     }
// SVE-NEXT:     fn %[[VALUE_svprfb:[0-9]+]] @svprfb(%[[VALUE4:[0-9]+]] <unnamed>: sizeless<svbool_t>, %[[VALUE5:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE6:[0-9]+]] <unnamed>: u32) -> void [linkage=external];
// SVE-NEXT:     fn %[[VALUE_prefetch:[0-9]+]] @prefetch(%[[VALUE_predicate_2:[0-9]+]] predicate: sizeless<svbool_t>, %[[VALUE_base:[0-9]+]] base: ptr<const i32>) -> void [linkage=external] [fallthrough=ret_void] {
// SVE-NEXT:         call<void>(%[[VALUE_svprfb]], read<sizeless<svbool_t>>(%[[VALUE_predicate_2]]), pointer_cast<ptr<const void>>(read<ptr<const i32>>(%[[VALUE_base]])), reinterpret<u32>(const<i32>(0)));
// SVE-NEXT:     }
// SVE-NEXT:     fn %[[VALUE_svzero_za:[0-9]+]] @svzero_za() -> void [linkage=external];
// SVE-NEXT:     fn %[[VALUE_clear:[0-9]+]] @clear() -> void [linkage=external] [fallthrough=ret_void] {
// SVE-NEXT:         call<void>(%[[VALUE_svzero_za]]);
// SVE-NEXT:     }
// SVE-NEXT: }
// SLATE-FILECHECK-END SVE
