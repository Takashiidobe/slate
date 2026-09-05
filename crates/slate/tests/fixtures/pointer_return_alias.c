#include <stdio.h>

// @lowering-fn-begin
// @rewrite-fn-begin
static int *identity_mut(int *value) { return value; }
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
static int *forward_mut(int *value) { return identity_mut(value); }
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
static const int *identity_const(const int *value) { return value; }
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
static int *choose_value(int *first, int *second, int choose_first) {
  if (choose_first)
    return first;
  return second;
}
// @rewrite-fn-end
// @lowering-fn-end

int main(void) {
  int  first             = 20;
  int  second            = 22;
  int *alias             = forward_mut(&first);
  *alias                += 2;
  const int *read_alias  = identity_const(&second);
  int       *ambiguous   = choose_value(&first, &second, 1);
  printf("%d %d %d\n", first, *read_alias, *ambiguous);
  return 0;
}

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: fn identity_mut({{arg[0-9]+}}: *mut i32) -> *mut i32 {
// COMMON-LOWERING-DAG:     return {{arg[0-9]+}};
// COMMON-LOWERING-DAG: }
// COMMON-LOWERING-DAG: fn forward_mut({{arg[0-9]+}}: *mut i32) -> *mut i32 {
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut i32 = identity_mut({{arg[0-9]+}});
// COMMON-LOWERING-DAG:     return {{__v[0-9]+}};
// COMMON-LOWERING-DAG: }
// COMMON-LOWERING-DAG: fn identity_const({{arg[0-9]+}}: *mut i32) -> *mut i32 {
// COMMON-LOWERING-DAG:     return {{arg[0-9]+}};
// COMMON-LOWERING-DAG: }
// COMMON-LOWERING-DAG: fn choose_value({{arg[0-9]+}}: *mut i32, {{arg[0-9]+}}: *mut i32, {{arg[0-9]+}}: i32) -> *mut i32 {
// COMMON-LOWERING-DAG:     let mut first: *mut i32 = std::ptr::null_mut();
// COMMON-LOWERING-DAG:     let mut choose_first: i32 = 0;
// COMMON-LOWERING-DAG:     let mut __retval: *mut i32 = std::ptr::null_mut();
// COMMON-LOWERING-DAG:     first = {{arg[0-9]+}};
// COMMON-LOWERING-DAG:     choose_first = {{arg[0-9]+}};
// COMMON-LOWERING-DAG:     {
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: i32 = choose_first;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-DAG:         if {{__v[0-9]+}} {
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = first;
// COMMON-LOWERING-DAG:             __retval = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = __retval;
// COMMON-LOWERING-DAG:             return {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         }
// COMMON-LOWERING-DAG:     }
// COMMON-LOWERING-DAG:     __retval = {{arg[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut i32 = __retval;
// COMMON-LOWERING-DAG:     return {{__v[0-9]+}};
// COMMON-LOWERING-DAG: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: fn identity_mut({{arg[0-9]+}}: &mut i32) -> *mut i32 {
// COMMON-REWRITES-DAG:     {{arg[0-9]+}} as *mut i32
// COMMON-REWRITES-DAG: }
// COMMON-REWRITES-DAG: fn forward_mut({{arg[0-9]+}}: &mut i32) -> *mut i32 {
// COMMON-REWRITES-DAG:     identity_mut({{arg[0-9]+}})
// COMMON-REWRITES-DAG: }
// COMMON-REWRITES-DAG: fn identity_const({{arg[0-9]+}}: &i32) -> *mut i32 {
// COMMON-REWRITES-DAG:     ({{arg[0-9]+}} as *const i32) as *mut i32
// COMMON-REWRITES-DAG: }
// COMMON-REWRITES-DAG: fn choose_value(mut first: *mut i32, {{arg[0-9]+}}: *mut i32, mut {{__v[0-9]+}}: i32) -> *mut i32 {
// COMMON-REWRITES-DAG:     if {{__v[0-9]+}} != 0 {
// COMMON-REWRITES-DAG:         return first;
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     {{arg[0-9]+}}
// COMMON-REWRITES-DAG: }
// SLATE-FILECHECK-END common-rewrites
