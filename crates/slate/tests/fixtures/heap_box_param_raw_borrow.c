#include <stdio.h>
#include <stdlib.h>

typedef struct {
  size_t loaded;
  size_t size;
} Ctx;

void consume(Ctx *ctx, size_t n) { ctx->loaded -= n; }

void (*hook)(Ctx *, size_t) = consume;

void destroy(Ctx *ctx) {
  consume(ctx, ctx->loaded);
  free(ctx);
}

int main(void) {
  Ctx *ctx = calloc(1, sizeof(Ctx));
  ctx->loaded = 5;
  ctx->size = 3;
  printf("%zu\n", ctx->size);
  destroy(ctx);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __SlateRecord_heap_box_param_raw_borrow_c_7 {
// CHECK-NEXT:     loaded: u64,
// CHECK-NEXT:     size: u64,
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: extern "C-unwind" fn consume(
// CHECK-NEXT:     [[V_0:__v[0-9]+]]: *mut __SlateRecord_heap_box_param_raw_borrow_c_7,
// CHECK-NEXT:     n: u64,
// CHECK-NEXT: ) {
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: u64 = unsafe { (*[[V_0]]).loaded };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*[[V_0]]).loaded = [[V_1]].wrapping_sub(n);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: use slate_support::calloc;
// CHECK-NEXT: use slate_support::free;
// CHECK-NEXT: static mut hook: Option<
// CHECK-NEXT:     unsafe extern "C-unwind" fn(*mut __SlateRecord_heap_box_param_raw_borrow_c_7, u64),
// CHECK-NEXT: > = Some(consume);
// CHECK-NEXT: fn destroy(mut ctx: Box<__SlateRecord_heap_box_param_raw_borrow_c_7>) {
// CHECK-NEXT:     consume(
// CHECK-NEXT:         &mut (*ctx) as *mut __SlateRecord_heap_box_param_raw_borrow_c_7,
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*(&(*ctx) as *const __SlateRecord_heap_box_param_raw_borrow_c_7)).loaded
// CHECK-NEXT:         },
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let ctx: *mut __SlateRecord_heap_box_param_raw_borrow_c_7 = (unsafe {
// CHECK-NEXT:         calloc(
// CHECK-NEXT:             1u64 as usize,
// CHECK-NEXT:             (std::mem::size_of::<__SlateRecord_heap_box_param_raw_borrow_c_7>() as u64)
// CHECK-NEXT:                 as usize,
// CHECK-NEXT:         ) as *mut ()
// CHECK-NEXT:     }) as *mut __SlateRecord_heap_box_param_raw_borrow_c_7;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*ctx).loaded = 5;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*ctx).size = 3;
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%zu\n".as_ptr(), (*ctx).size) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     destroy(unsafe { Box::from_raw(ctx) });
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
