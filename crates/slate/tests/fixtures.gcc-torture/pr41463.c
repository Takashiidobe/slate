#include <stdlib.h>

union tree_node;

struct tree_common {
  int   a;
  long  b;
  long  c;
  void *p;
  int   d;
};

struct other_tree {
  struct tree_common common;
  int                arr[14];
};

struct tree_vec {
  struct tree_common common;
  int                length;
  union tree_node   *a[1];
};

union tree_node {
  struct other_tree othr;
  struct tree_vec   vec;
};

union tree_node global;

union tree_node *__attribute__((noinline)) foo(union tree_node *p, int i) {
  union tree_node **q;
  p->vec.a[i] = (union tree_node *)0;
  q           = &p->vec.a[1];
  *q          = &global;
  return p->vec.a[i];
}

extern void  abort(void);
extern void *malloc(__SIZE_TYPE__);

int main() {
  union tree_node *p = malloc(sizeof(union tree_node));
  if (foo(p, 1) != &global)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn malloc(__size: u64) -> *mut ();
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct tree_common {
// CHECK-NEXT:     a: i32,
// CHECK-NEXT:     b: i64,
// CHECK-NEXT:     c: i64,
// CHECK-NEXT:     p: *mut (),
// CHECK-NEXT:     d: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct other_tree {
// CHECK-NEXT:     common: tree_common,
// CHECK-NEXT:     arr: [i32; 14],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct tree_vec {
// CHECK-NEXT:     common: tree_common,
// CHECK-NEXT:     length: i32,
// CHECK-NEXT:     a: [*mut tree_node; 1],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union tree_node {
// CHECK-NEXT:     othr: other_tree,
// CHECK-NEXT:     vec: tree_vec,
// CHECK-NEXT: }
// CHECK-NEXT: static mut global: tree_node = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn foo(mut p: *mut tree_node, mut i: i32) -> *mut tree_node {
// CHECK-NEXT:     let mut q: *mut *mut tree_node = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { (*p).vec.a.as_mut_ptr() as *mut *mut tree_node }.offset(i as isize)
// CHECK-NEXT:         } = std::ptr::null_mut::<tree_node>();
// CHECK-NEXT:     }
// CHECK-NEXT:     q = unsafe {
// CHECK-NEXT:         unsafe { (*p).vec.a.as_mut_ptr() as *mut *mut tree_node }
// CHECK-NEXT:             .offset((1 as i32) as isize)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *q = unsafe { std::ptr::addr_of_mut!(global) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { (*p).vec.a.as_mut_ptr() as *mut *mut tree_node }.offset(i as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut p: *mut tree_node = (unsafe { malloc(96 as u64) }) as *mut tree_node;
// CHECK-NEXT:     if foo(p, 1 as i32) != unsafe { std::ptr::addr_of_mut!(global) } {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
