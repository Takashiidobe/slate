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

static void freeCResources(cRess_t *const ress) {
  freeDict(&(ress->dict));
  syncDestroy(&ress->io);
}

int main(void) {
  cRess_t r = {.dict = {.x = 5}, .cctx = 9, .io = {.y = 7}};
  freeCResources(&r);
  printf("%d %d %d\n", r.dict.x, r.io.y, r.cctx);
  return 0;
}


