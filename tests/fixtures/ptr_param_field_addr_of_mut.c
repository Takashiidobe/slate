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
  // @lowering-begin
  // @rewrite-begin
  freeDict(&(ress->dict));
  syncDestroy(&ress->io);
  // @rewrite-end
  // @lowering-end
}

// @lowering-fn-begin
// @rewrite-fn-begin
int main(void) {
  cRess_t r = {.dict = {.x = 5}, .cctx = 9, .io = {.y = 7}};
  freeCResources(&r);
  printf("%d %d %d\n", r.dict.x, r.io.y, r.cctx);
  return 0;
}
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: freeDict(unsafe { std::ptr::addr_of_mut!((*{{arg[0-9]+}}).dict) });
// LOWERING-DAG: syncDestroy(unsafe { std::ptr::addr_of_mut!((*{{arg[0-9]+}}).io) });
// LOWERING-DAG: fn main() {
// LOWERING-DAG:     let mut __retval: i32 = 0;
// LOWERING-DAG:     let mut r: cRess_t = cRess_t {
// LOWERING-DAG:         dict: FIO_Dict_t { x: 0 },
// LOWERING-DAG:         cctx: 0,
// LOWERING-DAG:         io: FIO_SyncCompressIO { y: 0 },
// LOWERING-DAG:     };
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     __retval = {{_v[0-9]+}};
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{_v[0-9]+}}: cRess_t = cRess_t {
// LOWERING-DAG:             dict: FIO_Dict_t { x: 5 },
// LOWERING-DAG:             cctx: 9,
// LOWERING-DAG:             io: FIO_SyncCompressIO { y: 7 },
// LOWERING-DAG:         };
// LOWERING-DAG:         r = {{_v[0-9]+}};
// LOWERING-DAG:         freeCResources(std::ptr::addr_of_mut!(r));
// LOWERING-DAG:         let {{_v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = r.dict.x;
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = r.io.y;
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = r.cctx;
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         __retval = {{_v[0-9]+}};
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-DAG:         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-DAG:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: freeDict(unsafe { std::ptr::addr_of_mut!((*({{arg[0-9]+}} as *mut cRess_t)).dict) });
// REWRITES-DAG: syncDestroy(unsafe { std::ptr::addr_of_mut!((*({{arg[0-9]+}} as *mut cRess_t)).io) });
// REWRITES-DAG: fn main() {
// REWRITES-DAG:     let mut __retval: i32 = 0;
// REWRITES-DAG:     let mut r: cRess_t = cRess_t {
// REWRITES-DAG:         dict: FIO_Dict_t { x: 0 },
// REWRITES-DAG:         cctx: 0,
// REWRITES-DAG:         io: FIO_SyncCompressIO { y: 0 },
// REWRITES-DAG:     };
// REWRITES-DAG:     r = cRess_t {
// REWRITES-DAG:         dict: FIO_Dict_t { x: 5 },
// REWRITES-DAG:         cctx: 9,
// REWRITES-DAG:         io: FIO_SyncCompressIO { y: 7 },
// REWRITES-DAG:     };
// REWRITES-DAG:     freeCResources(unsafe { &mut (*std::ptr::addr_of_mut!(r)) });
// REWRITES-DAG:     let {{_v[0-9]+}}: i32 = r.dict.x;
// REWRITES-DAG:     let {{_v[0-9]+}}: i32 = r.io.y;
// REWRITES-DAG:     let {{_v[0-9]+}}: i32 = r.cctx;
// REWRITES-DAG:     unsafe { printf(c"%d %d %d\n".as_ptr(), {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-DAG:     __retval = 0;
// REWRITES-DAG:     std::process::exit(__retval as i32);
// REWRITES-DAG:     std::process::exit(__retval as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
