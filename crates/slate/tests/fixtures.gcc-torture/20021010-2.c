/* cse.c failure on x86 target.
   Contributed by Stuart Hastings 10 Oct 2002 <stuart@apple.com> */
#include <stdlib.h>

typedef signed short SInt16;

typedef struct {
  SInt16 minx;
  SInt16 maxx;
  SInt16 miny;
  SInt16 maxy;
} IOGBounds;

int expectedwidth = 50;

unsigned int *global_vramPtr = (unsigned int *)0xa000;

IOGBounds global_bounds   = {100, 150, 100, 150};
IOGBounds global_saveRect = {75, 175, 75, 175};

int main(void) {
  unsigned int *vramPtr;
  int           width;
  IOGBounds     saveRect = global_saveRect;
  IOGBounds     bounds   = global_bounds;

  if (saveRect.minx < bounds.minx)
    saveRect.minx = bounds.minx;
  if (saveRect.maxx > bounds.maxx)
    saveRect.maxx = bounds.maxx;

  vramPtr = global_vramPtr + (saveRect.miny - bounds.miny);
  width   = saveRect.maxx - saveRect.minx;

  if (width != expectedwidth)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! cse.c failure on x86 target.
// CHECK-NEXT: //! Contributed by Stuart Hastings 10 Oct 2002 <stuart@apple.com>
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit(__status: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     minx: i16,
// CHECK-NEXT:     maxx: i16,
// CHECK-NEXT:     miny: i16,
// CHECK-NEXT:     maxy: i16,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut expectedwidth: i32 = 50 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut global_vramPtr: *mut u32 = (40960 as i32) as *mut u32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut global_bounds: [[SLATERECORD_0]] = [[SLATERECORD_0]] {
// CHECK-NEXT:     minx: (100 as i32) as i16,
// CHECK-NEXT:     maxx: (150 as i32) as i16,
// CHECK-NEXT:     miny: (100 as i32) as i16,
// CHECK-NEXT:     maxy: (150 as i32) as i16,
// CHECK-NEXT: };
// CHECK-EMPTY:
// CHECK-NEXT: static mut global_saveRect: [[SLATERECORD_0]] = [[SLATERECORD_0]] {
// CHECK-NEXT:     minx: (75 as i32) as i16,
// CHECK-NEXT:     maxx: (175 as i32) as i16,
// CHECK-NEXT:     miny: (75 as i32) as i16,
// CHECK-NEXT:     maxy: (175 as i32) as i16,
// CHECK-NEXT: };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut vramPtr: *mut u32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut width: i32 = 0 as i32;
// CHECK-NEXT:     let mut saveRect: [[SLATERECORD_0]] = unsafe { global_saveRect };
// CHECK-NEXT:     let mut bounds: [[SLATERECORD_0]] = unsafe { global_bounds };
// CHECK-NEXT:     if (saveRect.minx as i32) < (bounds.minx as i32) {
// CHECK-NEXT:         saveRect.minx = bounds.minx;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (saveRect.maxx as i32) > (bounds.maxx as i32) {
// CHECK-NEXT:         saveRect.maxx = bounds.maxx;
// CHECK-NEXT:     }
// CHECK-NEXT:     vramPtr = unsafe {
// CHECK-NEXT:         unsafe { global_vramPtr }.offset(((saveRect.miny as i32) - (bounds.miny as i32)) as isize)
// CHECK-NEXT:     };
// CHECK-NEXT:     width = (saveRect.maxx as i32) - (saveRect.minx as i32);
// CHECK-NEXT:     if width != unsafe { expectedwidth } {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
