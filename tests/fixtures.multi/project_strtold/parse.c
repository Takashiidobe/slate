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
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut i8 = c"2.75".as_ptr() as *mut i8;
// REWRITES-DAG:     let {{__v[0-9]+}}: LongDouble = unsafe {
// REWRITES-DAG:         __slate_strtold__rf80_pc_ppc(
// REWRITES-DAG:             {{__v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-DAG:             std::ptr::addr_of_mut!(end) as *mut *mut core::ffi::c_char,
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     value = {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut i8 = end;
// REWRITES-DAG:     let {{__v[0-9]+}}: i8 = unsafe { *{{__v[0-9]+}} };
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: LongDouble = value;
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = __slate_f80_to_i32({{__v[0-9]+}});
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = -1;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     {{__v[0-9]+}}
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
