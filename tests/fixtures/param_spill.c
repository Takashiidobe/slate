#include <stdio.h>

// @lowering-fn-begin
// @rewrite-fn-begin
int adjust(_Atomic int value, int delta) {
  value = value + delta;
  return value;
}
// @rewrite-fn-end
// @lowering-fn-end

int main(void) {
  printf("%d\n", adjust(4, 3));
  return 0;
}

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: fn adjust({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-DAG:     let mut value: i32 = 0;
// COMMON-LOWERING-DAG:     value = {{arg[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-DAG:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(value))
// COMMON-LOWERING-DAG:             .load(std::sync::atomic::Ordering::SeqCst)
// COMMON-LOWERING-DAG:     };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{arg[0-9]+}};
// COMMON-LOWERING-DAG:     unsafe {
// COMMON-LOWERING-DAG:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(value))
// COMMON-LOWERING-DAG:             .store({{__v[0-9]+}}, std::sync::atomic::Ordering::SeqCst)
// COMMON-LOWERING-DAG:     };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-DAG:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(value))
// COMMON-LOWERING-DAG:             .load(std::sync::atomic::Ordering::SeqCst)
// COMMON-LOWERING-DAG:     };
// COMMON-LOWERING-DAG:     return {{__v[0-9]+}};
// COMMON-LOWERING-DAG: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: fn adjust(mut value: i32, {{arg[0-9]+}}: i32) -> i32 {
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-DAG:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(value))
// COMMON-REWRITES-DAG:             .load(std::sync::atomic::Ordering::SeqCst)
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     unsafe {
// COMMON-REWRITES-DAG:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(value))
// COMMON-REWRITES-DAG:             .store({{__v[0-9]+}} + {{arg[0-9]+}}, std::sync::atomic::Ordering::SeqCst)
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-DAG:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(value))
// COMMON-REWRITES-DAG:             .load(std::sync::atomic::Ordering::SeqCst)
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     {{__v[0-9]+}}
// COMMON-REWRITES-DAG: }
// SLATE-FILECHECK-END common-rewrites
