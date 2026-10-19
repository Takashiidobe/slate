#include <stdint.h>
#include <stdio.h>

typedef int (*Callback)(int);

static int add_one(int x) { return x + 1; }

static void *store_fn(void *fn) { return fn; }

int main(void) {
  void    *slot = (void *)add_one;
  Callback cb   = (Callback)slot;

  void    *slot2 = store_fn((void *)add_one);
  Callback cb2   = (Callback)slot2;

  printf("%d %d\n", cb(41), cb2(99));

  uintptr_t address = (uintptr_t)add_one;
  Callback  cb3     = (Callback)address;
  printf("%d %d\n", cb3(1), address == (uintptr_t)(void *)add_one);

  typedef long (*Other)(long, long);
  Other    other = (Other)add_one;
  Callback cb4   = (Callback)other;
  printf("%d\n", cb4(7));

  Callback none = (Callback)(void *)0;
  void    *gone = (void *)none;
  printf("%d %d\n", none == 0, gone == 0);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: extern "C-unwind" fn add_one(x: i32) -> i32 {
// CHECK-NEXT:     x + 1
// CHECK-NEXT: }
// CHECK-NEXT: fn store_fn(r#fn: *mut ()) -> *mut () {
// CHECK-NEXT:     r#fn
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let slot: *mut () = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             Option<unsafe extern "C-unwind" fn(i32) -> i32>,
// CHECK-NEXT:             *mut (),
// CHECK-NEXT:         >(Some(add_one))
// CHECK-NEXT:     };
// CHECK-NEXT:     let cb: Option<unsafe extern "C-unwind" fn(i32) -> i32> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             *mut (),
// CHECK-NEXT:             Option<unsafe extern "C-unwind" fn(i32) -> i32>,
// CHECK-NEXT:         >(slot)
// CHECK-NEXT:     };
// CHECK-NEXT:     let slot2: *mut () = store_fn(unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             Option<unsafe extern "C-unwind" fn(i32) -> i32>,
// CHECK-NEXT:             *mut (),
// CHECK-NEXT:         >(Some(add_one))
// CHECK-NEXT:     });
// CHECK-NEXT:     let cb2: Option<unsafe extern "C-unwind" fn(i32) -> i32> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             *mut (),
// CHECK-NEXT:             Option<unsafe extern "C-unwind" fn(i32) -> i32>,
// CHECK-NEXT:         >(slot2)
// CHECK-NEXT:     };
// CHECK-NEXT:     println!("{} {}", unsafe { cb.unwrap() (41i32) }, unsafe { cb2.unwrap() (99i32) });
// CHECK-NEXT:     let address: u64 = (unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             Option<unsafe extern "C-unwind" fn(i32) -> i32>,
// CHECK-NEXT:             usize,
// CHECK-NEXT:         >(Some(add_one))
// CHECK-NEXT:     }) as u64;
// CHECK-NEXT:     let cb3: Option<unsafe extern "C-unwind" fn(i32) -> i32> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             usize,
// CHECK-NEXT:             Option<unsafe extern "C-unwind" fn(i32) -> i32>,
// CHECK-NEXT:         >(address as usize)
// CHECK-NEXT:     };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}", unsafe { cb3.unwrap() (1i32) }, (address == ((unsafe {
// CHECK-NEXT:         std::mem::transmute::< Option < unsafe extern "C-unwind" fn (i32) -> i32 >, * mut
// CHECK-NEXT:         () > (Some(add_one)) }) as u64)) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     let other: Option<unsafe extern "C-unwind" fn(i64, i64) -> i64> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             Option<unsafe extern "C-unwind" fn(i32) -> i32>,
// CHECK-NEXT:             Option<unsafe extern "C-unwind" fn(i64, i64) -> i64>,
// CHECK-NEXT:         >(Some(add_one))
// CHECK-NEXT:     };
// CHECK-NEXT:     let cb4: Option<unsafe extern "C-unwind" fn(i32) -> i32> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             Option<unsafe extern "C-unwind" fn(i64, i64) -> i64>,
// CHECK-NEXT:             Option<unsafe extern "C-unwind" fn(i32) -> i32>,
// CHECK-NEXT:         >(other)
// CHECK-NEXT:     };
// CHECK-NEXT:     println!("{}", unsafe { cb4.unwrap() (7i32) });
// CHECK-NEXT:     let none: Option<unsafe extern "C-unwind" fn(i32) -> i32> = None;
// CHECK-NEXT:     let gone: *mut () = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             Option<unsafe extern "C-unwind" fn(i32) -> i32>,
// CHECK-NEXT:             *mut (),
// CHECK-NEXT:         >(none)
// CHECK-NEXT:     };
// CHECK-NEXT:     println!("{} {}", (none == None) as i32, gone.is_null() as i32);
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
