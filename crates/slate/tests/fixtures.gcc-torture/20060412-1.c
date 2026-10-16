extern void abort(void);

struct S {
  long o;
};

struct T {
  long     o;
  struct S m[82];
};

struct T t;

int main() {
  struct S *p, *q;

  p = (struct S *)&t;
  p = &((struct T *)p)->m[0];
  q = p + 82;
  while (--q > p)
    q->o = -1;
  q->o = 0;

  if (q > p)
    abort();
  if (q - p > 0)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     o: i64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct T {
// CHECK-NEXT:     o: i64,
// CHECK-NEXT:     m: [S; 82],
// CHECK-NEXT: }
// CHECK-NEXT: static mut t: T = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut p: *mut S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut q: *mut S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     p = (unsafe { std::ptr::addr_of_mut!(t) }) as *mut S;
// CHECK-NEXT:     p = unsafe {
// CHECK-NEXT:         unsafe { (*(p as *mut T)).m.as_mut_ptr() as *mut S }.offset((0 as i32) as isize)
// CHECK-NEXT:     };
// CHECK-NEXT:     q = unsafe { p.offset((82 as i32) as isize) };
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: *mut S = q;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: *mut S = unsafe { [[V_0]].offset(-((1 as i32) as isize)) };
// CHECK-NEXT:         q = [[V_1]];
// CHECK-NEXT:         if !([[V_1]] > p) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*q).o = -(1 as i32) as i64;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*q).o = (0 as i32) as i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     if q > p {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((unsafe { q.offset_from(p as *mut S) }) as i64) > ((0 as i32) as i64) {
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
