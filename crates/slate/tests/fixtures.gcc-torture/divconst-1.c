void abort(void);
void exit(int);

typedef struct {
  unsigned a, b, c, d;
} t1;

void f(t1 *ps) {
  ps->a = 10000;
  ps->b = ps->a / 3;
  ps->c = 10000;
  ps->d = ps->c / 3;
}

int main(void) {
  t1 s;
  f(&s);
  if (s.a != 10000 || s.b != 3333 || s.c != 10000 || s.d != 3333)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     a: u32,
// CHECK-NEXT:     b: u32,
// CHECK-NEXT:     c: u32,
// CHECK-NEXT:     d: u32,
// CHECK-NEXT: }
// CHECK-NEXT: fn f(mut ps: &mut [[SLATERECORD_0]]) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(ps as *mut [[SLATERECORD_0]])).a = (10000 as i32) as u32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(ps as *mut [[SLATERECORD_0]])).b = (unsafe { (*(ps as *mut [[SLATERECORD_0]])).a })
// CHECK-NEXT:             / ((3 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(ps as *mut [[SLATERECORD_0]])).c = (10000 as i32) as u32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(ps as *mut [[SLATERECORD_0]])).d = (unsafe { (*(ps as *mut [[SLATERECORD_0]])).c })
// CHECK-NEXT:             / ((3 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut s: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     f(unsafe { &mut (*std::ptr::addr_of_mut!(s)) });
// CHECK-NEXT:     if s.a != ((10000 as i32) as u32) || s.b != ((3333 as i32) as u32)
// CHECK-NEXT:         || s.c != ((10000 as i32) as u32) || s.d != ((3333 as i32) as u32)
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
