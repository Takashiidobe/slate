void exit(int);

#ifdef __OPTIMIZE__
extern void undef(void);

void bar(unsigned x) {}
void baz(unsigned x) {}

extern inline void foo(int a, int b) {
  int c = 0;
  while (c++ < b)
    (__builtin_constant_p(a) ? ((a) > 20000 ? undef() : bar(a)) : baz(a));
}
#else
void foo(int a, int b) {}
#endif

int main(void) {
  foo(10, 100);
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut a: i32, mut b: i32) {}
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     foo(10 as i32, 100 as i32);
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
