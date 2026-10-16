extern void abort(void);

typedef struct foo_t {
  unsigned int blksz;
  unsigned int bf_cnt;
} foo_t;

#define _RNDUP(x, unit)   ((((x) + (unit) - 1) / (unit)) * (unit))
#define _RNDDOWN(x, unit) ((x) - ((x) % (unit)))

long long foo(foo_t *const pxp, long long offset, unsigned int extent) {
  long long    blkoffset = _RNDDOWN(offset, (long long)pxp->blksz);
  unsigned int diff      = (unsigned int)(offset - blkoffset);
  unsigned int blkextent = _RNDUP(diff + extent, pxp->blksz);

  if (pxp->blksz < blkextent)
    return -1LL;

  if (pxp->bf_cnt > pxp->blksz)
    pxp->bf_cnt = pxp->blksz;

  return blkoffset;
}

int main() {
  foo_t     x;
  long long xx;

  x.blksz  = 8192;
  x.bf_cnt = 0;
  xx       = foo(&x, 0, 4096);
  if (xx != 0LL)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct foo_t {
// CHECK-NEXT:     blksz: u32,
// CHECK-NEXT:     bf_cnt: u32,
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut pxp: &mut foo_t, mut offset: i64, mut extent: u32) -> i64 {
// CHECK-NEXT:     let mut blkoffset: i64 = offset
// CHECK-NEXT:         - offset % (((unsafe { (*(pxp as *mut foo_t)).blksz }) as u64) as i64);
// CHECK-NEXT:     let mut diff: u32 = ((offset - blkoffset) as i32) as u32;
// CHECK-NEXT:     let mut blkextent: u32 = (diff
// CHECK-NEXT:         .wrapping_add(extent)
// CHECK-NEXT:         .wrapping_add(unsafe { (*(pxp as *mut foo_t)).blksz })
// CHECK-NEXT:         .wrapping_sub((1 as i32) as u32) / unsafe { (*(pxp as *mut foo_t)).blksz })
// CHECK-NEXT:         .wrapping_mul(unsafe { (*(pxp as *mut foo_t)).blksz });
// CHECK-NEXT:     if (unsafe { (*(pxp as *mut foo_t)).blksz }) < blkextent {
// CHECK-NEXT:         return -(1 as i64);
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { (*(pxp as *mut foo_t)).bf_cnt })
// CHECK-NEXT:         > unsafe { (*(pxp as *mut foo_t)).blksz }
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*(pxp as *mut foo_t)).bf_cnt = unsafe { (*(pxp as *mut foo_t)).blksz };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     blkoffset
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: foo_t = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut xx: i64 = 0 as i64;
// CHECK-NEXT:     x.blksz = (8192 as i32) as u32;
// CHECK-NEXT:     x.bf_cnt = (0 as i32) as u32;
// CHECK-NEXT:     xx = foo(
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(x)) },
// CHECK-NEXT:         (0 as i32) as i64,
// CHECK-NEXT:         (4096 as i32) as u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     if xx != (0 as i64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
