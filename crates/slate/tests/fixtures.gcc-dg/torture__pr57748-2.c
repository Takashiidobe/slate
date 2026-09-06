/* PR middle-end/57748 */
/* { dg-do run } */
/* wrong code in expand_assignment:
   misalignp == true, !MEM_P (to_rtx),
   offset == 0, bitpos >= GET_MODE_PRECISION,
   => result = NULL.  */

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

void __attribute__((noinline, noclone)) foo(P *p) {
  V a     = {3, 4};
  p->b[0] = a;
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
// LOWERING-DAG: fn main() {
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
// LOWERING-DAG:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn main() {
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void =
// REWRITES-DAG:         unsafe { calloc((128 as u64) as usize, (1 as u64) as usize) };
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut T = {{__v[0-9]+}} as *mut T;
// REWRITES-DAG:     unsafe { foo(unsafe { std::ptr::addr_of_mut!((*{{__v[0-9]+}}).s) }) };
// REWRITES-DAG:     unsafe { check({{__v[0-9]+}}) };
// REWRITES-DAG:     unsafe { free({{__v[0-9]+}} as *mut core::ffi::c_void) };
// REWRITES-DAG:     std::process::exit(0 as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
