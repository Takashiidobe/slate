#include <stdint.h>
int printf(const char *, ...);
int x = 5;
int arr[4] = {1, 2, 3, 4};
static int f(void) { return 7; }
struct holder { void *p; int tag; };
static void *direct = (void *)(uintptr_t)&x;
static void *function = (void *)(uintptr_t)f;
static void *unsigned_long = (void *)(unsigned long)&x;
static int *signed_width = (int *)(intptr_t)&x;
static void *nested = (void *)(uintptr_t)(char *)(uintptr_t)&x;
static void *mixed_sign = (void *)(long)(unsigned long)&x;
static int *element = (int *)(uintptr_t)&arr[2];
static struct holder held = {(void *)(uintptr_t)&x, 3};
static void *table[] = {(void *)(uintptr_t)&x, (void *)(uintptr_t)&arr[1], 0};
#define __ADDRESSABLE(sym) static void *__addressable_##sym __attribute__((used)) = (void *)(uintptr_t)&sym;
__ADDRESSABLE(x)
int main(void) {
  printf("%d %d %d %d\n", direct == &x, function == (void *)f, unsigned_long == &x, *signed_width);
  printf("%d %d %d\n", nested == &x, mixed_sign == &x, *element);
  printf("%d %d\n", held.p == &x, held.tag);
  printf("%d %d %d\n", *(int *)table[0], *(int *)table[1], table[2] == 0);
  printf("%d %d\n", __addressable_x == &x, ((int (*)(void))function)());
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: static mut x: i32 = 5;
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut arr: __SlateAlign16<[i32; 4]> = __SlateAlign16([1i32, 2i32, 3i32, 4i32]);
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.f")]
// CHECK-NEXT: extern "C-unwind" fn f() -> i32 {
// CHECK-NEXT:     7
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct holder {
// CHECK-NEXT:     p: *mut (),
// CHECK-NEXT:     tag: i32,
// CHECK-NEXT: }
// CHECK-NEXT: static mut direct: *mut () = (unsafe { std::ptr::addr_of_mut!(x) }) as *mut ();
// CHECK-NEXT: static mut function: *mut () = unsafe {
// CHECK-NEXT:     std::mem::transmute::<Option<unsafe extern "C-unwind" fn() -> i32>, *mut ()>(Some(f))
// CHECK-NEXT: };
// CHECK-NEXT: static mut unsigned_long: *mut () = (unsafe { std::ptr::addr_of_mut!(x) }) as *mut ();
// CHECK-NEXT: static mut signed_width: *mut i32 = (unsafe { std::ptr::addr_of_mut!(x) }) as *mut i32;
// CHECK-NEXT: static mut nested: *mut () = (unsafe { std::ptr::addr_of_mut!(x) }) as *mut ();
// CHECK-NEXT: static mut mixed_sign: *mut () = (unsafe { std::ptr::addr_of_mut!(x) }) as *mut ();
// CHECK-NEXT: static mut element: *mut i32 = unsafe { std::ptr::addr_of_mut!(arr.0) as *mut i32 }
// CHECK-NEXT:     .wrapping_offset(2i32 as isize) as *mut i32;
// CHECK-NEXT: static mut held: holder = {
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: holder = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].p = (unsafe { std::ptr::addr_of_mut!(x) }) as *mut ();
// CHECK-NEXT:     [[T_0]].tag = 3i32;
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut table: __SlateAlign16<[*mut (); 3]> = __SlateAlign16([
// CHECK-NEXT:     (unsafe { std::ptr::addr_of_mut!(x) }) as *mut (),
// CHECK-NEXT:     unsafe { std::ptr::addr_of_mut!(arr.0) as *mut i32 }.wrapping_offset(1i32 as isize)
// CHECK-NEXT:         as *mut (),
// CHECK-NEXT:     std::ptr::null_mut::<()>(),
// CHECK-NEXT: ]);
// CHECK-NEXT: #[used]
// CHECK-NEXT: static mut __addressable_x: *mut () = (unsafe { std::ptr::addr_of_mut!(x) }) as *mut ();
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %d %d %d\n".as_ptr(),
// CHECK-NEXT:             (direct == (std::ptr::addr_of_mut!(x) as *mut ())) as i32,
// CHECK-NEXT:             (function
// CHECK-NEXT:                 == unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         Option<unsafe extern "C-unwind" fn() -> i32>,
// CHECK-NEXT:                         *mut (),
// CHECK-NEXT:                     >(Some(f))
// CHECK-NEXT:                 }) as i32,
// CHECK-NEXT:             (unsigned_long == (std::ptr::addr_of_mut!(x) as *mut ())) as i32,
// CHECK-NEXT:             *signed_width,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %d %d\n".as_ptr(),
// CHECK-NEXT:             (nested == (std::ptr::addr_of_mut!(x) as *mut ())) as i32,
// CHECK-NEXT:             (mixed_sign == (std::ptr::addr_of_mut!(x) as *mut ())) as i32,
// CHECK-NEXT:             *element,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %d\n".as_ptr(),
// CHECK-NEXT:             (held.p == (std::ptr::addr_of_mut!(x) as *mut ())) as i32,
// CHECK-NEXT:             held.tag,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %d %d\n".as_ptr(),
// CHECK-NEXT:             *(table.0[0] as *mut i32),
// CHECK-NEXT:             *(table.0[1] as *mut i32),
// CHECK-NEXT:             unsafe { table.0[2] }.is_null() as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %d\n".as_ptr(),
// CHECK-NEXT:             (__addressable_x == (std::ptr::addr_of_mut!(x) as *mut ())) as i32,
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     *mut (),
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn() -> i32>,
// CHECK-NEXT:                 >(function)
// CHECK-NEXT:             }
// CHECK-NEXT:                 .unwrap()(),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
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
