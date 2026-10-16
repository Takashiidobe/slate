int x;

void __attribute__((noinline))        foo(void) { x = -x; }
void __attribute__((const, noinline)) bar(void) {}

int __attribute__((noinline)) test(int c) {
  int tmp = x;
  (c ? foo : bar)();
  return tmp + x;
}

extern void abort(void);
int         main() {
  x = 1;
  if (test(1) != 0)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: static mut x: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.foo")]
// CHECK-NEXT: extern "C-unwind" fn foo() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         x = -unsafe { x };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.bar")]
// CHECK-NEXT: extern "C-unwind" fn bar() {}
// CHECK-NEXT: fn test(mut c: i32) -> i32 {
// CHECK-NEXT:     let mut tmp: i32 = unsafe { x };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         {
// CHECK-NEXT:             let [[T_0:__t[0-9]+]]: Option<unsafe extern "C-unwind" fn()> = if c != (0 as i32) {
// CHECK-NEXT:                 Some(foo)
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 Some(bar)
// CHECK-NEXT:             };
// CHECK-NEXT:             [[T_0]]
// CHECK-NEXT:         }
// CHECK-NEXT:             .unwrap()()
// CHECK-NEXT:     };
// CHECK-NEXT:     tmp + unsafe { x }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         x = 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if test(1 as i32) != (0 as i32) {
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
