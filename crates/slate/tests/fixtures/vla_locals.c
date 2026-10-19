#include <stdio.h>
#include <string.h>

typedef struct {
  int  kind;
  long payload;
} Object;

static long wrap_objects(int argc) {
  Object  objects[argc];
  Object *argv[argc];
  for (int j = 0; j < argc; j++) {
    objects[j].kind    = j;
    objects[j].payload = (long)j * 100;
    argv[j]            = objects + j;
  }
  long total = 0;
  for (int j = 0; j < argc; j++) {
    total += argv[j]->kind + argv[j]->payload;
  }
  return total + (long)sizeof(objects) + (long)sizeof(argv);
}

static int format_port(int port) {
  int  length = snprintf(NULL, 0, "%d", port);
  char buf[length + 1];
  snprintf(buf, sizeof(buf), "%d", port);
  return (int)strlen(buf) * 1000 + buf[0];
}

static int const_bound(void) {
  const int bufflen = 16;
  char      buf[bufflen];
  memset(buf, 'x', sizeof(buf) - 1);
  buf[bufflen - 1] = '\0';
  return (int)strlen(buf);
}

static int matrix(int rows, int cols) {
  int grid[rows][cols];
  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++) {
      grid[r][c] = r * 10 + c;
    }
  }
  int (*row)[cols] = grid + 1;
  int sum          = 0;
  for (int c = 0; c < cols; c++) {
    sum += row[0][c] + row[1][c];
  }
  long distance = (grid + rows - 1) - grid;
  return sum * 100 + (int)distance * 10 + (int)(sizeof(grid) / sizeof(grid[0]));
}

static int per_iteration(int rounds) {
  int total = 0;
  for (int i = 1; i <= rounds; i++) {
    int scratch[i];
    for (int k = 0; k < i; k++) {
      scratch[k] = k + i;
    }
    total += scratch[i - 1];
  }
  return total;
}

static int sum_row(int n, int (*values)[n]) {
  int total = 0;
  for (int i = 0; i < n; i++) {
    total += (*values)[i];
  }
  return total;
}

int main(void) {
  int length = 5;
  int values[length];
  for (int i = 0; i < length; i++) {
    values[i] = i * i;
  }
  printf("%ld\n", wrap_objects(4));
  printf("%d\n", format_port(6379));
  printf("%d\n", const_bound());
  printf("%d\n", matrix(3, 4));
  printf("%d\n", per_iteration(6));
  printf("%d\n", sum_row(length, &values));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![allow(arithmetic_overflow, unconditional_panic)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn snprintf(__s: *mut i8, __maxlen: u64, __format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn memset(__s: *mut (), __c: i32, __n: u64) -> *mut ();
// CHECK-NEXT:     fn strlen(__s: *const i8) -> u64;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     kind: i32,
// CHECK-NEXT:     payload: i64,
// CHECK-NEXT: }
// CHECK-NEXT: fn wrap_objects(argc: i32) -> i64 {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: u64 = (argc as i64) as u64;
// CHECK-NEXT:     let mut objects: Vec<[[SLATERECORD_0]]> = {
// CHECK-NEXT:         let mut __slate_vla: Vec<[[SLATERECORD_0]]> = Vec::with_capacity([[V_0]] as usize);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             __slate_vla.as_mut_ptr().write_bytes(0u8, [[V_0]] as usize);
// CHECK-NEXT:         }
// CHECK-NEXT:         __slate_vla
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: u64 = (argc as i64) as u64;
// CHECK-NEXT:     let mut argv: Vec<*mut [[SLATERECORD_0]]> = {
// CHECK-NEXT:         let mut __slate_vla: Vec<*mut [[SLATERECORD_0]]> = Vec::with_capacity(
// CHECK-NEXT:             [[V_1]] as usize,
// CHECK-NEXT:         );
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             __slate_vla.as_mut_ptr().write_bytes(0u8, [[V_1]] as usize);
// CHECK-NEXT:         }
// CHECK-NEXT:         __slate_vla
// CHECK-NEXT:     };
// CHECK-NEXT:     for j in 0..argc {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*(objects.as_mut_ptr() as *mut [[SLATERECORD_0]]).offset(j as isize)).kind = j;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*(objects.as_mut_ptr() as *mut [[SLATERECORD_0]]).offset(j as isize))
// CHECK-NEXT:                 .payload = (j as i64) * 100;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *(argv.as_mut_ptr() as *mut *mut [[SLATERECORD_0]]).offset(j as isize) = (objects
// CHECK-NEXT:                 .as_mut_ptr() as *mut [[SLATERECORD_0]])
// CHECK-NEXT:                 .offset(j as isize);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut total: i64 = 0;
// CHECK-NEXT:     for j in 0..argc {
// CHECK-NEXT:         let [[V_2:__v[0-9]+]]: i64 = total;
// CHECK-NEXT:         total = [[V_2]]
// CHECK-NEXT:             + (((unsafe {
// CHECK-NEXT:                 (*(*(argv.as_mut_ptr() as *mut *mut [[SLATERECORD_0]]).offset(j as isize)))
// CHECK-NEXT:                     .kind
// CHECK-NEXT:             }) as i64)
// CHECK-NEXT:                 + unsafe {
// CHECK-NEXT:                     (*(*(argv.as_mut_ptr() as *mut *mut [[SLATERECORD_0]])
// CHECK-NEXT:                         .offset(j as isize)))
// CHECK-NEXT:                         .payload
// CHECK-NEXT:                 });
// CHECK-NEXT:     }
// CHECK-NEXT:     total + ([[V_0]].wrapping_mul(16) as i64) + ([[V_1]].wrapping_mul(8) as i64)
// CHECK-NEXT: }
// CHECK-NEXT: fn format_port(port: i32) -> i32 {
// CHECK-NEXT:     let length: i32 = unsafe {
// CHECK-NEXT:         snprintf(std::ptr::null_mut::<i8>(), 0u64, c"%d".as_ptr(), port)
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: u64 = ((length + 1) as i64) as u64;
// CHECK-NEXT:     let mut buf: Vec<i8> = {
// CHECK-NEXT:         let mut __slate_vla: Vec<i8> = Vec::with_capacity([[V_3]] as usize);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             __slate_vla.as_mut_ptr().write_bytes(0u8, [[V_3]] as usize);
// CHECK-NEXT:         }
// CHECK-NEXT:         __slate_vla
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         snprintf(
// CHECK-NEXT:             buf.as_mut_ptr() as *mut i8,
// CHECK-NEXT:             [[V_3]].wrapping_mul(1),
// CHECK-NEXT:             c"%d".as_ptr(),
// CHECK-NEXT:             port,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     (((unsafe { strlen(buf.as_mut_ptr() as *const i8) }) as u32) as i32) * 1000
// CHECK-NEXT:         + ((unsafe { *(buf.as_mut_ptr() as *mut i8).offset(0 as isize) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn const_bound() -> i32 {
// CHECK-NEXT:     let bufflen: i32 = 16;
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: u64 = (bufflen as i64) as u64;
// CHECK-NEXT:     let mut buf: Vec<i8> = {
// CHECK-NEXT:         let mut __slate_vla: Vec<i8> = Vec::with_capacity([[V_4]] as usize);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             __slate_vla.as_mut_ptr().write_bytes(0u8, [[V_4]] as usize);
// CHECK-NEXT:         }
// CHECK-NEXT:         __slate_vla
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::write_bytes(
// CHECK-NEXT:             buf.as_mut_ptr() as *mut u8,
// CHECK-NEXT:             120u8,
// CHECK-NEXT:             [[V_4]].wrapping_mul(1).wrapping_sub(1) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(buf.as_mut_ptr() as *mut i8).offset((bufflen - 1) as isize) = 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     ((unsafe { strlen(buf.as_mut_ptr() as *const i8) }) as u32) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn matrix(rows: i32, cols: i32) -> i32 {
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: u64 = (rows as i64) as u64;
// CHECK-NEXT:     let mut [[V_6:__v[0-9]+]]: u64 = (cols as i64) as u64;
// CHECK-NEXT:     let mut grid: Vec<i32> = {
// CHECK-NEXT:         let mut __slate_vla: Vec<i32> = Vec::with_capacity(
// CHECK-NEXT:             ([[V_5]] as usize) * ([[V_6]] as usize),
// CHECK-NEXT:         );
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             __slate_vla
// CHECK-NEXT:                 .as_mut_ptr()
// CHECK-NEXT:                 .write_bytes(0u8, ([[V_5]] as usize) * ([[V_6]] as usize));
// CHECK-NEXT:         }
// CHECK-NEXT:         __slate_vla
// CHECK-NEXT:     };
// CHECK-NEXT:     for r in 0..rows {
// CHECK-NEXT:         for c in 0..cols {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *((grid.as_mut_ptr() as *mut [i32; 0])
// CHECK-NEXT:                     .byte_offset(
// CHECK-NEXT:                         (r as isize)
// CHECK-NEXT:                             * ((([[V_6]] as usize) * std::mem::size_of::<i32>()) as isize),
// CHECK-NEXT:                     ) as *mut i32)
// CHECK-NEXT:                     .offset(c as isize) = r * 10 + c;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_7:__v[0-9]+]]: u64 = (cols as i64) as u64;
// CHECK-NEXT:     let row: *mut [i32; 0] = (unsafe {
// CHECK-NEXT:         (grid.as_mut_ptr() as *mut [i32; 0])
// CHECK-NEXT:             .byte_offset(1 * ((([[V_6]] as usize) * std::mem::size_of::<i32>()) as isize))
// CHECK-NEXT:     }) as *mut [i32; 0];
// CHECK-NEXT:     let mut sum: i32 = 0;
// CHECK-NEXT:     for c in 0..cols {
// CHECK-NEXT:         let [[V_8:__v[0-9]+]]: i32 = sum;
// CHECK-NEXT:         sum = [[V_8]]
// CHECK-NEXT:             + ((unsafe {
// CHECK-NEXT:                 *(row
// CHECK-NEXT:                     .byte_offset(
// CHECK-NEXT:                         0 * ((([[V_7]] as usize) * std::mem::size_of::<i32>()) as isize),
// CHECK-NEXT:                     ) as *mut i32)
// CHECK-NEXT:                     .offset(c as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:                 + unsafe {
// CHECK-NEXT:                     *(row
// CHECK-NEXT:                         .byte_offset(
// CHECK-NEXT:                             1
// CHECK-NEXT:                                 * ((([[V_7]] as usize) * std::mem::size_of::<i32>()) as isize),
// CHECK-NEXT:                         ) as *mut i32)
// CHECK-NEXT:                         .offset(c as isize)
// CHECK-NEXT:                 });
// CHECK-NEXT:     }
// CHECK-NEXT:     let distance: i64 = ((unsafe {
// CHECK-NEXT:         ((grid.as_mut_ptr() as *mut [i32; 0])
// CHECK-NEXT:             .byte_offset(
// CHECK-NEXT:                 (rows as isize)
// CHECK-NEXT:                     * ((([[V_6]] as usize) * std::mem::size_of::<i32>()) as isize),
// CHECK-NEXT:             )
// CHECK-NEXT:             .byte_offset(-1 * ((([[V_6]] as usize) * std::mem::size_of::<i32>()) as isize))
// CHECK-NEXT:             as *const u8)
// CHECK-NEXT:             .offset_from(grid.as_mut_ptr() as *const u8)
// CHECK-NEXT:     }) / ((([[V_6]] as usize) * std::mem::size_of::<i32>()) as isize)) as i64;
// CHECK-NEXT:     sum * 100 + (distance as i32) * 10
// CHECK-NEXT:         + ((([[V_5]].wrapping_mul([[V_6]].wrapping_mul(4)) / [[V_6]].wrapping_mul(4)) as u32)
// CHECK-NEXT:             as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn per_iteration(rounds: i32) -> i32 {
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     let mut i: i32 = 1;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i <= rounds {
// CHECK-NEXT:         let mut [[V_9:__v[0-9]+]]: u64 = (i as i64) as u64;
// CHECK-NEXT:         let mut scratch: Vec<i32> = {
// CHECK-NEXT:             let mut __slate_vla: Vec<i32> = Vec::with_capacity([[V_9]] as usize);
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 __slate_vla.as_mut_ptr().write_bytes(0u8, [[V_9]] as usize);
// CHECK-NEXT:             }
// CHECK-NEXT:             __slate_vla
// CHECK-NEXT:         };
// CHECK-NEXT:         for k in 0..i {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *(scratch.as_mut_ptr() as *mut i32).offset(k as isize) = k + i;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         let [[V_10:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:         total = [[V_10]]
// CHECK-NEXT:             + unsafe { *(scratch.as_mut_ptr() as *mut i32).offset((i - 1) as isize) };
// CHECK-NEXT:         i += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn sum_row(n: i32, values: *mut [i32; 0]) -> i32 {
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         total += unsafe { *(values as *mut i32).offset(i as isize) };
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let length: i32 = 5;
// CHECK-NEXT:     let mut [[V_11:__v[0-9]+]]: u64 = (length as i64) as u64;
// CHECK-NEXT:     let mut values: Vec<i32> = {
// CHECK-NEXT:         let mut __slate_vla: Vec<i32> = Vec::with_capacity([[V_11]] as usize);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             __slate_vla.as_mut_ptr().write_bytes(0u8, [[V_11]] as usize);
// CHECK-NEXT:         }
// CHECK-NEXT:         __slate_vla
// CHECK-NEXT:     };
// CHECK-NEXT:     for i in 0..length {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *(values.as_mut_ptr() as *mut i32).offset(i as isize) = i * i;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%ld\n".as_ptr(), wrap_objects(4i32)) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!("{}", format_port(6379i32));
// CHECK-NEXT:     println!("{}", const_bound());
// CHECK-NEXT:     println!("{}", matrix(3i32, 4i32));
// CHECK-NEXT:     println!("{}", per_iteration(6i32));
// CHECK-NEXT:     let __slate_printf_arg0 = sum_row(length, values.as_mut_ptr() as *mut [i32; 0]);
// CHECK-NEXT:     println!("{}", __slate_printf_arg0);
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
