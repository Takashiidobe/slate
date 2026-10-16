typedef enum { C = 1, D = 2 } B;
extern void abort(void);

struct S {
  B __attribute__((mode(byte))) a;
  B __attribute__((mode(byte))) b;
};

void foo(struct S *x) {
  if (x->a != C || x->b != D)
    abort();
}

int main(void) {
  struct S s;
  s.a = C;
  s.b = D;
  foo(&s);
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
// CHECK-NEXT:     a: u8,
// CHECK-NEXT:     b: u8,
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut x: &S) {
// CHECK-NEXT:     if (((unsafe { (*(x as *const S)).a }) as u32) as i32) != (1 as i32)
// CHECK-NEXT:         || (((unsafe { (*(x as *const S)).b }) as u32) as i32) != (2 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut s: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     s.a = ((1 as i32) as i8) as u8;
// CHECK-NEXT:     s.b = ((2 as i32) as i8) as u8;
// CHECK-NEXT:     foo(unsafe { &(*std::ptr::addr_of_mut!(s)) });
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
