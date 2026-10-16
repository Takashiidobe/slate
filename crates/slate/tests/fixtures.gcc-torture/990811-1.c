void abort(void);
void exit(int);

struct s {
  long a;
  int  b;
};

int foo(int x, void *y) {
  switch (x) {
  case 0:
    return ((struct s *)y)->a;
  case 1:
    return *(signed char *)y;
  case 2:
    return *(short *)y;
  }
  abort();
}

int main() {
  struct s    s;
  short       sh[10];
  signed char c[10];
  int         i;

  s.a = 1;
  s.b = 2;
  for (i = 0; i < 10; i++) {
    sh[i] = i;
    c[i]  = i;
  }

  if (foo(0, &s) != 1)
    abort();
  if (foo(1, c + 3) != 3)
    abort();
  if (foo(2, sh + 3) != 3)
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
// CHECK-NEXT: struct s {
// CHECK-NEXT:     a: i64,
// CHECK-NEXT:     b: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn foo(mut x: i32, mut y: *mut ()) -> i32 {
// CHECK-NEXT:     match x {
// CHECK-NEXT:         0 => {
// CHECK-NEXT:             return (unsafe { (*(y as *mut s)).a }) as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         1 => {
// CHECK-NEXT:             return (unsafe { *(y as *mut i8) }) as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         2 => {
// CHECK-NEXT:             return (unsafe { *(y as *mut i16) }) as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {}
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { abort() };
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut s: s = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut sh: __SlateAlign16<[i16; 10]> = __SlateAlign16([0 as i16; 10]);
// CHECK-NEXT:     let mut c: [i8; 10] = [0 as i8; 10];
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     s.a = (1 as i32) as i64;
// CHECK-NEXT:     s.b = 2 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (10 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (sh.0.as_mut_ptr() as *mut i16).offset(i as isize) } = i as i16;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (c.as_mut_ptr() as *mut i8).offset(i as isize) } = i as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:         i = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if foo(0 as i32, std::ptr::addr_of_mut!(s) as *mut ()) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if foo(
// CHECK-NEXT:         1 as i32,
// CHECK-NEXT:         (unsafe { (c.as_mut_ptr() as *mut i8).offset((3 as i32) as isize) }) as *mut (),
// CHECK-NEXT:     ) != (3 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if foo(
// CHECK-NEXT:         2 as i32,
// CHECK-NEXT:         (unsafe { (sh.0.as_mut_ptr() as *mut i16).offset((3 as i32) as isize) })
// CHECK-NEXT:             as *mut (),
// CHECK-NEXT:     ) != (3 as i32)
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
