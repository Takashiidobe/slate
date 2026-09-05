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

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![allow(
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
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct FlexibleArray {
// LOWERING-NEXT:     count: u64,
// LOWERING-NEXT:     values: [i32; 0],
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn free(_0: *mut core::ffi::c_void);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut flexible: *mut FlexibleArray = std::ptr::null_mut();
// LOWERING-NEXT:     let mut total: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{__v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = std::mem::size_of::<FlexibleArray>() as u64;
// LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = 8;
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-NEXT:         if {{__v[0-9]+}} {
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:             __retval = {{__v[0-9]+}};
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:             std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 3;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} * {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe { malloc({{__v[0-9]+}} as usize) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut FlexibleArray = {{__v[0-9]+}} as *mut FlexibleArray;
// LOWERING-NEXT:     flexible = {{__v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{__v[0-9]+}}: *mut FlexibleArray = flexible;
// LOWERING-NEXT:         let {{__v[0-9]+}}: *mut FlexibleArray = std::ptr::null_mut();
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:         if {{__v[0-9]+}} {
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:             __retval = {{__v[0-9]+}};
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:             std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 3;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut FlexibleArray = flexible;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*{{__v[0-9]+}}).count = {{__v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut index: u64 = 0;
// LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = 0;
// LOWERING-NEXT:         index = {{__v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = index;
// LOWERING-NEXT:             let {{__v[0-9]+}}: *mut FlexibleArray = flexible;
// LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = unsafe { (*{{__v[0-9]+}}).count };
// LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-NEXT:             if !{{__v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let {{__v[0-9]+}}: u64 = index;
// LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:                 let {{__v[0-9]+}}: u64 = index;
// LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut FlexibleArray = flexible;
// LOWERING-NEXT:                 unsafe {
// LOWERING-NEXT:                     *(*{{__v[0-9]+}}).values.as_mut_ptr().add({{__v[0-9]+}} as usize) = {{__v[0-9]+}};
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = index;
// LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + 1;
// LOWERING-NEXT:             index = {{__v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     total = {{__v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut index2: u64 = 0;
// LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = 0;
// LOWERING-NEXT:         index2 = {{__v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = index2;
// LOWERING-NEXT:             let {{__v[0-9]+}}: *mut FlexibleArray = flexible;
// LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = unsafe { (*{{__v[0-9]+}}).count };
// LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-NEXT:             if !{{__v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let {{__v[0-9]+}}: u64 = index2;
// LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut FlexibleArray = flexible;
// LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = unsafe { *(*{{__v[0-9]+}}).values.as_mut_ptr().add({{__v[0-9]+}} as usize) };
// LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = total;
// LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:                 total = {{__v[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = index2;
// LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + 1;
// LOWERING-NEXT:             index2 = {{__v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut FlexibleArray = flexible;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     unsafe { free({{__v[0-9]+}} as *mut core::ffi::c_void) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = total;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 6;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// LOWERING-NEXT:     __retval = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![allow(
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
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct FlexibleArray {
// REWRITES-NEXT:     count: u64,
// REWRITES-NEXT:     values: [i32; 0],
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn free(_0: *mut core::ffi::c_void);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut __retval: i32 = 0;
// REWRITES-NEXT:     let mut flexible: *mut FlexibleArray = std::ptr::null_mut();
// REWRITES-NEXT:     let mut total: i32 = 0;
// REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = std::mem::size_of::<FlexibleArray>() as u64;
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 8;
// REWRITES-NEXT:     if {{__v[0-9]+}} {
// REWRITES-NEXT:         __retval = 1;
// REWRITES-NEXT:         std::process::exit(__retval as i32);
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = 4;
// REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = 8 + 3 * {{__v[0-9]+}};
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe { malloc({{__v[0-9]+}} as usize) };
// REWRITES-NEXT:     flexible = {{__v[0-9]+}} as *mut FlexibleArray;
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = flexible == std::ptr::null_mut();
// REWRITES-NEXT:     if {{__v[0-9]+}} {
// REWRITES-NEXT:         __retval = 2;
// REWRITES-NEXT:         std::process::exit(__retval as i32);
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*flexible).count = 3;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     for index in 0..unsafe { (*flexible).count } {
// REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = (index as i32) + 1;
// REWRITES-NEXT:         let {{__v[0-9]+}}: u64 = index;
// REWRITES-NEXT:         let {{__v[0-9]+}}: *mut FlexibleArray = flexible;
// REWRITES-NEXT:         unsafe {
// REWRITES-NEXT:             *(*{{__v[0-9]+}}).values.as_mut_ptr().add({{__v[0-9]+}} as usize) = {{__v[0-9]+}};
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT:     for index2 in 0..unsafe { (*flexible).count } {
// REWRITES-NEXT:         let {{__v[0-9]+}}: u64 = index2;
// REWRITES-NEXT:         let {{__v[0-9]+}}: *mut FlexibleArray = flexible;
// REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { *(*{{__v[0-9]+}}).values.as_mut_ptr().add({{__v[0-9]+}} as usize) };
// REWRITES-NEXT:         total += {{__v[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe { free(flexible as *mut core::ffi::c_void) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// REWRITES-NEXT:     __retval = if total == 6 { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// REWRITES-NEXT:     std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
