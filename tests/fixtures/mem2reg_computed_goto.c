#include <stdio.h>

// @lowering-fn-begin
// @rewrite-fn-begin
void walk(int *position) {
  static void *labels[] = {&&again, &&done};
  goto *labels[*position];
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
// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: unsafe fn walk({{arg[0-9]+}}: *mut i32) {
// LOWERING-DAG: let mut position: *mut i32 = std::ptr::null_mut();
// LOWERING-DAG: let mut {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-DAG: let mut {{__state[0-9]+}}: i32 = 0;
// LOWERING-DAG: '{{__dispatch[0-9]+}}: loop {
// LOWERING-DAG: match {{__state[0-9]+}} {
// LOWERING-DAG: 0 => {
// LOWERING-DAG: position = {{arg[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i32 = position;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-DAG: let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-DAG: let {{_v[0-9]+}}: *mut core::ffi::c_void = 1usize as *mut core::ffi::c_void;
// LOWERING-DAG: {{__state[0-9]+}} = [1, 2][({{_v[0-9]+}} as usize)];
// LOWERING-DAG: continue '{{__dispatch[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: 1 => {
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i32 = position;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = 1;
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(1) };
// LOWERING-DAG: position = {{_v[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i32 = position;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-DAG: let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-DAG: let {{_v[0-9]+}}: *mut core::ffi::c_void = 1usize as *mut core::ffi::c_void;
// LOWERING-DAG: {{__state[0-9]+}} = [1, 2][({{_v[0-9]+}} as usize)];
// LOWERING-DAG: continue '{{__dispatch[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: 2 => {
// LOWERING-DAG: return;
// LOWERING-DAG: }
// LOWERING-DAG: 3 => {
// LOWERING-DAG: unreachable!();
// LOWERING-DAG: }
// LOWERING-DAG: _ => {
// LOWERING-DAG: break '{{__dispatch[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: }
// LOWERING-DAG: }
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: unsafe fn walk({{arg[0-9]+}}: *mut i32) {
// REWRITES-DAG: let mut position: *mut i32 = std::ptr::null_mut();
// REWRITES-DAG: let mut {{__state[0-9]+}}: i32 = 0;
// REWRITES-DAG: '{{__dispatch[0-9]+}}: loop {
// REWRITES-DAG: match {{__state[0-9]+}} {
// REWRITES-DAG: 0 => {
// REWRITES-DAG: position = {{arg[0-9]+}};
// REWRITES-DAG: {{__state[0-9]+}} = [1, 2][(((unsafe { *position }) as i64) as usize)];
// REWRITES-DAG: continue '{{__dispatch[0-9]+}};
// REWRITES-DAG: }
// REWRITES-DAG: 1 => {
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i32 = position;
// REWRITES-DAG: position = unsafe { {{_v[0-9]+}}.add(1) };
// REWRITES-DAG: {{__state[0-9]+}} = [1, 2][(((unsafe { *position }) as i64) as usize)];
// REWRITES-DAG: continue '{{__dispatch[0-9]+}};
// REWRITES-DAG: }
// REWRITES-DAG: 2 => {
// REWRITES-DAG: return;
// REWRITES-DAG: }
// REWRITES-DAG: 3 => {
// REWRITES-DAG: unreachable!();
// REWRITES-DAG: }
// REWRITES-DAG: _ => {
// REWRITES-DAG: break '{{__dispatch[0-9]+}};
// REWRITES-DAG: }
// REWRITES-DAG: }
// REWRITES-DAG: }
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
