#include <errno.h>
#include <stdio.h>

// @lowering-fn-begin
// @rewrite-fn-begin
int probe_errno_and_streams(void) {
  errno = 0;
  fputs("hi\n", stdout);
  fputs("bye\n", stderr);
  return errno;
}
// @rewrite-fn-end
// @lowering-fn-end

int main(void) { return probe_errno_and_streams(); }

// SLATE-FILECHECK-BEGIN lowering-msvc
// LOWERING-MSVC-DAG: fn probe_errno_and_streams() -> i32 {
// LOWERING-MSVC-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-MSVC-DAG:     let {{__v[0-9]+}}: *mut i32 = unsafe { _errno() };
// LOWERING-MSVC-DAG:     unsafe {
// LOWERING-MSVC-DAG:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// LOWERING-MSVC-DAG:     }
// LOWERING-MSVC-DAG:     let {{__v[0-9]+}}: *mut i8 = b"hi\n\0".as_ptr() as *mut i8;
// LOWERING-MSVC-DAG:     let {{__v[0-9]+}}: u32 = 1;
// LOWERING-MSVC-DAG:     let {{__v[0-9]+}}: *mut _iobuf = unsafe { __acrt_iob_func({{__v[0-9]+}} as u32) };
// LOWERING-MSVC-DAG:     let {{__v[0-9]+}}: i32 = unsafe { fputs({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}} as *mut _iobuf) };
// LOWERING-MSVC-DAG:     let {{__v[0-9]+}}: *mut i8 = b"bye\n\0".as_ptr() as *mut i8;
// LOWERING-MSVC-DAG:     let {{__v[0-9]+}}: u32 = 2;
// LOWERING-MSVC-DAG:     let {{__v[0-9]+}}: *mut _iobuf = unsafe { __acrt_iob_func({{__v[0-9]+}} as u32) };
// LOWERING-MSVC-DAG:     let {{__v[0-9]+}}: i32 = unsafe { fputs({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}} as *mut _iobuf) };
// LOWERING-MSVC-DAG:     let {{__v[0-9]+}}: *mut i32 = unsafe { _errno() };
// LOWERING-MSVC-DAG:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-MSVC-DAG:     return {{__v[0-9]+}};
// LOWERING-MSVC-DAG: }
// SLATE-FILECHECK-END lowering-msvc

// SLATE-FILECHECK-BEGIN rewrites-msvc
// REWRITES-MSVC-DAG: fn probe_errno_and_streams() -> i32 {
// REWRITES-MSVC-DAG:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-MSVC-DAG:     let {{__v[0-9]+}}: *mut i32 = unsafe { _errno() };
// REWRITES-MSVC-DAG:     unsafe {
// REWRITES-MSVC-DAG:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// REWRITES-MSVC-DAG:     }
// REWRITES-MSVC-DAG:     unsafe {
// REWRITES-MSVC-DAG:         fputs(
// REWRITES-MSVC-DAG:             c"hi\n".as_ptr(),
// REWRITES-MSVC-DAG:             (unsafe { __acrt_iob_func(1 as u32) }) as *mut _iobuf,
// REWRITES-MSVC-DAG:         )
// REWRITES-MSVC-DAG:     };
// REWRITES-MSVC-DAG:     unsafe {
// REWRITES-MSVC-DAG:         fputs(
// REWRITES-MSVC-DAG:             c"bye\n".as_ptr(),
// REWRITES-MSVC-DAG:             (unsafe { __acrt_iob_func(2 as u32) }) as *mut _iobuf,
// REWRITES-MSVC-DAG:         )
// REWRITES-MSVC-DAG:     };
// REWRITES-MSVC-DAG:     let {{__v[0-9]+}}: *mut i32 = unsafe { _errno() };
// REWRITES-MSVC-DAG:     unsafe { *{{__v[0-9]+}} }
// REWRITES-MSVC-DAG: }
// SLATE-FILECHECK-END rewrites-msvc
