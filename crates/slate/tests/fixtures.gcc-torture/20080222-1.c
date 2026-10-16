extern void abort(void);

struct container {
  unsigned char data[1];
};

unsigned char space[6] = {1, 2, 3, 4, 5, 6};

int foo(struct container *p) { return p->data[4]; }

int main() {
  if (foo((struct container *)space) != 5)
    abort();
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
// CHECK-NEXT: struct container {
// CHECK-NEXT:     data: [u8; 1],
// CHECK-NEXT: }
// CHECK-NEXT: static mut space: [u8; 6] = [
// CHECK-NEXT:     ((1 as i32) as i8) as u8,
// CHECK-NEXT:     ((2 as i32) as i8) as u8,
// CHECK-NEXT:     ((3 as i32) as i8) as u8,
// CHECK-NEXT:     ((4 as i32) as i8) as u8,
// CHECK-NEXT:     ((5 as i32) as i8) as u8,
// CHECK-NEXT:     ((6 as i32) as i8) as u8,
// CHECK-NEXT: ];
// CHECK-NEXT: fn foo(mut p: *mut container) -> i32 {
// CHECK-NEXT:     ((unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { (*p).data.as_mut_ptr() as *mut u8 }.offset((4 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     }) as u32) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if foo((unsafe { std::ptr::addr_of_mut!(space) as *mut u8 }) as *mut container)
// CHECK-NEXT:         != (5 as i32)
// CHECK-NEXT:     {
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
