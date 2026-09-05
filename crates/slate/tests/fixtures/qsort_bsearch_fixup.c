#include <stdio.h>
#include <stdlib.h>

struct Item {
  int key;
  int value;
};

static int cmp_int(const void *a, const void *b) {
  return *(const int *)a - *(const int *)b;
}

static int cmp_item(const void *a, const void *b) {
  const struct Item *ia = (const struct Item *)a;
  const struct Item *ib = (const struct Item *)b;
  return ia->key - ib->key;
}

int main(void) {
  int nums[5] = {4, 1, 5, 3, 2};
  int key     = 3;
  qsort(nums, 5, sizeof(int), cmp_int);
  int *hit = bsearch(&key, nums, 5, sizeof(int), cmp_int);

  struct Item items[4] = {{3, 30}, {1, 10}, {4, 40}, {2, 20}};
  struct Item needle   = {4, 0};
  qsort(items, 4, sizeof(struct Item), cmp_item);
  struct Item *found =
      bsearch(&needle, items, 4, sizeof(struct Item), cmp_item);

  printf("%d %d %d %d\n", nums[0], nums[4], hit ? *hit : -1,
         found ? found->value : -1);
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
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct Item {
// COMMON-LOWERING-NEXT:     key: i32,
// COMMON-LOWERING-NEXT:     value: i32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn qsort(
// COMMON-LOWERING-NEXT:         _0: *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:         _1: usize,
// COMMON-LOWERING-NEXT:         _2: usize,
// COMMON-LOWERING-NEXT:         _3: Option<
// COMMON-LOWERING-NEXT:             unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut core::ffi::c_void) -> i32,
// COMMON-LOWERING-NEXT:         >,
// COMMON-LOWERING-NEXT:     );
// COMMON-LOWERING-NEXT:     fn bsearch(
// COMMON-LOWERING-NEXT:         _0: *const core::ffi::c_void,
// COMMON-LOWERING-NEXT:         _1: *const core::ffi::c_void,
// COMMON-LOWERING-NEXT:         _2: usize,
// COMMON-LOWERING-NEXT:         _3: usize,
// COMMON-LOWERING-NEXT:         _4: Option<
// COMMON-LOWERING-NEXT:             unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut core::ffi::c_void) -> i32,
// COMMON-LOWERING-NEXT:         >,
// COMMON-LOWERING-NEXT:     ) -> *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut key: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut hit: *mut i32 = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:         aligned::Aligned([Item { key: 0, value: 0 }; 4]);
// COMMON-LOWERING-NEXT:     let mut needle: Item = Item { key: 0, value: 0 };
// COMMON-LOWERING-NEXT:     let mut found: *mut Item = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: [i32; 5] = [4, 1, 5, 3, 2];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:     key = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = nums.as_mut_ptr() as *mut i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 5;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 4;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         qsort(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// COMMON-LOWERING-NEXT:             Some(cmp_int),
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(key) as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = nums.as_mut_ptr() as *mut i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 5;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 4;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// COMMON-LOWERING-NEXT:         bsearch(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_void,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_void,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// COMMON-LOWERING-NEXT:             Some(cmp_int),
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = {{__v[0-9]+}} as *mut i32;
// COMMON-LOWERING-NEXT:     hit = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: [Item; 4] = [
// COMMON-LOWERING-NEXT:         Item { key: 3, value: 30 },
// COMMON-LOWERING-NEXT:         Item { key: 1, value: 10 },
// COMMON-LOWERING-NEXT:         Item { key: 4, value: 40 },
// COMMON-LOWERING-NEXT:         Item { key: 2, value: 20 },
// COMMON-LOWERING-NEXT:     ];
// COMMON-LOWERING-NEXT:     *items = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: Item = Item { key: 4, value: 0 };
// COMMON-LOWERING-NEXT:     needle = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut Item = items.as_mut_ptr() as *mut Item;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 4;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = std::mem::size_of::<Item>() as u64;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         qsort(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// COMMON-LOWERING-NEXT:             Some(cmp_item),
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(needle) as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut Item = items.as_mut_ptr() as *mut Item;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 4;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = std::mem::size_of::<Item>() as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// COMMON-LOWERING-NEXT:         bsearch(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_void,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_void,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// COMMON-LOWERING-NEXT:             Some(cmp_item),
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut Item = {{__v[0-9]+}} as *mut Item;
// COMMON-LOWERING-NEXT:     found = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = nums[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 4;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = nums[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = hit;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: *mut i32 = hit;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = -1;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut Item = found;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: *mut Item = found;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).value };
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = -1;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         printf(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: extern "C-unwind" fn cmp_int({{arg[0-9]+}}: *mut core::ffi::c_void, {{arg[0-9]+}}: *mut core::ffi::c_void) -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = {{arg[0-9]+}} as *mut i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = {{arg[0-9]+}} as *mut i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} - {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: extern "C-unwind" fn cmp_item({{arg[0-9]+}}: *mut core::ffi::c_void, {{arg[0-9]+}}: *mut core::ffi::c_void) -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut Item = {{arg[0-9]+}} as *mut Item;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut Item = {{arg[0-9]+}} as *mut Item;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).key };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).key };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} - {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let mut nums: aligned::Aligned<aligned::A16, [i32; 5]> = aligned::Aligned([0; 5]);
// LOWERING-X86_64-GNU-NEXT:     let mut items: aligned::Aligned<aligned::A16, [Item; 4]> =
// LOWERING-X86_64-GNU-NEXT:     *nums = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     let mut nums: [i32; 5] = [0; 5];
// LOWERING-AARCH64-GNU-NEXT:     let mut items: aligned::Aligned<aligned::A4, [Item; 4]> =
// LOWERING-AARCH64-GNU-NEXT:     nums = {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d %d\n\0".as_ptr() as *mut u8;
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
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct Item {
// COMMON-REWRITES-NEXT:     key: i32,
// COMMON-REWRITES-NEXT:     value: i32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn qsort(
// COMMON-REWRITES-NEXT:         _0: *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:         _1: usize,
// COMMON-REWRITES-NEXT:         _2: usize,
// COMMON-REWRITES-NEXT:         _3: Option<
// COMMON-REWRITES-NEXT:             unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut core::ffi::c_void) -> i32,
// COMMON-REWRITES-NEXT:         >,
// COMMON-REWRITES-NEXT:     );
// COMMON-REWRITES-NEXT:     fn bsearch(
// COMMON-REWRITES-NEXT:         _0: *const core::ffi::c_void,
// COMMON-REWRITES-NEXT:         _1: *const core::ffi::c_void,
// COMMON-REWRITES-NEXT:         _2: usize,
// COMMON-REWRITES-NEXT:         _3: usize,
// COMMON-REWRITES-NEXT:         _4: Option<
// COMMON-REWRITES-NEXT:             unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut core::ffi::c_void) -> i32,
// COMMON-REWRITES-NEXT:         >,
// COMMON-REWRITES-NEXT:     ) -> *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let mut key: i32 = 0;
// COMMON-REWRITES-NEXT:     let mut hit: *mut i32 = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:         aligned::Aligned([Item { key: 0, value: 0 }; 4]);
// COMMON-REWRITES-NEXT:     let mut needle: Item = Item { key: 0, value: 0 };
// COMMON-REWRITES-NEXT:     let mut found: *mut Item = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     key = 3;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         qsort(
// COMMON-REWRITES-NEXT:             nums.as_mut_ptr() as *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:             (5 as u64) as usize,
// COMMON-REWRITES-NEXT:             (4 as u64) as usize,
// COMMON-REWRITES-NEXT:             Some(cmp_int),
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(key) as *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = nums.as_mut_ptr() as *mut i32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// COMMON-REWRITES-NEXT:         bsearch(
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_void,
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_void,
// COMMON-REWRITES-NEXT:             (5 as u64) as usize,
// COMMON-REWRITES-NEXT:             (4 as u64) as usize,
// COMMON-REWRITES-NEXT:             Some(cmp_int),
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     hit = {{__v[0-9]+}} as *mut i32;
// COMMON-REWRITES-NEXT:     *items = [
// COMMON-REWRITES-NEXT:         Item { key: 3, value: 30 },
// COMMON-REWRITES-NEXT:         Item { key: 1, value: 10 },
// COMMON-REWRITES-NEXT:         Item { key: 4, value: 40 },
// COMMON-REWRITES-NEXT:         Item { key: 2, value: 20 },
// COMMON-REWRITES-NEXT:     ];
// COMMON-REWRITES-NEXT:     needle = Item { key: 4, value: 0 };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut Item = items.as_mut_ptr() as *mut Item;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         qsort(
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:             (4 as u64) as usize,
// COMMON-REWRITES-NEXT:             (std::mem::size_of::<Item>() as u64) as usize,
// COMMON-REWRITES-NEXT:             Some(cmp_item),
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(needle) as *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut Item = items.as_mut_ptr() as *mut Item;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = std::mem::size_of::<Item>() as u64;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// COMMON-REWRITES-NEXT:         bsearch(
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_void,
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_void,
// COMMON-REWRITES-NEXT:             (4 as u64) as usize,
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as usize,
// COMMON-REWRITES-NEXT:             Some(cmp_item),
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     found = {{__v[0-9]+}} as *mut Item;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = nums[0];
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = nums[4];
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = if hit != std::ptr::null_mut() {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { *hit };
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = -1;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = if found != std::ptr::null_mut() {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { (*found).value };
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = -1;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: extern "C-unwind" fn cmp_int({{arg[0-9]+}}: *mut core::ffi::c_void, {{arg[0-9]+}}: *mut core::ffi::c_void) -> i32 {
// COMMON-REWRITES-NEXT:     (unsafe { *({{arg[0-9]+}} as *mut i32) }) - unsafe { *({{arg[0-9]+}} as *mut i32) }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: extern "C-unwind" fn cmp_item({{arg[0-9]+}}: *mut core::ffi::c_void, {{arg[0-9]+}}: *mut core::ffi::c_void) -> i32 {
// COMMON-REWRITES-NEXT:     (unsafe { (*({{arg[0-9]+}} as *mut Item)).key }) - unsafe { (*({{arg[0-9]+}} as *mut Item)).key }
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT:     let mut nums: aligned::Aligned<aligned::A16, [i32; 5]> = aligned::Aligned([0; 5]);
// REWRITES-X86_64-GNU-NEXT:     let mut items: aligned::Aligned<aligned::A16, [Item; 4]> =
// REWRITES-X86_64-GNU-NEXT:     *nums = [4, 1, 5, 3, 2];
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"%d %d %d %d\n".as_ptr() as *mut i8;
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT:     let mut nums: [i32; 5] = [4, 1, 5, 3, 2];
// REWRITES-AARCH64-GNU-NEXT:     let mut items: aligned::Aligned<aligned::A4, [Item; 4]> =
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"%d %d %d %d\n".as_ptr() as *mut u8;
// SLATE-FILECHECK-END rewrites-aarch64-gnu
