void abort(void);
void exit(int);

struct s {
  int f[4];
};

int foo(struct s s, int x1, int x2, int x3, int x4, int x5, int x6, int x7) {
  return s.f[3] + x7;
}

int main() {
  struct s s = {1, 2, 3, 4};

  if (foo(s, 100, 200, 300, 400, 500, 600, 700) != 704)
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
// CHECK-NEXT:     f: [i32; 4],
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(
// CHECK-NEXT:     mut s: s,
// CHECK-NEXT:     mut x1: i32,
// CHECK-NEXT:     mut x2: i32,
// CHECK-NEXT:     mut x3: i32,
// CHECK-NEXT:     mut x4: i32,
// CHECK-NEXT:     mut x5: i32,
// CHECK-NEXT:     mut x6: i32,
// CHECK-NEXT:     mut x7: i32,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     (unsafe { *unsafe { (s.f.as_mut_ptr() as *mut i32).offset((3 as i32) as isize) } })
// CHECK-NEXT:         + x7
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut s: s = s {
// CHECK-NEXT:         f: [1 as i32, 2 as i32, 3 as i32, 4 as i32],
// CHECK-NEXT:     };
// CHECK-NEXT:     if foo(
// CHECK-NEXT:         s,
// CHECK-NEXT:         100 as i32,
// CHECK-NEXT:         200 as i32,
// CHECK-NEXT:         300 as i32,
// CHECK-NEXT:         400 as i32,
// CHECK-NEXT:         500 as i32,
// CHECK-NEXT:         600 as i32,
// CHECK-NEXT:         700 as i32,
// CHECK-NEXT:     ) != (704 as i32)
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
