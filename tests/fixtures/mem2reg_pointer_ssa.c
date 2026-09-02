#include <stdio.h>

int  value         = 7;
int *value_pointer = &value;
int  values[]      = {11, 13, 17};

// @lowering-fn-begin
// @rewrite-fn-begin
int *return_global(void) { return &value; }
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
int **return_global_pointer(void) { return &value_pointer; }
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
int *return_element(void) { return &values[1]; }
// @rewrite-fn-end
// @lowering-fn-end

int main(void) {
  printf("%d\n",
         *return_global() + **return_global_pointer() + *return_element());
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn return_global() -> *mut i32 {
// LOWERING-DAG:     return std::ptr::addr_of_mut!(value);
// LOWERING-DAG: }
// LOWERING-DAG: fn return_global_pointer() -> *mut *mut i32 {
// LOWERING-DAG:     return std::ptr::addr_of_mut!(value_pointer);
// LOWERING-DAG: }
// LOWERING-DAG: fn return_element() -> *mut i32 {
// LOWERING-DAG:     let {{_v[0-9]+}}: i64 = 1;
// LOWERING-DAG:     return unsafe { std::ptr::addr_of_mut!(values[({{_v[0-9]+}} as usize)]) };
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn return_global() -> *mut i32 {
// REWRITES-DAG:     std::ptr::addr_of_mut!(value)
// REWRITES-DAG: }
// REWRITES-DAG: fn return_global_pointer() -> *mut *mut i32 {
// REWRITES-DAG:     std::ptr::addr_of_mut!(value_pointer)
// REWRITES-DAG: }
// REWRITES-DAG: fn return_element() -> *mut i32 {
// REWRITES-DAG:     unsafe { std::ptr::addr_of_mut!(values[((1 as i64) as usize)]) }
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
