#include <stdarg.h>
#include <stdio.h>

struct block {
  int a[64];
  char tag[4];
};

static struct block make(int seed) {
  struct block b = {};
  for (int i = 0; i < 64; i++)
    b.a[i] = seed * i;
  b.tag[0] = 'a' + seed % 26;
  return b;
}

static struct block summed(int count, ...) {
  struct block b = {};
  va_list ap;
  va_start(ap, count);
  for (int i = 0; i < count; i++)
    b.a[i] = va_arg(ap, int);
  va_end(ap);
  return b;
}

int main(void) {
  int total = 0;
  for (int i = 0; i < 4; i++)
    total += make(i).a[i + 1];
  printf("%d %d %c\n", make(3).a[5], total, make(4).tag[0]);
  printf("%d %d\n", summed(3, 7, 8, 9).a[2], make(make(2).a[3]).a[2]);
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
// CHECK-NEXT: struct block {
// CHECK-NEXT:     a: [i32; 64],
// CHECK-NEXT:     tag: [i8; 4],
// CHECK-NEXT: }
// CHECK-NEXT: fn make(seed: i32) -> block {
// CHECK-NEXT:     let mut b: block = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: block = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     for i in 0i32..64i32 {
// CHECK-NEXT:         b.a[i as usize] = seed * i;
// CHECK-NEXT:     }
// CHECK-NEXT:     b.tag[0] = (97 + seed % 26) as i8;
// CHECK-NEXT:     return b;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C-unwind" fn summed(count: i32, mut __va_args: ...) -> block {
// CHECK-NEXT:     let mut b: block = {
// CHECK-NEXT:         let mut [[T_0]]: block = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'static> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::ffi::VaList<'_>,
// CHECK-NEXT:             core::ffi::VaList<'static>,
// CHECK-NEXT:         >(__va_args.clone())
// CHECK-NEXT:     };
// CHECK-NEXT:     for i in 0..count {
// CHECK-NEXT:         b.a[i as usize] = unsafe { ap.next_arg::<i32>() };
// CHECK-NEXT:     }
// CHECK-NEXT:     return b;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: block = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_1:__slate_compound_[0-9]+]]: block = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_2:__slate_compound_[0-9]+]]: block = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_3:__slate_compound_[0-9]+]]: block = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_4:__slate_compound_[0-9]+]]: block = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_5:__slate_compound_[0-9]+]]: block = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: for i in 0i32..4i32 {
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             *((*{
// CHECK-NEXT:                 [[SLATE_COMPOUND_0]] = make(i);
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:             })
// CHECK-NEXT:                 .a
// CHECK-NEXT:                 .as_mut_ptr() as *mut i32)
// CHECK-NEXT:                 .offset((i + 1) as isize)
// CHECK-NEXT:         };
// CHECK-NEXT:         total += [[V_0]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %d %c\n".as_ptr(),
// CHECK-NEXT:             *((*{
// CHECK-NEXT:                 [[SLATE_COMPOUND_1]] = make(3i32);
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_1]])
// CHECK-NEXT:             })
// CHECK-NEXT:                 .a
// CHECK-NEXT:                 .as_mut_ptr() as *mut i32)
// CHECK-NEXT:                 .add(5),
// CHECK-NEXT:             total,
// CHECK-NEXT:             *((*{
// CHECK-NEXT:                 [[SLATE_COMPOUND_2]] = make(4i32);
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_2]])
// CHECK-NEXT:             })
// CHECK-NEXT:                 .tag
// CHECK-NEXT:                 .as_mut_ptr() as *mut i8)
// CHECK-NEXT:                 .add(0) as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let __slate_printf_arg0 = unsafe {
// CHECK-NEXT:         *((*{
// CHECK-NEXT:             [[SLATE_COMPOUND_3]] = summed(3i32, 7i32, 8i32, 9i32);
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_3]])
// CHECK-NEXT:         })
// CHECK-NEXT:             .a
// CHECK-NEXT:             .as_mut_ptr() as *mut i32)
// CHECK-NEXT:             .add(2)
// CHECK-NEXT:     };
// CHECK-NEXT:     let __slate_printf_arg1 = unsafe {
// CHECK-NEXT:         *((*{
// CHECK-NEXT:             [[SLATE_COMPOUND_5]] = make(
// CHECK-NEXT:                 *((*{
// CHECK-NEXT:                     [[SLATE_COMPOUND_4]] = make(2i32);
// CHECK-NEXT:                     std::ptr::addr_of_mut!([[SLATE_COMPOUND_4]])
// CHECK-NEXT:                 })
// CHECK-NEXT:                     .a
// CHECK-NEXT:                     .as_mut_ptr() as *mut i32)
// CHECK-NEXT:                     .add(3),
// CHECK-NEXT:             );
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_5]])
// CHECK-NEXT:         })
// CHECK-NEXT:             .a
// CHECK-NEXT:             .as_mut_ptr() as *mut i32)
// CHECK-NEXT:             .add(2)
// CHECK-NEXT:     };
// CHECK-NEXT:     println!("{} {}", __slate_printf_arg0, __slate_printf_arg1);
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
