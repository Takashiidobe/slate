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
// CHECK-LABEL: {{^}}fn freeCResources(ress: &mut {{__SlateRecord[0-9]+}}) {
// CHECK-NEXT:     freeDict(unsafe { std::ptr::addr_of_mut!((*(ress as *mut [[SLATERECORD_0:__SlateRecord[0-9]+]])).dict) });
// CHECK-NEXT:     syncDestroy(unsafe { std::ptr::addr_of_mut!((*(ress as *mut [[SLATERECORD_0]])).io) });
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut r: [[SLATERECORD_0:__SlateRecord[0-9]+]] = [[SLATERECORD_0]] {
// CHECK-NEXT:         dict: [[SLATERECORD_1:__SlateRecord[0-9]+]] { x: 5 },
// CHECK-NEXT:         cctx: 9,
// CHECK-NEXT:         io: [[SLATERECORD_2:__SlateRecord[0-9]+]] { y: 7 },
// CHECK-NEXT:     };
// CHECK-NEXT:     freeCResources(unsafe { &mut (*std::ptr::addr_of_mut!(r)) });
// CHECK-NEXT:     println!("{} {} {}", r.dict.x, r.io.y, r.cctx);
// CHECK-NEXT:     0
// CHECK: {{^}}}
// SLATE-FILECHECK-END
