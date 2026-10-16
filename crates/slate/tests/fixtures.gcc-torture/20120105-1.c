struct __attribute__((packed)) S {
  int a, b, c;
};

static int __attribute__((noinline, noclone)) extract(const char *p) {
  struct S s;
  __builtin_memcpy(&s, p, sizeof(struct S));
  return s.a;
}

volatile int i;

int main(void) {
  char p[sizeof(struct S) + 1];

  __builtin_memset(p, 0, sizeof(struct S) + 1);
  i = extract(p + 1);

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "memcpy"]
// CHECK-NEXT:     fn __builtin_memcpy([[V_0:__v[0-9]+]]: *mut (), [[V_1:__v[0-9]+]]: *const (), [[V_2:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT:     #[link_name = "memset"]
// CHECK-NEXT:     fn __builtin_memset([[V_3:__v[0-9]+]]: *mut (), [[V_4:__v[0-9]+]]: i32, [[V_5:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     a: i32,
// CHECK-NEXT:     b: i32,
// CHECK-NEXT:     c: i32,
// CHECK-NEXT: }
// CHECK-NEXT: fn extract(mut p: *const i8) -> i32 {
// CHECK-NEXT:     let mut s: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memcpy(std::ptr::addr_of_mut!(s) as *mut (), p as *const (), 12 as u64)
// CHECK-NEXT:     };
// CHECK-NEXT:     s.a
// CHECK-NEXT: }
// CHECK-NEXT: static mut i: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut p: [i8; 13] = [0 as i8; 13];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memset(
// CHECK-NEXT:             p.as_mut_ptr() as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (12 as u64).wrapping_add(((1 as i32) as i64) as u64),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::write_volatile(
// CHECK-NEXT:             std::ptr::addr_of_mut!(i),
// CHECK-NEXT:             extract(
// CHECK-NEXT:                 (unsafe { (p.as_mut_ptr() as *mut i8).offset((1 as i32) as isize) })
// CHECK-NEXT:                     as *const i8,
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
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
