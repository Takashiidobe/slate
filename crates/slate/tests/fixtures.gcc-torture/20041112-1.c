/* This was failing on Alpha because the comparison (p != -1) was rewritten
   as (p+1 != 0) and p+1 isn't allowed to wrap for pointers.  */

extern void abort(void);

typedef __SIZE_TYPE__ size_t;

int global;

static void *foo(int p) {
  if (p == 0) {
    global++;
    return &global;
  }

  return (void *)(size_t)-1;
}

int bar(void) {
  void *p;

  p = foo(global);
  if (p != (void *)(size_t)-1)
    return 1;

  global++;
  return 0;
}

int main(void) {
  global = 1;
  if (bar() != 0)
    abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! This was failing on Alpha because the comparison (p != -1) was rewritten
// CHECK-NEXT: //! as (p+1 != 0) and p+1 isn't allowed to wrap for pointers.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut global: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut p: i32) -> *mut () {
// CHECK-NEXT:     if p == (0 as i32) {
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = unsafe { global };
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             global = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:         return (unsafe { std::ptr::addr_of_mut!(global) }) as *mut ();
// CHECK-NEXT:     }
// CHECK-NEXT:     ((-(1 as i32) as i64) as u64) as *mut ()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar() -> i32 {
// CHECK-NEXT:     let mut p: *mut () = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     p = foo(unsafe { global });
// CHECK-NEXT:     if p != (((-(1 as i32) as i64) as u64) as *mut ()) {
// CHECK-NEXT:         return 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: i32 = unsafe { global };
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         global = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         global = 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if bar() != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
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
