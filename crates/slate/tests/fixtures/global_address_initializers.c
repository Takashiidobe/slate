#include <stdio.h>

struct Pair {
  int left;
  int right;
};

static int counter = 5;
static int table[4] = {1, 2, 3, 4};
static int grid[2][3] = {{1, 2, 3}, {4, 5, 6}};
static struct Pair pair = {8, 9};
static char name[] = "slate";

static int *counter_ptr = &counter;
static int *table_ptr = table;
static int *table_third = &table[2];
static int *table_end = table + 4;
static int (*table_whole)[4] = &table;
static int *grid_row = grid[1];
static int *pair_right = &pair.right;
static struct Pair *pair_ptr = &pair;
static char *name_ptr = name;
static const char *name_tail = name + 2;
static int *const fixed_counter = &counter;
static int **counter_ptr_ptr = &counter_ptr;

static int *slots[3] = {&counter, &table[1], &pair.left};

struct Cursor {
  const char *label;
  int *value;
  struct Pair *pair;
};

static struct Cursor cursors[2] = {
    {"counter", &counter, &pair},
    {"table", table + 3, 0},
};

int main(void) {
  *counter_ptr += 1;
  table_ptr[0] = 10;
  *table_third += 20;
  (*table_whole)[3] = 40;
  grid_row[2] = 60;
  *pair_right = 90;
  pair_ptr->left = 80;
  name_ptr[0] = 'S';
  **counter_ptr_ptr += 100;
  *slots[1] = 22;
  *slots[2] += 1;
  *cursors[1].value += 400;
  int sum = 0;
  for (int *it = table; it != table_end; ++it) {
    sum += *it;
  }
  printf("%d %d %d %d %d\n", counter, *fixed_counter, table[0], table[1],
         table[2]);
  printf("%d %d %d %d\n", table[3], grid[1][2], pair.left, pair.right);
  printf("%s %s %d\n", name, name_tail, sum);
  printf("%s %d %d %s %d\n", cursors[0].label, *cursors[0].value,
         cursors[0].pair->right, cursors[1].label, cursors[1].pair == 0);
  return sum % 50;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Pair {
// CHECK-NEXT:     left: i32,
// CHECK-NEXT:     right: i32,
// CHECK-NEXT: }
// CHECK-NEXT: static mut counter: i32 = 5;
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut table: __SlateAlign16<[i32; 4]> = __SlateAlign16([1i32, 2i32, 3i32, 4i32]);
// CHECK-NEXT: static mut grid: __SlateAlign16<{{\[\[}}i32; 3]; 2]> = __SlateAlign16([
// CHECK-NEXT:     [1i32, 2i32, 3i32],
// CHECK-NEXT:     [4i32, 5i32, 6i32],
// CHECK-NEXT: ]);
// CHECK-NEXT: static mut pair: Pair = Pair { left: 8i32, right: 9i32 };
// CHECK-NEXT: static mut name: [i8; 6] = [115, 108, 97, 116, 101, 0];
// CHECK-NEXT: static mut counter_ptr: *mut i32 = unsafe { std::ptr::addr_of_mut!(counter) };
// CHECK-NEXT: static mut table_ptr: *mut i32 = unsafe { std::ptr::addr_of_mut!(table.0) as *mut i32 };
// CHECK-NEXT: static mut table_third: *mut i32 = unsafe {
// CHECK-NEXT:     unsafe { std::ptr::addr_of_mut!(table.0) as *mut i32 }.offset(2i32 as isize)
// CHECK-NEXT: };
// CHECK-NEXT: static mut table_end: *mut i32 = unsafe {
// CHECK-NEXT:     unsafe { std::ptr::addr_of_mut!(table.0) as *mut i32 }.offset(4i32 as isize)
// CHECK-NEXT: };
// CHECK-NEXT: static mut table_whole: *mut [i32; 4] = unsafe { std::ptr::addr_of_mut!(table.0) };
// CHECK-NEXT: static mut grid_row: *mut i32 = unsafe {
// CHECK-NEXT:     std::ptr::addr_of_mut!(grid.0[1i32 as usize]) as *mut i32
// CHECK-NEXT: };
// CHECK-NEXT: static mut pair_right: *mut i32 = unsafe { std::ptr::addr_of_mut!(pair.right) };
// CHECK-NEXT: static mut pair_ptr: *mut Pair = unsafe { std::ptr::addr_of_mut!(pair) };
// CHECK-NEXT: static mut name_ptr: *mut i8 = unsafe { std::ptr::addr_of_mut!(name) as *mut i8 };
// CHECK-NEXT: static mut name_tail: *const i8 = (unsafe {
// CHECK-NEXT:     unsafe { std::ptr::addr_of_mut!(name) as *mut i8 }.offset(2i32 as isize)
// CHECK-NEXT: }) as *const i8;
// CHECK-NEXT: static mut fixed_counter: *mut i32 = unsafe { std::ptr::addr_of_mut!(counter) };
// CHECK-NEXT: static mut counter_ptr_ptr: *mut *mut i32 = unsafe {
// CHECK-NEXT:     std::ptr::addr_of_mut!(counter_ptr)
// CHECK-NEXT: };
// CHECK-NEXT: static mut slots: __SlateAlign16<[*mut i32; 3]> = __SlateAlign16([
// CHECK-NEXT:     unsafe { std::ptr::addr_of_mut!(counter) },
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(table.0) as *mut i32 }.offset(1i32 as isize)
// CHECK-NEXT:     },
// CHECK-NEXT:     unsafe { std::ptr::addr_of_mut!(pair.left) },
// CHECK-NEXT: ]);
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Cursor {
// CHECK-NEXT:     label: *const i8,
// CHECK-NEXT:     value: *mut i32,
// CHECK-NEXT:     pair: *mut Pair,
// CHECK-NEXT: }
// CHECK-NEXT: static mut cursors: __SlateAlign16<[Cursor; 2]> = __SlateAlign16([
// CHECK-NEXT:     Cursor {
// CHECK-NEXT:         label: (b"counter\0".as_ptr() as *mut i8) as *const i8,
// CHECK-NEXT:         value: unsafe { std::ptr::addr_of_mut!(counter) },
// CHECK-NEXT:         pair: unsafe { std::ptr::addr_of_mut!(pair) },
// CHECK-NEXT:     },
// CHECK-NEXT:     Cursor {
// CHECK-NEXT:         label: (b"table\0".as_ptr() as *mut i8) as *const i8,
// CHECK-NEXT:         value: unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(table.0) as *mut i32 }.offset(3i32 as isize)
// CHECK-NEXT:         },
// CHECK-NEXT:         pair: std::ptr::null_mut::<Pair>(),
// CHECK-NEXT:     },
// CHECK-NEXT: ]);
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: *mut i32 = unsafe { counter_ptr };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_0]] = *[[V_0]] + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { table_ptr }.add(0) = 10;
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: *mut i32 = unsafe { table_third };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_1]] = *[[V_1]] + 20;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *((*table_whole).as_mut_ptr() as *mut i32).add(3) = 40;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { grid_row }.add(2) = 60;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *pair_right = 90;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*pair_ptr).left = 80;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { name_ptr }.add(0) = 83;
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_2:__v[0-9]+]]: *mut i32 = unsafe { *counter_ptr_ptr };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_2]] = *[[V_2]] + 100;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *slots.0[1] = 22;
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_3:__v[0-9]+]]: *mut i32 = unsafe { slots.0[2] };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_3]] = *[[V_3]] + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_4:__v[0-9]+]]: *mut i32 = unsafe { cursors.0[1].value };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_4]] = *[[V_4]] + 400;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut sum: i32 = 0;
// CHECK-NEXT:     let mut it: *mut i32 = unsafe { std::ptr::addr_of_mut!(table.0) as *mut i32 };
// CHECK-NEXT:     while it != unsafe { table_end } {
// CHECK-NEXT:         sum += unsafe { *it };
// CHECK-NEXT:         let [[V_5:__v[0-9]+]]: *mut i32 = it;
// CHECK-NEXT:         it = unsafe { [[V_5]].add(1) };
// CHECK-NEXT:     }
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {} {}", unsafe { counter }, unsafe { * fixed_counter }, unsafe { table
// CHECK-NEXT:         .0[0] }, unsafe { table.0[1] }, unsafe { table.0[2] }
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {}", unsafe { table.0[3] }, unsafe { grid.0[1] [2] }, unsafe { pair
// CHECK-NEXT:         .left }, unsafe { pair.right }
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%s %s %d\n".as_ptr(),
// CHECK-NEXT:             std::ptr::addr_of_mut!(name) as *mut i8,
// CHECK-NEXT:             name_tail,
// CHECK-NEXT:             sum,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%s %d %d %s %d\n".as_ptr(),
// CHECK-NEXT:             cursors.0[0].label,
// CHECK-NEXT:             *cursors.0[0].value,
// CHECK-NEXT:             (*cursors.0[0].pair).right,
// CHECK-NEXT:             cursors.0[1].label,
// CHECK-NEXT:             unsafe { cursors.0[1].pair }.is_null() as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     sum % 50
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
