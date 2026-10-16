void abort(void);
void exit(int);

struct S {
  int *sp, fc, *sc, a[2];
};

void f(struct S *x) {
  int *t  = x->sc;
  int  t1 = t[0];
  int  t2 = t[1];
  int  t3 = t[2];
  int  a0 = x->a[0];
  int  a1 = x->a[1];
  t[2]    = t1;
  t[0]    = a1;
  x->a[1] = a0;
  x->a[0] = t3;
  x->fc   = t2;
  x->sp   = t;
}

int main(void) {
  struct S   s;
  static int sc[3] = {2, 3, 4};
  s.sc             = sc;
  s.a[0]           = 10;
  s.a[1]           = 11;
  f(&s);
  if (s.sp[2] != 2)
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
// CHECK-NEXT: struct S {
// CHECK-NEXT:     sp: *mut i32,
// CHECK-NEXT:     fc: i32,
// CHECK-NEXT:     sc: *mut i32,
// CHECK-NEXT:     a: [i32; 2],
// CHECK-NEXT: }
// CHECK-NEXT: fn f(mut x: &mut S) {
// CHECK-NEXT:     let mut t: *mut i32 = unsafe { (*(x as *mut S)).sc };
// CHECK-NEXT:     let mut t1: i32 = unsafe { *unsafe { t.offset((0 as i32) as isize) } };
// CHECK-NEXT:     let mut t2: i32 = unsafe { *unsafe { t.offset((1 as i32) as isize) } };
// CHECK-NEXT:     let mut t3: i32 = unsafe { *unsafe { t.offset((2 as i32) as isize) } };
// CHECK-NEXT:     let mut a0: i32 = unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { (*(x as *mut S)).a.as_mut_ptr() as *mut i32 }
// CHECK-NEXT:                 .offset((0 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut a1: i32 = unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { (*(x as *mut S)).a.as_mut_ptr() as *mut i32 }
// CHECK-NEXT:                 .offset((1 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { t.offset((2 as i32) as isize) } = t1;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { t.offset((0 as i32) as isize) } = a1;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { (*(x as *mut S)).a.as_mut_ptr() as *mut i32 }
// CHECK-NEXT:                 .offset((1 as i32) as isize)
// CHECK-NEXT:         } = a0;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { (*(x as *mut S)).a.as_mut_ptr() as *mut i32 }
// CHECK-NEXT:                 .offset((0 as i32) as isize)
// CHECK-NEXT:         } = t3;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(x as *mut S)).fc = t2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(x as *mut S)).sp = t;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut s: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     s.sc = unsafe { std::ptr::addr_of_mut!(sc) as *mut i32 };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (s.a.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } = 10
// CHECK-NEXT:             as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (s.a.as_mut_ptr() as *mut i32).offset((1 as i32) as isize) } = 11
// CHECK-NEXT:             as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     f(unsafe { &mut (*std::ptr::addr_of_mut!(s)) });
// CHECK-NEXT:     if (unsafe { *unsafe { s.sp.offset((2 as i32) as isize) } }) != (2 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: static mut sc: [i32; 3] = [2 as i32, 3 as i32, 4 as i32];
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
