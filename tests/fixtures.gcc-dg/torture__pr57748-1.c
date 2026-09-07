/* PR middle-end/57748 */
/* { dg-do run } */
/* ICE in expand_assignment:
   misalignp == true, !MEM_P (to_rtx), offset != 0,
   => gcc_assert (TREE_CODE (offset) == INTEGER_CST) */

#include <stdlib.h>

extern void abort(void);

typedef long long V
    __attribute__((vector_size(2 * sizeof(long long)), may_alias));

typedef struct S {
  V a;
  V b[0];
} P __attribute__((aligned(1)));

struct __attribute__((packed)) T {
  char c;
  P    s;
};

void __attribute__((noinline, noclone)) check(struct T *t) {
  if (t->s.b[0][0] != 3 || t->s.b[0][1] != 4)
    abort();
}

int __attribute__((noinline, noclone)) get_i(void) { return 0; }

void __attribute__((noinline, noclone)) foo(P *p) {
  V   a   = {3, 4};
  int i   = get_i();
  p->b[i] = a;
}

int
// @lowering-fn-begin
// @rewrite-fn-begin
main() {
  struct T *t = (struct T *)calloc(128, 1);

  foo(&t->s);
  check(t);

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
// LOWERING-DAG:     unsafe { foo(unsafe { std::ptr::addr_of_mut!((*{{__v[0-9]+}}).s) }) };
// LOWERING-DAG:     unsafe { check({{__v[0-9]+}}) };
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
// REWRITES-DAG:     unsafe { foo(unsafe { std::ptr::addr_of_mut!((*{{__v[0-9]+}}).s) }) };
// REWRITES-DAG:     unsafe { check({{__v[0-9]+}}) };
// REWRITES-DAG:     unsafe { free({{__v[0-9]+}} as *mut core::ffi::c_void) };
// REWRITES-DAG:     return std::process::ExitCode::SUCCESS;
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
