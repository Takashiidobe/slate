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

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct gs_imager_state_s {
// CHECK-NEXT:     line_params: [[SLATERECORD_0:__SlateRecord[0-9]+]],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0]] {
// CHECK-NEXT:     half_width: i32,
// CHECK-NEXT:     cap: i32,
// CHECK-NEXT:     miter_limit: f32,
// CHECK-NEXT: }
// CHECK-NEXT: static mut gstate_initial: gs_imager_state_s = gs_imager_state_s {
// CHECK-NEXT:     line_params: {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].half_width = 1 as i32;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     },
// CHECK-NEXT: };
// CHECK-NEXT: fn gstate_path_memory(mut pgs: &mut gs_imager_state_s) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(pgs as *mut gs_imager_state_s) = unsafe { gstate_initial };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn gs_state_update_overprint() -> i32 {
// CHECK-NEXT:     unsafe { gstate_initial.line_params.half_width }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if gs_state_update_overprint() != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
