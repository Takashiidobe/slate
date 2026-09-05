#include <stddef.h>
#include <stdlib.h>

struct FlexibleArray {
  size_t count;
  int    values[];
};

int main(void) {
  if (sizeof(struct FlexibleArray) != sizeof(size_t)) {
    return 1;
  }

  struct FlexibleArray *flexible =
      malloc(sizeof(*flexible) + 3 * sizeof(flexible->values[0]));
  if (flexible == NULL) {
    return 2;
  }

  flexible->count = 3;
  for (size_t index = 0; index < flexible->count; ++index) {
    flexible->values[index] = (int)index + 1;
  }

  int total = 0;
  for (size_t index = 0; index < flexible->count; ++index) {
    total += flexible->values[index];
  }

  free(flexible);
  return total == 6 ? 0 : 3;
}

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING: #![allow(
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
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct FlexibleArray {
// COMMON-LOWERING-NEXT:     count: u64,
// COMMON-LOWERING-NEXT:     values: [i32; 0],
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     fn free(_0: *mut core::ffi::c_void);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut __retval: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut flexible: *mut FlexibleArray = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let mut total: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = std::mem::size_of::<FlexibleArray>() as u64;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = 8;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:             __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:             std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 4;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe { malloc({{__v[0-9]+}} as usize) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut FlexibleArray = {{__v[0-9]+}} as *mut FlexibleArray;
// COMMON-LOWERING-NEXT:     flexible = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: *mut FlexibleArray = flexible;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: *mut FlexibleArray = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 2;
// COMMON-LOWERING-NEXT:             __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:             std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut FlexibleArray = flexible;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         (*{{__v[0-9]+}}).count = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let mut index: u64 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = 0;
// COMMON-LOWERING-NEXT:         index = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = index;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: *mut FlexibleArray = flexible;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = unsafe { (*{{__v[0-9]+}}).count };
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: u64 = index;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: u64 = index;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut FlexibleArray = flexible;
// COMMON-LOWERING-NEXT:                 unsafe {
// COMMON-LOWERING-NEXT:                     *(*{{__v[0-9]+}}).values.as_mut_ptr().add({{__v[0-9]+}} as usize) = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = index;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:             index = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     total = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let mut index2: u64 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = 0;
// COMMON-LOWERING-NEXT:         index2 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = index2;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: *mut FlexibleArray = flexible;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = unsafe { (*{{__v[0-9]+}}).count };
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: u64 = index2;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut FlexibleArray = flexible;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = unsafe { *(*{{__v[0-9]+}}).values.as_mut_ptr().add({{__v[0-9]+}} as usize) };
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = total;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 total = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = index2;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:             index2 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut FlexibleArray = flexible;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     unsafe { free({{__v[0-9]+}} as *mut core::ffi::c_void) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = total;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 6;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES: #![allow(
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
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct FlexibleArray {
// COMMON-REWRITES-NEXT:     count: u64,
// COMMON-REWRITES-NEXT:     values: [i32; 0],
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     fn free(_0: *mut core::ffi::c_void);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let mut __retval: i32 = 0;
// COMMON-REWRITES-NEXT:     let mut flexible: *mut FlexibleArray = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     let mut total: i32 = 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = std::mem::size_of::<FlexibleArray>() as u64;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 8;
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         __retval = 1;
// COMMON-REWRITES-NEXT:         std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = 4;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = 8 + 3 * {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe { malloc({{__v[0-9]+}} as usize) };
// COMMON-REWRITES-NEXT:     flexible = {{__v[0-9]+}} as *mut FlexibleArray;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = flexible == std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         __retval = 2;
// COMMON-REWRITES-NEXT:         std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         (*flexible).count = 3;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     for index in 0..unsafe { (*flexible).count } {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = (index as i32) + 1;
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: u64 = index;
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut FlexibleArray = flexible;
// COMMON-REWRITES-NEXT:         unsafe {
// COMMON-REWRITES-NEXT:             *(*{{__v[0-9]+}}).values.as_mut_ptr().add({{__v[0-9]+}} as usize) = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     for index2 in 0..unsafe { (*flexible).count } {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: u64 = index2;
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut FlexibleArray = flexible;
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { *(*{{__v[0-9]+}}).values.as_mut_ptr().add({{__v[0-9]+}} as usize) };
// COMMON-REWRITES-NEXT:         total += {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     unsafe { free(flexible as *mut core::ffi::c_void) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-REWRITES-NEXT:     __retval = if total == 6 { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-REWRITES-NEXT:     std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites
