#include <stdlib.h>

// @rewrite-fn-begin
int parse_value(void) {
  char       *end;
  long double value = strtold("2.75", &end);
  return *end == '\0' ? (int)value : -1;
}
// @rewrite-fn-end

// LOWERING-NOT: #[cfg(target_arch =

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: #[unsafe(no_mangle)]
// REWRITES-DAG: pub extern "C-unwind" fn parse_value() -> i32 {
// REWRITES-DAG:     let mut end: *mut i8 = std::ptr::null_mut();
// REWRITES-DAG:     let mut value: LongDouble = LongDouble([0; 10]);
// REWRITES-DAG:     value = unsafe {
// REWRITES-DAG:         __slate_strtold__rf80_pc_ppc(
// REWRITES-DAG:             c"2.75".as_ptr(),
// REWRITES-DAG:             std::ptr::addr_of_mut!(end) as *mut *mut core::ffi::c_char,
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = ((unsafe { *end }) as i32) == 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = __slate_f80_to_i32(value);
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = -1;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     {{__v[0-9]+}}
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-DAG: #[unsafe(no_mangle)]
// REWRITES-X86_64-GNU-DAG: pub extern "C-unwind" fn parse_value() -> i32 {
// REWRITES-X86_64-GNU-DAG:     let mut end: *mut i8 = std::ptr::null_mut();
// REWRITES-X86_64-GNU-DAG:     let mut value: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-DAG:     value = unsafe {
// REWRITES-X86_64-GNU-DAG:         __slate_strtold__rf80_pc_ppc(
// REWRITES-X86_64-GNU-DAG:             c"2.75".as_ptr(),
// REWRITES-X86_64-GNU-DAG:             std::ptr::addr_of_mut!(end) as *mut *mut core::ffi::c_char,
// REWRITES-X86_64-GNU-DAG:         )
// REWRITES-X86_64-GNU-DAG:     };
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: bool = ((unsafe { *end }) as i32) == 0;
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: i32 = __slate_f80_to_i32(value);
// REWRITES-X86_64-GNU-DAG:         {{__v[0-9]+}}
// REWRITES-X86_64-GNU-DAG:     } else {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: i32 = -1;
// REWRITES-X86_64-GNU-DAG:         {{__v[0-9]+}}
// REWRITES-X86_64-GNU-DAG:     };
// REWRITES-X86_64-GNU-DAG:     {{__v[0-9]+}}
// REWRITES-X86_64-GNU-DAG: }
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-DAG: #[unsafe(no_mangle)]
// REWRITES-AARCH64-GNU-DAG: pub extern "C-unwind" fn parse_value() -> i32 {
// REWRITES-AARCH64-GNU-DAG:     let mut end: *mut i8 = std::ptr::null_mut();
// REWRITES-AARCH64-GNU-DAG:     let mut value: LongDouble = LongDouble([0; 10]);
// REWRITES-AARCH64-GNU-DAG:     value = unsafe {
// REWRITES-AARCH64-GNU-DAG:         __slate_strtold__rf80_pc_ppc(
// REWRITES-AARCH64-GNU-DAG:             c"2.75".as_ptr(),
// REWRITES-AARCH64-GNU-DAG:             std::ptr::addr_of_mut!(end) as *mut *mut core::ffi::c_char,
// REWRITES-AARCH64-GNU-DAG:         )
// REWRITES-AARCH64-GNU-DAG:     };
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: bool = ((unsafe { *end }) as i32) == 0;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} {
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: i32 = __slate_f80_to_i32(value);
// REWRITES-AARCH64-GNU-DAG:         {{__v[0-9]+}}
// REWRITES-AARCH64-GNU-DAG:     } else {
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: i32 = -1;
// REWRITES-AARCH64-GNU-DAG:         {{__v[0-9]+}}
// REWRITES-AARCH64-GNU-DAG:     };
// REWRITES-AARCH64-GNU-DAG:     {{__v[0-9]+}}
// REWRITES-AARCH64-GNU-DAG: }
// SLATE-FILECHECK-END rewrites-aarch64-gnu
