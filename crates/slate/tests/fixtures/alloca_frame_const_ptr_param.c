typedef struct {
  int windowLog;
} Params;

static void use_params(const Params *params, unsigned *out, const char *lo,
                       const char *hi) {
  long diff = hi - lo;
  *out      = (unsigned)(params->windowLog + (int)diff);
}

int main(void) {
  Params p;
  p.windowLog = 5;
  char     buf[8];
  unsigned out = 0;
  use_params(&p, &out, buf, buf + 4);
  return (int)out;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __SlateRecord_alloca_frame_const_ptr_param_c_3 {
// CHECK-NEXT:     windowLog: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn use_params(
// CHECK-NEXT:     params: &__SlateRecord_alloca_frame_const_ptr_param_c_3,
// CHECK-NEXT:     out: &mut u32,
// CHECK-NEXT:     lo: &[i8],
// CHECK-NEXT:     hi: *const i8,
// CHECK-NEXT: ) {
// CHECK-NEXT:     let diff: i64 = (unsafe { hi.offset_from(lo.as_ptr() as *const i8) }) as i64;
// CHECK-NEXT:     *out = ((*params).windowLog + (diff as i32)) as u32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut p: __SlateRecord_alloca_frame_const_ptr_param_c_3 = unsafe {
// CHECK-NEXT:         core::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     p.windowLog = 5;
// CHECK-NEXT:     let mut buf: [i8; 8] = [0i8; 8];
// CHECK-NEXT:     let mut out: u32 = 0;
// CHECK-NEXT:     {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]] = (unsafe { (buf.as_mut_ptr() as *mut i8).add(4) }) as *const i8;
// CHECK-NEXT:         use_params(&p, &mut out, buf.as_slice(), [[T_0]])
// CHECK-NEXT:     };
// CHECK-NEXT:     out as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
