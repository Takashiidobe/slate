#include <stdio.h>

struct point {
  int x;
  int y;
};

struct row {
  int tag;
  struct point pts[3];
};

static int flat[8] = {[1 ... 3] = 7, [5] = 9, [6 ... 7] = -1};
static int grid[3][4] = {[0 ... 2][1 ... 2] = 4, [1][2] = 5};
static struct row rows[3] = {[0 ... 2].pts[0 ... 1] = {1, 2}, [2].tag = 8};
static int scalar = {};
static double real = {};
static void *pointer = {};

int main(void) {
  int seed = 3;
  int local[6] = {[0 ... 4] = seed * 2, [2] = seed};
  struct point many[4] = {[1 ... 3] = {seed, -seed}};
  int empty = {};
  float empty_f = {};
  for (int i = 0; i < 8; i++)
    printf("flat[%d]=%d\n", i, flat[i]);
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 4; j++)
      printf("grid[%d][%d]=%d\n", i, j, grid[i][j]);
  for (int i = 0; i < 3; i++) {
    printf("rows[%d].tag=%d", i, rows[i].tag);
    for (int j = 0; j < 3; j++)
      printf(" (%d,%d)", rows[i].pts[j].x, rows[i].pts[j].y);
    printf("\n");
  }
  for (int i = 0; i < 6; i++)
    printf("local[%d]=%d\n", i, local[i]);
  for (int i = 0; i < 4; i++)
    printf("many[%d]=(%d,%d)\n", i, many[i].x, many[i].y);
  printf("%d %g %d %d %g %d\n", scalar, real, pointer == 0, empty, empty_f, (int){} + (int)(float){});
  return 0;
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
// CHECK-NEXT: struct point {
// CHECK-NEXT:     x: i32,
// CHECK-NEXT:     y: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct row {
// CHECK-NEXT:     tag: i32,
// CHECK-NEXT:     pts: [point; 3],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut flat: __SlateAlign16<[i32; 8]> = __SlateAlign16({
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: [i32; 8] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let [[T_1:__t[0-9]+]] = 7 as i32;
// CHECK-NEXT:     let mut [[T_2:__t[0-9]+]]: usize = 1;
// CHECK-NEXT:     while [[T_2]] <= 3 {
// CHECK-NEXT:         [[T_0]][[[T_2]]] = [[T_1]];
// CHECK-NEXT:         [[T_2]] += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     [[T_0]][5] = 9 as i32;
// CHECK-NEXT:     let [[T_3:__t[0-9]+]] = -(1 as i32);
// CHECK-NEXT:     let mut [[T_4:__t[0-9]+]]: usize = 6;
// CHECK-NEXT:     while [[T_4]] <= 7 {
// CHECK-NEXT:         [[T_0]][[[T_4]]] = [[T_3]];
// CHECK-NEXT:         [[T_4]] += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: });
// CHECK-NEXT: static mut grid: __SlateAlign16<{{\[\[}}i32; 4]; 3]> = __SlateAlign16([
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut [[T_0]]: [i32; 4] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let [[T_1]] = 4 as i32;
// CHECK-NEXT:         let mut [[T_2]]: usize = 1;
// CHECK-NEXT:         while [[T_2]] <= 2 {
// CHECK-NEXT:             [[T_0]][[[T_2]]] = [[T_1]];
// CHECK-NEXT:             [[T_2]] += 1;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut [[T_3]]: [i32; 4] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_3]][1] = 4 as i32;
// CHECK-NEXT:         [[T_3]][2] = 5 as i32;
// CHECK-NEXT:         [[T_3]]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut [[T_4]]: [i32; 4] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let [[T_5:__t[0-9]+]] = 4 as i32;
// CHECK-NEXT:         let mut [[T_6:__t[0-9]+]]: usize = 1;
// CHECK-NEXT:         while [[T_6]] <= 2 {
// CHECK-NEXT:             [[T_4]][[[T_6]]] = [[T_5]];
// CHECK-NEXT:             [[T_6]] += 1;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_4]]
// CHECK-NEXT:     },
// CHECK-NEXT: ]);
// CHECK-NEXT: static mut rows: __SlateAlign16<[row; 3]> = __SlateAlign16({
// CHECK-NEXT:     let mut [[T_0]]: [row; 3] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let [[T_1]] = {
// CHECK-NEXT:         let mut [[T_3]]: row = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_3]].pts = {
// CHECK-NEXT:             let mut [[T_4]]: [point; 3] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             let [[T_5]] = point { x: 1 as i32, y: 2 as i32 };
// CHECK-NEXT:             let mut [[T_6]]: usize = 0;
// CHECK-NEXT:             while [[T_6]] <= 1 {
// CHECK-NEXT:                 [[T_4]][[[T_6]]] = [[T_5]];
// CHECK-NEXT:                 [[T_6]] += 1;
// CHECK-NEXT:             }
// CHECK-NEXT:             [[T_4]]
// CHECK-NEXT:         };
// CHECK-NEXT:         [[T_3]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[T_2]]: usize = 0;
// CHECK-NEXT:     while [[T_2]] <= 1 {
// CHECK-NEXT:         [[T_0]][[[T_2]]] = [[T_1]];
// CHECK-NEXT:         [[T_2]] += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     [[T_0]][2] = row {
// CHECK-NEXT:         tag: 8 as i32,
// CHECK-NEXT:         pts: {
// CHECK-NEXT:             let mut [[T_7:__t[0-9]+]]: [point; 3] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             let [[T_8:__t[0-9]+]] = point { x: 1 as i32, y: 2 as i32 };
// CHECK-NEXT:             let mut [[T_9:__t[0-9]+]]: usize = 0;
// CHECK-NEXT:             while [[T_9]] <= 1 {
// CHECK-NEXT:                 [[T_7]][[[T_9]]] = [[T_8]];
// CHECK-NEXT:                 [[T_9]] += 1;
// CHECK-NEXT:             }
// CHECK-NEXT:             [[T_7]]
// CHECK-NEXT:         },
// CHECK-NEXT:     };
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: });
// CHECK-NEXT: static mut scalar: i32 = {
// CHECK-NEXT:     let mut [[T_0]]: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut real: f64 = {
// CHECK-NEXT:     let mut [[T_0]]: f64 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut pointer: *mut () = {
// CHECK-NEXT:     let mut [[T_0]]: *mut () = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_1:__slate_compound_[0-9]+]]: f32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let seed: i32 = 3;
// CHECK-NEXT:     let local: __SlateAlign16<[i32; 6]> = __SlateAlign16({
// CHECK-NEXT:         let mut [[T_0]]: [i32; 6] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let [[T_1]] = seed * 2;
// CHECK-NEXT:         let mut [[T_2]]: usize = 0;
// CHECK-NEXT:         while [[T_2]] <= 1 {
// CHECK-NEXT:             [[T_0]][[[T_2]]] = [[T_1]];
// CHECK-NEXT:             [[T_2]] += 1;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_0]][2] = seed;
// CHECK-NEXT:         let [[T_3]] = seed * 2;
// CHECK-NEXT:         let mut [[T_4]]: usize = 3;
// CHECK-NEXT:         while [[T_4]] <= 4 {
// CHECK-NEXT:             [[T_0]][[[T_4]]] = [[T_3]];
// CHECK-NEXT:             [[T_4]] += 1;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     });
// CHECK-NEXT:     let many: __SlateAlign16<[point; 4]> = __SlateAlign16({
// CHECK-NEXT:         let mut [[T_5]]: [point; 4] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let [[T_6]] = point { x: seed, y: -seed };
// CHECK-NEXT:         let mut [[T_7]]: usize = 1;
// CHECK-NEXT:         while [[T_7]] <= 3 {
// CHECK-NEXT:             [[T_5]][[[T_7]]] = [[T_6]];
// CHECK-NEXT:             [[T_7]] += 1;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_5]]
// CHECK-NEXT:     });
// CHECK-NEXT:     let empty: i32 = {
// CHECK-NEXT:         let mut [[T_8]]: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_8]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let empty_f: f32 = {
// CHECK-NEXT:         let mut [[T_9]]: f32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_9]]
// CHECK-NEXT:     };
// CHECK-NEXT:     for i in 0 as i32..8 as i32 {
// CHECK-NEXT:         println!("flat[{}]={}", i, unsafe { flat.0[i as usize] });
// CHECK-NEXT:     }
// CHECK-NEXT:     for i in 0 as i32..3 as i32 {
// CHECK-NEXT:         for j in 0 as i32..4 as i32 {
// CHECK-NEXT:             println!(
// CHECK-NEXT:                 "grid[{}][{}]={}", i, j, unsafe { grid.0[i as usize] [j as usize] }
// CHECK-NEXT:             );
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     for i in 0 as i32..3 as i32 {
// CHECK-NEXT:         print!("rows[{}].tag={}", i, unsafe { rows.0[i as usize].tag });
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         for j in 0 as i32..3 as i32 {
// CHECK-NEXT:             print!(
// CHECK-NEXT:                 " ({},{})", unsafe { rows.0[i as usize].pts[j as usize].x }, unsafe {
// CHECK-NEXT:                 rows.0[i as usize].pts[j as usize].y }
// CHECK-NEXT:             );
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         }
// CHECK-NEXT:         println!("");
// CHECK-NEXT:     }
// CHECK-NEXT:     for i in 0 as i32..6 as i32 {
// CHECK-NEXT:         println!("local[{}]={}", i, local.0[i as usize]);
// CHECK-NEXT:     }
// CHECK-NEXT:     for i in 0 as i32..4 as i32 {
// CHECK-NEXT:         println!("many[{}]=({},{})", i, many.0[i as usize].x, many.0[i as usize].y);
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %g %d %d %g %d\n".as_ptr(),
// CHECK-NEXT:             unsafe { scalar },
// CHECK-NEXT:             unsafe { real },
// CHECK-NEXT:             ((unsafe { pointer }) == std::ptr::null_mut::<()>()) as i32,
// CHECK-NEXT:             empty,
// CHECK-NEXT:             empty_f as f64,
// CHECK-NEXT:             (unsafe {
// CHECK-NEXT:                 *{
// CHECK-NEXT:                     [[SLATE_COMPOUND_0]] = {
// CHECK-NEXT:                         let mut [[T_10:__t[0-9]+]]: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                         [[T_10]]
// CHECK-NEXT:                     };
// CHECK-NEXT:                     std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:                 }
// CHECK-NEXT:             })
// CHECK-NEXT:                 + ((unsafe {
// CHECK-NEXT:                     *{
// CHECK-NEXT:                         [[SLATE_COMPOUND_1]] = {
// CHECK-NEXT:                             let mut [[T_11:__t[0-9]+]]: f32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                             [[T_11]]
// CHECK-NEXT:                         };
// CHECK-NEXT:                         std::ptr::addr_of_mut!([[SLATE_COMPOUND_1]])
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }) as i32),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
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
