struct __attribute__((__packed__)) s {
  char               c;
  unsigned long long x;
};
void __attribute__((__noinline__)) foo(struct s *s) { s->x = 0; }
int                                main(void) {
  struct s s = {1, ~0ULL};
  foo(&s);
  return s.x != 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct s {
// CHECK-NEXT:     c: i8,
// CHECK-NEXT:     x: u64,
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut s: &mut s) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(s as *mut s)).x = ((0 as i32) as i64) as u64;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut s: s = s {
// CHECK-NEXT:         c: (1 as i32) as i8,
// CHECK-NEXT:         x: !(0 as u64),
// CHECK-NEXT:     };
// CHECK-NEXT:     foo(unsafe { &mut (*std::ptr::addr_of_mut!(s)) });
// CHECK-NEXT:     (s.x != (((0 as i32) as i64) as u64)) as i32
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
