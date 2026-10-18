#define _GNU_SOURCE
#define obstack_chunk_alloc malloc
#define obstack_chunk_free  free
#include <malloc.h>
#include <obstack.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int gnu_allocation_extensions(void) {
  int  *values    = reallocarray(NULL, 4, sizeof(*values));
  void *aligned   = memalign(64, 80);
  void *page      = valloc(1);
  void *rounded   = pvalloc(1);
  long  page_size = sysconf(_SC_PAGESIZE);
  int   total     = 0;

  for (int index = 0; index < 4; ++index) {
    values[index] = index + 1;
  }
  total += values[0] + values[1] + values[2] + values[3];
  total += malloc_usable_size(values) >= 4 * sizeof(*values);
  total += aligned != NULL && (uintptr_t)aligned % 64 == 0;
  total += page != NULL && (uintptr_t)page % (uintptr_t)page_size == 0;
  total += rounded != NULL && (uintptr_t)rounded % (uintptr_t)page_size == 0;
  total += malloc_usable_size(rounded) >= (size_t)page_size;
  total += mallopt(M_CHECK_ACTION, 1) != 0;

  free(values);
  free(aligned);
  free(page);
  free(rounded);
  return total;
}

static int gnu_obstack_extensions(void) {
  struct obstack storage;
  char          *first;
  char          *second;
  int            total = 0;

  obstack_init(&storage);
  first   = obstack_copy0(&storage, "gnu", 3);
  second  = obstack_copy0(&storage, "libc", 4);
  total  += strcmp(first, "gnu") == 0;
  total  += strcmp(second, "libc") == 0;
  total  += obstack_object_size(&storage) == 0;
  obstack_free(&storage, NULL);
  return total;
}

int main(void) {
  printf("%d %d\n", gnu_allocation_extensions(), gnu_obstack_extensions());
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![allow(arithmetic_overflow, unconditional_panic)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn malloc(__size: u64) -> *mut ();
// CHECK-NEXT:     fn reallocarray(__ptr: *mut (), __nmemb: u64, __size: u64) -> *mut ();
// CHECK-NEXT:     fn free(__ptr: *mut ());
// CHECK-NEXT:     fn memalign(__alignment: u64, __size: u64) -> *mut ();
// CHECK-NEXT:     fn valloc(__size: u64) -> *mut ();
// CHECK-NEXT:     fn pvalloc(__size: u64) -> *mut ();
// CHECK-NEXT:     fn mallopt(__param: i32, __val: i32) -> i32;
// CHECK-NEXT:     fn malloc_usable_size(__ptr: *mut ()) -> u64;
// CHECK-NEXT:     fn memcpy(__dest: *mut (), __src: *const (), __n: u64) -> *mut ();
// CHECK-NEXT:     fn strcmp(__s1: *const i8, __s2: *const i8) -> i32;
// CHECK-NEXT:     fn index(__s: *const i8, __c: i32) -> *mut i8;
// CHECK-NEXT:     fn _obstack_newchunk([[V_0:__v[0-9]+]]: *mut obstack, [[V_1:__v[0-9]+]]: i32);
// CHECK-NEXT:     fn _obstack_begin(
// CHECK-NEXT:         [[V_2:__v[0-9]+]]: *mut obstack,
// CHECK-NEXT:         [[V_3:__v[0-9]+]]: i32,
// CHECK-NEXT:         [[V_4:__v[0-9]+]]: i32,
// CHECK-NEXT:         [[V_5:__v[0-9]+]]: Option<unsafe extern "C-unwind" fn(i64) -> *mut ()>,
// CHECK-NEXT:         [[V_6:__v[0-9]+]]: Option<unsafe extern "C-unwind" fn(*mut ())>,
// CHECK-NEXT:     ) -> i32;
// CHECK-NEXT:     fn obstack_free([[V_7:__v[0-9]+]]: *mut obstack, [[V_8:__v[0-9]+]]: *mut ());
// CHECK-NEXT:     fn sysconf(__name: i32) -> i64;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _obstack_chunk {
// CHECK-NEXT:     limit: *mut i8,
// CHECK-NEXT:     prev: *mut _obstack_chunk,
// CHECK-NEXT:     contents: [i8; 4],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct obstack {
// CHECK-NEXT:     chunk_size: i64,
// CHECK-NEXT:     chunk: *mut _obstack_chunk,
// CHECK-NEXT:     object_base: *mut i8,
// CHECK-NEXT:     next_free: *mut i8,
// CHECK-NEXT:     chunk_limit: *mut i8,
// CHECK-NEXT:     temp: [[SLATERECORD_0:__SlateRecord[0-9]+]],
// CHECK-NEXT:     alignment_mask: i32,
// CHECK-NEXT:     chunkfun: Option<unsafe extern "C-unwind" fn(*mut (), i64) -> *mut _obstack_chunk>,
// CHECK-NEXT:     freefun: Option<unsafe extern "C-unwind" fn(*mut (), *mut _obstack_chunk)>,
// CHECK-NEXT:     extra_arg: *mut (),
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits1U0,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[SLATERECORD_0]] {
// CHECK-NEXT:     tempint: i64,
// CHECK-NEXT:     tempptr: *mut (),
// CHECK-NEXT: }
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u8,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits1U0 {
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub use_extra_arg: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub maybe_empty_object: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub alloc_failed: u32,
// CHECK-NEXT:         #[bits(5, access = na)]
// CHECK-NEXT:         pub __slate_pad_3: u8,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_allocation_extensions() -> i32 {
// CHECK-NEXT:     let values: *mut i32 = (unsafe {
// CHECK-NEXT:         reallocarray(std::ptr::null_mut::<()>(), 4 as u64, 4 as u64)
// CHECK-NEXT:     }) as *mut i32;
// CHECK-NEXT:     let aligned: *mut () = unsafe { memalign(64 as u64, 80 as u64) };
// CHECK-NEXT:     let page: *mut () = unsafe { valloc(1 as u64) };
// CHECK-NEXT:     let rounded: *mut () = unsafe { pvalloc(1 as u64) };
// CHECK-NEXT:     let page_size: i64 = unsafe { sysconf(30 as i32) };
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     for index in 0 as i32..4 as i32 {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *values.offset(index as isize) = index + 1;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT:         += (unsafe { *values.offset(0 as isize) })
// CHECK-NEXT:             + unsafe { *values.offset(1 as isize) }
// CHECK-NEXT:             + unsafe { *values.offset(2 as isize) }
// CHECK-NEXT:             + unsafe { *values.offset(3 as isize) };
// CHECK-NEXT:     let [[V_9:__v[0-9]+]]: i32 = ((unsafe { malloc_usable_size(values as *mut ()) })
// CHECK-NEXT:         >= (4 as u64).wrapping_mul(4 as u64)) as i32;
// CHECK-NEXT:     total += [[V_9]];
// CHECK-NEXT:     let [[V_10:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_10]] + ((!aligned.is_null() && (aligned as u64) % 64 == 0) as i32);
// CHECK-NEXT:     let [[V_11:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_11]]
// CHECK-NEXT:         + ((!page.is_null() && (page as u64) % (page_size as u64) == 0) as i32);
// CHECK-NEXT:     let [[V_12:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_12]]
// CHECK-NEXT:         + ((!rounded.is_null() && (rounded as u64) % (page_size as u64) == 0) as i32);
// CHECK-NEXT:     let [[V_13:__v[0-9]+]]: i32 = ((unsafe { malloc_usable_size(rounded) }) >= (page_size as u64))
// CHECK-NEXT:         as i32;
// CHECK-NEXT:     total += [[V_13]];
// CHECK-NEXT:     let [[V_14:__v[0-9]+]]: i32 = ((unsafe { mallopt(-(5 as i32), 1 as i32) }) != 0) as i32;
// CHECK-NEXT:     total += [[V_14]];
// CHECK-NEXT:     unsafe { free(values as *mut ()) };
// CHECK-NEXT:     unsafe { free(aligned) };
// CHECK-NEXT:     unsafe { free(page) };
// CHECK-NEXT:     unsafe { free(rounded) };
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_obstack_extensions() -> i32 {
// CHECK-NEXT:     let mut storage: obstack = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         _obstack_begin(
// CHECK-NEXT:             std::ptr::addr_of_mut!(storage),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn(u64) -> *mut ()>,
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn(i64) -> *mut ()>,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         *const (),
// CHECK-NEXT:                         Option<unsafe extern "C-unwind" fn(u64) -> *mut ()>,
// CHECK-NEXT:                     >(malloc as *const ())
// CHECK-NEXT:                 })
// CHECK-NEXT:             },
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     *const (),
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn(*mut ())>,
// CHECK-NEXT:                 >(free as *const ())
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[V_15:__v[0-9]+]]: *mut ();
// CHECK-NEXT:     {
// CHECK-NEXT:         let __h: *mut obstack = std::ptr::addr_of_mut!(storage);
// CHECK-NEXT:         let __o: *mut obstack = __h;
// CHECK-NEXT:         let __len: i32 = 3;
// CHECK-NEXT:         if (unsafe {
// CHECK-NEXT:             unsafe { (*__o).next_free }.offset(__len as isize).offset(1 as isize)
// CHECK-NEXT:         }) > unsafe { (*__o).chunk_limit }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { _obstack_newchunk(__o, __len + 1) };
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             memcpy(
// CHECK-NEXT:                 (*__o).next_free as *mut (),
// CHECK-NEXT:                 b"gnu\0".as_ptr() as *const (),
// CHECK-NEXT:                 (__len as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let [[V_16:__v[0-9]+]]: *mut i8 = unsafe { (*__o).next_free };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*__o).next_free = [[V_16]].offset(__len as isize);
// CHECK-NEXT:         }
// CHECK-NEXT:         let [[V_17:__v[0-9]+]]: *mut i8 = unsafe { (*__o).next_free };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*__o).next_free = [[V_17]].offset(1 as isize);
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_17]] = 0;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_18:__v[0-9]+]]: *mut ();
// CHECK-NEXT:         let __o1: *mut obstack = __h;
// CHECK-NEXT:         let __value: *mut () = (unsafe { (*__o1).object_base }) as *mut ();
// CHECK-NEXT:         if (unsafe { (*__o1).next_free }) == (__value as *mut i8) {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*__o1).__slate_bits_0.__set_maybe_empty_object(1 as u32);
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*__o1).next_free = if (8 as u64) < (8 as u64) {
// CHECK-NEXT:                 (*__o1).object_base
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 std::ptr::null_mut::<i8>()
// CHECK-NEXT:             }
// CHECK-NEXT:                 .offset(
// CHECK-NEXT:                     ((unsafe { (*__o1).next_free }
// CHECK-NEXT:                         .offset_from(
// CHECK-NEXT:                             (if (8 as u64) < (8 as u64) {
// CHECK-NEXT:                                 (*__o1).object_base
// CHECK-NEXT:                             } else {
// CHECK-NEXT:                                 std::ptr::null_mut::<i8>()
// CHECK-NEXT:                             }) as *mut i8,
// CHECK-NEXT:                         ) as i64) + ((*__o1).alignment_mask as i64)
// CHECK-NEXT:                         & (!(*__o1).alignment_mask as i64)) as isize,
// CHECK-NEXT:                 );
// CHECK-NEXT:         }
// CHECK-NEXT:         if ((unsafe {
// CHECK-NEXT:             unsafe { (*__o1).next_free }.offset_from((*__o1).chunk as *mut i8)
// CHECK-NEXT:         }) as i64)
// CHECK-NEXT:             > ((unsafe {
// CHECK-NEXT:                 unsafe { (*__o1).chunk_limit }.offset_from((*__o1).chunk as *mut i8)
// CHECK-NEXT:             }) as i64)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*__o1).next_free = (*__o1).chunk_limit;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*__o1).object_base = (*__o1).next_free;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[V_18]] = __value;
// CHECK-NEXT:         [[V_15]] = [[V_18]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let first: *mut i8 = [[V_15]] as *mut i8;
// CHECK-NEXT:     let mut [[V_19:__v[0-9]+]]: *mut ();
// CHECK-NEXT:     let __h: *mut obstack = std::ptr::addr_of_mut!(storage);
// CHECK-NEXT:     {
// CHECK-NEXT:         let __o: *mut obstack = __h;
// CHECK-NEXT:         let __len: i32 = 4;
// CHECK-NEXT:         if (unsafe {
// CHECK-NEXT:             unsafe { (*__o).next_free }.offset(__len as isize).offset(1 as isize)
// CHECK-NEXT:         }) > unsafe { (*__o).chunk_limit }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { _obstack_newchunk(__o, __len + 1) };
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             memcpy(
// CHECK-NEXT:                 (*__o).next_free as *mut (),
// CHECK-NEXT:                 b"libc\0".as_ptr() as *const (),
// CHECK-NEXT:                 (__len as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let [[V_20:__v[0-9]+]]: *mut i8 = unsafe { (*__o).next_free };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*__o).next_free = [[V_20]].offset(__len as isize);
// CHECK-NEXT:         }
// CHECK-NEXT:         let [[V_21:__v[0-9]+]]: *mut i8 = unsafe { (*__o).next_free };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*__o).next_free = [[V_21]].offset(1 as isize);
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_21]] = 0;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_22:__v[0-9]+]]: *mut ();
// CHECK-NEXT:     let __o1: *mut obstack = __h;
// CHECK-NEXT:     let __value: *mut () = (unsafe { (*__o1).object_base }) as *mut ();
// CHECK-NEXT:     if (unsafe { (*__o1).next_free }) == (__value as *mut i8) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*__o1).__slate_bits_0.__set_maybe_empty_object(1 as u32);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*__o1).next_free = if (8 as u64) < (8 as u64) {
// CHECK-NEXT:             (*__o1).object_base
// CHECK-NEXT:         } else {
// CHECK-NEXT:             std::ptr::null_mut::<i8>()
// CHECK-NEXT:         }
// CHECK-NEXT:             .offset(
// CHECK-NEXT:                 ((unsafe { (*__o1).next_free }
// CHECK-NEXT:                     .offset_from(
// CHECK-NEXT:                         (if (8 as u64) < (8 as u64) {
// CHECK-NEXT:                             (*__o1).object_base
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             std::ptr::null_mut::<i8>()
// CHECK-NEXT:                         }) as *mut i8,
// CHECK-NEXT:                     ) as i64) + ((*__o1).alignment_mask as i64)
// CHECK-NEXT:                     & (!(*__o1).alignment_mask as i64)) as isize,
// CHECK-NEXT:             );
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((unsafe { unsafe { (*__o1).next_free }.offset_from((*__o1).chunk as *mut i8) })
// CHECK-NEXT:         as i64)
// CHECK-NEXT:         > ((unsafe {
// CHECK-NEXT:             unsafe { (*__o1).chunk_limit }.offset_from((*__o1).chunk as *mut i8)
// CHECK-NEXT:         }) as i64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*__o1).next_free = (*__o1).chunk_limit;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*__o1).object_base = (*__o1).next_free;
// CHECK-NEXT:     }
// CHECK-NEXT:     [[V_22]] = __value;
// CHECK-NEXT:     [[V_19]] = [[V_22]];
// CHECK-NEXT:     let second: *mut i8 = [[V_19]] as *mut i8;
// CHECK-NEXT:     let [[V_23:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_23]]
// CHECK-NEXT:         + (((unsafe { strcmp(first as *const i8, c"gnu".as_ptr()) }) == 0) as i32);
// CHECK-NEXT:     let [[V_24:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_24]]
// CHECK-NEXT:         + (((unsafe { strcmp(second as *const i8, c"libc".as_ptr()) }) == 0) as i32);
// CHECK-NEXT:     let mut [[V_25:__v[0-9]+]]: u32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let __o: *const obstack = std::ptr::addr_of_mut!(storage) as *const obstack;
// CHECK-NEXT:         [[V_25]] = (((unsafe {
// CHECK-NEXT:             unsafe { (*__o).next_free }.offset_from((*__o).object_base as *mut i8)
// CHECK-NEXT:         }) as i64) as i32) as u32;
// CHECK-NEXT:     }
// CHECK-NEXT:     total += ([[V_25]] == 0) as i32;
// CHECK-NEXT:     let __o: *mut obstack = std::ptr::addr_of_mut!(storage);
// CHECK-NEXT:     let __obj: *mut () = std::ptr::null_mut::<()>();
// CHECK-NEXT:     if __obj > ((unsafe { (*__o).chunk }) as *mut ())
// CHECK-NEXT:         && __obj < ((unsafe { (*__o).chunk_limit }) as *mut ())
// CHECK-NEXT:     {
// CHECK-NEXT:         let [[V_26:__v[0-9]+]]: *mut i8 = __obj as *mut i8;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*__o).object_base = [[V_26]];
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*__o).next_free = [[V_26]];
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe { obstack_free(__o, __obj) };
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %d\n".as_ptr(),
// CHECK-NEXT:             gnu_allocation_extensions(),
// CHECK-NEXT:             gnu_obstack_extensions(),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
