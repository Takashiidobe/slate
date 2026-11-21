/* { dg-options "-fno-strict-overflow" } */
#include <stdio.h>
#include <string.h>

typedef unsigned char u8;

struct driver {
  const u8 *(*fixup)(int product, u8 *buf, unsigned *rsize);
};

static u8 *inner_fixup(u8 *rdesc, unsigned *rsize) {
  if (*rsize == 6 && !memcmp(&rdesc[1], "\x02\x07", 2) && rdesc[4] == 0x40) {
    rdesc[1] = 0xff;
    rdesc[3] = 0x08;
  }
  return rdesc;
}

static const u8 *outer_fixup(int product, u8 *rdesc, unsigned *rsize) {
  if (product == 1)
    rdesc = inner_fixup(rdesc, rsize);
  return rdesc;
}

static struct driver drv = {.fixup = outer_fixup};

static void fill(int **slots, int n, int *v) {
  for (int i = 0; i < n; i++)
    slots[i] = v;
}

static int sum(const int *values, int n) {
  int total = 0;
  for (int i = 0; i < n; i++)
    total += values[i];
  return total;
}

int main(void) {
  u8 buf[6] = {1, 2, 7, 4, 0x40, 0};
  unsigned n = 6;
  const u8 *r = drv.fixup(1, buf, &n);
  printf("%d %d %d\n", r[1], r[3], r[4]);
  int x = 3, *slots[3];
  fill(slots, 3, &x);
  printf("%d\n", *slots[0] + *slots[2]);
  int values[4] = {1, 2, 3, 4};
  printf("%d\n", sum(values, 4));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-fno-strict-overflow" }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct driver {
// CHECK-NEXT:     fixup: Option<unsafe extern "C-unwind" fn(i32, *mut u8, *mut u32) -> *const u8>,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: unsafe impl<T> Sync for __SlateAlign16<T> {}
// CHECK-EMPTY:
// CHECK-NEXT: fn inner_fixup(rdesc: *mut u8, rsize: *mut u32) -> *mut u8 {
// CHECK-NEXT:     if (unsafe { *rsize }) == 6
// CHECK-NEXT:         && (unsafe {
// CHECK-NEXT:             memcmp(
// CHECK-NEXT:                 (rdesc.wrapping_offset(1 as isize) as *const ())
// CHECK-NEXT:                     as *const core::ffi::c_void,
// CHECK-NEXT:                 (b"\x02\x07\0".as_ptr() as *const ()) as *const core::ffi::c_void,
// CHECK-NEXT:                 2u64 as usize,
// CHECK-NEXT:             ) as i32
// CHECK-NEXT:         }) == 0 && ((unsafe { *rdesc.wrapping_offset(4 as isize) }) as i32) == 64 {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *rdesc.wrapping_offset(1 as isize) = 255;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *rdesc.wrapping_offset(3 as isize) = 8;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     rdesc
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: use slate_support::memcmp;
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn outer_fixup(
// CHECK-NEXT:     product: i32,
// CHECK-NEXT:     mut rdesc: *mut u8,
// CHECK-NEXT:     rsize: *mut u32,
// CHECK-NEXT: ) -> *const u8 {
// CHECK-NEXT:     if product == 1 {
// CHECK-NEXT:         rdesc = inner_fixup(rdesc, rsize);
// CHECK-NEXT:     }
// CHECK-NEXT:     rdesc as *const u8
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut drv: driver = driver { fixup: Some(outer_fixup) };
// CHECK-EMPTY:
// CHECK-NEXT: fn fill(slots: &mut [*mut i32], v: *mut i32) {
// CHECK-NEXT:     let n: i32 = slots.len() as i32;
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         slots[i as usize] = v;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn sum(values: &[i32]) -> i32 {
// CHECK-NEXT:     let n: i32 = values.len() as i32;
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         total = total.wrapping_add(values[i as usize]);
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut buf: [u8; 6] = [1, 2, 7, 4, 64, 0];
// CHECK-NEXT:     let mut n: u32 = 6;
// CHECK-NEXT:     let r: *const u8 = unsafe {
// CHECK-NEXT:         unsafe { drv.fixup }
// CHECK-NEXT:             .unwrap()(1i32, buf.as_mut_ptr() as *mut u8, core::ptr::addr_of_mut!(n))
// CHECK-NEXT:     };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}",
// CHECK-NEXT:         (unsafe { *r.wrapping_offset(1 as isize) }) as i32,
// CHECK-NEXT:         (unsafe { *r.wrapping_offset(3 as isize) }) as i32,
// CHECK-NEXT:         (unsafe { *r.wrapping_offset(4 as isize) }) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     let mut x: i32 = 3;
// CHECK-NEXT:     let mut slots: __SlateAlign16<[*mut i32; 3]> = __SlateAlign16([0 as *mut i32; 3]);
// CHECK-NEXT:     {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]] = core::ptr::addr_of_mut!(x);
// CHECK-NEXT:         fill(slots.0.as_mut_slice(), [[T_0]])
// CHECK-NEXT:     };
// CHECK-NEXT:     println!("{}", unsafe { *slots.0[0] }.wrapping_add(unsafe { *slots.0[2] }));
// CHECK-NEXT:     let mut values: __SlateAlign16<[i32; 4]> = __SlateAlign16([1i32, 2i32, 3i32, 4i32]);
// CHECK-NEXT:     println!("{}", sum(values.0.as_slice()));
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
