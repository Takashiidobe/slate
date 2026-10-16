void abort(void);
void exit(int);

typedef struct _RenderInfo RenderInfo;
struct _RenderInfo {
  int   y;
  float scaley;
  int   src_y;
};

static void bar(void) {}

static int render_image_rgb_a(RenderInfo *info) {
  int   y, ye;
  float error;
  float step;

  y  = info->y;
  ye = 256;

  step = 1.0 / info->scaley;

  error  = y * step;
  error -= ((int)error) - step;

  for (; y < ye; y++) {
    if (error >= 1.0) {
      info->src_y += (int)error;
      error       -= (int)error;
      bar();
    }
    error += step;
  }
  return info->src_y;
}

int main(void) {
  RenderInfo info;

  info.y      = 0;
  info.src_y  = 0;
  info.scaley = 1.0;

  if (render_image_rgb_a(&info) != 256)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _RenderInfo {
// CHECK-NEXT:     y: i32,
// CHECK-NEXT:     scaley: f32,
// CHECK-NEXT:     src_y: i32,
// CHECK-NEXT: }
// CHECK-NEXT: fn bar() {}
// CHECK-NEXT: fn render_image_rgb_a(mut info: &mut _RenderInfo) -> i32 {
// CHECK-NEXT:     let mut y: i32 = unsafe { (*(info as *mut _RenderInfo)).y };
// CHECK-NEXT:     let mut ye: i32 = 256 as i32;
// CHECK-NEXT:     let mut error: f32 = 0 as f32;
// CHECK-NEXT:     let mut step: f32 = (1.0f64
// CHECK-NEXT:         / ((unsafe { (*(info as *mut _RenderInfo)).scaley }) as f64)) as f32;
// CHECK-NEXT:     error = (y as f32) * step;
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: f32 = error;
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: f32 = [[V_1]] - (((error as i32) as f32) - step);
// CHECK-NEXT:     error = [[V_2]];
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while y < ye {
// CHECK-NEXT:         if (error as f64) >= 1.0f64 {
// CHECK-NEXT:             let mut [[V_3:__v[0-9]+]]: i32 = unsafe { (*info).src_y };
// CHECK-NEXT:             let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] + (error as i32);
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*info).src_y = [[V_4]];
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_5:__v[0-9]+]]: f32 = error;
// CHECK-NEXT:             let mut [[V_6:__v[0-9]+]]: f32 = [[V_5]] - ((error as i32) as f32);
// CHECK-NEXT:             error = [[V_6]];
// CHECK-NEXT:             bar();
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_7:__v[0-9]+]]: f32 = error;
// CHECK-NEXT:         let mut [[V_8:__v[0-9]+]]: f32 = [[V_7]] + step;
// CHECK-NEXT:         error = [[V_8]];
// CHECK-NEXT:         let mut [[V_9:__v[0-9]+]]: i32 = y;
// CHECK-NEXT:         let mut [[V_10:__v[0-9]+]]: i32 = [[V_9]] + (1 as i32);
// CHECK-NEXT:         y = [[V_10]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { (*(info as *mut _RenderInfo)).src_y }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut info: _RenderInfo = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     info.y = 0 as i32;
// CHECK-NEXT:     info.src_y = 0 as i32;
// CHECK-NEXT:     info.scaley = 1.0f64 as f32;
// CHECK-NEXT:     if render_image_rgb_a(unsafe { &mut (*std::ptr::addr_of_mut!(info)) })
// CHECK-NEXT:         != (256 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
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
