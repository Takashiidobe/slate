/* { dg-add-options stack_size } */

#include <string.h>

void abort(void);
void exit(int);

#if defined(STACK_SIZE)
#define MEMCPY_SIZE (STACK_SIZE / 3)
#else
#define MEMCPY_SIZE (1 << 17)
#endif

void *copy(void *o, const void *i, unsigned l) { return memcpy(o, i, l); }

int main(void) {
  unsigned      i;
  unsigned char src[MEMCPY_SIZE];
  unsigned char dst[MEMCPY_SIZE];

  for (i = 0; i < MEMCPY_SIZE; i++)
    src[i] = (unsigned char)i, dst[i] = 0;

  (void)memcpy(dst, src, MEMCPY_SIZE / 128);

  for (i = 0; i < MEMCPY_SIZE / 128; i++)
    if (dst[i] != (unsigned char)i)
      abort();

  (void)memset(dst, 1, MEMCPY_SIZE / 128);

  for (i = 0; i < MEMCPY_SIZE / 128; i++)
    if (dst[i] != 1)
      abort();

  (void)memcpy(dst, src, MEMCPY_SIZE);

  for (i = 0; i < MEMCPY_SIZE; i++)
    if (dst[i] != (unsigned char)i)
      abort();

  (void)memset(dst, 0, MEMCPY_SIZE);

  for (i = 0; i < MEMCPY_SIZE; i++)
    if (dst[i] != 0)
      abort();

  (void)copy(dst, src, MEMCPY_SIZE / 128);

  for (i = 0; i < MEMCPY_SIZE / 128; i++)
    if (dst[i] != (unsigned char)i)
      abort();

  (void)memset(dst, 0, MEMCPY_SIZE);

  (void)copy(dst, src, MEMCPY_SIZE);

  for (i = 0; i < MEMCPY_SIZE; i++)
    if (dst[i] != (unsigned char)i)
      abort();

  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-add-options stack_size }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn memcpy(__dest: *mut (), __src: *const (), __n: u64) -> *mut ();
// CHECK-NEXT:     fn memset(__s: *mut (), __c: i32, __n: u64) -> *mut ();
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn copy(mut o: *mut (), mut i: *const (), mut l: u32) -> *mut () {
// CHECK-NEXT:     unsafe { memcpy(o, i, l as u64) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: u32 = (0 as i32) as u32;
// CHECK-NEXT:     let mut src: __SlateAlign16<[u8; 131072]> = __SlateAlign16([0 as u8; 131072]);
// CHECK-NEXT:     let mut dst: __SlateAlign16<[u8; 131072]> = __SlateAlign16([0 as u8; 131072]);
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (((1 as i32) << (17 as i32)) as u32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (src.0.as_mut_ptr() as *mut u8).offset(i as isize) } = i as u8;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (dst.0.as_mut_ptr() as *mut u8).offset(i as isize) } =
// CHECK-NEXT:                 ((0 as i32) as i8) as u8;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: u32 = i;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: u32 = [[V_1]].wrapping_add((1 as i32) as u32);
// CHECK-NEXT:         i = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             dst.0.as_mut_ptr() as *mut (),
// CHECK-NEXT:             src.0.as_mut_ptr() as *const (),
// CHECK-NEXT:             ((((1 as i32) << (17 as i32)) / (128 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     i = (0 as i32) as u32;
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < ((((1 as i32) << (17 as i32)) / (128 as i32)) as u32) {
// CHECK-NEXT:         if (((unsafe { *unsafe { (dst.0.as_mut_ptr() as *mut u8).offset(i as isize) } }) as u32)
// CHECK-NEXT:             as i32)
// CHECK-NEXT:             != (((i as u8) as u32) as i32)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: u32 = i;
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: u32 = [[V_3]].wrapping_add((1 as i32) as u32);
// CHECK-NEXT:         i = [[V_4]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             dst.0.as_mut_ptr() as *mut (),
// CHECK-NEXT:             1 as i32,
// CHECK-NEXT:             ((((1 as i32) << (17 as i32)) / (128 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     i = (0 as i32) as u32;
// CHECK-NEXT:     '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while i < ((((1 as i32) << (17 as i32)) / (128 as i32)) as u32) {
// CHECK-NEXT:         if (((unsafe { *unsafe { (dst.0.as_mut_ptr() as *mut u8).offset(i as isize) } }) as u32)
// CHECK-NEXT:             as i32)
// CHECK-NEXT:             != (1 as i32)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: u32 = i;
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: u32 = [[V_5]].wrapping_add((1 as i32) as u32);
// CHECK-NEXT:         i = [[V_6]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             dst.0.as_mut_ptr() as *mut (),
// CHECK-NEXT:             src.0.as_mut_ptr() as *const (),
// CHECK-NEXT:             (((1 as i32) << (17 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     i = (0 as i32) as u32;
// CHECK-NEXT:     '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: while i < (((1 as i32) << (17 as i32)) as u32) {
// CHECK-NEXT:         if (((unsafe { *unsafe { (dst.0.as_mut_ptr() as *mut u8).offset(i as isize) } }) as u32)
// CHECK-NEXT:             as i32)
// CHECK-NEXT:             != (((i as u8) as u32) as i32)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_7:__v[0-9]+]]: u32 = i;
// CHECK-NEXT:         let mut [[V_8:__v[0-9]+]]: u32 = [[V_7]].wrapping_add((1 as i32) as u32);
// CHECK-NEXT:         i = [[V_8]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             dst.0.as_mut_ptr() as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((1 as i32) << (17 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     i = (0 as i32) as u32;
// CHECK-NEXT:     '[[SLATE_BREAK_4:__slate_break_[0-9]+]]: while i < (((1 as i32) << (17 as i32)) as u32) {
// CHECK-NEXT:         if (((unsafe { *unsafe { (dst.0.as_mut_ptr() as *mut u8).offset(i as isize) } }) as u32)
// CHECK-NEXT:             as i32)
// CHECK-NEXT:             != (0 as i32)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_9:__v[0-9]+]]: u32 = i;
// CHECK-NEXT:         let mut [[V_10:__v[0-9]+]]: u32 = [[V_9]].wrapping_add((1 as i32) as u32);
// CHECK-NEXT:         i = [[V_10]];
// CHECK-NEXT:     }
// CHECK-NEXT:     copy(
// CHECK-NEXT:         dst.0.as_mut_ptr() as *mut (),
// CHECK-NEXT:         src.0.as_mut_ptr() as *const (),
// CHECK-NEXT:         (((1 as i32) << (17 as i32)) / (128 as i32)) as u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     i = (0 as i32) as u32;
// CHECK-NEXT:     '[[SLATE_BREAK_5:__slate_break_[0-9]+]]: while i < ((((1 as i32) << (17 as i32)) / (128 as i32)) as u32) {
// CHECK-NEXT:         if (((unsafe { *unsafe { (dst.0.as_mut_ptr() as *mut u8).offset(i as isize) } }) as u32)
// CHECK-NEXT:             as i32)
// CHECK-NEXT:             != (((i as u8) as u32) as i32)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_11:__v[0-9]+]]: u32 = i;
// CHECK-NEXT:         let mut [[V_12:__v[0-9]+]]: u32 = [[V_11]].wrapping_add((1 as i32) as u32);
// CHECK-NEXT:         i = [[V_12]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             dst.0.as_mut_ptr() as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((1 as i32) << (17 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     copy(
// CHECK-NEXT:         dst.0.as_mut_ptr() as *mut (),
// CHECK-NEXT:         src.0.as_mut_ptr() as *const (),
// CHECK-NEXT:         ((1 as i32) << (17 as i32)) as u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     i = (0 as i32) as u32;
// CHECK-NEXT:     '[[SLATE_BREAK_6:__slate_break_[0-9]+]]: while i < (((1 as i32) << (17 as i32)) as u32) {
// CHECK-NEXT:         if (((unsafe { *unsafe { (dst.0.as_mut_ptr() as *mut u8).offset(i as isize) } }) as u32)
// CHECK-NEXT:             as i32)
// CHECK-NEXT:             != (((i as u8) as u32) as i32)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_13:__v[0-9]+]]: u32 = i;
// CHECK-NEXT:         let mut [[V_14:__v[0-9]+]]: u32 = [[V_13]].wrapping_add((1 as i32) as u32);
// CHECK-NEXT:         i = [[V_14]];
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
