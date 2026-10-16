extern void abort(void);
extern void exit(int);

struct baz {
  int a, b, c, d, e;
};

void bar(struct baz *x, int f, int g, int h, int i, int j) {
  if (x->a != 1 || x->b != 2 || x->c != 3 || x->d != 4 || x->e != 5 || f != 6 ||
      g != 7 || h != 8 || i != 9 || j != 10)
    abort();
}

void foo(struct baz x, char **y) { bar(&x, 6, 7, 8, 9, 10); }

int main() {
  struct baz x;

  x.a = 1;
  x.b = 2;
  x.c = 3;
  x.d = 4;
  x.e = 5;
  foo(x, (char **)0);
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
// CHECK-NEXT: struct baz {
// CHECK-NEXT:     a: i32,
// CHECK-NEXT:     b: i32,
// CHECK-NEXT:     c: i32,
// CHECK-NEXT:     d: i32,
// CHECK-NEXT:     e: i32,
// CHECK-NEXT: }
// CHECK-NEXT: fn bar(mut x: &baz, mut f: i32, mut g: i32, mut h: i32, mut i: i32, mut j: i32) {
// CHECK-NEXT:     if (unsafe { (*(x as *const baz)).a }) != (1 as i32)
// CHECK-NEXT:         || (unsafe { (*(x as *const baz)).b }) != (2 as i32)
// CHECK-NEXT:         || (unsafe { (*(x as *const baz)).c }) != (3 as i32)
// CHECK-NEXT:         || (unsafe { (*(x as *const baz)).d }) != (4 as i32)
// CHECK-NEXT:         || (unsafe { (*(x as *const baz)).e }) != (5 as i32) || f != (6 as i32)
// CHECK-NEXT:         || g != (7 as i32) || h != (8 as i32) || i != (9 as i32) || j != (10 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut x: baz, mut y: *mut *mut i8) {
// CHECK-NEXT:     bar(
// CHECK-NEXT:         unsafe { &(*std::ptr::addr_of_mut!(x)) },
// CHECK-NEXT:         6 as i32,
// CHECK-NEXT:         7 as i32,
// CHECK-NEXT:         8 as i32,
// CHECK-NEXT:         9 as i32,
// CHECK-NEXT:         10 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: baz = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     x.a = 1 as i32;
// CHECK-NEXT:     x.b = 2 as i32;
// CHECK-NEXT:     x.c = 3 as i32;
// CHECK-NEXT:     x.d = 4 as i32;
// CHECK-NEXT:     x.e = 5 as i32;
// CHECK-NEXT:     foo(x, std::ptr::null_mut::<*mut i8>());
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
