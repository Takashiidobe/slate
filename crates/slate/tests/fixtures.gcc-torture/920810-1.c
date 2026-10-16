#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  void *super;
  int   name;
  int   size;
} t;
t *f(t *clas, int size) {
  t *child = (t *)malloc(size);
  memcpy(child, clas, clas->size);
  child->super = clas;
  child->name  = 0;
  child->size  = size;
  return child;
}
int main(void) {
  t foo, *bar;
  memset(&foo, 37, sizeof(t));
  foo.size = sizeof(t);
  bar      = f(&foo, sizeof(t));
  if (bar->super != &foo || bar->name != 0 || bar->size != sizeof(t))
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn malloc(__size: u64) -> *mut ();
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit(__status: i32);
// CHECK-NEXT:     fn memcpy(__dest: *mut (), __src: *const (), __n: u64) -> *mut ();
// CHECK-NEXT:     fn memset(__s: *mut (), __c: i32, __n: u64) -> *mut ();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     super_: *mut (),
// CHECK-NEXT:     name: i32,
// CHECK-NEXT:     size: i32,
// CHECK-NEXT: }
// CHECK-NEXT: fn f(mut clas: *mut [[SLATERECORD_0]], mut size: i32) -> *mut [[SLATERECORD_0]] {
// CHECK-NEXT:     let mut child: *mut [[SLATERECORD_0]] = (unsafe { malloc((size as i64) as u64) })
// CHECK-NEXT:         as *mut [[SLATERECORD_0]];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             child as *mut (),
// CHECK-NEXT:             clas as *const (),
// CHECK-NEXT:             ((unsafe { (*clas).size }) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*child).super_ = clas as *mut ();
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*child).name = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*child).size = size;
// CHECK-NEXT:     }
// CHECK-NEXT:     child
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut foo: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut bar: *mut [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe { memset(std::ptr::addr_of_mut!(foo) as *mut (), 37 as i32, 16 as u64) };
// CHECK-NEXT:     foo.size = ((16 as u64) as u32) as i32;
// CHECK-NEXT:     bar = f(std::ptr::addr_of_mut!(foo), ((16 as u64) as u32) as i32);
// CHECK-NEXT:     if (unsafe { (*bar).super_ }) != (std::ptr::addr_of_mut!(foo) as *mut ())
// CHECK-NEXT:         || (unsafe { (*bar).name }) != (0 as i32)
// CHECK-NEXT:         || (((unsafe { (*bar).size }) as i64) as u64) != (16 as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
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
// SLATE-FILECHECK-END
