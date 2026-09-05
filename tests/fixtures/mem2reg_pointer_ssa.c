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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: fn return_global() -> *mut i32 {
// COMMON-LOWERING-DAG:     return std::ptr::addr_of_mut!(value);
// COMMON-LOWERING-DAG: }
// COMMON-LOWERING-DAG: fn return_global_pointer() -> *mut *mut i32 {
// COMMON-LOWERING-DAG:     return std::ptr::addr_of_mut!(value_pointer);
// COMMON-LOWERING-DAG: }
// COMMON-LOWERING-DAG: fn return_element() -> *mut i32 {
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-DAG:     return unsafe { std::ptr::addr_of_mut!(values[({{__v[0-9]+}} as usize)]) };
// COMMON-LOWERING-DAG: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: fn return_global() -> *mut i32 {
// COMMON-REWRITES-DAG:     std::ptr::addr_of_mut!(value)
// COMMON-REWRITES-DAG: }
// COMMON-REWRITES-DAG: fn return_global_pointer() -> *mut *mut i32 {
// COMMON-REWRITES-DAG:     std::ptr::addr_of_mut!(value_pointer)
// COMMON-REWRITES-DAG: }
// COMMON-REWRITES-DAG: fn return_element() -> *mut i32 {
// COMMON-REWRITES-DAG:     unsafe { std::ptr::addr_of_mut!(values[1]) }
// COMMON-REWRITES-DAG: }
// SLATE-FILECHECK-END common-rewrites
