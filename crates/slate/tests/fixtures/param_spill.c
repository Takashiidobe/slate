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

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn adjust({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-DAG:     let mut value: i32 = 0;
// LOWERING-DAG:     value = {{arg[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-DAG:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(value))
// LOWERING-DAG:             .load(std::sync::atomic::Ordering::SeqCst)
// LOWERING-DAG:     };
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(value))
// LOWERING-DAG:             .store({{__v[0-9]+}}, std::sync::atomic::Ordering::SeqCst)
// LOWERING-DAG:     };
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-DAG:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(value))
// LOWERING-DAG:             .load(std::sync::atomic::Ordering::SeqCst)
// LOWERING-DAG:     };
// LOWERING-DAG:     return {{__v[0-9]+}};
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn adjust(mut value: i32, {{arg[0-9]+}}: i32) -> i32 {
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = unsafe {
// REWRITES-DAG:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(value))
// REWRITES-DAG:             .load(std::sync::atomic::Ordering::SeqCst)
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{arg[0-9]+}};
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(value))
// REWRITES-DAG:             .store({{__v[0-9]+}}, std::sync::atomic::Ordering::SeqCst)
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = unsafe {
// REWRITES-DAG:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(value))
// REWRITES-DAG:             .load(std::sync::atomic::Ordering::SeqCst)
// REWRITES-DAG:     };
// REWRITES-DAG:     {{__v[0-9]+}}
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
