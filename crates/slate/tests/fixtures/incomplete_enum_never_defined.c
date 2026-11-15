int printf(const char *, ...);

enum mode_status;
enum access_mode;

struct mode_funcs {
  enum mode_status (*mode_valid)(int width, int height);
  int (*attach)(enum access_mode *mode, int flags);
  int id;
};

struct mode_config {
  const struct mode_funcs *funcs;
  enum access_mode *pending;
  int count;
};

static int attach_noop(enum access_mode *mode, int flags) { return (mode == 0) + flags; }

static const struct mode_funcs funcs = {0, attach_noop, 7};

static int config_score(const struct mode_config *config) {
  int score = config->count * 10 + config->funcs->id;
  if (config->funcs->mode_valid == 0)
    score += 100;
  if (config->pending == 0)
    score += 1000;
  return score;
}

int main(void) {
  struct mode_config config = {&funcs, 0, 3};
  printf("%d %d\n", config_score(&config), config.funcs->attach(config.pending, 2));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct mode_funcs {
// CHECK-NEXT:     mode_valid: Option<unsafe extern "C-unwind" fn(i32, i32) -> i32>,
// CHECK-NEXT:     attach: Option<unsafe extern "C-unwind" fn(*mut i32, i32) -> i32>,
// CHECK-NEXT:     id: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct mode_config {
// CHECK-NEXT:     funcs: *const mode_funcs,
// CHECK-NEXT:     pending: *mut i32,
// CHECK-NEXT:     count: i32,
// CHECK-NEXT: }
// CHECK-NEXT: extern "C-unwind" fn attach_noop(mode: *mut i32, flags: i32) -> i32 {
// CHECK-NEXT:     (mode.is_null() as i32) + flags
// CHECK-NEXT: }
// CHECK-NEXT: static mut funcs: mode_funcs = {
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: mode_funcs = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].mode_valid = None;
// CHECK-NEXT:     [[T_0]].attach = Some(attach_noop);
// CHECK-NEXT:     [[T_0]].id = 7i32;
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: fn config_score(config: &mode_config) -> i32 {
// CHECK-NEXT:     let mut score: i32 = (unsafe { (*(config as *const mode_config)).count }) * 10
// CHECK-NEXT:         + unsafe { (*(*(config as *const mode_config)).funcs).id };
// CHECK-NEXT:     if (unsafe { (*(*(config as *const mode_config)).funcs).mode_valid }) == None {
// CHECK-NEXT:         score += 100;
// CHECK-NEXT:     }
// CHECK-NEXT:     if unsafe { (*(config as *const mode_config)).pending }.is_null() {
// CHECK-NEXT:         score += 1000;
// CHECK-NEXT:     }
// CHECK-NEXT:     score
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut config: mode_config = {
// CHECK-NEXT:         let mut [[T_0]]: mode_config = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].funcs = unsafe { core::ptr::addr_of!(funcs) };
// CHECK-NEXT:         [[T_0]].pending = core::ptr::null_mut::<i32>();
// CHECK-NEXT:         [[T_0]].count = 3i32;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %d\n".as_ptr(),
// CHECK-NEXT:             config_score(&(*(core::ptr::addr_of_mut!(config) as *const mode_config))),
// CHECK-NEXT:             unsafe { (*config.funcs).attach }.unwrap()(config.pending, 2i32),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     0
// CHECK-NEXT: }
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
