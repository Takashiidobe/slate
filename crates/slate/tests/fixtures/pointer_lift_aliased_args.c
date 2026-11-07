#include <stdio.h>

typedef struct {
  unsigned w[4];
} bigval;

struct poly {
  bigval h;
};

// @check-fn-begin
static void bigval_add(bigval *r, const bigval *a, const bigval *b) {
  unsigned v0 = a->w[0] + b->w[0];
  unsigned v1 = a->w[1] + b->w[1];
  r->w[0]     = v0;
  r->w[1]     = v1 + (v0 < a->w[0]);
}
// @check-fn-end

// @check-fn-begin
static unsigned bigval_sum(const bigval *a, const bigval *b) {
  return a->w[0] + a->w[1] + b->w[0] + b->w[1];
}
// @check-fn-end

// @check-fn-begin
static void poly_feed(struct poly *ctx, unsigned x) {
  bigval c = {{x, x + 1, 0, 0}};
  bigval_add(&c, &c, &ctx->h);
  bigval_add(&ctx->h, &c, &ctx->h);
}
// @check-fn-end

int main(void) {
  struct poly ctx = {{{0xfffffff0u, 2, 0, 0}}};
  poly_feed(&ctx, 3);
  poly_feed(&ctx, 0x20);
  bigval d = {{5, 6, 0, 0}};
  printf("%u %u %u\n", ctx.h.w[0], ctx.h.w[1], bigval_sum(&d, &d));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn bigval_add(
// CHECK-NEXT:     r: *mut __SlateRecord_pointer_lift_aliased_args_c_5,
// CHECK-NEXT:     a: *const __SlateRecord_pointer_lift_aliased_args_c_5,
// CHECK-NEXT:     b: *const __SlateRecord_pointer_lift_aliased_args_c_5,
// CHECK-NEXT: ) {
// CHECK-NEXT:     let v0: u32 = unsafe { *((*a).w.as_ptr() as *const u32).add(0) }
// CHECK-NEXT:         .wrapping_add(unsafe { *((*b).w.as_ptr() as *const u32).add(0) });
// CHECK-NEXT:     let v1: u32 = unsafe { *((*a).w.as_ptr() as *const u32).add(1) }
// CHECK-NEXT:         .wrapping_add(unsafe { *((*b).w.as_ptr() as *const u32).add(1) });
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *((*r).w.as_mut_ptr() as *mut u32).add(0) = v0;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *((*r).w.as_mut_ptr() as *mut u32).add(1) = v1
// CHECK-NEXT:             .wrapping_add(
// CHECK-NEXT:                 ((v0 < *((*a).w.as_ptr() as *const u32).add(0)) as i32) as u32,
// CHECK-NEXT:             );
// CHECK-NEXT:     }
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn bigval_sum(
// CHECK-NEXT:     a: &__SlateRecord_pointer_lift_aliased_args_c_5,
// CHECK-NEXT:     b: &__SlateRecord_pointer_lift_aliased_args_c_5,
// CHECK-NEXT: ) -> u32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *((*(a as *const __SlateRecord_pointer_lift_aliased_args_c_5)).w.as_ptr()
// CHECK-NEXT:             as *const u32)
// CHECK-NEXT:             .add(0)
// CHECK-NEXT:     }
// CHECK-NEXT:         .wrapping_add(unsafe {
// CHECK-NEXT:             *((*(a as *const __SlateRecord_pointer_lift_aliased_args_c_5)).w.as_ptr()
// CHECK-NEXT:                 as *const u32)
// CHECK-NEXT:                 .add(1)
// CHECK-NEXT:         })
// CHECK-NEXT:         .wrapping_add(unsafe {
// CHECK-NEXT:             *((*(b as *const __SlateRecord_pointer_lift_aliased_args_c_5)).w.as_ptr()
// CHECK-NEXT:                 as *const u32)
// CHECK-NEXT:                 .add(0)
// CHECK-NEXT:         })
// CHECK-NEXT:         .wrapping_add(unsafe {
// CHECK-NEXT:             *((*(b as *const __SlateRecord_pointer_lift_aliased_args_c_5)).w.as_ptr()
// CHECK-NEXT:                 as *const u32)
// CHECK-NEXT:                 .add(1)
// CHECK-NEXT:         })
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn poly_feed(ctx: &mut poly, x: u32) {
// CHECK-NEXT:     let mut c: __SlateRecord_pointer_lift_aliased_args_c_5 = __SlateRecord_pointer_lift_aliased_args_c_5 {
// CHECK-NEXT:         w: [x, x.wrapping_add(1), 0, 0],
// CHECK-NEXT:     };
// CHECK-NEXT:     bigval_add(
// CHECK-NEXT:         std::ptr::addr_of_mut!(c),
// CHECK-NEXT:         std::ptr::addr_of_mut!(c) as *const __SlateRecord_pointer_lift_aliased_args_c_5,
// CHECK-NEXT:         (unsafe { std::ptr::addr_of_mut!((* (ctx as * mut poly)).h) })
// CHECK-NEXT:             as *const __SlateRecord_pointer_lift_aliased_args_c_5,
// CHECK-NEXT:     );
// CHECK-NEXT:     bigval_add(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!((* (ctx as * mut poly)).h) },
// CHECK-NEXT:         std::ptr::addr_of_mut!(c) as *const __SlateRecord_pointer_lift_aliased_args_c_5,
// CHECK-NEXT:         (unsafe { std::ptr::addr_of_mut!((* (ctx as * mut poly)).h) })
// CHECK-NEXT:             as *const __SlateRecord_pointer_lift_aliased_args_c_5,
// CHECK-NEXT:     );
// CHECK: {{^}}}
// SLATE-FILECHECK-END
