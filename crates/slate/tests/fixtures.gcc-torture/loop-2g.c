/* { dg-require-effective-target mmap } */
/* { dg-skip-if "the executable is at the same position the test tries to remap"
 * { m68k-*-linux* } } */

#include <limits.h>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#ifndef MAP_ANON
#ifdef MAP_ANONYMOUS
#define MAP_ANON MAP_ANONYMOUS
#else
#define MAP_ANON MAP_FILE
#endif
#endif
#ifndef MAP_FILE
#define MAP_FILE 0
#endif
#ifndef MAP_FIXED
#define MAP_FIXED 0
#endif

#define MAP_START (void *)0x7fff8000
#define MAP_LEN   0x10000

#define OFFSET (MAP_LEN / 2 - 2 * sizeof(char));

void f(int s, char *p) {
  int i;
  for (i = s; &p[i] < &p[40] && i >= 0; i++) {
    p[i] = -2;
  }
}

int main(void) {
#ifdef MAP_ANON
  char *p;
  int   dev_zero;

  dev_zero = open("/dev/zero", O_RDONLY);
  /* -1 is OK when we have MAP_ANON; else mmap will flag an error.  */
  if (INT_MAX != 0x7fffffffL || sizeof(char *) != sizeof(int))
    return 0;
  p = mmap(MAP_START, MAP_LEN, PROT_READ | PROT_WRITE,
           MAP_ANON | MAP_FIXED | MAP_PRIVATE, dev_zero, 0);
  if (p != (char *)-1) {
    p     += OFFSET;
    p[39]  = 0;
    f(0, p);
    if (p[39] != (char)-2)
      __builtin_abort();
    p[39] = 0;
    f(-1, p);
    if (p[39] != 0)
      __builtin_abort();
  }
#endif
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-require-effective-target mmap }
// CHECK-NEXT: //!
// CHECK-NEXT: //! { dg-skip-if "the executable is at the same position the test tries to remap"
// CHECK-NEXT: //! { m68k-*-linux* } }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn open(__file: *const i8, __oflag: i32, ...) -> i32;
// CHECK-NEXT:     fn mmap(
// CHECK-NEXT:         __addr: *mut (),
// CHECK-NEXT:         __len: u64,
// CHECK-NEXT:         __prot: i32,
// CHECK-NEXT:         __flags: i32,
// CHECK-NEXT:         __fd: i32,
// CHECK-NEXT:         __offset: i64,
// CHECK-NEXT:     ) -> *mut ();
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f(mut i: i32, mut p: *mut i8) {
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (unsafe { p.offset(i as isize) })
// CHECK-NEXT:         < unsafe { p.offset((40 as i32) as isize) }
// CHECK-NEXT:         && i >= (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { p.offset(i as isize) } = -(2 as i32) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         i = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut p: *mut i8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut dev_zero: i32 = unsafe { open(c"/dev/zero".as_ptr() as *const i8, 0 as i32) };
// CHECK-NEXT:     // -1 is OK when we have MAP_ANON; else mmap will flag an error.
// CHECK-NEXT:     if ((2147483647 as i32) as i64) != (2147483647 as i64) || (8 as u64) != (4 as u64) {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     p = (unsafe {
// CHECK-NEXT:         mmap(
// CHECK-NEXT:             (2147450880 as i32) as *mut (),
// CHECK-NEXT:             ((65536 as i32) as i64) as u64,
// CHECK-NEXT:             (1 as i32) | (2 as i32),
// CHECK-NEXT:             (32 as i32) | (16 as i32) | (2 as i32),
// CHECK-NEXT:             dev_zero,
// CHECK-NEXT:             (0 as i32) as i64,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) as *mut i8;
// CHECK-NEXT:     if p != (-(1 as i32) as *mut i8) {
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: *mut i8 = p;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: *mut i8 = unsafe {
// CHECK-NEXT:             [[V_2]].offset(
// CHECK-NEXT:                 ((((65536 as i32) / (2 as i32)) as i64) as u64)
// CHECK-NEXT:                     .wrapping_sub((((2 as i32) as i64) as u64).wrapping_mul(1 as u64))
// CHECK-NEXT:                     as isize,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         p = [[V_3]];
// CHECK-NEXT:         {}
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { p.offset((39 as i32) as isize) } = (0 as i32) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         f(0 as i32, p);
// CHECK-NEXT:         if ((unsafe { *unsafe { p.offset((39 as i32) as isize) } }) as i32)
// CHECK-NEXT:             != ((-(2 as i32) as i8) as i32)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { p.offset((39 as i32) as isize) } = (0 as i32) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         f(-(1 as i32), p);
// CHECK-NEXT:         if ((unsafe { *unsafe { p.offset((39 as i32) as isize) } }) as i32) != (0 as i32) {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
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
