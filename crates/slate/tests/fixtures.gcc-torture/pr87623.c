/* PR middle-end/87623 */
/* Testcase by George Thopas <george.thopas@gmail.com> */

struct be {
  unsigned short pad[1];
  unsigned char  a;
  unsigned char  b;
} __attribute__((scalar_storage_order("big-endian")));

typedef struct be t_be;

struct le {
  unsigned short pad[3];
  unsigned char  a;
  unsigned char  b;
};

typedef struct le t_le;

int a_or_b_different(t_be *x, t_le *y) {
  return (x->a != y->a) || (x->b != y->b);
}

int main(void) {
  t_be x = {.a = 1, .b = 2};
  t_le y = {.a = 1, .b = 2};

  if (a_or_b_different(&x, &y))
    __builtin_abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR middle-end/87623
// CHECK-NEXT: //!
// CHECK-NEXT: //! Testcase by George Thopas <george.thopas@gmail.com>
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct be {
// CHECK-NEXT:     pad: [u16; 1],
// CHECK-NEXT:     a: u8,
// CHECK-NEXT:     b: u8,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct le {
// CHECK-NEXT:     pad: [u16; 3],
// CHECK-NEXT:     a: u8,
// CHECK-NEXT:     b: u8,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn a_or_b_different(mut x: &be, mut y: &le) -> i32 {
// CHECK-NEXT:     ((((unsafe { (*(x as *const be)).a }) as u32) as i32)
// CHECK-NEXT:         != (((unsafe { (*(y as *const le)).a }) as u32) as i32)
// CHECK-NEXT:         || (((unsafe { (*(x as *const be)).b }) as u32) as i32)
// CHECK-NEXT:             != (((unsafe { (*(y as *const le)).b }) as u32) as i32)) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: be = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: be = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].a = ((1 as i32) as i8) as u8;
// CHECK-NEXT:         [[T_0]].b = ((2 as i32) as i8) as u8;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut y: le = {
// CHECK-NEXT:         let mut [[T_1:__t[0-9]+]]: le = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]].a = ((1 as i32) as i8) as u8;
// CHECK-NEXT:         [[T_1]].b = ((2 as i32) as i8) as u8;
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     if a_or_b_different(unsafe { &(*std::ptr::addr_of_mut!(x)) }, unsafe {
// CHECK-NEXT:         &(*std::ptr::addr_of_mut!(y))
// CHECK-NEXT:     }) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
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
