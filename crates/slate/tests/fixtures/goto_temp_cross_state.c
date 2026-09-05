// { dg-additional-options "-O2" }
#include <stdio.h>

typedef struct {
  int pad;
  int x;
  int y;
} inner_t;

typedef struct {
  int     lead;
  inner_t in;
} outer_t;

typedef struct {
  outer_t *dict;
} state_t;

int compute(const state_t *const ms, int flag) {
  const outer_t *const o   = ms->dict;
  const inner_t *const q   = &o->in;
  int                  acc = q->x;
  if (flag) {
    goto second;
  }
  acc += 100;
second:
  acc += q->y;
  return acc;
}

int main(void) {
  inner_t inr;
  inr.pad = 0;
  inr.x   = 3;
  inr.y   = 4;
  outer_t ou;
  ou.lead = 0;
  ou.in   = inr;
  state_t s;
  s.dict = &ou;
  // @lowering-begin
  // @rewrite-begin
  printf("%d %d\n", compute(&s, 0), compute(&s, 1));
  // @rewrite-end
  // @lowering-end
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = unsafe { compute(std::ptr::addr_of_mut!(s), {{_v[0-9]+}}) };
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = 1;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = unsafe { compute(std::ptr::addr_of_mut!(s), {{_v[0-9]+}}) };
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: unsafe {
// REWRITES-DAG:     printf(
// REWRITES-DAG:         c"%d %d\n".as_ptr(),
// REWRITES-DAG:         unsafe { compute(std::ptr::addr_of_mut!(s), 0) },
// REWRITES-DAG:         unsafe { compute(std::ptr::addr_of_mut!(s), 1) },
// REWRITES-DAG:     )
// REWRITES-DAG: };
// SLATE-FILECHECK-END rewrites
