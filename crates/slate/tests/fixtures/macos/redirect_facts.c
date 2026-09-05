#define _DARWIN_C_SOURCE
#include <errno.h>
#include <stdio.h>

// @lowering-fn-begin
// @rewrite-fn-begin
FILE *open_darwin_stream(const char *path) { return fopen(path, "r"); }
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
int darwin_errno(void) { return errno; }
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
FILE *darwin_stdin(void) { return stdin; }
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering-macos
// LOWERING-MACOS-DAG: fn open_darwin_stream({{arg[0-9]+}}: *mut i8) -> *mut __sFILE {
// LOWERING-MACOS-DAG:     let {{__v[0-9]+}}: *mut i8 = b"r\0".as_ptr() as *mut i8;
// LOWERING-MACOS-DAG:     let {{__v[0-9]+}}: *mut __sFILE = unsafe { __fopen_DARWIN_EXTSN({{arg[0-9]+}} as *mut i8, {{__v[0-9]+}} as *mut i8) };
// LOWERING-MACOS-DAG:     return {{__v[0-9]+}};
// LOWERING-MACOS-DAG: }
// LOWERING-MACOS-DAG: fn darwin_errno() -> i32 {
// LOWERING-MACOS-DAG:     let {{__v[0-9]+}}: *mut i32 = unsafe { __error() };
// LOWERING-MACOS-DAG:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-MACOS-DAG:     return {{__v[0-9]+}};
// LOWERING-MACOS-DAG: }
// LOWERING-MACOS-DAG: fn darwin_stdin() -> *mut __sFILE {
// LOWERING-MACOS-DAG:     let {{__v[0-9]+}}: *mut __sFILE = unsafe { __stdinp };
// LOWERING-MACOS-DAG:     return {{__v[0-9]+}};
// LOWERING-MACOS-DAG: }
// SLATE-FILECHECK-END lowering-macos

// SLATE-FILECHECK-BEGIN rewrites-macos
// REWRITES-MACOS-DAG: fn open_darwin_stream({{arg[0-9]+}}: *mut i8) -> *mut __sFILE {
// REWRITES-MACOS-DAG:     let {{__v[0-9]+}}: *mut i8 = c"r".as_ptr() as *mut i8;
// REWRITES-MACOS-DAG:     let {{__v[0-9]+}}: *mut __sFILE = unsafe { __fopen_DARWIN_EXTSN({{arg[0-9]+}} as *mut i8, {{__v[0-9]+}} as *mut i8) };
// REWRITES-MACOS-DAG:     {{__v[0-9]+}}
// REWRITES-MACOS-DAG: }
// REWRITES-MACOS-DAG: fn darwin_errno() -> i32 {
// REWRITES-MACOS-DAG:     let {{__v[0-9]+}}: *mut i32 = unsafe { __error() };
// REWRITES-MACOS-DAG:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// REWRITES-MACOS-DAG:     {{__v[0-9]+}}
// REWRITES-MACOS-DAG: }
// REWRITES-MACOS-DAG: fn darwin_stdin() -> *mut __sFILE {
// REWRITES-MACOS-DAG:     let {{__v[0-9]+}}: *mut __sFILE = unsafe { __stdinp };
// REWRITES-MACOS-DAG:     {{__v[0-9]+}}
// REWRITES-MACOS-DAG: }
// SLATE-FILECHECK-END rewrites-macos
