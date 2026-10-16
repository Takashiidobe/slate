void abort(void);
void exit(int);

struct s {
  char *p;
  int   t;
};

extern void bar(void);
extern void foo(struct s *);

int main(void) {
  bar();
  bar();
  exit(0);
}

void bar(void) { foo(&(struct s){"hi", 1}); }

void foo(struct s *p) {
  if (p->t != 1)
    abort();
  p->t = 2;
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
// CHECK-NEXT:     p: *mut i8,
// CHECK-NEXT:     t: i32,
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     bar();
// CHECK-NEXT:     bar();
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: fn bar() {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: s = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     foo(unsafe {
// CHECK-NEXT:         std::ptr::addr_of_mut!(
// CHECK-NEXT:             * { [[SLATE_COMPOUND_0]] = s { p : c"hi".as_ptr() as * mut i8, t : 1 as i32 };
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]]) }
// CHECK-NEXT:         )
// CHECK-NEXT:     });
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut p: *mut s) {
// CHECK-NEXT:     if (unsafe { (*p).t }) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*p).t = 2 as i32;
// CHECK-NEXT:     }
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
