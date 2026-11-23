static int check(int n, int m, int a[n][6][m], int (*p)[6][m]) {
  if (sizeof(*p) != 6 * m * sizeof(int) ||
      __alignof__(*p) != _Alignof(int))
    return 1;
  if (a[0][0][0] != 11 || a[0][1][0] != 23 || a[1][0][0] != 35)
    return 2;
  ++p;
  if (p - a != 1 || (*p)[0][m - 1] != 47)
    return 3;
  return 0;
}

static int mixed(int n, int m) {
  int width = m;
  int a[n][6][m++];
  int fixed[2][6][width];
  int (*p)[6][width] = a;

  if (m != 4)
    return 4;
  n = 1;
  m = 1;
  if (sizeof(a) != 2 * 6 * 3 * sizeof(int) ||
      sizeof(fixed) != sizeof(a) || sizeof(*p) != sizeof(a) / 2)
    return 5;

  a[0][0][0] = 11;
  a[0][1][0] = 23;
  a[1][0][0] = 35;
  a[1][0][width - 1] = 47;
  fixed[1][5][width - 1] = 59;
  if (fixed[1][5][width - 1] != 59)
    return 6;
  return check(2, width, a, p);
}

int main(void) { return mixed(2, 3); }

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: fn check(n: i32, m: i32, a: *mut {{\[\[}}i32; 0]; 6], mut p: *mut {{\[\[}}i32; 0]; 6]) -> i32 {
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: u64 = m as u64;
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: u64 = m as u64;
// CHECK-NEXT:     if 6u64.wrapping_mul([[V_1]].wrapping_mul(4))
// CHECK-NEXT:         != ((6 * m) as u64).wrapping_mul(core::mem::size_of::<i32>() as u64)
// CHECK-NEXT:         || 4 != (core::mem::align_of::<i32>() as u64) {
// CHECK-NEXT:         return 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         *((a
// CHECK-NEXT:                     .byte_offset(
// CHECK-NEXT:                         0
// CHECK-NEXT:                             * ((6usize * ([[V_0]] as usize) * core::mem::size_of::<i32>())
// CHECK-NEXT:                                 as isize),
// CHECK-NEXT:                     ) as *mut [i32; 0])
// CHECK-NEXT:                 .byte_offset(
// CHECK-NEXT:                     0 * ((([[V_0]] as usize) * core::mem::size_of::<i32>()) as isize),
// CHECK-NEXT:                 ) as *mut i32)
// CHECK-NEXT:             .add(0)
// CHECK-NEXT:     }) != 11
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             *((a
// CHECK-NEXT:                         .byte_offset(
// CHECK-NEXT:                             0
// CHECK-NEXT:                                 * ((6usize * ([[V_0]] as usize) * core::mem::size_of::<i32>())
// CHECK-NEXT:                                     as isize),
// CHECK-NEXT:                         ) as *mut [i32; 0])
// CHECK-NEXT:                     .byte_offset(
// CHECK-NEXT:                         1 * ((([[V_0]] as usize) * core::mem::size_of::<i32>()) as isize),
// CHECK-NEXT:                     ) as *mut i32)
// CHECK-NEXT:                 .add(0)
// CHECK-NEXT:         }) != 23
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             *((a
// CHECK-NEXT:                         .byte_offset(
// CHECK-NEXT:                             1
// CHECK-NEXT:                                 * ((6usize * ([[V_0]] as usize) * core::mem::size_of::<i32>())
// CHECK-NEXT:                                     as isize),
// CHECK-NEXT:                         ) as *mut [i32; 0])
// CHECK-NEXT:                     .byte_offset(
// CHECK-NEXT:                         0 * ((([[V_0]] as usize) * core::mem::size_of::<i32>()) as isize),
// CHECK-NEXT:                     ) as *mut i32)
// CHECK-NEXT:                 .add(0)
// CHECK-NEXT:         }) != 35 {
// CHECK-NEXT:         return 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     p = unsafe {
// CHECK-NEXT:         p
// CHECK-NEXT:             .byte_offset(
// CHECK-NEXT:                 1 * ((6usize * ([[V_1]] as usize) * core::mem::size_of::<i32>()) as isize),
// CHECK-NEXT:             )
// CHECK-NEXT:     };
// CHECK-NEXT:     if (((unsafe { (p as *const u8).offset_from(a as *const u8) })
// CHECK-NEXT:         / ((6usize * ([[V_1]] as usize) * core::mem::size_of::<i32>()) as isize)) as i64)
// CHECK-NEXT:         != 1
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             *((p as *mut [i32; 0])
// CHECK-NEXT:                     .byte_offset(
// CHECK-NEXT:                         0 * ((([[V_1]] as usize) * core::mem::size_of::<i32>()) as isize),
// CHECK-NEXT:                     ) as *mut i32)
// CHECK-NEXT:                 .offset((m - 1) as isize)
// CHECK-NEXT:         }) != 47 {
// CHECK-NEXT:         return 3;
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn mixed(mut n: i32, mut m: i32) -> i32 {
// CHECK-NEXT:     let width: i32 = m;
// CHECK-NEXT:     let [[V_0]]: u64 = n as u64;
// CHECK-NEXT:     let [[V_1]]: i32 = m;
// CHECK-NEXT:     m = [[V_1]] + 1;
// CHECK-NEXT:     let [[V_2:__v[0-9]+]]: u64 = [[V_1]] as u64;
// CHECK-NEXT:     let mut a: Vec<i32> = {
// CHECK-NEXT:         let mut __slate_vla: Vec<i32> = Vec::with_capacity(
// CHECK-NEXT:             ([[V_0]] as usize) * 6usize * ([[V_2]] as usize),
// CHECK-NEXT:         );
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             __slate_vla
// CHECK-NEXT:                 .as_mut_ptr()
// CHECK-NEXT:                 .write_bytes(0u8, ([[V_0]] as usize) * 6usize * ([[V_2]] as usize))
// CHECK-NEXT:         };
// CHECK-NEXT:         __slate_vla
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_3:__v[0-9]+]]: u64 = width as u64;
// CHECK-NEXT:     let mut fixed: Vec<i32> = {
// CHECK-NEXT:         let mut __slate_vla: Vec<i32> = Vec::with_capacity(
// CHECK-NEXT:             2usize * 6usize * ([[V_3]] as usize),
// CHECK-NEXT:         );
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             __slate_vla.as_mut_ptr().write_bytes(0u8, 2usize * 6usize * ([[V_3]] as usize))
// CHECK-NEXT:         };
// CHECK-NEXT:         __slate_vla
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_4:__v[0-9]+]]: u64 = width as u64;
// CHECK-NEXT:     let p: *mut {{\[\[}}i32; 0]; 6] = (a.as_mut_ptr() as *mut {{\[\[}}[i32; 0]; 6]; 0])
// CHECK-NEXT:         as *mut {{\[\[}}i32; 0]; 6];
// CHECK-NEXT:     if m != 4 {
// CHECK-NEXT:         return 4;
// CHECK-NEXT:     }
// CHECK-NEXT:     n = 1;
// CHECK-NEXT:     m = 1;
// CHECK-NEXT:     if [[V_0]].wrapping_mul(6u64.wrapping_mul([[V_2]].wrapping_mul(4)))
// CHECK-NEXT:         != ((2i32 * 6i32 * 3) as u64).wrapping_mul(core::mem::size_of::<i32>() as u64)
// CHECK-NEXT:         || 2u64.wrapping_mul(6u64.wrapping_mul([[V_3]].wrapping_mul(4)))
// CHECK-NEXT:             != [[V_0]].wrapping_mul(6u64.wrapping_mul([[V_2]].wrapping_mul(4)))
// CHECK-NEXT:         || 6u64.wrapping_mul([[V_4]].wrapping_mul(4))
// CHECK-NEXT:             != [[V_0]].wrapping_mul(6u64.wrapping_mul([[V_2]].wrapping_mul(4))) / 2 {
// CHECK-NEXT:         return 5;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *((((a.as_mut_ptr() as *mut {{\[\[}}[i32; 0]; 6]; 0]) as *mut {{\[\[}}i32; 0]; 6])
// CHECK-NEXT:                     .byte_offset(
// CHECK-NEXT:                         0
// CHECK-NEXT:                             * ((6usize * ([[V_2]] as usize) * core::mem::size_of::<i32>())
// CHECK-NEXT:                                 as isize),
// CHECK-NEXT:                     ) as *mut [i32; 0])
// CHECK-NEXT:                 .byte_offset(
// CHECK-NEXT:                     0 * ((([[V_2]] as usize) * core::mem::size_of::<i32>()) as isize),
// CHECK-NEXT:                 ) as *mut i32)
// CHECK-NEXT:             .add(0) = 11;
// CHECK-NEXT:         *((((a.as_mut_ptr() as *mut {{\[\[}}[i32; 0]; 6]; 0]) as *mut {{\[\[}}i32; 0]; 6])
// CHECK-NEXT:                     .byte_offset(
// CHECK-NEXT:                         0
// CHECK-NEXT:                             * ((6usize * ([[V_2]] as usize) * core::mem::size_of::<i32>())
// CHECK-NEXT:                                 as isize),
// CHECK-NEXT:                     ) as *mut [i32; 0])
// CHECK-NEXT:                 .byte_offset(
// CHECK-NEXT:                     1 * ((([[V_2]] as usize) * core::mem::size_of::<i32>()) as isize),
// CHECK-NEXT:                 ) as *mut i32)
// CHECK-NEXT:             .add(0) = 23;
// CHECK-NEXT:         *((((a.as_mut_ptr() as *mut {{\[\[}}[i32; 0]; 6]; 0]) as *mut {{\[\[}}i32; 0]; 6])
// CHECK-NEXT:                     .byte_offset(
// CHECK-NEXT:                         1
// CHECK-NEXT:                             * ((6usize * ([[V_2]] as usize) * core::mem::size_of::<i32>())
// CHECK-NEXT:                                 as isize),
// CHECK-NEXT:                     ) as *mut [i32; 0])
// CHECK-NEXT:                 .byte_offset(
// CHECK-NEXT:                     0 * ((([[V_2]] as usize) * core::mem::size_of::<i32>()) as isize),
// CHECK-NEXT:                 ) as *mut i32)
// CHECK-NEXT:             .add(0) = 35;
// CHECK-NEXT:         *((((a.as_mut_ptr() as *mut {{\[\[}}[i32; 0]; 6]; 0]) as *mut {{\[\[}}i32; 0]; 6])
// CHECK-NEXT:                     .byte_offset(
// CHECK-NEXT:                         1
// CHECK-NEXT:                             * ((6usize * ([[V_2]] as usize) * core::mem::size_of::<i32>())
// CHECK-NEXT:                                 as isize),
// CHECK-NEXT:                     ) as *mut [i32; 0])
// CHECK-NEXT:                 .byte_offset(
// CHECK-NEXT:                     0 * ((([[V_2]] as usize) * core::mem::size_of::<i32>()) as isize),
// CHECK-NEXT:                 ) as *mut i32)
// CHECK-NEXT:             .offset((width - 1) as isize) = 47;
// CHECK-NEXT:         *((((fixed.as_mut_ptr() as *mut {{\[\[}}[i32; 0]; 6]; 2]) as *mut {{\[\[}}i32; 0]; 6])
// CHECK-NEXT:                     .byte_offset(
// CHECK-NEXT:                         1
// CHECK-NEXT:                             * ((6usize * ([[V_3]] as usize) * core::mem::size_of::<i32>())
// CHECK-NEXT:                                 as isize),
// CHECK-NEXT:                     ) as *mut [i32; 0])
// CHECK-NEXT:                 .byte_offset(
// CHECK-NEXT:                     5 * ((([[V_3]] as usize) * core::mem::size_of::<i32>()) as isize),
// CHECK-NEXT:                 ) as *mut i32)
// CHECK-NEXT:             .offset((width - 1) as isize) = 59;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         *((((fixed.as_mut_ptr() as *mut {{\[\[}}[i32; 0]; 6]; 2]) as *mut {{\[\[}}i32; 0]; 6])
// CHECK-NEXT:                     .byte_offset(
// CHECK-NEXT:                         1
// CHECK-NEXT:                             * ((6usize * ([[V_3]] as usize) * core::mem::size_of::<i32>())
// CHECK-NEXT:                                 as isize),
// CHECK-NEXT:                     ) as *mut [i32; 0])
// CHECK-NEXT:                 .byte_offset(
// CHECK-NEXT:                     5 * ((([[V_3]] as usize) * core::mem::size_of::<i32>()) as isize),
// CHECK-NEXT:                 ) as *mut i32)
// CHECK-NEXT:             .offset((width - 1) as isize)
// CHECK-NEXT:     }) != 59 {
// CHECK-NEXT:         return 6;
// CHECK-NEXT:     }
// CHECK-NEXT:     check(
// CHECK-NEXT:         2i32,
// CHECK-NEXT:         width,
// CHECK-NEXT:         (a.as_mut_ptr() as *mut {{\[\[}}[i32; 0]; 6]; 0]) as *mut {{\[\[}}i32; 0]; 6],
// CHECK-NEXT:         p as *mut {{\[\[}}i32; 0]; 6],
// CHECK-NEXT:     )
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     mixed(2i32, 3i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
