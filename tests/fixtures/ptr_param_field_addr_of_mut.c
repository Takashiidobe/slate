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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: freeDict(unsafe { std::ptr::addr_of_mut!((*{{arg[0-9]+}}).dict) });
// COMMON-LOWERING-DAG: syncDestroy(unsafe { std::ptr::addr_of_mut!((*{{arg[0-9]+}}).io) });
// COMMON-LOWERING-DAG: fn main() {
// COMMON-LOWERING-DAG:     let mut __retval: i32 = 0;
// COMMON-LOWERING-DAG:     let mut r: cRess_t = cRess_t {
// COMMON-LOWERING-DAG:         dict: FIO_Dict_t { x: 0 },
// COMMON-LOWERING-DAG:         cctx: 0,
// COMMON-LOWERING-DAG:         io: FIO_SyncCompressIO { y: 0 },
// COMMON-LOWERING-DAG:     };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:     __retval = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     {
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: cRess_t = cRess_t {
// COMMON-LOWERING-DAG:             dict: FIO_Dict_t { x: 5 },
// COMMON-LOWERING-DAG:             cctx: 9,
// COMMON-LOWERING-DAG:             io: FIO_SyncCompressIO { y: 7 },
// COMMON-LOWERING-DAG:         };
// COMMON-LOWERING-DAG:         r = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         freeCResources(std::ptr::addr_of_mut!(r));
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: i32 = r.dict.x;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: i32 = r.io.y;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: i32 = r.cctx;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-DAG:         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-DAG:     }
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-DAG:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-DAG: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: *mut u8 = b"%d %d %d\n\0".as_ptr() as *mut u8;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: freeDict(unsafe { std::ptr::addr_of_mut!((*({{arg[0-9]+}} as *mut cRess_t)).dict) });
// COMMON-REWRITES-DAG: syncDestroy(unsafe { std::ptr::addr_of_mut!((*({{arg[0-9]+}} as *mut cRess_t)).io) });
// COMMON-REWRITES-DAG: fn main() {
// COMMON-REWRITES-DAG:     let mut __retval: i32 = 0;
// COMMON-REWRITES-DAG:     let mut r: cRess_t = cRess_t {
// COMMON-REWRITES-DAG:         dict: FIO_Dict_t { x: 0 },
// COMMON-REWRITES-DAG:         cctx: 0,
// COMMON-REWRITES-DAG:         io: FIO_SyncCompressIO { y: 0 },
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     r = cRess_t {
// COMMON-REWRITES-DAG:         dict: FIO_Dict_t { x: 5 },
// COMMON-REWRITES-DAG:         cctx: 9,
// COMMON-REWRITES-DAG:         io: FIO_SyncCompressIO { y: 7 },
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     freeCResources(unsafe { &mut (*std::ptr::addr_of_mut!(r)) });
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = r.dict.x;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = r.io.y;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = r.cctx;
// COMMON-REWRITES-DAG:     unsafe { printf(c"%d %d %d\n".as_ptr(), {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-REWRITES-DAG:     __retval = 0;
// COMMON-REWRITES-DAG:     std::process::exit(__retval as i32);
// COMMON-REWRITES-DAG:     std::process::exit(__retval as i32);
// COMMON-REWRITES-DAG: }
// SLATE-FILECHECK-END common-rewrites
