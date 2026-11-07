/* { dg-options "-std=gnu99" } */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned checksum(const void *p, size_t n) {
  const unsigned char *bytes = p;
  unsigned sum = 0;
  for (size_t i = 0; i < n; i++)
    sum = sum * 31 + bytes[i];
  return sum;
}

static int trailing(int n) {
  struct S { int head; char tail[n]; } x;
  x.head = n;
  for (int i = 0; i < n; i++)
    x.tail[i] = (char)('a' + i);
  printf("trailing %d: size %zu/%zu head %d tail %.*s sum %u\n", n, sizeof x,
         sizeof(struct S), x.head, n, x.tail, checksum(x.tail, (size_t)n));
  return x.tail[n - 1];
}

static void grid(int rows, int cols) {
  struct { short tag; int cells[rows][cols]; } g;
  g.tag = (short)(rows * 10 + cols);
  for (int r = 0; r < rows; r++)
    for (int c = 0; c < cols; c++)
      g.cells[r][c] = r * 100 + c;
  int total = 0;
  for (int r = 0; r < rows; r++)
    for (int c = 0; c < cols; c++)
      total += g.cells[r][c];
  printf("grid %dx%d: size %zu tag %d total %d last %d\n", rows, cols, sizeof g,
         g.tag, total, g.cells[rows - 1][cols - 1]);
}

static void only_member(int n) {
  union U { int ar[1][n]; } u;
  u.ar[0][0] = 7;
  u.ar[0][n - 1] = 9;
  printf("union %d: size %zu first %d last %d\n", n, sizeof u, u.ar[0][0],
         u.ar[0][n - 1]);
}

static void per_iteration(void) {
  for (int n = 1; n <= 4; n++) {
    struct { char flag; double values[n]; } v;
    v.flag = 'x';
    for (int i = 0; i < n; i++)
      v.values[i] = i * 1.5;
    printf("iteration %d: size %zu flag %c last %.1f\n", n, sizeof v, v.flag,
           v.values[n - 1]);
  }
}

static void heap(int n) {
  struct H { long id; int data[n]; };
  struct H *h = malloc(sizeof(struct H));
  h->id = 42;
  for (int i = 0; i < n; i++)
    h->data[i] = i * i;
  int *last = &h->data[n - 1];
  printf("heap %d: size %zu id %ld last %d bytes %zu\n", n, sizeof *h, h->id,
         *last, (size_t)((char *)last - (char *)h));
  free(h);
}

static void captured_once(int n) {
  struct C { char c[n]; };
  n += 10;
  struct C a;
  memset(a.c, 'z', sizeof a);
  printf("captured %d: size %zu first %c\n", n, sizeof a, a.c[0]);
}

int main(void) {
  int sum = trailing(3) + trailing(10);
  printf("sum %d\n", sum);
  grid(2, 3);
  grid(3, 1);
  only_member(1);
  only_member(5);
  per_iteration();
  heap(4);
  captured_once(6);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-std=gnu99" }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     head: i32,
// CHECK-NEXT:     tail: [i8; 0],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __SlateRecord_vla_record_members_c_25 {
// CHECK-NEXT:     tag: i16,
// CHECK-NEXT:     cells: {{\[\[}}i32; 0]; 0],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union U {
// CHECK-NEXT:     ar: {{\[\[}}i32; 0]; 1],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __SlateRecord_vla_record_members_c_48 {
// CHECK-NEXT:     flag: i8,
// CHECK-NEXT:     values: [f64; 0],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct H {
// CHECK-NEXT:     id: i64,
// CHECK-NEXT:     data: [i32; 0],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct C {
// CHECK-NEXT:     c: [i8; 0],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: use slate_support::free;
// CHECK-NEXT: use slate_support::malloc;
// CHECK-NEXT: use slate_support::memset;
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn checksum(p: *const (), n: u64) -> u32 {
// CHECK-NEXT:     let bytes: *const u8 = p as *const u8;
// CHECK-NEXT:     let mut sum: u32 = 0;
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         sum = sum
// CHECK-NEXT:             .wrapping_mul(31)
// CHECK-NEXT:             .wrapping_add(((unsafe { *bytes.add(i as usize) }) as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     sum
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn trailing(n: i32) -> i32 {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: u64 = n as u64;
// CHECK-NEXT:     let mut x: Vec<u32> = {
// CHECK-NEXT:         let mut __slate_vla: Vec<u32> = Vec::with_capacity(
// CHECK-NEXT:             (7usize + (((([[V_0]] as usize) * std::mem::size_of::<i8>()) as isize) as usize)) / 4usize,
// CHECK-NEXT:         );
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             __slate_vla.as_mut_ptr().write_bytes(
// CHECK-NEXT:                 0u8,
// CHECK-NEXT:                 (7usize + (((([[V_0]] as usize) * std::mem::size_of::<i8>()) as isize) as usize))
// CHECK-NEXT:                     / 4usize,
// CHECK-NEXT:             );
// CHECK-NEXT:         }
// CHECK-NEXT:         __slate_vla
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(x.as_mut_ptr() as *mut S)).head = n;
// CHECK-NEXT:     }
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *((&raw mut (*(x.as_mut_ptr() as *mut S)).tail as *mut [i8; 0]) as *mut i8)
// CHECK-NEXT:                 .offset(i as isize) = (97 + i) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"trailing %d: size %zu/%zu head %d tail %.*s sum %u\n".as_ptr(),
// CHECK-NEXT:             n,
// CHECK-NEXT:             (7u64.wrapping_add([[V_0]].wrapping_mul(1)) / 4).wrapping_mul(4),
// CHECK-NEXT:             (7u64.wrapping_add([[V_0]].wrapping_mul(1)) / 4).wrapping_mul(4),
// CHECK-NEXT:             (*(x.as_mut_ptr() as *mut S)).head,
// CHECK-NEXT:             n,
// CHECK-NEXT:             (&raw mut (*(x.as_mut_ptr() as *mut S)).tail as *mut [i8; 0]) as *mut i8,
// CHECK-NEXT:             checksum(
// CHECK-NEXT:                 (&raw mut (*(x.as_mut_ptr() as *mut S)).tail as *mut [i8; 0]) as *const (),
// CHECK-NEXT:                 n as u64,
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     (unsafe {
// CHECK-NEXT:         *((&raw mut (*(x.as_mut_ptr() as *mut S)).tail as *mut [i8; 0]) as *mut i8)
// CHECK-NEXT:             .offset((n - 1) as isize)
// CHECK-NEXT:     }) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn grid(rows: i32, cols: i32) {
// CHECK-NEXT:     let mut [[V_0]]: u64 = rows as u64;
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: u64 = cols as u64;
// CHECK-NEXT:     let mut g: Vec<u32> = {
// CHECK-NEXT:         let mut __slate_vla: Vec<u32> = Vec::with_capacity(
// CHECK-NEXT:             (7usize
// CHECK-NEXT:                 + (((([[V_0]] as usize) * ([[V_1]] as usize) * std::mem::size_of::<i32>()) as isize)
// CHECK-NEXT:                     as usize))
// CHECK-NEXT:                 / 4usize,
// CHECK-NEXT:         );
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             __slate_vla.as_mut_ptr().write_bytes(
// CHECK-NEXT:                 0u8,
// CHECK-NEXT:                 (7usize
// CHECK-NEXT:                     + (((([[V_0]] as usize) * ([[V_1]] as usize) * std::mem::size_of::<i32>()) as isize)
// CHECK-NEXT:                         as usize))
// CHECK-NEXT:                     / 4usize,
// CHECK-NEXT:             );
// CHECK-NEXT:         }
// CHECK-NEXT:         __slate_vla
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(g.as_mut_ptr() as *mut __SlateRecord_vla_record_members_c_25)).tag =
// CHECK-NEXT:             (rows * 10 + cols) as i16;
// CHECK-NEXT:     }
// CHECK-NEXT:     for r in 0..rows {
// CHECK-NEXT:         for c in 0..cols {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *(((&raw mut (*(g.as_mut_ptr() as *mut __SlateRecord_vla_record_members_c_25)).cells as *mut {{\[\[}}i32; 0]; 0]) as *mut [i32; 0]).byte_offset((r as isize) * ((([[V_1]] as usize) * std::mem::size_of::<i32>()) as isize)) as *mut i32).offset(c as isize) = r * 100 + c;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     for r in 0..rows {
// CHECK-NEXT:         for c in 0..cols {
// CHECK-NEXT:             let [[V_2:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:             total = [[V_2]]
// CHECK-NEXT:                 + unsafe {
// CHECK-NEXT:                     *(((&raw mut (*(g.as_mut_ptr() as *mut __SlateRecord_vla_record_members_c_25))
// CHECK-NEXT:                         .cells as *mut {{\[\[}}i32; 0]; 0]) as *mut [i32; 0])
// CHECK-NEXT:                         .byte_offset(
// CHECK-NEXT:                             (r as isize)
// CHECK-NEXT:                                 * ((([[V_1]] as usize) * std::mem::size_of::<i32>()) as isize),
// CHECK-NEXT:                         ) as *mut i32)
// CHECK-NEXT:                         .offset(c as isize)
// CHECK-NEXT:                 };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"grid %dx%d: size %zu tag %d total %d last %d\n".as_ptr(),
// CHECK-NEXT:             rows,
// CHECK-NEXT:             cols,
// CHECK-NEXT:             (7u64.wrapping_add([[V_0]].wrapping_mul([[V_1]].wrapping_mul(4))) / 4).wrapping_mul(4),
// CHECK-NEXT:             (*(g.as_mut_ptr() as *mut __SlateRecord_vla_record_members_c_25)).tag as i32,
// CHECK-NEXT:             total,
// CHECK-NEXT:             *(((&raw mut (*(g.as_mut_ptr() as *mut __SlateRecord_vla_record_members_c_25)).cells
// CHECK-NEXT:                 as *mut {{\[\[}}i32; 0]; 0]) as *mut [i32; 0])
// CHECK-NEXT:                 .byte_offset(
// CHECK-NEXT:                     ((rows - 1) as isize)
// CHECK-NEXT:                         * ((([[V_1]] as usize) * std::mem::size_of::<i32>()) as isize),
// CHECK-NEXT:                 ) as *mut i32)
// CHECK-NEXT:                 .offset((cols - 1) as isize),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn only_member(n: i32) {
// CHECK-NEXT:     let mut [[V_0]]: u64 = n as u64;
// CHECK-NEXT:     let mut u: Vec<u32> = {
// CHECK-NEXT:         let mut __slate_vla: Vec<u32> = Vec::with_capacity(
// CHECK-NEXT:             (3usize
// CHECK-NEXT:                 + (((1usize * ([[V_0]] as usize) * std::mem::size_of::<i32>()) as isize) as usize))
// CHECK-NEXT:                 / 4usize,
// CHECK-NEXT:         );
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             __slate_vla.as_mut_ptr().write_bytes(
// CHECK-NEXT:                 0u8,
// CHECK-NEXT:                 (3usize
// CHECK-NEXT:                     + (((1usize * ([[V_0]] as usize) * std::mem::size_of::<i32>()) as isize)
// CHECK-NEXT:                         as usize))
// CHECK-NEXT:                     / 4usize,
// CHECK-NEXT:             );
// CHECK-NEXT:         }
// CHECK-NEXT:         __slate_vla
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(((&raw mut (*(u.as_mut_ptr() as *mut U)).ar as *mut {{\[\[}}i32; 0]; 1]) as *mut [i32; 0])
// CHECK-NEXT:             .byte_offset(0 * ((([[V_0]] as usize) * std::mem::size_of::<i32>()) as isize))
// CHECK-NEXT:             as *mut i32)
// CHECK-NEXT:             .add(0) = 7;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(((&raw mut (*(u.as_mut_ptr() as *mut U)).ar as *mut {{\[\[}}i32; 0]; 1]) as *mut [i32; 0])
// CHECK-NEXT:             .byte_offset(0 * ((([[V_0]] as usize) * std::mem::size_of::<i32>()) as isize))
// CHECK-NEXT:             as *mut i32)
// CHECK-NEXT:             .offset((n - 1) as isize) = 9;
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"union %d: size %zu first %d last %d\n".as_ptr(),
// CHECK-NEXT:             n,
// CHECK-NEXT:             (3u64.wrapping_add(1u64.wrapping_mul([[V_0]].wrapping_mul(4))) / 4).wrapping_mul(4),
// CHECK-NEXT:             *(((&raw mut (*(u.as_mut_ptr() as *mut U)).ar as *mut {{\[\[}}i32; 0]; 1]) as *mut [i32; 0])
// CHECK-NEXT:                 .byte_offset(0 * ((([[V_0]] as usize) * std::mem::size_of::<i32>()) as isize))
// CHECK-NEXT:                 as *mut i32)
// CHECK-NEXT:                 .add(0),
// CHECK-NEXT:             *(((&raw mut (*(u.as_mut_ptr() as *mut U)).ar as *mut {{\[\[}}i32; 0]; 1]) as *mut [i32; 0])
// CHECK-NEXT:                 .byte_offset(0 * ((([[V_0]] as usize) * std::mem::size_of::<i32>()) as isize))
// CHECK-NEXT:                 as *mut i32)
// CHECK-NEXT:                 .offset((n - 1) as isize),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn per_iteration() {
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: for n in 1..5i32 {
// CHECK-NEXT:         let mut [[V_0]]: u64 = n as u64;
// CHECK-NEXT:         let mut v: Vec<u64> = {
// CHECK-NEXT:             let mut __slate_vla: Vec<u64> = Vec::with_capacity(
// CHECK-NEXT:                 (15usize + (((([[V_0]] as usize) * std::mem::size_of::<f64>()) as isize) as usize))
// CHECK-NEXT:                     / 8usize,
// CHECK-NEXT:             );
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 __slate_vla.as_mut_ptr().write_bytes(
// CHECK-NEXT:                     0u8,
// CHECK-NEXT:                     (15usize
// CHECK-NEXT:                         + (((([[V_0]] as usize) * std::mem::size_of::<f64>()) as isize) as usize))
// CHECK-NEXT:                         / 8usize,
// CHECK-NEXT:                 );
// CHECK-NEXT:             }
// CHECK-NEXT:             __slate_vla
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*(v.as_mut_ptr() as *mut __SlateRecord_vla_record_members_c_48)).flag = 120;
// CHECK-NEXT:         }
// CHECK-NEXT:         for i in 0..n {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *((&raw mut (*(v.as_mut_ptr() as *mut __SlateRecord_vla_record_members_c_48)).values as *mut [f64; 0]) as *mut f64).offset(i as isize) = (i as f64) * 1.5f64;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(c"iteration %d: size %zu flag %c last %.1f\n".as_ptr(), n, (15u64.wrapping_add([[V_0]].wrapping_mul(8)) / 8).wrapping_mul(8), (*(v.as_mut_ptr() as *mut __SlateRecord_vla_record_members_c_48)).flag as i32, *((&raw mut (*(v.as_mut_ptr() as *mut __SlateRecord_vla_record_members_c_48)).values as *mut [f64; 0]) as *mut f64).offset((n - 1) as isize))
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn heap(n: i32) {
// CHECK-NEXT:     let [[V_0]]: u64 = n as u64;
// CHECK-NEXT:     let h: *mut H = (unsafe {
// CHECK-NEXT:         malloc((15u64.wrapping_add([[V_0]].wrapping_mul(4)) / 8).wrapping_mul(8) as usize) as *mut ()
// CHECK-NEXT:     }) as *mut H;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*h).id = 42;
// CHECK-NEXT:     }
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *((&raw mut (*h).data as *mut [i32; 0]) as *mut i32).offset(i as isize) = i * i;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let last: *mut i32 =
// CHECK-NEXT:         unsafe { ((&raw mut (*h).data as *mut [i32; 0]) as *mut i32).offset((n - 1) as isize) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"heap %d: size %zu id %ld last %d bytes %zu\n".as_ptr(),
// CHECK-NEXT:             n,
// CHECK-NEXT:             (15u64.wrapping_add([[V_0]].wrapping_mul(4)) / 8).wrapping_mul(8),
// CHECK-NEXT:             (*h).id,
// CHECK-NEXT:             *last,
// CHECK-NEXT:             ((last as *mut i8).offset_from(h as *mut i8) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     unsafe { free((h as *mut ()) as *mut core::ffi::c_void) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn captured_once(mut n: i32) {
// CHECK-NEXT:     let mut [[V_0]]: u64 = n as u64;
// CHECK-NEXT:     n += 10;
// CHECK-NEXT:     let mut a: Vec<u8> = {
// CHECK-NEXT:         let mut __slate_vla: Vec<u8> = Vec::with_capacity(
// CHECK-NEXT:             (0usize + (((([[V_0]] as usize) * std::mem::size_of::<i8>()) as isize) as usize)) / 1usize,
// CHECK-NEXT:         );
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             __slate_vla.as_mut_ptr().write_bytes(
// CHECK-NEXT:                 0u8,
// CHECK-NEXT:                 (0usize + (((([[V_0]] as usize) * std::mem::size_of::<i8>()) as isize) as usize))
// CHECK-NEXT:                     / 1usize,
// CHECK-NEXT:             );
// CHECK-NEXT:         }
// CHECK-NEXT:         __slate_vla
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::write_bytes(
// CHECK-NEXT:             (((&raw mut (*(a.as_mut_ptr() as *mut C)).c as *mut [i8; 0]) as *mut ())
// CHECK-NEXT:                 as *mut core::ffi::c_void) as *mut u8,
// CHECK-NEXT:             (122i32 as i32) as u8,
// CHECK-NEXT:             (0u64.wrapping_add([[V_0]].wrapping_mul(1)) as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"captured %d: size %zu first %c\n".as_ptr(),
// CHECK-NEXT:             n,
// CHECK-NEXT:             0u64.wrapping_add([[V_0]].wrapping_mul(1)),
// CHECK-NEXT:             *((&raw mut (*(a.as_mut_ptr() as *mut C)).c as *mut [i8; 0]) as *mut i8).add(0) as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let sum: i32 = trailing(3i32) + trailing(10i32);
// CHECK-NEXT:     println!("sum {}", sum);
// CHECK-NEXT:     grid(2i32, 3i32);
// CHECK-NEXT:     grid(3i32, 1i32);
// CHECK-NEXT:     only_member(1i32);
// CHECK-NEXT:     only_member(5i32);
// CHECK-NEXT:     per_iteration();
// CHECK-NEXT:     heap(4i32);
// CHECK-NEXT:     captured_once(6i32);
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
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
