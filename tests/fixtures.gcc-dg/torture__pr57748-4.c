/* PR middle-end/57748 */
/* { dg-do run } */
/* wrong code in expand_expr_real_1.  */

#include <stdlib.h>

extern void abort(void);

typedef long long V
    __attribute__((vector_size(2 * sizeof(long long)), may_alias));

typedef struct S {
  V b[1];
} P __attribute__((aligned(1)));

struct __attribute__((packed)) T {
  char c;
  P    s;
};

void __attribute__((noinline, noclone)) check(P *p) {
  if (p->b[1][0] != 3 || p->b[1][1] != 4)
    abort();
}

void __attribute__((noinline, noclone)) foo(struct T *t) {
  V a       = {3, 4};
  t->s.b[1] = a;
}

int
// @lowering-fn-begin
// @rewrite-fn-begin
main() {
  struct T *t = (struct T *)calloc(128, 1);

  foo(t);
  check(&t->s);

  free(t);
  return 0;
}
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn main() -> std::process::ExitCode {
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: u64 = 128;
// LOWERING-DAG:     let {{__v[0-9]+}}: u64 = 1;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe { calloc({{__v[0-9]+}} as usize, {{__v[0-9]+}} as usize) };
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut T = {{__v[0-9]+}} as *mut T;
// LOWERING-DAG:     unsafe { foo({{__v[0-9]+}}) };
// LOWERING-DAG:     unsafe { check(unsafe { std::ptr::addr_of_mut!((*{{__v[0-9]+}}).s) }) };
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-DAG:     unsafe { free({{__v[0-9]+}} as *mut core::ffi::c_void) };
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     return std::process::ExitCode::SUCCESS;
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn main() -> std::process::ExitCode {
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void =
// REWRITES-DAG:         unsafe { calloc((128 as u64) as usize, (1 as u64) as usize) };
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut T = {{__v[0-9]+}} as *mut T;
// REWRITES-DAG:     unsafe { foo({{__v[0-9]+}}) };
// REWRITES-DAG:     unsafe { check(unsafe { std::ptr::addr_of_mut!((*{{__v[0-9]+}}).s) }) };
// REWRITES-DAG:     unsafe { free({{__v[0-9]+}} as *mut core::ffi::c_void) };
// REWRITES-DAG:     return std::process::ExitCode::SUCCESS;
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
