struct S {
  char c1, c2, c3, c4;
} __attribute__((aligned(4)));

static char     bar(char **p) __attribute__((noclone, noinline));
static struct S foo(void) __attribute__((noclone, noinline));

int i;

static char bar(char **p) {
  i = 1;
  return 0;
}

static struct S foo(void) {
  struct S ret;
  char     r, s, c1, c2;
  char    *p = &r;

  s = bar(&p);
  if (s)
    c2 = *p;
  c1 = 0;

  ret.c1 = c1;
  ret.c2 = c2;
  return ret;
}

int main(void) {
  struct S s = foo();
  if (s.c1 != 0)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(4))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     c1: i8,
// CHECK-NEXT:     c2: i8,
// CHECK-NEXT:     c3: i8,
// CHECK-NEXT:     c4: i8,
// CHECK-NEXT: }
// CHECK-NEXT: static mut i: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn bar(mut p: &*mut i8) -> i8 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         i = 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     (0 as i32) as i8
// CHECK-NEXT: }
// CHECK-NEXT: fn foo() -> S {
// CHECK-NEXT:     let mut ret: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut r: i8 = 0 as i8;
// CHECK-NEXT:     let mut s: i8 = 0 as i8;
// CHECK-NEXT:     let mut c1: i8 = (0 as i32) as i8;
// CHECK-NEXT:     let mut c2: i8 = 0 as i8;
// CHECK-NEXT:     let mut p: *mut i8 = std::ptr::addr_of_mut!(r);
// CHECK-NEXT:     s = bar(unsafe { &(*std::ptr::addr_of_mut!(p)) });
// CHECK-NEXT:     if s != (0 as i8) {
// CHECK-NEXT:         c2 = unsafe { *p };
// CHECK-NEXT:     }
// CHECK-NEXT:     ret.c1 = c1;
// CHECK-NEXT:     ret.c2 = c2;
// CHECK-NEXT:     return ret;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut s: S = foo();
// CHECK-NEXT:     if (s.c1 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
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
