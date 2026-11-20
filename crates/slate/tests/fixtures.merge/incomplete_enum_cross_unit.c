// merge-sources: units/incomplete_enum_modes.c
#include <stdio.h>
#include "units/incomplete_enum_modes.h"

static int config_score(const struct mode_config *config) {
    int score = config->count * 10 + config->funcs->id;
    if (config->funcs->mode_valid != 0)
        score += 100;
    if (config->pending != 0)
        score += 1000;
    return score;
}

int main(void) {
    struct mode_config *config = mode_config_new();
    printf("%d\n", config_score(config));
    printf("%d %d %d\n", mode_config_check(config, 5, 5), mode_config_check(config, 20, 20),
           mode_config_check(config, 0, 3));
    printf("%d %d\n", mode_config_access(config, 1), mode_config_access(config, 2));
    mode_config_free(config);
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 2 compiles
// CHECK-NEXT:   incomplete_enum_cross_unit.c: [] 1 compiled
// CHECK-NEXT:   units/incomplete_enum_modes.c: [] 1 compiled
// CHECK-NEXT: target windows-x86_64: 2 compiles
// CHECK-NEXT:   incomplete_enum_cross_unit.c: [] 1 compiled
// CHECK-NEXT:   units/incomplete_enum_modes.c: [] 1 compiled
// CHECK-NEXT: total: 4 compiles across 2 targets
// CHECK-NEXT: unit incomplete_enum_cross_unit: 13 items, 2 gated, 0 duplicated functions
// CHECK-NEXT:   spliced fn:__slate_main: 2 variants
// CHECK-NEXT: unit incomplete_enum_modes: 16 items, 1 gated, 0 duplicated functions
// CHECK-NEXT:   spliced fn:mode_valid: 2 variants
// CHECK-NEXT:   spliced fn:access_for: 2 variants
// CHECK-NEXT: macros: 0 function-like, 0 lifted at least once (expansions counted per distinct compile)
// CHECK-NEXT: projection: 12 checked, 0 not translatable, 0 mismatches
// CHECK-NEXT: signatures: 8 shared symbols, 0 mismatches
// CHECK-NEXT: exit: 0
// CHECK-NEXT: file incomplete_enum_modes.rs
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: mod __slate_oldnames {
// CHECK-NEXT:     #[link(name = "oldnames")]
// CHECK-NEXT:     unsafe extern "C" {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg_attr(unix, unsafe(link_section = ".text.slate_distinct.incomplete_enum_modes.access_for"))]
// CHECK-NEXT: extern "C-unwind" fn access_for(flags: i32) -> u32 {
// CHECK-NEXT:     (if flags & 1 != 0 { 3i32 } else { 1i32 }) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg_attr(unix, unsafe(link_section = ".text.slate_distinct.incomplete_enum_modes.mode_valid"))]
// CHECK-NEXT: extern "C-unwind" fn mode_valid(width: i32, height: i32) -> i32 {
// CHECK-NEXT:     if width <= 0 || height <= 0 {
// CHECK-NEXT:         return -1;
// CHECK-NEXT:     }
// CHECK-NEXT:     (if width * height > 100 { -2i32 } else { 0i32 }) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct mode_config {
// CHECK-NEXT:     funcs: *const mode_funcs,
// CHECK-NEXT:     pending: *mut u32,
// CHECK-NEXT:     count: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct mode_funcs {
// CHECK-NEXT:     mode_valid: Option<unsafe extern "C-unwind" fn(i32, i32) -> i32>,
// CHECK-NEXT:     access: Option<unsafe extern "C-unwind" fn(i32) -> u32>,
// CHECK-NEXT:     id: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(transparent)]
// CHECK-NEXT: struct __SlateConst<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn mode_config_access(config: *mut mode_config, flags: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(*config).pending = unsafe { (*(*config).funcs).access }.unwrap()(flags);
// CHECK-NEXT:     }
// CHECK-NEXT:     ((unsafe { *(*config).pending }) as u32) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn mode_config_check(
// CHECK-NEXT:     config: *const mode_config,
// CHECK-NEXT:     width: i32,
// CHECK-NEXT:     height: i32,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     (unsafe { unsafe { (*(*config).funcs).mode_valid }.unwrap()(width, height) }) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn mode_config_free(config: *mut mode_config) {
// CHECK-NEXT:     unsafe { free((config as *mut ()) as *mut core::ffi::c_void) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn mode_config_new() -> *mut mode_config {
// CHECK-NEXT:     let config: *mut mode_config = (unsafe {
// CHECK-NEXT:         malloc((core::mem::size_of::<mode_config>() as size_t) as usize) as *mut ()
// CHECK-NEXT:     }) as *mut mode_config;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*config).funcs = core::ptr::addr_of!(funcs.0);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*config).pending = core::ptr::addr_of_mut!(pending_access);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*config).count = 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     config
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static funcs: __SlateConst<mode_funcs> = __SlateConst(
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: mode_funcs = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].mode_valid = Some(mode_valid);
// CHECK-NEXT:         [[T_0]].access = Some(access_for);
// CHECK-NEXT:         [[T_0]].id = 7i32;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     },
// CHECK-NEXT: );
// CHECK-EMPTY:
// CHECK-NEXT: static mut pending_access: u32 = 2;
// CHECK-EMPTY:
// CHECK-NEXT: unsafe impl<T> Sync for __SlateConst<T> {}
// CHECK-EMPTY:
// CHECK-NEXT: use slate_support::malloc;
// CHECK-NEXT: use slate_support::free;
// CHECK-EMPTY:
// CHECK-NEXT: use slate_support::size_t;
// CHECK-NEXT: file main.rs
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: mod __slate_oldnames {
// CHECK-NEXT:     #[link(name = "oldnames")]
// CHECK-NEXT:     unsafe extern "C" {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[path = "incomplete_enum_modes.rs"]
// CHECK-NEXT: mod __slate_unit_incomplete_enum_modes;
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct mode_config {
// CHECK-NEXT:     funcs: *const mode_funcs,
// CHECK-NEXT:     pending: *mut i32,
// CHECK-NEXT:     count: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct mode_funcs {
// CHECK-NEXT:     mode_valid: Option<unsafe extern "C-unwind" fn(i32, i32) -> i32>,
// CHECK-NEXT:     access: Option<unsafe extern "C-unwind" fn(i32) -> i32>,
// CHECK-NEXT:     id: i32,
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
// CHECK-EMPTY:
// CHECK-NEXT: //! merge-sources: units/incomplete_enum_modes.c
// CHECK-NEXT: #[cfg(not(any(all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"), all(target_arch = "x86_64", windows, target_env = "msvc"))))]
// CHECK-NEXT: compile_error!("this crate was not translated for this target");
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let config: *mut mode_config = unsafe { mode_config_new() };
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(
// CHECK-NEXT:                 c"%d\n".as_ptr(),
// CHECK-NEXT:                 config_score(config as *const mode_config),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(
// CHECK-NEXT:                 c"%d %d %d\n".as_ptr(),
// CHECK-NEXT:                 mode_config_check(config as *const mode_config, 5i32, 5i32),
// CHECK-NEXT:                 mode_config_check(config as *const mode_config, 20i32, 20i32),
// CHECK-NEXT:                 mode_config_check(config as *const mode_config, 0i32, 3i32),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(
// CHECK-NEXT:                 c"%d %d\n".as_ptr(),
// CHECK-NEXT:                 mode_config_access(config, 1i32),
// CHECK-NEXT:                 mode_config_access(config, 2i32),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{}", config_score(config as *const mode_config));
// CHECK-NEXT:         println!(
// CHECK-NEXT:             "{} {} {}",
// CHECK-NEXT:             unsafe { mode_config_check(config as *const mode_config, 5i32, 5i32) },
// CHECK-NEXT:             unsafe { mode_config_check(config as *const mode_config, 20i32, 20i32) },
// CHECK-NEXT:             unsafe { mode_config_check(config as *const mode_config, 0i32, 3i32) }
// CHECK-NEXT:         );
// CHECK-NEXT:         println!(
// CHECK-NEXT:             "{} {}",
// CHECK-NEXT:             unsafe { mode_config_access(config, 1i32) },
// CHECK-NEXT:             unsafe { mode_config_access(config, 2i32) }
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { mode_config_free(config) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn config_score(config: *const mode_config) -> i32 {
// CHECK-NEXT:     let mut score: i32 = (unsafe { (*config).count }) * 10
// CHECK-NEXT:         + unsafe { (*(*config).funcs).id };
// CHECK-NEXT:     if (unsafe { (*(*config).funcs).mode_valid }) != None {
// CHECK-NEXT:         score += 100;
// CHECK-NEXT:     }
// CHECK-NEXT:     if !unsafe { (*config).pending }.is_null() {
// CHECK-NEXT:         score += 1000;
// CHECK-NEXT:     }
// CHECK-NEXT:     score
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn mode_config_new() -> *mut mode_config;
// CHECK-NEXT:     fn mode_config_check([[ARG_0:arg[0-9]+]]: *const mode_config, [[ARG_1:arg[0-9]+]]: i32, [[ARG_2:arg[0-9]+]]: i32) -> i32;
// CHECK-NEXT:     fn mode_config_access([[ARG_0]]: *mut mode_config, [[ARG_1]]: i32) -> i32;
// CHECK-NEXT:     fn mode_config_free([[ARG_0]]: *mut mode_config);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: use core::ffi::c_char;
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
