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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING: #![feature(c_variadic)]
// COMMON-LOWERING-NEXT: #![allow(
// COMMON-LOWERING-NEXT:     dead_code,
// COMMON-LOWERING-NEXT:     unused,
// COMMON-LOWERING-NEXT:     non_camel_case_types,
// COMMON-LOWERING-NEXT:     non_snake_case,
// COMMON-LOWERING-NEXT:     non_upper_case_globals,
// COMMON-LOWERING-NEXT:     arithmetic_overflow,
// COMMON-LOWERING-NEXT:     unconditional_panic,
// COMMON-LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-LOWERING-NEXT:     unused_comparisons
// COMMON-LOWERING-NEXT: )]
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: mod __slate_bitfields {
// COMMON-LOWERING-NEXT:     #[bitfields::bitfield(
// COMMON-LOWERING-NEXT:         u8,
// COMMON-LOWERING-NEXT:         new = false,
// COMMON-LOWERING-NEXT:         from_into_bits = false,
// COMMON-LOWERING-NEXT:         from_traits = false,
// COMMON-LOWERING-NEXT:         default = false,
// COMMON-LOWERING-NEXT:         debug = false,
// COMMON-LOWERING-NEXT:         builder = false,
// COMMON-LOWERING-NEXT:         bit_ops = false
// COMMON-LOWERING-NEXT:     )]
// COMMON-LOWERING-NEXT:     pub struct __SlateBitfield_obstack_10 {
// COMMON-LOWERING-NEXT:         #[bits(1)]
// COMMON-LOWERING-NEXT:         pub _reserved_0: u128,
// COMMON-LOWERING-NEXT:         #[bits(1)]
// COMMON-LOWERING-NEXT:         pub maybe_empty_object: u32,
// COMMON-LOWERING-NEXT:         #[bits(6)]
// COMMON-LOWERING-NEXT:         pub _reserved_1: u128,
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct _obstack_chunk {
// COMMON-LOWERING-NEXT:     prev: *mut _obstack_chunk,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: union {{_unnamed_at_[0-9A-Za-z_]+}} {
// COMMON-LOWERING-NEXT:     tempint: i64,
// COMMON-LOWERING-NEXT:     tempptr: *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: union {{anon_[0-9]+}} {
// COMMON-LOWERING-NEXT:     tempint: i64,
// COMMON-LOWERING-NEXT:     tempptr: *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct obstack {
// COMMON-LOWERING-NEXT:     __bitfield_0: i64,
// COMMON-LOWERING-NEXT:     __bitfield_1: *mut _obstack_chunk,
// COMMON-LOWERING-NEXT:     __bitfield_5: {{anon_[0-9]+}},
// COMMON-LOWERING-NEXT:     __bitfield_6: i32,
// COMMON-LOWERING-NEXT:     __bitfield_7:
// COMMON-LOWERING-NEXT:         Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, i64) -> *mut _obstack_chunk>,
// COMMON-LOWERING-NEXT:     __bitfield_8: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut _obstack_chunk)>,
// COMMON-LOWERING-NEXT:     __bitfield_9: *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:     __bitfield_10: __slate_bitfields::__SlateBitfield_obstack_10,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT:     fn reallocarray(_0: *mut core::ffi::c_void, _1: usize, _2: usize) -> *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     fn memalign(_0: usize, _1: usize) -> *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     fn valloc(_0: usize) -> *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     fn pvalloc(_0: usize) -> *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     fn sysconf(_0: i32) -> i64;
// COMMON-LOWERING-NEXT:     fn malloc_usable_size(_0: *mut core::ffi::c_void) -> usize;
// COMMON-LOWERING-NEXT:     fn mallopt(_0: i32, _1: i32) -> i32;
// COMMON-LOWERING-NEXT:     fn free(_0: *mut core::ffi::c_void);
// COMMON-LOWERING-NEXT:     fn _obstack_begin(
// COMMON-LOWERING-NEXT:         _0: *mut obstack,
// COMMON-LOWERING-NEXT:         _1: i32,
// COMMON-LOWERING-NEXT:         _2: i32,
// COMMON-LOWERING-NEXT:         _3: Option<unsafe extern "C-unwind" fn(i64) -> *mut core::ffi::c_void>,
// COMMON-LOWERING-NEXT:         _4: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void)>,
// COMMON-LOWERING-NEXT:     ) -> i32;
// COMMON-LOWERING-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     fn _obstack_newchunk(_0: *mut obstack, _1: i32);
// COMMON-LOWERING-NEXT:     fn memcpy(
// COMMON-LOWERING-NEXT:         _0: *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:         _1: *const core::ffi::c_void,
// COMMON-LOWERING-NEXT:         _2: usize,
// COMMON-LOWERING-NEXT:     ) -> *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     fn strcmp(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> i32;
// COMMON-LOWERING-NEXT:     fn obstack_free(_0: *mut obstack, _1: *mut core::ffi::c_void);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = gnu_allocation_extensions();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = gnu_obstack_extensions();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn gnu_allocation_extensions() -> i32 {
// COMMON-LOWERING-NEXT:     let mut values: *mut i32 = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let mut aligned: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let mut page: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let mut rounded: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let mut page_size: i64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 4;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 4;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void =
// COMMON-LOWERING-NEXT:         unsafe { reallocarray({{__v[0-9]+}} as *mut core::ffi::c_void, {{__v[0-9]+}} as usize, {{__v[0-9]+}} as usize) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = {{__v[0-9]+}} as *mut i32;
// COMMON-LOWERING-NEXT:     values = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 80;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe { memalign({{__v[0-9]+}} as usize, {{__v[0-9]+}} as usize) };
// COMMON-LOWERING-NEXT:     aligned = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe { valloc({{__v[0-9]+}} as usize) };
// COMMON-LOWERING-NEXT:     page = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe { pvalloc({{__v[0-9]+}} as usize) };
// COMMON-LOWERING-NEXT:     rounded = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 30;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { sysconf({{__v[0-9]+}} as i32) };
// COMMON-LOWERING-NEXT:     page_size = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let mut index: i32 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         index = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = index;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 4;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = index;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = index;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut i32 = values;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// COMMON-LOWERING-NEXT:                 unsafe {
// COMMON-LOWERING-NEXT:                     *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = index;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:             index = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = values;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(0) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = values;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(1) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = values;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(2) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = values;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(3) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = values;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe { malloc_usable_size({{__v[0-9]+}} as *mut core::ffi::c_void) }) as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 4;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 4;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} >= {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = aligned;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = aligned;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = 64;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} % {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = page;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = page;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i64 = page_size;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} % {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = rounded;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = rounded;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i64 = page_size;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} % {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = rounded;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe { malloc_usable_size({{__v[0-9]+}} as *mut core::ffi::c_void) }) as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = page_size;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} >= {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = -5;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { mallopt({{__v[0-9]+}} as i32, {{__v[0-9]+}} as i32) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = values;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     unsafe { free({{__v[0-9]+}} as *mut core::ffi::c_void) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = aligned;
// COMMON-LOWERING-NEXT:     unsafe { free({{__v[0-9]+}} as *mut core::ffi::c_void) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = page;
// COMMON-LOWERING-NEXT:     unsafe { free({{__v[0-9]+}} as *mut core::ffi::c_void) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = rounded;
// COMMON-LOWERING-NEXT:     unsafe { free({{__v[0-9]+}} as *mut core::ffi::c_void) };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn gnu_obstack_extensions() -> i32 {
// COMMON-LOWERING-NEXT:     let mut storage: obstack = obstack {
// COMMON-LOWERING-NEXT:         __bitfield_0: 0,
// COMMON-LOWERING-NEXT:         __bitfield_1: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:         __bitfield_2: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:         __bitfield_3: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:         __bitfield_4: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:         __bitfield_5: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// COMMON-LOWERING-NEXT:         __bitfield_6: 0,
// COMMON-LOWERING-NEXT:         __bitfield_7: None,
// COMMON-LOWERING-NEXT:         __bitfield_8: None,
// COMMON-LOWERING-NEXT:         __bitfield_9: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:         __bitfield_10: unsafe {
// COMMON-LOWERING-NEXT:             std::mem::transmute::<u8, __slate_bitfields::__SlateBitfield_obstack_10>(0)
// COMMON-LOWERING-NEXT:         },
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(i64) -> *mut core::ffi::c_void> = unsafe {
// COMMON-LOWERING-NEXT:         std::mem::transmute::<
// COMMON-LOWERING-NEXT:             *const (),
// COMMON-LOWERING-NEXT:             Option<unsafe extern "C-unwind" fn(i64) -> *mut core::ffi::c_void>,
// COMMON-LOWERING-NEXT:         >(malloc as *const ())
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         _obstack_begin(
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(storage) as *mut obstack,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as i32,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as i32,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             unsafe {
// COMMON-LOWERING-NEXT:                 std::mem::transmute::<
// COMMON-LOWERING-NEXT:                     *const (),
// COMMON-LOWERING-NEXT:                     Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void)>,
// COMMON-LOWERING-NEXT:                 >(free as *const ())
// COMMON-LOWERING-NEXT:             },
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 3;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         storage.__bitfield_5.tempint = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { storage.__bitfield_5.tempint };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i64 = unsafe { storage.__bitfield_5.tempint };
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:         unsafe {
// COMMON-LOWERING-NEXT:             _obstack_newchunk(
// COMMON-LOWERING-NEXT:                 std::ptr::addr_of_mut!(storage) as *mut obstack,
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}} as i32,
// COMMON-LOWERING-NEXT:             )
// COMMON-LOWERING-NEXT:         };
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { storage.__bitfield_5.tempint };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// COMMON-LOWERING-NEXT:         memcpy(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_void,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { storage.__bitfield_5.tempint };
// COMMON-LOWERING-NEXT:     storage.__bitfield_3 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     storage.__bitfield_3 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u32 = 1;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u32 = ({{__v[0-9]+}} as u32) << 31 >> 31;
// COMMON-LOWERING-NEXT:         storage
// COMMON-LOWERING-NEXT:             .__bitfield_10
// COMMON-LOWERING-NEXT:             .set_maybe_empty_object(({{__v[0-9]+}} as u32) << 31 >> 31);
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         storage.__bitfield_5.tempptr = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = storage.__bitfield_6;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = storage.__bitfield_6;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = !{{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} & {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     storage.__bitfield_3 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut _obstack_chunk = storage.__bitfield_1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut _obstack_chunk = storage.__bitfield_1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         storage.__bitfield_3 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     storage.__bitfield_2 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe { storage.__bitfield_5.tempptr };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 4;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         storage.__bitfield_5.tempint = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { storage.__bitfield_5.tempint };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i64 = unsafe { storage.__bitfield_5.tempint };
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:         unsafe {
// COMMON-LOWERING-NEXT:             _obstack_newchunk(
// COMMON-LOWERING-NEXT:                 std::ptr::addr_of_mut!(storage) as *mut obstack,
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}} as i32,
// COMMON-LOWERING-NEXT:             )
// COMMON-LOWERING-NEXT:         };
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { storage.__bitfield_5.tempint };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// COMMON-LOWERING-NEXT:         memcpy(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_void,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { storage.__bitfield_5.tempint };
// COMMON-LOWERING-NEXT:     storage.__bitfield_3 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     storage.__bitfield_3 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u32 = 1;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u32 = ({{__v[0-9]+}} as u32) << 31 >> 31;
// COMMON-LOWERING-NEXT:         storage
// COMMON-LOWERING-NEXT:             .__bitfield_10
// COMMON-LOWERING-NEXT:             .set_maybe_empty_object(({{__v[0-9]+}} as u32) << 31 >> 31);
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         storage.__bitfield_5.tempptr = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = storage.__bitfield_6;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = storage.__bitfield_6;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = !{{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} & {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     storage.__bitfield_3 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut _obstack_chunk = storage.__bitfield_1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut _obstack_chunk = storage.__bitfield_1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         storage.__bitfield_3 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     storage.__bitfield_2 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe { storage.__bitfield_5.tempptr };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         strcmp(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         strcmp(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} as u32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut _obstack_chunk = storage.__bitfield_1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 };
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         storage.__bitfield_5.tempint = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { storage.__bitfield_5.tempint };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i64 = unsafe { storage.__bitfield_5.tempint };
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: *mut _obstack_chunk = storage.__bitfield_1;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 };
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     limit: *mut i8,
// LOWERING-X86_64-GNU-NEXT:     contents: [i8; 4],
// LOWERING-X86_64-GNU-NEXT:     __bitfield_2: *mut i8,
// LOWERING-X86_64-GNU-NEXT:     __bitfield_3: *mut i8,
// LOWERING-X86_64-GNU-NEXT:     __bitfield_4: *mut i8,
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(1) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = storage.__bitfield_4;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"gnu\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(1) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = storage.__bitfield_2;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = storage.__bitfield_2;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = storage.__bitfield_4;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = if {{__v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = storage.__bitfield_4;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(1) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = storage.__bitfield_4;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"libc\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(1) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = storage.__bitfield_2;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = storage.__bitfield_2;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = storage.__bitfield_4;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = if {{__v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = storage.__bitfield_4;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"gnu\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"libc\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = storage.__bitfield_2;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = storage.__bitfield_4;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     limit: *mut u8,
// LOWERING-AARCH64-GNU-NEXT:     contents: [u8; 4],
// LOWERING-AARCH64-GNU-NEXT:     __bitfield_2: *mut u8,
// LOWERING-AARCH64-GNU-NEXT:     __bitfield_3: *mut u8,
// LOWERING-AARCH64-GNU-NEXT:     __bitfield_4: *mut u8,
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = storage.__bitfield_3;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(1) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = storage.__bitfield_4;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = storage.__bitfield_3;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"gnu\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = storage.__bitfield_3;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = 0;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = storage.__bitfield_3;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(1) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = storage.__bitfield_3;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = storage.__bitfield_2;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = storage.__bitfield_2;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = storage.__bitfield_3;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = storage.__bitfield_3;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = storage.__bitfield_4;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = if {{__v[0-9]+}} {
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = storage.__bitfield_4;
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = storage.__bitfield_3;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = storage.__bitfield_3;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(1) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = storage.__bitfield_4;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = storage.__bitfield_3;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"libc\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = storage.__bitfield_3;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = 0;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = storage.__bitfield_3;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(1) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = storage.__bitfield_3;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = storage.__bitfield_2;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = storage.__bitfield_2;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = storage.__bitfield_3;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = storage.__bitfield_3;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = storage.__bitfield_4;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = if {{__v[0-9]+}} {
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = storage.__bitfield_4;
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = storage.__bitfield_3;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"gnu\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"libc\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = storage.__bitfield_3;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = storage.__bitfield_2;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = storage.__bitfield_4;
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES: #![feature(c_variadic)]
// COMMON-REWRITES-NEXT: #![allow(
// COMMON-REWRITES-NEXT:     dead_code,
// COMMON-REWRITES-NEXT:     unused,
// COMMON-REWRITES-NEXT:     non_camel_case_types,
// COMMON-REWRITES-NEXT:     non_snake_case,
// COMMON-REWRITES-NEXT:     non_upper_case_globals,
// COMMON-REWRITES-NEXT:     arithmetic_overflow,
// COMMON-REWRITES-NEXT:     unconditional_panic,
// COMMON-REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-REWRITES-NEXT:     unused_comparisons
// COMMON-REWRITES-NEXT: )]
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: mod __slate_bitfields {
// COMMON-REWRITES-NEXT:     #[bitfields::bitfield(
// COMMON-REWRITES-NEXT:         u8,
// COMMON-REWRITES-NEXT:         new = false,
// COMMON-REWRITES-NEXT:         from_into_bits = false,
// COMMON-REWRITES-NEXT:         from_traits = false,
// COMMON-REWRITES-NEXT:         default = false,
// COMMON-REWRITES-NEXT:         debug = false,
// COMMON-REWRITES-NEXT:         builder = false,
// COMMON-REWRITES-NEXT:         bit_ops = false
// COMMON-REWRITES-NEXT:     )]
// COMMON-REWRITES-NEXT:     pub struct __SlateBitfield_obstack_10 {
// COMMON-REWRITES-NEXT:         #[bits(1)]
// COMMON-REWRITES-NEXT:         pub _reserved_0: u128,
// COMMON-REWRITES-NEXT:         #[bits(1)]
// COMMON-REWRITES-NEXT:         pub maybe_empty_object: u32,
// COMMON-REWRITES-NEXT:         #[bits(6)]
// COMMON-REWRITES-NEXT:         pub _reserved_1: u128,
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct _obstack_chunk {
// COMMON-REWRITES-NEXT:     prev: *mut _obstack_chunk,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: union {{_unnamed_at_[0-9A-Za-z_]+}} {
// COMMON-REWRITES-NEXT:     tempint: i64,
// COMMON-REWRITES-NEXT:     tempptr: *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: union {{anon_[0-9]+}} {
// COMMON-REWRITES-NEXT:     tempint: i64,
// COMMON-REWRITES-NEXT:     tempptr: *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct obstack {
// COMMON-REWRITES-NEXT:     __bitfield_0: i64,
// COMMON-REWRITES-NEXT:     __bitfield_1: *mut _obstack_chunk,
// COMMON-REWRITES-NEXT:     __bitfield_5: {{anon_[0-9]+}},
// COMMON-REWRITES-NEXT:     __bitfield_6: i32,
// COMMON-REWRITES-NEXT:     __bitfield_7:
// COMMON-REWRITES-NEXT:         Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, i64) -> *mut _obstack_chunk>,
// COMMON-REWRITES-NEXT:     __bitfield_8: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut _obstack_chunk)>,
// COMMON-REWRITES-NEXT:     __bitfield_9: *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:     __bitfield_10: __slate_bitfields::__SlateBitfield_obstack_10,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT:     fn reallocarray(_0: *mut core::ffi::c_void, _1: usize, _2: usize) -> *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     fn memalign(_0: usize, _1: usize) -> *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     fn valloc(_0: usize) -> *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     fn pvalloc(_0: usize) -> *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     fn sysconf(_0: i32) -> i64;
// COMMON-REWRITES-NEXT:     fn malloc_usable_size(_0: *mut core::ffi::c_void) -> usize;
// COMMON-REWRITES-NEXT:     fn mallopt(_0: i32, _1: i32) -> i32;
// COMMON-REWRITES-NEXT:     fn free(_0: *mut core::ffi::c_void);
// COMMON-REWRITES-NEXT:     fn _obstack_begin(
// COMMON-REWRITES-NEXT:         _0: *mut obstack,
// COMMON-REWRITES-NEXT:         _1: i32,
// COMMON-REWRITES-NEXT:         _2: i32,
// COMMON-REWRITES-NEXT:         _3: Option<unsafe extern "C-unwind" fn(i64) -> *mut core::ffi::c_void>,
// COMMON-REWRITES-NEXT:         _4: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void)>,
// COMMON-REWRITES-NEXT:     ) -> i32;
// COMMON-REWRITES-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     fn _obstack_newchunk(_0: *mut obstack, _1: i32);
// COMMON-REWRITES-NEXT:     fn memcpy(
// COMMON-REWRITES-NEXT:         _0: *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:         _1: *const core::ffi::c_void,
// COMMON-REWRITES-NEXT:         _2: usize,
// COMMON-REWRITES-NEXT:     ) -> *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     fn strcmp(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> i32;
// COMMON-REWRITES-NEXT:     fn obstack_free(_0: *mut obstack, _1: *mut core::ffi::c_void);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%d %d\n".as_ptr(),
// COMMON-REWRITES-NEXT:             gnu_allocation_extensions(),
// COMMON-REWRITES-NEXT:             gnu_obstack_extensions(),
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn gnu_allocation_extensions() -> i32 {
// COMMON-REWRITES-NEXT:     let mut values: *mut i32 = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     let mut aligned: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     let mut page: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     let mut rounded: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     let mut page_size: i64 = 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// COMMON-REWRITES-NEXT:         reallocarray(
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:             (4 as u64) as usize,
// COMMON-REWRITES-NEXT:             (4 as u64) as usize,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     values = {{__v[0-9]+}} as *mut i32;
// COMMON-REWRITES-NEXT:     aligned = unsafe { memalign((64 as u64) as usize, (80 as u64) as usize) };
// COMMON-REWRITES-NEXT:     page = unsafe { valloc((1 as u64) as usize) };
// COMMON-REWRITES-NEXT:     rounded = unsafe { pvalloc((1 as u64) as usize) };
// COMMON-REWRITES-NEXT:     page_size = unsafe { sysconf(30 as i32) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     for index in 0..4 {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = index + 1;
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut i32 = values;
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset((index as i64) as isize) };
// COMMON-REWRITES-NEXT:         unsafe {
// COMMON-REWRITES-NEXT:             *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = values;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(0) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = values;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(1) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + unsafe { *{{__v[0-9]+}} };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = values;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(2) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + unsafe { *{{__v[0-9]+}} };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = values;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(3) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + ({{__v[0-9]+}} + unsafe { *{{__v[0-9]+}} });
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe { malloc_usable_size(values as *mut core::ffi::c_void) }) as u64;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = 4;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} >= 4 * {{__v[0-9]+}}) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if aligned != std::ptr::null_mut() {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = (aligned as u64) % 64 == 0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + ({{__v[0-9]+}} as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if page != std::ptr::null_mut() {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = (page as u64) % (page_size as u64) == 0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + ({{__v[0-9]+}} as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if rounded != std::ptr::null_mut() {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = (rounded as u64) % (page_size as u64) == 0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + ({{__v[0-9]+}} as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe { malloc_usable_size(rounded as *mut core::ffi::c_void) }) as u64;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} >= (page_size as u64)) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { mallopt(-5 as i32, 1 as i32) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} != 0) as i32);
// COMMON-REWRITES-NEXT:     unsafe { free(values as *mut core::ffi::c_void) };
// COMMON-REWRITES-NEXT:     unsafe { free(aligned as *mut core::ffi::c_void) };
// COMMON-REWRITES-NEXT:     unsafe { free(page as *mut core::ffi::c_void) };
// COMMON-REWRITES-NEXT:     unsafe { free(rounded as *mut core::ffi::c_void) };
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}}
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn gnu_obstack_extensions() -> i32 {
// COMMON-REWRITES-NEXT:     let mut storage: obstack = obstack {
// COMMON-REWRITES-NEXT:         __bitfield_0: 0,
// COMMON-REWRITES-NEXT:         __bitfield_1: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:         __bitfield_2: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:         __bitfield_3: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:         __bitfield_4: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:         __bitfield_5: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// COMMON-REWRITES-NEXT:         __bitfield_6: 0,
// COMMON-REWRITES-NEXT:         __bitfield_7: None,
// COMMON-REWRITES-NEXT:         __bitfield_8: None,
// COMMON-REWRITES-NEXT:         __bitfield_9: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:         __bitfield_10: unsafe {
// COMMON-REWRITES-NEXT:             std::mem::transmute::<u8, __slate_bitfields::__SlateBitfield_obstack_10>(0)
// COMMON-REWRITES-NEXT:         },
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(i64) -> *mut core::ffi::c_void> = unsafe {
// COMMON-REWRITES-NEXT:         std::mem::transmute::<
// COMMON-REWRITES-NEXT:             *const (),
// COMMON-REWRITES-NEXT:             Option<unsafe extern "C-unwind" fn(i64) -> *mut core::ffi::c_void>,
// COMMON-REWRITES-NEXT:         >(malloc as *const ())
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         _obstack_begin(
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(storage) as *mut obstack,
// COMMON-REWRITES-NEXT:             0 as i32,
// COMMON-REWRITES-NEXT:             0 as i32,
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             unsafe {
// COMMON-REWRITES-NEXT:                 std::mem::transmute::<
// COMMON-REWRITES-NEXT:                     *const (),
// COMMON-REWRITES-NEXT:                     Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void)>,
// COMMON-REWRITES-NEXT:                 >(free as *const ())
// COMMON-REWRITES-NEXT:             },
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         storage.__bitfield_5.tempint = 3;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { storage.__bitfield_5.tempint };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > storage.__bitfield_4;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         unsafe {
// COMMON-REWRITES-NEXT:             _obstack_newchunk(
// COMMON-REWRITES-NEXT:                 std::ptr::addr_of_mut!(storage) as *mut obstack,
// COMMON-REWRITES-NEXT:                 ((unsafe { storage.__bitfield_5.tempint }) + 1) as i32,
// COMMON-REWRITES-NEXT:             )
// COMMON-REWRITES-NEXT:         };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         std::ptr::copy_nonoverlapping(
// COMMON-REWRITES-NEXT:             c"gnu".as_ptr() as *const u8,
// COMMON-REWRITES-NEXT:             (storage.__bitfield_3 as *mut core::ffi::c_void) as *mut u8,
// COMMON-REWRITES-NEXT:             ((unsafe { storage.__bitfield_5.tempint }) as u64) as usize,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { storage.__bitfield_5.tempint };
// COMMON-REWRITES-NEXT:     storage.__bitfield_3 = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     storage.__bitfield_3 = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = storage.__bitfield_3 == storage.__bitfield_2;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         storage
// COMMON-REWRITES-NEXT:             .__bitfield_10
// COMMON-REWRITES-NEXT:             .set_maybe_empty_object((1 as u32) << 31 >> 31);
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         storage.__bitfield_5.tempptr = storage.__bitfield_2 as *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} + (storage.__bitfield_6 as i64);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = !storage.__bitfield_6;
// COMMON-REWRITES-NEXT:         storage.__bitfield_3 = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     storage.__bitfield_2 = storage.__bitfield_3;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         storage.__bitfield_5.tempint = 4;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { storage.__bitfield_5.tempint };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > storage.__bitfield_4;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         unsafe {
// COMMON-REWRITES-NEXT:             _obstack_newchunk(
// COMMON-REWRITES-NEXT:                 std::ptr::addr_of_mut!(storage) as *mut obstack,
// COMMON-REWRITES-NEXT:                 ((unsafe { storage.__bitfield_5.tempint }) + 1) as i32,
// COMMON-REWRITES-NEXT:             )
// COMMON-REWRITES-NEXT:         };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         std::ptr::copy_nonoverlapping(
// COMMON-REWRITES-NEXT:             c"libc".as_ptr() as *const u8,
// COMMON-REWRITES-NEXT:             (storage.__bitfield_3 as *mut core::ffi::c_void) as *mut u8,
// COMMON-REWRITES-NEXT:             ((unsafe { storage.__bitfield_5.tempint }) as u64) as usize,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { storage.__bitfield_5.tempint };
// COMMON-REWRITES-NEXT:     storage.__bitfield_3 = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     storage.__bitfield_3 = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = storage.__bitfield_3 == storage.__bitfield_2;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         storage
// COMMON-REWRITES-NEXT:             .__bitfield_10
// COMMON-REWRITES-NEXT:             .set_maybe_empty_object((1 as u32) << 31 >> 31);
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         storage.__bitfield_5.tempptr = storage.__bitfield_2 as *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} + (storage.__bitfield_6 as i64);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = !storage.__bitfield_6;
// COMMON-REWRITES-NEXT:         storage.__bitfield_3 = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     storage.__bitfield_2 = storage.__bitfield_3;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { strcmp({{__v[0-9]+}} as *const core::ffi::c_char, c"gnu".as_ptr()) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 0) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { strcmp({{__v[0-9]+}} as *const core::ffi::c_char, c"libc".as_ptr()) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 0) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + ((({{__v[0-9]+}} as u32) == 0) as i32);
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         storage.__bitfield_5.tempint = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = (unsafe { storage.__bitfield_5.tempint }) > 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i64 = unsafe { storage.__bitfield_5.tempint };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}}
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT:     limit: *mut i8,
// REWRITES-X86_64-GNU-NEXT:     contents: [i8; 4],
// REWRITES-X86_64-GNU-NEXT:     __bitfield_2: *mut i8,
// REWRITES-X86_64-GNU-NEXT:     __bitfield_3: *mut i8,
// REWRITES-X86_64-GNU-NEXT:     __bitfield_4: *mut i8,
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(1) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = 0;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(1) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// REWRITES-X86_64-GNU-NEXT:     storage.__bitfield_3 = ({{__v[0-9]+}} & ({{__v[0-9]+}} as i64)) as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from(storage.__bitfield_1 as *mut i8) as i64 };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = storage.__bitfield_4;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from(storage.__bitfield_1 as *mut i8) as i64 };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = if {{__v[0-9]+}} > {{__v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = storage.__bitfield_4;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = (unsafe { storage.__bitfield_5.tempptr }) as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(1) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = 0;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(1) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// REWRITES-X86_64-GNU-NEXT:     storage.__bitfield_3 = ({{__v[0-9]+}} & ({{__v[0-9]+}} as i64)) as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from(storage.__bitfield_1 as *mut i8) as i64 };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = storage.__bitfield_4;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from(storage.__bitfield_1 as *mut i8) as i64 };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = if {{__v[0-9]+}} > {{__v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = storage.__bitfield_4;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = (unsafe { storage.__bitfield_5.tempptr }) as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = storage.__bitfield_3;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = storage.__bitfield_2;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from(storage.__bitfield_1 as *mut i8) as i64 };
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = storage.__bitfield_4;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from(storage.__bitfield_1 as *mut i8) as i64 };
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT:     limit: *mut u8,
// REWRITES-AARCH64-GNU-NEXT:     contents: [u8; 4],
// REWRITES-AARCH64-GNU-NEXT:     __bitfield_2: *mut u8,
// REWRITES-AARCH64-GNU-NEXT:     __bitfield_3: *mut u8,
// REWRITES-AARCH64-GNU-NEXT:     __bitfield_4: *mut u8,
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = storage.__bitfield_3;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(1) };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = storage.__bitfield_3;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = 0;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = storage.__bitfield_3;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(1) };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = storage.__bitfield_3;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// REWRITES-AARCH64-GNU-NEXT:     storage.__bitfield_3 = ({{__v[0-9]+}} & ({{__v[0-9]+}} as i64)) as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = storage.__bitfield_3;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from(storage.__bitfield_1 as *mut u8) as i64 };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = storage.__bitfield_4;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from(storage.__bitfield_1 as *mut u8) as i64 };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = if {{__v[0-9]+}} > {{__v[0-9]+}} {
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = storage.__bitfield_4;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = (unsafe { storage.__bitfield_5.tempptr }) as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = storage.__bitfield_3;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(1) };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = storage.__bitfield_3;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = 0;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = storage.__bitfield_3;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(1) };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = storage.__bitfield_3;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// REWRITES-AARCH64-GNU-NEXT:     storage.__bitfield_3 = ({{__v[0-9]+}} & ({{__v[0-9]+}} as i64)) as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = storage.__bitfield_3;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from(storage.__bitfield_1 as *mut u8) as i64 };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = storage.__bitfield_4;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from(storage.__bitfield_1 as *mut u8) as i64 };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = if {{__v[0-9]+}} > {{__v[0-9]+}} {
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = storage.__bitfield_4;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = (unsafe { storage.__bitfield_5.tempptr }) as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = storage.__bitfield_3;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = storage.__bitfield_2;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from(storage.__bitfield_1 as *mut u8) as i64 };
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = storage.__bitfield_4;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from(storage.__bitfield_1 as *mut u8) as i64 };
// SLATE-FILECHECK-END rewrites-aarch64-gnu
