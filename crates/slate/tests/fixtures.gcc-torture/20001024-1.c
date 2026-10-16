void abort(void);
void exit(int);

struct a;

extern int baz(struct a *__restrict x);

struct a {
  long v;
  long w;
};

struct b {
  struct a c;
  struct a d;
};

int bar(int x, const struct b *__restrict y, struct b *__restrict z) {
  if (y->c.v || y->c.w != 250000 || y->d.v || y->d.w != 250000)
    abort();
}

void foo(void) {
  struct b x;
  x.c.v = 0;
  x.c.w = 250000;
  x.d   = x.c;
  bar(0, &x, ((void *)0));
}

int main() {
  foo();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT:     fn baz(x: *mut a) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct a {
// CHECK-NEXT:     v: i64,
// CHECK-NEXT:     w: i64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct b {
// CHECK-NEXT:     c: a,
// CHECK-NEXT:     d: a,
// CHECK-NEXT: }
// CHECK-NEXT: fn bar(mut x: i32, mut y: &b, mut z: *mut b) -> i32 {
// CHECK-NEXT:     if (unsafe { (*(y as *const b)).c.v }) != (0 as i64)
// CHECK-NEXT:         || (unsafe { (*(y as *const b)).c.w }) != ((250000 as i32) as i64)
// CHECK-NEXT:         || (unsafe { (*(y as *const b)).d.v }) != (0 as i64)
// CHECK-NEXT:         || (unsafe { (*(y as *const b)).d.w }) != ((250000 as i32) as i64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn foo() {
// CHECK-NEXT:     let mut x: b = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     x.c.v = (0 as i32) as i64;
// CHECK-NEXT:     x.c.w = (250000 as i32) as i64;
// CHECK-NEXT:     x.d = x.c;
// CHECK-NEXT:     bar(
// CHECK-NEXT:         0 as i32,
// CHECK-NEXT:         unsafe { &(*(std::ptr::addr_of_mut!(x) as *const b)) },
// CHECK-NEXT:         std::ptr::null_mut::<b>(),
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     foo();
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
