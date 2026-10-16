// { dg-additional-options "-O2" }
#include <stdio.h>

typedef struct {
  int x;
} FIO_Dict_t;

typedef struct {
  int y;
} FIO_SyncCompressIO;

typedef struct {
  FIO_Dict_t         dict;
  int                cctx;
  FIO_SyncCompressIO io;
} cRess_t;

static void freeDict(FIO_Dict_t *dict) { dict->x = 0; }

static void syncDestroy(FIO_SyncCompressIO *io) { io->y = 0; }

// @check-fn-begin
static void freeCResources(cRess_t *const ress) {
  freeDict(&(ress->dict));
  syncDestroy(&ress->io);
}
// @check-fn-end

// @check-fn-begin
int main(void) {
  cRess_t r = {.dict = {.x = 5}, .cctx = 9, .io = {.y = 7}};
  freeCResources(&r);
  printf("%d %d %d\n", r.dict.x, r.io.y, r.cctx);
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn freeCResources(mut ress: &mut {{__SlateRecord[0-9]+}}) {
// CHECK-NEXT:     freeDict(unsafe { std::ptr::addr_of_mut!((*(ress as *mut [[SLATERECORD_0:__SlateRecord[0-9]+]])).dict) });
// CHECK-NEXT:     syncDestroy(unsafe { std::ptr::addr_of_mut!((*(ress as *mut [[SLATERECORD_0]])).io) });
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut r: [[SLATERECORD_0:__SlateRecord[0-9]+]] = [[SLATERECORD_0]] {
// CHECK-NEXT:         dict: [[SLATERECORD_1:__SlateRecord[0-9]+]] { x: 5 as i32 },
// CHECK-NEXT:         cctx: 9 as i32,
// CHECK-NEXT:         io: [[SLATERECORD_2:__SlateRecord[0-9]+]] { y: 7 as i32 },
// CHECK-NEXT:     };
// CHECK-NEXT:     freeCResources(unsafe { &mut (*std::ptr::addr_of_mut!(r)) });
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %d %d\n".as_ptr() as *const i8,
// CHECK-NEXT:             r.dict.x,
// CHECK-NEXT:             r.io.y,
// CHECK-NEXT:             r.cctx,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     0 as i32
// CHECK: {{^}}}
// SLATE-FILECHECK-END
