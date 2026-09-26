typedef struct gs_imager_state_s {
  struct {
    int   half_width;
    int   cap;
    float miter_limit;
  } line_params;
} gs_imager_state;
static const gs_imager_state gstate_initial = {{1}};
void gstate_path_memory(gs_imager_state *pgs) { *pgs = gstate_initial; }
int  gs_state_update_overprint(void) {
  return gstate_initial.line_params.half_width;
}

extern void abort(void);
int         main() {
  if (gs_state_update_overprint() != 1)
    abort();
  return 0;
}



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
// DEFAULT-NEXT:     type @type0 gs_imager_state_s = struct {
// DEFAULT-NEXT:         field0 line_params: @type1;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type1 = struct {
// DEFAULT-NEXT:         field0 half_width: i32;
// DEFAULT-NEXT:         field1 cap: i32;
// DEFAULT-NEXT:         field2 miter_limit: f32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type2 gs_imager_state = @type0;
// DEFAULT-NEXT:     global %3 gstate_initial: @type0 [storage=static] [const] = aggregate<@type0, zero_fill=false>(field0 = aggregate<@type1, zero_fill=true>(field0 = const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     fn %4 @gstate_path_memory(%5 pgs: ptr<@type0>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<@type0>(deref(read<ptr<@type0>>(%5)), copy<@type0, reason=assign>(read<@type0>(%3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @gs_state_update_overprint() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(field0(field0(%3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn() -> i32>(%6), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
