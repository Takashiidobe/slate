
#include <stdio.h>

int real_impl(int x) { return x * 3 + 1; }

extern int alias_impl(int x) __attribute__((alias("real_impl")));

// @lowering-fn-begin
// @rewrite-fn-begin
int call_alias(int x) { return alias_impl(x) + real_impl(x); }
// @rewrite-fn-end
// @lowering-fn-end

int main(void) {
  printf("%d %d\n", alias_impl(5), call_alias(2));
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn call_alias({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = alias_impl({{arg[0-9]+}});
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = real_impl({{arg[0-9]+}});
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:     return {{__v[0-9]+}};
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn call_alias({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = alias_impl({{arg[0-9]+}});
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = real_impl({{arg[0-9]+}});
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// REWRITES-DAG:     {{__v[0-9]+}}
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
