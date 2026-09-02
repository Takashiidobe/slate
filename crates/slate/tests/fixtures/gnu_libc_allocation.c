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

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(
// LOWERING-NEXT:     dead_code,
// LOWERING-NEXT:     unused,
// LOWERING-NEXT:     non_camel_case_types,
// LOWERING-NEXT:     non_snake_case,
// LOWERING-NEXT:     non_upper_case_globals,
// LOWERING-NEXT:     arithmetic_overflow,
// LOWERING-NEXT:     unconditional_panic,
// LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-NEXT:     unused_comparisons
// LOWERING-NEXT: )]
// LOWERING-EMPTY:
// LOWERING-NEXT: mod __slate_bitfields {
// LOWERING-NEXT:     #[bitfields::bitfield(
// LOWERING-NEXT:         u8,
// LOWERING-NEXT:         new = false,
// LOWERING-NEXT:         from_into_bits = false,
// LOWERING-NEXT:         from_traits = false,
// LOWERING-NEXT:         default = false,
// LOWERING-NEXT:         debug = false,
// LOWERING-NEXT:         builder = false,
// LOWERING-NEXT:         bit_ops = false
// LOWERING-NEXT:     )]
// LOWERING-NEXT:     pub struct __SlateBitfield_obstack_10 {
// LOWERING-NEXT:         #[bits(1)]
// LOWERING-NEXT:         pub _reserved_0: u128,
// LOWERING-NEXT:         #[bits(1)]
// LOWERING-NEXT:         pub maybe_empty_object: u32,
// LOWERING-NEXT:         #[bits(6)]
// LOWERING-NEXT:         pub _reserved_1: u128,
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct _obstack_chunk {
// LOWERING-NEXT:     limit: *mut i8,
// LOWERING-NEXT:     prev: *mut _obstack_chunk,
// LOWERING-NEXT:     contents: [i8; 4],
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: union {{_unnamed_at_[0-9A-Za-z_]+}} {
// LOWERING-NEXT:     tempint: i64,
// LOWERING-NEXT:     tempptr: *mut core::ffi::c_void,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: union {{anon_[0-9]+}} {
// LOWERING-NEXT:     tempint: i64,
// LOWERING-NEXT:     tempptr: *mut core::ffi::c_void,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct obstack {
// LOWERING-NEXT:     __bitfield_0: i64,
// LOWERING-NEXT:     __bitfield_1: *mut _obstack_chunk,
// LOWERING-NEXT:     __bitfield_2: *mut i8,
// LOWERING-NEXT:     __bitfield_3: *mut i8,
// LOWERING-NEXT:     __bitfield_4: *mut i8,
// LOWERING-NEXT:     __bitfield_5: {{anon_[0-9]+}},
// LOWERING-NEXT:     __bitfield_6: i32,
// LOWERING-NEXT:     __bitfield_7: Option<unsafe extern "C" fn(*mut core::ffi::c_void, i64) -> *mut _obstack_chunk>,
// LOWERING-NEXT:     __bitfield_8: Option<unsafe extern "C" fn(*mut core::ffi::c_void, *mut _obstack_chunk)>,
// LOWERING-NEXT:     __bitfield_9: *mut core::ffi::c_void,
// LOWERING-NEXT:     __bitfield_10: __slate_bitfields::__SlateBitfield_obstack_10,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT:     fn reallocarray(_0: *mut core::ffi::c_void, _1: usize, _2: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn memalign(_0: usize, _1: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn valloc(_0: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn pvalloc(_0: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn sysconf(_0: i32) -> i64;
// LOWERING-NEXT:     fn malloc_usable_size(_0: *mut core::ffi::c_void) -> usize;
// LOWERING-NEXT:     fn mallopt(_0: i32, _1: i32) -> i32;
// LOWERING-NEXT:     fn free(_0: *mut core::ffi::c_void);
// LOWERING-NEXT:     fn _obstack_begin(
// LOWERING-NEXT:         _0: *mut obstack,
// LOWERING-NEXT:         _1: i32,
// LOWERING-NEXT:         _2: i32,
// LOWERING-NEXT:         _3: Option<unsafe extern "C" fn(i64) -> *mut core::ffi::c_void>,
// LOWERING-NEXT:         _4: Option<unsafe extern "C" fn(*mut core::ffi::c_void)>,
// LOWERING-NEXT:     ) -> i32;
// LOWERING-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn _obstack_newchunk(_0: *mut obstack, _1: i32);
// LOWERING-NEXT:     fn memcpy(
// LOWERING-NEXT:         _0: *mut core::ffi::c_void,
// LOWERING-NEXT:         _1: *const core::ffi::c_void,
// LOWERING-NEXT:         _2: usize,
// LOWERING-NEXT:     ) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn strcmp(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> i32;
// LOWERING-NEXT:     fn obstack_free(_0: *mut obstack, _1: *mut core::ffi::c_void);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_allocation_extensions() -> i32 {
// LOWERING-NEXT:     let mut values: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut aligned: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut page: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut rounded: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut page_size: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void =
// LOWERING-NEXT:         unsafe { reallocarray({{_v[0-9]+}} as *mut core::ffi::c_void, {{_v[0-9]+}} as usize, {{_v[0-9]+}} as usize) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = {{_v[0-9]+}} as *mut i32;
// LOWERING-NEXT:     values = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 80;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { memalign({{_v[0-9]+}} as usize, {{_v[0-9]+}} as usize) };
// LOWERING-NEXT:     aligned = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { valloc({{_v[0-9]+}} as usize) };
// LOWERING-NEXT:     page = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { pvalloc({{_v[0-9]+}} as usize) };
// LOWERING-NEXT:     rounded = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 30;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { sysconf({{_v[0-9]+}} as i32) };
// LOWERING-NEXT:     page_size = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut index: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         index = {{_v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = index;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = index;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = index;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut i32 = values;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.offset({{_v[0-9]+}} as isize) };
// LOWERING-NEXT:                 unsafe {
// LOWERING-NEXT:                     *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = index;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:             index = {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = values;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(0) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = values;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(1) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = values;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(2) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = values;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(3) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = values;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = (unsafe { malloc_usable_size({{_v[0-9]+}} as *mut core::ffi::c_void) }) as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} >= {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = aligned;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut core::ffi::c_void = aligned;
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = 64;
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} % {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = page;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut core::ffi::c_void = page;
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i64 = page_size;
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} % {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = rounded;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut core::ffi::c_void = rounded;
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i64 = page_size;
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} % {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = rounded;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = (unsafe { malloc_usable_size({{_v[0-9]+}} as *mut core::ffi::c_void) }) as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = page_size;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} >= {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = -5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { mallopt({{_v[0-9]+}} as i32, {{_v[0-9]+}} as i32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = values;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     unsafe { free({{_v[0-9]+}} as *mut core::ffi::c_void) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = aligned;
// LOWERING-NEXT:     unsafe { free({{_v[0-9]+}} as *mut core::ffi::c_void) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = page;
// LOWERING-NEXT:     unsafe { free({{_v[0-9]+}} as *mut core::ffi::c_void) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = rounded;
// LOWERING-NEXT:     unsafe { free({{_v[0-9]+}} as *mut core::ffi::c_void) };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_obstack_extensions() -> i32 {
// LOWERING-NEXT:     let mut storage: obstack = obstack {
// LOWERING-NEXT:         __bitfield_0: 0,
// LOWERING-NEXT:         __bitfield_1: std::ptr::null_mut(),
// LOWERING-NEXT:         __bitfield_2: std::ptr::null_mut(),
// LOWERING-NEXT:         __bitfield_3: std::ptr::null_mut(),
// LOWERING-NEXT:         __bitfield_4: std::ptr::null_mut(),
// LOWERING-NEXT:         __bitfield_5: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// LOWERING-NEXT:         __bitfield_6: 0,
// LOWERING-NEXT:         __bitfield_7: None,
// LOWERING-NEXT:         __bitfield_8: None,
// LOWERING-NEXT:         __bitfield_9: std::ptr::null_mut(),
// LOWERING-NEXT:         __bitfield_10: unsafe {
// LOWERING-NEXT:             std::mem::transmute::<u8, __slate_bitfields::__SlateBitfield_obstack_10>(0)
// LOWERING-NEXT:         },
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C" fn(i64) -> *mut core::ffi::c_void> = unsafe {
// LOWERING-NEXT:         std::mem::transmute::<*const (), Option<unsafe extern "C" fn(i64) -> *mut core::ffi::c_void>>(
// LOWERING-NEXT:             malloc as *const (),
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         _obstack_begin(
// LOWERING-NEXT:             std::ptr::addr_of_mut!(storage) as *mut obstack,
// LOWERING-NEXT:             {{_v[0-9]+}} as i32,
// LOWERING-NEXT:             {{_v[0-9]+}} as i32,
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             Some(free),
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         storage.__bitfield_5.tempint = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { storage.__bitfield_5.tempint };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.offset({{_v[0-9]+}} as isize) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.add(1) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = storage.__bitfield_4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} > {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i64 = unsafe { storage.__bitfield_5.tempint };
// LOWERING-NEXT:         let {{_v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:         unsafe { _obstack_newchunk(std::ptr::addr_of_mut!(storage) as *mut obstack, {{_v[0-9]+}} as i32) };
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"gnu\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { storage.__bitfield_5.tempint };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// LOWERING-NEXT:         memcpy(
// LOWERING-NEXT:             {{_v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_void,
// LOWERING-NEXT:             {{_v[0-9]+}} as usize,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { storage.__bitfield_5.tempint };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.offset({{_v[0-9]+}} as isize) };
// LOWERING-NEXT:     storage.__bitfield_3 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.add(1) };
// LOWERING-NEXT:     storage.__bitfield_3 = {{_v[0-9]+}};
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = storage.__bitfield_2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: u32 = 1;
// LOWERING-NEXT:         let {{_v[0-9]+}}: u32 = ({{_v[0-9]+}} as u32) << 31 >> 31;
// LOWERING-NEXT:         storage
// LOWERING-NEXT:             .__bitfield_10
// LOWERING-NEXT:             .set_maybe_empty_object(({{_v[0-9]+}} as u32) << 31 >> 31);
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = storage.__bitfield_2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         storage.__bitfield_5.tempptr = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = storage.__bitfield_6;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = storage.__bitfield_6;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = !{{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} & {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{_v[0-9]+}} as *mut i8;
// LOWERING-NEXT:     storage.__bitfield_3 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut _obstack_chunk = storage.__bitfield_1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{_v[0-9]+}} as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = storage.__bitfield_4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut _obstack_chunk = storage.__bitfield_1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{_v[0-9]+}} as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} > {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i8 = storage.__bitfield_4;
// LOWERING-NEXT:         storage.__bitfield_3 = {{_v[0-9]+}};
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// LOWERING-NEXT:     storage.__bitfield_2 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { storage.__bitfield_5.tempptr };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{_v[0-9]+}} as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 4;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         storage.__bitfield_5.tempint = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { storage.__bitfield_5.tempint };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.offset({{_v[0-9]+}} as isize) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.add(1) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = storage.__bitfield_4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} > {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i64 = unsafe { storage.__bitfield_5.tempint };
// LOWERING-NEXT:         let {{_v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:         unsafe { _obstack_newchunk(std::ptr::addr_of_mut!(storage) as *mut obstack, {{_v[0-9]+}} as i32) };
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"libc\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { storage.__bitfield_5.tempint };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// LOWERING-NEXT:         memcpy(
// LOWERING-NEXT:             {{_v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_void,
// LOWERING-NEXT:             {{_v[0-9]+}} as usize,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { storage.__bitfield_5.tempint };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.offset({{_v[0-9]+}} as isize) };
// LOWERING-NEXT:     storage.__bitfield_3 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.add(1) };
// LOWERING-NEXT:     storage.__bitfield_3 = {{_v[0-9]+}};
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = storage.__bitfield_2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: u32 = 1;
// LOWERING-NEXT:         let {{_v[0-9]+}}: u32 = ({{_v[0-9]+}} as u32) << 31 >> 31;
// LOWERING-NEXT:         storage
// LOWERING-NEXT:             .__bitfield_10
// LOWERING-NEXT:             .set_maybe_empty_object(({{_v[0-9]+}} as u32) << 31 >> 31);
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = storage.__bitfield_2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         storage.__bitfield_5.tempptr = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = storage.__bitfield_6;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = storage.__bitfield_6;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = !{{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} & {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{_v[0-9]+}} as *mut i8;
// LOWERING-NEXT:     storage.__bitfield_3 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut _obstack_chunk = storage.__bitfield_1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{_v[0-9]+}} as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = storage.__bitfield_4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut _obstack_chunk = storage.__bitfield_1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{_v[0-9]+}} as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} > {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i8 = storage.__bitfield_4;
// LOWERING-NEXT:         storage.__bitfield_3 = {{_v[0-9]+}};
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// LOWERING-NEXT:     storage.__bitfield_2 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { storage.__bitfield_5.tempptr };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{_v[0-9]+}} as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"gnu\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         strcmp(
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"libc\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         strcmp(
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = storage.__bitfield_2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} as u32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut _obstack_chunk = storage.__bitfield_1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{_v[0-9]+}} as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         storage.__bitfield_5.tempint = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { storage.__bitfield_5.tempint };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} > {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i64 = unsafe { storage.__bitfield_5.tempint };
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i8 = storage.__bitfield_4;
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut _obstack_chunk = storage.__bitfield_1;
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i8 = {{_v[0-9]+}} as *mut i8;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = gnu_allocation_extensions();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = gnu_obstack_extensions();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(
// REWRITES-NEXT:     dead_code,
// REWRITES-NEXT:     unused,
// REWRITES-NEXT:     non_camel_case_types,
// REWRITES-NEXT:     non_snake_case,
// REWRITES-NEXT:     non_upper_case_globals,
// REWRITES-NEXT:     arithmetic_overflow,
// REWRITES-NEXT:     unconditional_panic,
// REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-NEXT:     unused_comparisons
// REWRITES-NEXT: )]
// REWRITES-EMPTY:
// REWRITES-NEXT: mod __slate_bitfields {
// REWRITES-NEXT:     #[bitfields::bitfield(
// REWRITES-NEXT:         u8,
// REWRITES-NEXT:         new = false,
// REWRITES-NEXT:         from_into_bits = false,
// REWRITES-NEXT:         from_traits = false,
// REWRITES-NEXT:         default = false,
// REWRITES-NEXT:         debug = false,
// REWRITES-NEXT:         builder = false,
// REWRITES-NEXT:         bit_ops = false
// REWRITES-NEXT:     )]
// REWRITES-NEXT:     pub struct __SlateBitfield_obstack_10 {
// REWRITES-NEXT:         #[bits(1)]
// REWRITES-NEXT:         pub _reserved_0: u128,
// REWRITES-NEXT:         #[bits(1)]
// REWRITES-NEXT:         pub maybe_empty_object: u32,
// REWRITES-NEXT:         #[bits(6)]
// REWRITES-NEXT:         pub _reserved_1: u128,
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct _obstack_chunk {
// REWRITES-NEXT:     limit: *mut i8,
// REWRITES-NEXT:     prev: *mut _obstack_chunk,
// REWRITES-NEXT:     contents: [i8; 4],
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: union {{_unnamed_at_[0-9A-Za-z_]+}} {
// REWRITES-NEXT:     tempint: i64,
// REWRITES-NEXT:     tempptr: *mut core::ffi::c_void,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: union {{anon_[0-9]+}} {
// REWRITES-NEXT:     tempint: i64,
// REWRITES-NEXT:     tempptr: *mut core::ffi::c_void,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct obstack {
// REWRITES-NEXT:     __bitfield_0: i64,
// REWRITES-NEXT:     __bitfield_1: *mut _obstack_chunk,
// REWRITES-NEXT:     __bitfield_2: *mut i8,
// REWRITES-NEXT:     __bitfield_3: *mut i8,
// REWRITES-NEXT:     __bitfield_4: *mut i8,
// REWRITES-NEXT:     __bitfield_5: {{anon_[0-9]+}},
// REWRITES-NEXT:     __bitfield_6: i32,
// REWRITES-NEXT:     __bitfield_7: Option<unsafe extern "C" fn(*mut core::ffi::c_void, i64) -> *mut _obstack_chunk>,
// REWRITES-NEXT:     __bitfield_8: Option<unsafe extern "C" fn(*mut core::ffi::c_void, *mut _obstack_chunk)>,
// REWRITES-NEXT:     __bitfield_9: *mut core::ffi::c_void,
// REWRITES-NEXT:     __bitfield_10: __slate_bitfields::__SlateBitfield_obstack_10,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT:     fn reallocarray(_0: *mut core::ffi::c_void, _1: usize, _2: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn memalign(_0: usize, _1: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn valloc(_0: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn pvalloc(_0: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn sysconf(_0: i32) -> i64;
// REWRITES-NEXT:     fn malloc_usable_size(_0: *mut core::ffi::c_void) -> usize;
// REWRITES-NEXT:     fn mallopt(_0: i32, _1: i32) -> i32;
// REWRITES-NEXT:     fn free(_0: *mut core::ffi::c_void);
// REWRITES-NEXT:     fn _obstack_begin(
// REWRITES-NEXT:         _0: *mut obstack,
// REWRITES-NEXT:         _1: i32,
// REWRITES-NEXT:         _2: i32,
// REWRITES-NEXT:         _3: Option<unsafe extern "C" fn(i64) -> *mut core::ffi::c_void>,
// REWRITES-NEXT:         _4: Option<unsafe extern "C" fn(*mut core::ffi::c_void)>,
// REWRITES-NEXT:     ) -> i32;
// REWRITES-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn _obstack_newchunk(_0: *mut obstack, _1: i32);
// REWRITES-NEXT:     fn memcpy(
// REWRITES-NEXT:         _0: *mut core::ffi::c_void,
// REWRITES-NEXT:         _1: *const core::ffi::c_void,
// REWRITES-NEXT:         _2: usize,
// REWRITES-NEXT:     ) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn strcmp(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> i32;
// REWRITES-NEXT:     fn obstack_free(_0: *mut obstack, _1: *mut core::ffi::c_void);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn gnu_allocation_extensions() -> i32 {
// REWRITES-NEXT:     let mut values: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT:     let mut aligned: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT:     let mut page: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT:     let mut rounded: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT:     let mut page_size: i64 = 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// REWRITES-NEXT:         reallocarray(
// REWRITES-NEXT:             {{_v[0-9]+}} as *mut core::ffi::c_void,
// REWRITES-NEXT:             (4 as u64) as usize,
// REWRITES-NEXT:             (4 as u64) as usize,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     values = {{_v[0-9]+}} as *mut i32;
// REWRITES-NEXT:     aligned = unsafe { memalign((64 as u64) as usize, (80 as u64) as usize) };
// REWRITES-NEXT:     page = unsafe { valloc((1 as u64) as usize) };
// REWRITES-NEXT:     rounded = unsafe { pvalloc((1 as u64) as usize) };
// REWRITES-NEXT:     page_size = unsafe { sysconf(30 as i32) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     let mut index: i32 = 0;
// REWRITES-NEXT:     index = 0;
// REWRITES-NEXT:     loop {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = index < 4;
// REWRITES-NEXT:         if !{{_v[0-9]+}} {
// REWRITES-NEXT:             break;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = index + 1;
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut i32 = values;
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.offset((index as i64) as isize) };
// REWRITES-NEXT:         unsafe {
// REWRITES-NEXT:             *{{_v[0-9]+}} = {{_v[0-9]+}};
// REWRITES-NEXT:         }
// REWRITES-NEXT:         index += 1;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = values;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(0) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = values;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(1) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = values;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(2) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = values;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(3) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + ({{_v[0-9]+}} + unsafe { *{{_v[0-9]+}} });
// REWRITES-NEXT:     let {{_v[0-9]+}}: u64 = (unsafe { malloc_usable_size(values as *mut core::ffi::c_void) }) as u64;
// REWRITES-NEXT:     let {{_v[0-9]+}}: u64 = 4;
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} >= 4 * {{_v[0-9]+}};
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + ({{_v[0-9]+}} as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = aligned != std::ptr::null_mut();
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: u64 = 0;
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = (aligned as u64) % 64 == {{_v[0-9]+}};
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     } else {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = false;
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + ({{_v[0-9]+}} as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = page != std::ptr::null_mut();
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = (page as u64) % (page_size as u64) == 0;
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     } else {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = false;
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + ({{_v[0-9]+}} as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = rounded != std::ptr::null_mut();
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = (rounded as u64) % (page_size as u64) == 0;
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     } else {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = false;
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + ({{_v[0-9]+}} as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: u64 = (unsafe { malloc_usable_size(rounded as *mut core::ffi::c_void) }) as u64;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} >= (page_size as u64)) as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { mallopt(-5 as i32, 1 as i32) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + ({{_v[0-9]+}} as i32);
// REWRITES-NEXT:     unsafe { free(values as *mut core::ffi::c_void) };
// REWRITES-NEXT:     unsafe { free(aligned as *mut core::ffi::c_void) };
// REWRITES-NEXT:     unsafe { free(page as *mut core::ffi::c_void) };
// REWRITES-NEXT:     unsafe { free(rounded as *mut core::ffi::c_void) };
// REWRITES-NEXT:     return {{_v[0-9]+}};
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn gnu_obstack_extensions() -> i32 {
// REWRITES-NEXT:     let mut storage: obstack = obstack {
// REWRITES-NEXT:         __bitfield_0: 0,
// REWRITES-NEXT:         __bitfield_1: std::ptr::null_mut(),
// REWRITES-NEXT:         __bitfield_2: std::ptr::null_mut(),
// REWRITES-NEXT:         __bitfield_3: std::ptr::null_mut(),
// REWRITES-NEXT:         __bitfield_4: std::ptr::null_mut(),
// REWRITES-NEXT:         __bitfield_5: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// REWRITES-NEXT:         __bitfield_6: 0,
// REWRITES-NEXT:         __bitfield_7: None,
// REWRITES-NEXT:         __bitfield_8: None,
// REWRITES-NEXT:         __bitfield_9: std::ptr::null_mut(),
// REWRITES-NEXT:         __bitfield_10: unsafe {
// REWRITES-NEXT:             std::mem::transmute::<u8, __slate_bitfields::__SlateBitfield_obstack_10>(0)
// REWRITES-NEXT:         },
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C" fn(i64) -> *mut core::ffi::c_void> = unsafe {
// REWRITES-NEXT:         std::mem::transmute::<*const (), Option<unsafe extern "C" fn(i64) -> *mut core::ffi::c_void>>(
// REWRITES-NEXT:             malloc as *const (),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         _obstack_begin(
// REWRITES-NEXT:             std::ptr::addr_of_mut!(storage) as *mut obstack,
// REWRITES-NEXT:             0 as i32,
// REWRITES-NEXT:             0 as i32,
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             Some(free),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         storage.__bitfield_5.tempint = 3;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { storage.__bitfield_5.tempint };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.offset({{_v[0-9]+}} as isize) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.add(1) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} > storage.__bitfield_4;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i64 = (unsafe { storage.__bitfield_5.tempint }) + 1;
// REWRITES-NEXT:         unsafe { _obstack_newchunk(std::ptr::addr_of_mut!(storage) as *mut obstack, {{_v[0-9]+}} as i32) };
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     } else {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         std::ptr::copy_nonoverlapping(
// REWRITES-NEXT:             b"gnu\0".as_ptr() as *const u8,
// REWRITES-NEXT:             (storage.__bitfield_3 as *mut core::ffi::c_void) as *mut u8,
// REWRITES-NEXT:             ((unsafe { storage.__bitfield_5.tempint }) as u64) as usize,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { storage.__bitfield_5.tempint };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.offset({{_v[0-9]+}} as isize) };
// REWRITES-NEXT:     storage.__bitfield_3 = {{_v[0-9]+}};
// REWRITES-NEXT:     let {{_v[0-9]+}}: i8 = 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.add(1) };
// REWRITES-NEXT:     storage.__bitfield_3 = {{_v[0-9]+}};
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = storage.__bitfield_3 == storage.__bitfield_2;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} {
// REWRITES-NEXT:         storage
// REWRITES-NEXT:             .__bitfield_10
// REWRITES-NEXT:             .set_maybe_empty_object((1 as u32) << 31 >> 31);
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     } else {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         storage.__bitfield_5.tempptr = storage.__bitfield_2 as *mut core::ffi::c_void;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} + (storage.__bitfield_6 as i64);
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = !storage.__bitfield_6;
// REWRITES-NEXT:     storage.__bitfield_3 = ({{_v[0-9]+}} & ({{_v[0-9]+}} as i64)) as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from(storage.__bitfield_1 as *mut i8) as i64 };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = storage.__bitfield_4;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from(storage.__bitfield_1 as *mut i8) as i64 };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = if {{_v[0-9]+}} > {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut i8 = storage.__bitfield_4;
// REWRITES-NEXT:         storage.__bitfield_3 = {{_v[0-9]+}};
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     } else {
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     };
// REWRITES-NEXT:     storage.__bitfield_2 = storage.__bitfield_3;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = (unsafe { storage.__bitfield_5.tempptr }) as *mut i8;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         storage.__bitfield_5.tempint = 4;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { storage.__bitfield_5.tempint };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.offset({{_v[0-9]+}} as isize) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.add(1) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} > storage.__bitfield_4;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i64 = (unsafe { storage.__bitfield_5.tempint }) + 1;
// REWRITES-NEXT:         unsafe { _obstack_newchunk(std::ptr::addr_of_mut!(storage) as *mut obstack, {{_v[0-9]+}} as i32) };
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     } else {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         std::ptr::copy_nonoverlapping(
// REWRITES-NEXT:             b"libc\0".as_ptr() as *const u8,
// REWRITES-NEXT:             (storage.__bitfield_3 as *mut core::ffi::c_void) as *mut u8,
// REWRITES-NEXT:             ((unsafe { storage.__bitfield_5.tempint }) as u64) as usize,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { storage.__bitfield_5.tempint };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.offset({{_v[0-9]+}} as isize) };
// REWRITES-NEXT:     storage.__bitfield_3 = {{_v[0-9]+}};
// REWRITES-NEXT:     let {{_v[0-9]+}}: i8 = 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.add(1) };
// REWRITES-NEXT:     storage.__bitfield_3 = {{_v[0-9]+}};
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = storage.__bitfield_3 == storage.__bitfield_2;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} {
// REWRITES-NEXT:         storage
// REWRITES-NEXT:             .__bitfield_10
// REWRITES-NEXT:             .set_maybe_empty_object((1 as u32) << 31 >> 31);
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     } else {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         storage.__bitfield_5.tempptr = storage.__bitfield_2 as *mut core::ffi::c_void;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} + (storage.__bitfield_6 as i64);
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = !storage.__bitfield_6;
// REWRITES-NEXT:     storage.__bitfield_3 = ({{_v[0-9]+}} & ({{_v[0-9]+}} as i64)) as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from(storage.__bitfield_1 as *mut i8) as i64 };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = storage.__bitfield_4;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from(storage.__bitfield_1 as *mut i8) as i64 };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = if {{_v[0-9]+}} > {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut i8 = storage.__bitfield_4;
// REWRITES-NEXT:         storage.__bitfield_3 = {{_v[0-9]+}};
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     } else {
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     };
// REWRITES-NEXT:     storage.__bitfield_2 = storage.__bitfield_3;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = (unsafe { storage.__bitfield_5.tempptr }) as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { strcmp({{_v[0-9]+}} as *const core::ffi::c_char, c"gnu".as_ptr()) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + ({{_v[0-9]+}} as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { strcmp({{_v[0-9]+}} as *const core::ffi::c_char, c"libc".as_ptr()) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + ({{_v[0-9]+}} as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = storage.__bitfield_2;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = ({{_v[0-9]+}} as u32) == 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + ({{_v[0-9]+}} as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from(storage.__bitfield_1 as *mut i8) as i64 };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         storage.__bitfield_5.tempint = {{_v[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = (unsafe { storage.__bitfield_5.tempint }) > 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i64 = unsafe { storage.__bitfield_5.tempint };
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut i8 = storage.__bitfield_4;
// REWRITES-NEXT:         let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from(storage.__bitfield_1 as *mut i8) as i64 };
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     } else {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = false;
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     };
// REWRITES-NEXT:     return {{_v[0-9]+}};
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%d %d\n".as_ptr(),
// REWRITES-NEXT:             gnu_allocation_extensions(),
// REWRITES-NEXT:             gnu_obstack_extensions(),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
