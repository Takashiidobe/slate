#include <stdio.h>

// @lowering-fn-begin
// @rewrite-fn-begin
void walk(int *position) {
  static void *labels[] = {&&again, &&done};
  goto        *labels[*position];
again:
  position++;
  goto *labels[*position];
done:
  return;
}
// @rewrite-fn-end
// @lowering-fn-end

int main(void) {
  int path[] = {0, 1};
  walk(path);
  printf("done\n");
  return 0;
}

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: unsafe fn walk({{arg[0-9]+}}: *mut i32) {
// COMMON-LOWERING-DAG:     let mut position: *mut i32 = std::ptr::null_mut();
// COMMON-LOWERING-DAG:     let mut {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-DAG:     let mut {{__state[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:     '{{__dispatch[0-9]+}}: loop {
// COMMON-LOWERING-DAG:         match {{__state[0-9]+}} {
// COMMON-LOWERING-DAG:             0 => {
// COMMON-LOWERING-DAG:                 position = {{arg[0-9]+}};
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: *mut i32 = position;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: *mut core::ffi::c_void = 1usize as *mut core::ffi::c_void;
// COMMON-LOWERING-DAG:                 {{__state[0-9]+}} = [1, 2][({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             1 => {
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: *mut i32 = position;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(1) };
// COMMON-LOWERING-DAG:                 position = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: *mut i32 = position;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: *mut core::ffi::c_void = 1usize as *mut core::ffi::c_void;
// COMMON-LOWERING-DAG:                 {{__state[0-9]+}} = [1, 2][({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             2 => {
// COMMON-LOWERING-DAG:                 return;
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             3 => {
// COMMON-LOWERING-DAG:                 unreachable!();
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             _ => {
// COMMON-LOWERING-DAG:                 break '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:         }
// COMMON-LOWERING-DAG:     }
// COMMON-LOWERING-DAG: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: unsafe fn walk({{arg[0-9]+}}: *mut i32) {
// COMMON-REWRITES-DAG:     let mut position: *mut i32 = std::ptr::null_mut();
// COMMON-REWRITES-DAG:     let mut {{__state[0-9]+}}: i32 = 0;
// COMMON-REWRITES-DAG:     '{{__dispatch[0-9]+}}: loop {
// COMMON-REWRITES-DAG:         match {{__state[0-9]+}} {
// COMMON-REWRITES-DAG:             0 => {
// COMMON-REWRITES-DAG:                 position = {{arg[0-9]+}};
// COMMON-REWRITES-DAG:                 {{__state[0-9]+}} = [1, 2][(((unsafe { *position }) as i64) as usize)];
// COMMON-REWRITES-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-REWRITES-DAG:             }
// COMMON-REWRITES-DAG:             1 => {
// COMMON-REWRITES-DAG:                 let {{__v[0-9]+}}: *mut i32 = position;
// COMMON-REWRITES-DAG:                 position = unsafe { {{__v[0-9]+}}.add(1) };
// COMMON-REWRITES-DAG:                 {{__state[0-9]+}} = [1, 2][(((unsafe { *position }) as i64) as usize)];
// COMMON-REWRITES-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-REWRITES-DAG:             }
// COMMON-REWRITES-DAG:             2 => {
// COMMON-REWRITES-DAG:                 return;
// COMMON-REWRITES-DAG:             }
// COMMON-REWRITES-DAG:             3 => {
// COMMON-REWRITES-DAG:                 unreachable!();
// COMMON-REWRITES-DAG:             }
// COMMON-REWRITES-DAG:             _ => {
// COMMON-REWRITES-DAG:                 break '{{__dispatch[0-9]+}};
// COMMON-REWRITES-DAG:             }
// COMMON-REWRITES-DAG:         }
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG: }
// SLATE-FILECHECK-END common-rewrites
