/* PR optimization/8634 */
/* Contributed by Glen Nakamura <glen at imodulo dot com> */

extern void abort(void);

struct foo {
  char a, b, c, d, e, f, g, h, i, j;
};

int test1() {
  const char X[8] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
  char       buffer[8];
  __builtin_memcpy(buffer, X, 8);
  if (buffer[0] != 'A' || buffer[1] != 'B' || buffer[2] != 'C' ||
      buffer[3] != 'D' || buffer[4] != 'E' || buffer[5] != 'F' ||
      buffer[6] != 'G' || buffer[7] != 'H')
    abort();
  return 0;
}

int test2() {
  const char X[10] = {'A', 'B', 'C', 'D', 'E'};
  char       buffer[10];
  __builtin_memcpy(buffer, X, 10);
  if (buffer[0] != 'A' || buffer[1] != 'B' || buffer[2] != 'C' ||
      buffer[3] != 'D' || buffer[4] != 'E' || buffer[5] != '\0' ||
      buffer[6] != '\0' || buffer[7] != '\0' || buffer[8] != '\0' ||
      buffer[9] != '\0')
    abort();
  return 0;
}

int test3() {
  const struct foo X = {a : 'A', c : 'C', e : 'E', g : 'G', i : 'I'};
  char             buffer[10];
  __builtin_memcpy(buffer, &X, 10);
  if (buffer[0] != 'A' || buffer[1] != '\0' || buffer[2] != 'C' ||
      buffer[3] != '\0' || buffer[4] != 'E' || buffer[5] != '\0' ||
      buffer[6] != 'G' || buffer[7] != '\0' || buffer[8] != 'I' ||
      buffer[9] != '\0')
    abort();
  return 0;
}

int test4() {
  const struct foo X = {.b = 'B', .d = 'D', .f = 'F', .h = 'H', .j = 'J'};
  char             buffer[10];
  __builtin_memcpy(buffer, &X, 10);
  if (buffer[0] != '\0' || buffer[1] != 'B' || buffer[2] != '\0' ||
      buffer[3] != 'D' || buffer[4] != '\0' || buffer[5] != 'F' ||
      buffer[6] != '\0' || buffer[7] != 'H' || buffer[8] != '\0' ||
      buffer[9] != 'J')
    abort();
  return 0;
}

int main() {
  test1();
  test2();
  test3();
  test4();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR optimization/8634
// CHECK-NEXT: //!
// CHECK-NEXT: //! Contributed by Glen Nakamura <glen at imodulo dot com>
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     #[link_name = "memcpy"]
// CHECK-NEXT:     fn __builtin_memcpy([[V_0:__v[0-9]+]]: *mut (), [[V_1:__v[0-9]+]]: *const (), [[V_2:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct foo {
// CHECK-NEXT:     a: i8,
// CHECK-NEXT:     b: i8,
// CHECK-NEXT:     c: i8,
// CHECK-NEXT:     d: i8,
// CHECK-NEXT:     e: i8,
// CHECK-NEXT:     f: i8,
// CHECK-NEXT:     g: i8,
// CHECK-NEXT:     h: i8,
// CHECK-NEXT:     i: i8,
// CHECK-NEXT:     j: i8,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test1() -> i32 {
// CHECK-NEXT:     let mut X: [i8; 8] = [
// CHECK-NEXT:         (65 as i32) as i8,
// CHECK-NEXT:         (66 as i32) as i8,
// CHECK-NEXT:         (67 as i32) as i8,
// CHECK-NEXT:         (68 as i32) as i8,
// CHECK-NEXT:         (69 as i32) as i8,
// CHECK-NEXT:         (70 as i32) as i8,
// CHECK-NEXT:         (71 as i32) as i8,
// CHECK-NEXT:         (72 as i32) as i8,
// CHECK-NEXT:     ];
// CHECK-NEXT:     let mut buffer: [i8; 8] = [0 as i8; 8];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memcpy(
// CHECK-NEXT:             buffer.as_mut_ptr() as *mut (),
// CHECK-NEXT:             X.as_ptr() as *const (),
// CHECK-NEXT:             ((8 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     if ((unsafe { *unsafe { (buffer.as_mut_ptr() as *mut i8).offset((0 as i32) as isize) } })
// CHECK-NEXT:         as i32)
// CHECK-NEXT:         != (65 as i32)
// CHECK-NEXT:         || ((unsafe { *unsafe { (buffer.as_mut_ptr() as *mut i8).offset((1 as i32) as isize) } })
// CHECK-NEXT:             as i32)
// CHECK-NEXT:             != (66 as i32)
// CHECK-NEXT:         || ((unsafe { *unsafe { (buffer.as_mut_ptr() as *mut i8).offset((2 as i32) as isize) } })
// CHECK-NEXT:             as i32)
// CHECK-NEXT:             != (67 as i32)
// CHECK-NEXT:         || ((unsafe { *unsafe { (buffer.as_mut_ptr() as *mut i8).offset((3 as i32) as isize) } })
// CHECK-NEXT:             as i32)
// CHECK-NEXT:             != (68 as i32)
// CHECK-NEXT:         || ((unsafe { *unsafe { (buffer.as_mut_ptr() as *mut i8).offset((4 as i32) as isize) } })
// CHECK-NEXT:             as i32)
// CHECK-NEXT:             != (69 as i32)
// CHECK-NEXT:         || ((unsafe { *unsafe { (buffer.as_mut_ptr() as *mut i8).offset((5 as i32) as isize) } })
// CHECK-NEXT:             as i32)
// CHECK-NEXT:             != (70 as i32)
// CHECK-NEXT:         || ((unsafe { *unsafe { (buffer.as_mut_ptr() as *mut i8).offset((6 as i32) as isize) } })
// CHECK-NEXT:             as i32)
// CHECK-NEXT:             != (71 as i32)
// CHECK-NEXT:         || ((unsafe { *unsafe { (buffer.as_mut_ptr() as *mut i8).offset((7 as i32) as isize) } })
// CHECK-NEXT:             as i32)
// CHECK-NEXT:             != (72 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test2() -> i32 {
// CHECK-NEXT:     let mut X: [i8; 10] = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: [i8; 10] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]][0] = (65 as i32) as i8;
// CHECK-NEXT:         [[T_0]][1] = (66 as i32) as i8;
// CHECK-NEXT:         [[T_0]][2] = (67 as i32) as i8;
// CHECK-NEXT:         [[T_0]][3] = (68 as i32) as i8;
// CHECK-NEXT:         [[T_0]][4] = (69 as i32) as i8;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut buffer: [i8; 10] = [0 as i8; 10];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memcpy(
// CHECK-NEXT:             buffer.as_mut_ptr() as *mut (),
// CHECK-NEXT:             X.as_ptr() as *const (),
// CHECK-NEXT:             ((10 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     if ((unsafe { *unsafe { (buffer.as_mut_ptr() as *mut i8).offset((0 as i32) as isize) } })
// CHECK-NEXT:         as i32)
// CHECK-NEXT:         != (65 as i32)
// CHECK-NEXT:         || ((unsafe { *unsafe { (buffer.as_mut_ptr() as *mut i8).offset((1 as i32) as isize) } })
// CHECK-NEXT:             as i32)
// CHECK-NEXT:             != (66 as i32)
// CHECK-NEXT:         || ((unsafe { *unsafe { (buffer.as_mut_ptr() as *mut i8).offset((2 as i32) as isize) } })
// CHECK-NEXT:             as i32)
// CHECK-NEXT:             != (67 as i32)
// CHECK-NEXT:         || ((unsafe { *unsafe { (buffer.as_mut_ptr() as *mut i8).offset((3 as i32) as isize) } })
// CHECK-NEXT:             as i32)
// CHECK-NEXT:             != (68 as i32)
// CHECK-NEXT:         || ((unsafe { *unsafe { (buffer.as_mut_ptr() as *mut i8).offset((4 as i32) as isize) } })
// CHECK-NEXT:             as i32)
// CHECK-NEXT:             != (69 as i32)
// CHECK-NEXT:         || ((unsafe { *unsafe { (buffer.as_mut_ptr() as *mut i8).offset((5 as i32) as isize) } })
// CHECK-NEXT:             as i32)
// CHECK-NEXT:             != (0 as i32)
// CHECK-NEXT:         || ((unsafe { *unsafe { (buffer.as_mut_ptr() as *mut i8).offset((6 as i32) as isize) } })
// CHECK-NEXT:             as i32)
// CHECK-NEXT:             != (0 as i32)
// CHECK-NEXT:         || ((unsafe { *unsafe { (buffer.as_mut_ptr() as *mut i8).offset((7 as i32) as isize) } })
// CHECK-NEXT:             as i32)
// CHECK-NEXT:             != (0 as i32)
// CHECK-NEXT:         || ((unsafe { *unsafe { (buffer.as_mut_ptr() as *mut i8).offset((8 as i32) as isize) } })
// CHECK-NEXT:             as i32)
// CHECK-NEXT:             != (0 as i32)
// CHECK-NEXT:         || ((unsafe { *unsafe { (buffer.as_mut_ptr() as *mut i8).offset((9 as i32) as isize) } })
// CHECK-NEXT:             as i32)
// CHECK-NEXT:             != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test3() -> i32 {
// CHECK-NEXT:     let mut X: foo = {
// CHECK-NEXT:         let mut [[T_0]]: foo = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].a = (65 as i32) as i8;
// CHECK-NEXT:         [[T_0]].c = (67 as i32) as i8;
// CHECK-NEXT:         [[T_0]].e = (69 as i32) as i8;
// CHECK-NEXT:         [[T_0]].g = (71 as i32) as i8;
// CHECK-NEXT:         [[T_0]].i = (73 as i32) as i8;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut buffer: [i8; 10] = [0 as i8; 10];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memcpy(
// CHECK-NEXT:             buffer.as_mut_ptr() as *mut (),
// CHECK-NEXT:             std::ptr::addr_of!(X) as *const (),
// CHECK-NEXT:             ((10 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     if ((unsafe { *unsafe { (buffer.as_mut_ptr() as *mut i8).offset((0 as i32) as isize) } })
// CHECK-NEXT:         as i32)
// CHECK-NEXT:         != (65 as i32)
// CHECK-NEXT:         || ((unsafe { *unsafe { (buffer.as_mut_ptr() as *mut i8).offset((1 as i32) as isize) } })
// CHECK-NEXT:             as i32)
// CHECK-NEXT:             != (0 as i32)
// CHECK-NEXT:         || ((unsafe { *unsafe { (buffer.as_mut_ptr() as *mut i8).offset((2 as i32) as isize) } })
// CHECK-NEXT:             as i32)
// CHECK-NEXT:             != (67 as i32)
// CHECK-NEXT:         || ((unsafe { *unsafe { (buffer.as_mut_ptr() as *mut i8).offset((3 as i32) as isize) } })
// CHECK-NEXT:             as i32)
// CHECK-NEXT:             != (0 as i32)
// CHECK-NEXT:         || ((unsafe { *unsafe { (buffer.as_mut_ptr() as *mut i8).offset((4 as i32) as isize) } })
// CHECK-NEXT:             as i32)
// CHECK-NEXT:             != (69 as i32)
// CHECK-NEXT:         || ((unsafe { *unsafe { (buffer.as_mut_ptr() as *mut i8).offset((5 as i32) as isize) } })
// CHECK-NEXT:             as i32)
// CHECK-NEXT:             != (0 as i32)
// CHECK-NEXT:         || ((unsafe { *unsafe { (buffer.as_mut_ptr() as *mut i8).offset((6 as i32) as isize) } })
// CHECK-NEXT:             as i32)
// CHECK-NEXT:             != (71 as i32)
// CHECK-NEXT:         || ((unsafe { *unsafe { (buffer.as_mut_ptr() as *mut i8).offset((7 as i32) as isize) } })
// CHECK-NEXT:             as i32)
// CHECK-NEXT:             != (0 as i32)
// CHECK-NEXT:         || ((unsafe { *unsafe { (buffer.as_mut_ptr() as *mut i8).offset((8 as i32) as isize) } })
// CHECK-NEXT:             as i32)
// CHECK-NEXT:             != (73 as i32)
// CHECK-NEXT:         || ((unsafe { *unsafe { (buffer.as_mut_ptr() as *mut i8).offset((9 as i32) as isize) } })
// CHECK-NEXT:             as i32)
// CHECK-NEXT:             != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test4() -> i32 {
// CHECK-NEXT:     let mut X: foo = {
// CHECK-NEXT:         let mut [[T_0]]: foo = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].b = (66 as i32) as i8;
// CHECK-NEXT:         [[T_0]].d = (68 as i32) as i8;
// CHECK-NEXT:         [[T_0]].f = (70 as i32) as i8;
// CHECK-NEXT:         [[T_0]].h = (72 as i32) as i8;
// CHECK-NEXT:         [[T_0]].j = (74 as i32) as i8;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut buffer: [i8; 10] = [0 as i8; 10];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memcpy(
// CHECK-NEXT:             buffer.as_mut_ptr() as *mut (),
// CHECK-NEXT:             std::ptr::addr_of!(X) as *const (),
// CHECK-NEXT:             ((10 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     if ((unsafe { *unsafe { (buffer.as_mut_ptr() as *mut i8).offset((0 as i32) as isize) } })
// CHECK-NEXT:         as i32)
// CHECK-NEXT:         != (0 as i32)
// CHECK-NEXT:         || ((unsafe { *unsafe { (buffer.as_mut_ptr() as *mut i8).offset((1 as i32) as isize) } })
// CHECK-NEXT:             as i32)
// CHECK-NEXT:             != (66 as i32)
// CHECK-NEXT:         || ((unsafe { *unsafe { (buffer.as_mut_ptr() as *mut i8).offset((2 as i32) as isize) } })
// CHECK-NEXT:             as i32)
// CHECK-NEXT:             != (0 as i32)
// CHECK-NEXT:         || ((unsafe { *unsafe { (buffer.as_mut_ptr() as *mut i8).offset((3 as i32) as isize) } })
// CHECK-NEXT:             as i32)
// CHECK-NEXT:             != (68 as i32)
// CHECK-NEXT:         || ((unsafe { *unsafe { (buffer.as_mut_ptr() as *mut i8).offset((4 as i32) as isize) } })
// CHECK-NEXT:             as i32)
// CHECK-NEXT:             != (0 as i32)
// CHECK-NEXT:         || ((unsafe { *unsafe { (buffer.as_mut_ptr() as *mut i8).offset((5 as i32) as isize) } })
// CHECK-NEXT:             as i32)
// CHECK-NEXT:             != (70 as i32)
// CHECK-NEXT:         || ((unsafe { *unsafe { (buffer.as_mut_ptr() as *mut i8).offset((6 as i32) as isize) } })
// CHECK-NEXT:             as i32)
// CHECK-NEXT:             != (0 as i32)
// CHECK-NEXT:         || ((unsafe { *unsafe { (buffer.as_mut_ptr() as *mut i8).offset((7 as i32) as isize) } })
// CHECK-NEXT:             as i32)
// CHECK-NEXT:             != (72 as i32)
// CHECK-NEXT:         || ((unsafe { *unsafe { (buffer.as_mut_ptr() as *mut i8).offset((8 as i32) as isize) } })
// CHECK-NEXT:             as i32)
// CHECK-NEXT:             != (0 as i32)
// CHECK-NEXT:         || ((unsafe { *unsafe { (buffer.as_mut_ptr() as *mut i8).offset((9 as i32) as isize) } })
// CHECK-NEXT:             as i32)
// CHECK-NEXT:             != (74 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     test1();
// CHECK-NEXT:     test2();
// CHECK-NEXT:     test3();
// CHECK-NEXT:     test4();
// CHECK-NEXT:     0 as i32
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
// SLATE-FILECHECK-END
