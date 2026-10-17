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

// @check-fn-begin
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
  printf("%d %d\n", compute(&s, 0), compute(&s, 1));
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut inr: [[SLATERECORD_0:__SlateRecord[0-9]+]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     inr.pad = 0;
// CHECK-NEXT:     inr.x = 3;
// CHECK-NEXT:     inr.y = 4;
// CHECK-NEXT:     let mut ou: [[SLATERECORD_1:__SlateRecord[0-9]+]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ou.lead = 0;
// CHECK-NEXT:     ou.r#in = inr;
// CHECK-NEXT:     let mut s: [[SLATERECORD_2:__SlateRecord[0-9]+]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     s.dict = std::ptr::addr_of_mut!(ou);
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}",
// CHECK-NEXT:         compute(
// CHECK-NEXT:             unsafe { &(*(std::ptr::addr_of_mut!(s) as *const [[SLATERECORD_2]])) },
// CHECK-NEXT:             0 as i32
// CHECK-NEXT:         ),
// CHECK-NEXT:         compute(
// CHECK-NEXT:             unsafe { &(*(std::ptr::addr_of_mut!(s) as *const [[SLATERECORD_2]])) },
// CHECK-NEXT:             1 as i32
// CHECK-NEXT:         )
// CHECK-NEXT:     );
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     0
// CHECK: {{^}}}
// SLATE-FILECHECK-END
