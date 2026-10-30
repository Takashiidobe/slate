#include <stdio.h>

struct Sample {
  int     id;
  double  scale;
  int    *target;
  int     data[4];
};

struct Cell {
  char tag;
  long value;
};

static int checksum(struct Sample s) {
  int total = s.id + (int)(s.scale * 10.0);
  if (s.id != 0) {
    total += *s.target;
  }
  for (int i = 0; i < 4; i++) {
    total = total * 3 + s.data[i];
  }
  return total;
}

int main(void) {
  int partial[5] = {1, 2};
  int sum        = 0;
  for (int i = 0; i < 5; i++) {
    sum = sum * 7 + partial[i];
  }
  printf("%d\n", sum);

  int           seed = 9;
  struct Sample zero = {0};
  struct Sample some = {.id = 3, .target = &seed, .data = {[2] = 5}};
  struct Sample full = {4, 1.5, &seed, {1, 2, 3, 4}};
  printf("%d %d %d\n", checksum(zero), checksum(some), checksum(full));

  struct Cell cells[4] = {[1] = {.value = 7}, [3] = {'z'}};
  for (int i = 0; i < 4; i++) {
    printf("%d %ld\n", cells[i].tag, cells[i].value);
  }
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Sample {
// CHECK-NEXT:     id: i32,
// CHECK-NEXT:     scale: f64,
// CHECK-NEXT:     target: *mut i32,
// CHECK-NEXT:     data: [i32; 4],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Cell {
// CHECK-NEXT:     tag: i8,
// CHECK-NEXT:     value: i64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn checksum(s: Sample) -> i32 {
// CHECK-NEXT:     let mut total: i32 = s.id + ((s.scale * 10.0f64) as i32);
// CHECK-NEXT:     if s.id != 0 {
// CHECK-NEXT:         total += unsafe { *s.target };
// CHECK-NEXT:     }
// CHECK-NEXT:     for i in 0i32..4i32 {
// CHECK-NEXT:         total = total * 3 + s.data[i as usize];
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let partial: __SlateAlign16<[i32; 5]> = __SlateAlign16({
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: [i32; 5] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]][0] = 1i32;
// CHECK-NEXT:         [[T_0]][1] = 2i32;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     });
// CHECK-NEXT:     let mut sum: i32 = 0;
// CHECK-NEXT:     for i in partial.0.iter().copied() {
// CHECK-NEXT:         sum = sum * 7 + i;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("{}", sum);
// CHECK-NEXT:     let mut seed: i32 = 9;
// CHECK-NEXT:     let zero: Sample = {
// CHECK-NEXT:         let mut [[T_1:__t[0-9]+]]: Sample = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]].id = 0i32;
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let some: Sample = {
// CHECK-NEXT:         let mut [[T_2:__t[0-9]+]]: Sample = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_2]].id = 3i32;
// CHECK-NEXT:         [[T_2]].target = std::ptr::addr_of_mut!(seed);
// CHECK-NEXT:         [[T_2]].data = {
// CHECK-NEXT:             let mut [[T_3:__t[0-9]+]]: [i32; 4] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_3]][2] = 5i32;
// CHECK-NEXT:             [[T_3]]
// CHECK-NEXT:         };
// CHECK-NEXT:         [[T_2]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let full: Sample = {
// CHECK-NEXT:         let mut [[T_4:__t[0-9]+]]: Sample = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_4]].id = 4i32;
// CHECK-NEXT:         [[T_4]].scale = 1.5f64;
// CHECK-NEXT:         [[T_4]].target = std::ptr::addr_of_mut!(seed);
// CHECK-NEXT:         [[T_4]].data = [1i32, 2i32, 3i32, 4i32];
// CHECK-NEXT:         [[T_4]]
// CHECK-NEXT:     };
// CHECK-NEXT:     println!("{} {} {}", checksum(zero), checksum(some), checksum(full));
// CHECK-NEXT:     let cells: __SlateAlign16<[Cell; 4]> = __SlateAlign16({
// CHECK-NEXT:         let mut [[T_5:__t[0-9]+]]: [Cell; 4] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_5]][1] = {
// CHECK-NEXT:             let mut [[T_6:__t[0-9]+]]: Cell = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_6]].value = 7i64;
// CHECK-NEXT:             [[T_6]]
// CHECK-NEXT:         };
// CHECK-NEXT:         [[T_5]][3] = {
// CHECK-NEXT:             let mut [[T_7:__t[0-9]+]]: Cell = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_7]].tag = 122i8;
// CHECK-NEXT:             [[T_7]]
// CHECK-NEXT:         };
// CHECK-NEXT:         [[T_5]]
// CHECK-NEXT:     });
// CHECK-NEXT:     for i in 0i32..4i32 {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%d %ld\n".as_ptr(),
// CHECK-NEXT:                 cells.0[i as usize].tag as i32,
// CHECK-NEXT:                 cells.0[i as usize].value,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
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
