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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = unsafe { compute(std::ptr::addr_of_mut!(s), {{__v[0-9]+}}) };
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = unsafe { compute(std::ptr::addr_of_mut!(s), {{__v[0-9]+}}) };
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = b"%d %d\n\0".as_ptr() as *mut u8;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: unsafe {
// COMMON-REWRITES-DAG:     printf(
// COMMON-REWRITES-DAG:         c"%d %d\n".as_ptr(),
// COMMON-REWRITES-DAG:         unsafe { compute(std::ptr::addr_of_mut!(s), 0) },
// COMMON-REWRITES-DAG:         unsafe { compute(std::ptr::addr_of_mut!(s), 1) },
// COMMON-REWRITES-DAG:     )
// COMMON-REWRITES-DAG: };
// SLATE-FILECHECK-END common-rewrites
