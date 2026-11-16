/* { dg-options "-ffreestanding" } */
#include <stddef.h>
#include <stdio.h>

typedef int (*extension_fn)(int *value, void *data);

static int add_one(int *value, void *data) { return *value + *(int *)data; }

static const extension_fn table[] = {add_one};

__attribute__((noinline)) static int run_extensions(int *value, const extension_fn *tbl,
                                                    unsigned count, void *data) {
  if (!tbl)
    return -(int)count - 1;
  return tbl[0](value, data);
}

int main(void) {
  int value = 4;
  int step = 3;
  printf("%d %d\n", run_extensions(&value, table, 1, &step),
         run_extensions(&value, NULL, 0, &step));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-ffreestanding" }
// CHECK-NEXT: #[repr(transparent)]
// CHECK-NEXT: struct __SlateConst<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: unsafe impl<T> Sync for __SlateConst<T> {}
// CHECK-EMPTY:
// CHECK-NEXT: extern "C" fn add_one(value: *mut i32, data: *mut ()) -> i32 {
// CHECK-NEXT:     (unsafe { *value }) + unsafe { *(data as *mut i32) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static table: __SlateConst<[Option<unsafe extern "C" fn(*mut i32, *mut ()) -> i32>; 1]> =
// CHECK-NEXT:     __SlateConst([Some(add_one)]);
// CHECK-EMPTY:
// CHECK-NEXT: #[inline(never)]
// CHECK-NEXT: fn run_extensions(
// CHECK-NEXT:     value: *mut i32,
// CHECK-NEXT:     tbl: *const Option<unsafe extern "C" fn(*mut i32, *mut ()) -> i32>,
// CHECK-NEXT:     count: u32,
// CHECK-NEXT:     data: *mut (),
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     if tbl.is_null() {
// CHECK-NEXT:         return -(count as i32) - 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { unsafe { *tbl.add(0) }.unwrap()(value, data) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C" fn main() -> i32 {
// CHECK-NEXT:     let mut value: i32 = 4;
// CHECK-NEXT:     let mut step: i32 = 3;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%d %d\n".as_ptr(),
// CHECK-NEXT:             run_extensions(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(value),
// CHECK-NEXT:                 core::ptr::addr_of!(table.0)
// CHECK-NEXT:                     as *const Option<unsafe extern "C" fn(*mut i32, *mut ()) -> i32>,
// CHECK-NEXT:                 1u32,
// CHECK-NEXT:                 core::ptr::addr_of_mut!(step) as *mut (),
// CHECK-NEXT:             ),
// CHECK-NEXT:             run_extensions(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(value),
// CHECK-NEXT:                 core::ptr::null::<Option<unsafe extern "C" fn(*mut i32, *mut ()) -> i32>>(),
// CHECK-NEXT:                 0u32,
// CHECK-NEXT:                 core::ptr::addr_of_mut!(step) as *mut (),
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
