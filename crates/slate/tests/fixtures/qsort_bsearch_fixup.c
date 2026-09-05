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
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct Item {
// LOWERING-NEXT:     key: i32,
// LOWERING-NEXT:     value: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn qsort(
// LOWERING-NEXT:         _0: *mut core::ffi::c_void,
// LOWERING-NEXT:         _1: usize,
// LOWERING-NEXT:         _2: usize,
// LOWERING-NEXT:         _3: Option<
// LOWERING-NEXT:             unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut core::ffi::c_void) -> i32,
// LOWERING-NEXT:         >,
// LOWERING-NEXT:     );
// LOWERING-NEXT:     fn bsearch(
// LOWERING-NEXT:         _0: *const core::ffi::c_void,
// LOWERING-NEXT:         _1: *const core::ffi::c_void,
// LOWERING-NEXT:         _2: usize,
// LOWERING-NEXT:         _3: usize,
// LOWERING-NEXT:         _4: Option<
// LOWERING-NEXT:             unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut core::ffi::c_void) -> i32,
// LOWERING-NEXT:         >,
// LOWERING-NEXT:     ) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut nums: aligned::Aligned<aligned::A16, [i32; 5]> = aligned::Aligned([0; 5]);
// LOWERING-NEXT:     let mut key: i32 = 0;
// LOWERING-NEXT:     let mut hit: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut items: aligned::Aligned<aligned::A16, [Item; 4]> =
// LOWERING-NEXT:         aligned::Aligned([Item { key: 0, value: 0 }; 4]);
// LOWERING-NEXT:     let mut needle: Item = Item { key: 0, value: 0 };
// LOWERING-NEXT:     let mut found: *mut Item = std::ptr::null_mut();
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: [i32; 5] = [4, 1, 5, 3, 2];
// LOWERING-NEXT:     *nums = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     key = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = nums.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 5;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         qsort(
// LOWERING-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// LOWERING-NEXT:             Some(cmp_int),
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(key) as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = nums.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 5;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// LOWERING-NEXT:         bsearch(
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_void,
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_void,
// LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// LOWERING-NEXT:             Some(cmp_int),
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = {{__v[0-9]+}} as *mut i32;
// LOWERING-NEXT:     hit = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: [Item; 4] = [
// LOWERING-NEXT:         Item { key: 3, value: 30 },
// LOWERING-NEXT:         Item { key: 1, value: 10 },
// LOWERING-NEXT:         Item { key: 4, value: 40 },
// LOWERING-NEXT:         Item { key: 2, value: 20 },
// LOWERING-NEXT:     ];
// LOWERING-NEXT:     *items = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: Item = Item { key: 4, value: 0 };
// LOWERING-NEXT:     needle = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut Item = items.as_mut_ptr() as *mut Item;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = std::mem::size_of::<Item>() as u64;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         qsort(
// LOWERING-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// LOWERING-NEXT:             Some(cmp_item),
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(needle) as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut Item = items.as_mut_ptr() as *mut Item;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = std::mem::size_of::<Item>() as u64;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// LOWERING-NEXT:         bsearch(
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_void,
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_void,
// LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// LOWERING-NEXT:             Some(cmp_item),
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut Item = {{__v[0-9]+}} as *mut Item;
// LOWERING-NEXT:     found = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = nums[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 4;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = nums[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = hit;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} {
// LOWERING-NEXT:         let {{__v[0-9]+}}: *mut i32 = hit;
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-NEXT:         {{__v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = -1;
// LOWERING-NEXT:         {{__v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut Item = found;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} {
// LOWERING-NEXT:         let {{__v[0-9]+}}: *mut Item = found;
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).value };
// LOWERING-NEXT:         {{__v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = -1;
// LOWERING-NEXT:         {{__v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C-unwind" fn cmp_int({{arg[0-9]+}}: *mut core::ffi::c_void, {{arg[0-9]+}}: *mut core::ffi::c_void) -> i32 {
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = {{arg[0-9]+}} as *mut i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = {{arg[0-9]+}} as *mut i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C-unwind" fn cmp_item({{arg[0-9]+}}: *mut core::ffi::c_void, {{arg[0-9]+}}: *mut core::ffi::c_void) -> i32 {
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut Item = {{arg[0-9]+}} as *mut Item;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut Item = {{arg[0-9]+}} as *mut Item;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).key };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).key };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-NEXT:     return {{__v[0-9]+}};
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
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct Item {
// REWRITES-NEXT:     key: i32,
// REWRITES-NEXT:     value: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn qsort(
// REWRITES-NEXT:         _0: *mut core::ffi::c_void,
// REWRITES-NEXT:         _1: usize,
// REWRITES-NEXT:         _2: usize,
// REWRITES-NEXT:         _3: Option<
// REWRITES-NEXT:             unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut core::ffi::c_void) -> i32,
// REWRITES-NEXT:         >,
// REWRITES-NEXT:     );
// REWRITES-NEXT:     fn bsearch(
// REWRITES-NEXT:         _0: *const core::ffi::c_void,
// REWRITES-NEXT:         _1: *const core::ffi::c_void,
// REWRITES-NEXT:         _2: usize,
// REWRITES-NEXT:         _3: usize,
// REWRITES-NEXT:         _4: Option<
// REWRITES-NEXT:             unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut core::ffi::c_void) -> i32,
// REWRITES-NEXT:         >,
// REWRITES-NEXT:     ) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut nums: aligned::Aligned<aligned::A16, [i32; 5]> = aligned::Aligned([0; 5]);
// REWRITES-NEXT:     let mut key: i32 = 0;
// REWRITES-NEXT:     let mut hit: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT:     let mut items: aligned::Aligned<aligned::A16, [Item; 4]> =
// REWRITES-NEXT:         aligned::Aligned([Item { key: 0, value: 0 }; 4]);
// REWRITES-NEXT:     let mut needle: Item = Item { key: 0, value: 0 };
// REWRITES-NEXT:     let mut found: *mut Item = std::ptr::null_mut();
// REWRITES-NEXT:     *nums = [4, 1, 5, 3, 2];
// REWRITES-NEXT:     key = 3;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         qsort(
// REWRITES-NEXT:             nums.as_mut_ptr() as *mut core::ffi::c_void,
// REWRITES-NEXT:             (5 as u64) as usize,
// REWRITES-NEXT:             (4 as u64) as usize,
// REWRITES-NEXT:             Some(cmp_int),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(key) as *mut core::ffi::c_void;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = nums.as_mut_ptr() as *mut i32;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// REWRITES-NEXT:         bsearch(
// REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_void,
// REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_void,
// REWRITES-NEXT:             (5 as u64) as usize,
// REWRITES-NEXT:             (4 as u64) as usize,
// REWRITES-NEXT:             Some(cmp_int),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     hit = {{__v[0-9]+}} as *mut i32;
// REWRITES-NEXT:     *items = [
// REWRITES-NEXT:         Item { key: 3, value: 30 },
// REWRITES-NEXT:         Item { key: 1, value: 10 },
// REWRITES-NEXT:         Item { key: 4, value: 40 },
// REWRITES-NEXT:         Item { key: 2, value: 20 },
// REWRITES-NEXT:     ];
// REWRITES-NEXT:     needle = Item { key: 4, value: 0 };
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut Item = items.as_mut_ptr() as *mut Item;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         qsort(
// REWRITES-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// REWRITES-NEXT:             (4 as u64) as usize,
// REWRITES-NEXT:             (std::mem::size_of::<Item>() as u64) as usize,
// REWRITES-NEXT:             Some(cmp_item),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(needle) as *mut core::ffi::c_void;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut Item = items.as_mut_ptr() as *mut Item;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = std::mem::size_of::<Item>() as u64;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// REWRITES-NEXT:         bsearch(
// REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_void,
// REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_void,
// REWRITES-NEXT:             (4 as u64) as usize,
// REWRITES-NEXT:             {{__v[0-9]+}} as usize,
// REWRITES-NEXT:             Some(cmp_item),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     found = {{__v[0-9]+}} as *mut Item;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"%d %d %d %d\n".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = nums[0];
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = nums[4];
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = if hit != std::ptr::null_mut() {
// REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { *hit };
// REWRITES-NEXT:         {{__v[0-9]+}}
// REWRITES-NEXT:     } else {
// REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = -1;
// REWRITES-NEXT:         {{__v[0-9]+}}
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = if found != std::ptr::null_mut() {
// REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { (*found).value };
// REWRITES-NEXT:         {{__v[0-9]+}}
// REWRITES-NEXT:     } else {
// REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = -1;
// REWRITES-NEXT:         {{__v[0-9]+}}
// REWRITES-NEXT:     };
// REWRITES-NEXT:     unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C-unwind" fn cmp_int({{arg[0-9]+}}: *mut core::ffi::c_void, {{arg[0-9]+}}: *mut core::ffi::c_void) -> i32 {
// REWRITES-NEXT:     (unsafe { *({{arg[0-9]+}} as *mut i32) }) - unsafe { *({{arg[0-9]+}} as *mut i32) }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C-unwind" fn cmp_item({{arg[0-9]+}}: *mut core::ffi::c_void, {{arg[0-9]+}}: *mut core::ffi::c_void) -> i32 {
// REWRITES-NEXT:     (unsafe { (*({{arg[0-9]+}} as *mut Item)).key }) - unsafe { (*({{arg[0-9]+}} as *mut Item)).key }
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
