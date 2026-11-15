#include <stdio.h>

static unsigned debug_level = 0x3;
static unsigned debug_layer = 0x10;
static const char module_name[] = "unit";

static void debug_print(unsigned level, unsigned line, const char *function,
                        const char *module, unsigned component,
                        const char *format, ...) {
  printf("%u %u %s %s %u %s", level, line, function, module, component,
         format);
}

#define COMPONENT 0x10
#define DEBUG_PARAMETERS __LINE__, __func__, module_name, COMPONENT
#define DEBUG_LEVEL(dl) (unsigned)dl, DEBUG_PARAMETERS
#define DB_EXEC DEBUG_LEVEL(0x2)
#define IS_DEBUG_ENABLED(level, component)                                     \
  ((level & debug_level) && (component & debug_layer))

#define DO_WHILE0(a) do a while (0)

#define DO_DEBUG_PRINT(function, level, line, filename, modulename,           \
                       component, ...)                                         \
  DO_WHILE0({                                                                  \
    if (IS_DEBUG_ENABLED(level, component)) {                                  \
      function(level, line, filename, modulename, component, __VA_ARGS__);     \
    }                                                                          \
  })

#define ACTUAL_DEBUG(level, line, filename, modulename, component, ...)        \
  DO_DEBUG_PRINT(debug_print, level, line, filename, modulename, component,   \
                 __VA_ARGS__)

#define DEBUG_PRINT(plist) ACTUAL_DEBUG plist

static void run(void) {
  DEBUG_PRINT((DB_EXEC, "exec\n"));
  DEBUG_PRINT((DEBUG_LEVEL(0x1), "level one\n"));
}

int main(void) {
  run();
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: macro_rules! IS_DEBUG_ENABLED {
// CHECK-NEXT:     ($level:expr, $component:expr) => {
// CHECK-NEXT:         ($level) & unsafe { debug_level } != 0 && (($component) as u32) & unsafe {
// CHECK-NEXT:         debug_layer } != 0
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: static mut debug_level: u32 = 3;
// CHECK-NEXT: static mut debug_layer: u32 = 16;
// CHECK-NEXT: static mut module_name: [i8; 5] = [117, 110, 105, 116, 0];
// CHECK-NEXT: unsafe extern "C-unwind" fn debug_print(
// CHECK-NEXT:     level: u32,
// CHECK-NEXT:     line: u32,
// CHECK-NEXT:     function: *const i8,
// CHECK-NEXT:     module: *const i8,
// CHECK-NEXT:     component: u32,
// CHECK-NEXT:     format: *const i8,
// CHECK-NEXT:     __va_args: ...
// CHECK-NEXT: ) {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%u %u %s %s %u %s".as_ptr(),
// CHECK-NEXT:             level,
// CHECK-NEXT:             line,
// CHECK-NEXT:             function,
// CHECK-NEXT:             module,
// CHECK-NEXT:             component,
// CHECK-NEXT:             format,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn run() {
// CHECK-NEXT:     loop {
// CHECK-NEXT:         if IS_DEBUG_ENABLED!(2u32, 16i32) {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 debug_print(
// CHECK-NEXT:                     2u32,
// CHECK-NEXT:                     38u32,
// CHECK-NEXT:                     c"run".as_ptr(),
// CHECK-NEXT:                     core::ptr::addr_of!(module_name) as *const i8,
// CHECK-NEXT:                     16u32,
// CHECK-NEXT:                     c"exec\n".as_ptr(),
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         if IS_DEBUG_ENABLED!(1u32, 16i32) {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 debug_print(
// CHECK-NEXT:                     1u32,
// CHECK-NEXT:                     39u32,
// CHECK-NEXT:                     c"run".as_ptr(),
// CHECK-NEXT:                     core::ptr::addr_of!(module_name) as *const i8,
// CHECK-NEXT:                     16u32,
// CHECK-NEXT:                     c"level one\n".as_ptr(),
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     run();
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
