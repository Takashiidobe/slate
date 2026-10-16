void abort(void);
void exit(int);

int foo1(void) {
  union {
    char     a[sizeof(unsigned)];
    unsigned b;
  } u;

  u.b = 0x01;
  return u.a[0];
}

int foo2(void) {
  volatile union {
    char     a[sizeof(unsigned)];
    unsigned b;
  } u;

  u.b = 0x01;
  return u.a[0];
}

int main(void) {
  if (foo1() != foo2())
    abort();
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
// CHECK-NEXT: union [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     a: [i8; 4],
// CHECK-NEXT:     b: u32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[SLATERECORD_1:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     a: [i8; 4],
// CHECK-NEXT:     b: u32,
// CHECK-NEXT: }
// CHECK-NEXT: fn foo1() -> i32 {
// CHECK-NEXT:     let mut u: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         u.b = (1 as i32) as u32;
// CHECK-NEXT:     }
// CHECK-NEXT:     (unsafe {
// CHECK-NEXT:         *unsafe { unsafe { u.a.as_mut_ptr() as *mut i8 }.offset((0 as i32) as isize) }
// CHECK-NEXT:     }) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn foo2() -> i32 {
// CHECK-NEXT:     let mut u: [[SLATERECORD_1]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(u.b), (1 as i32) as u32) };
// CHECK-NEXT:     (unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(unsafe {
// CHECK-NEXT:             unsafe { u.a.as_mut_ptr() as *mut i8 }.offset((0 as i32) as isize)
// CHECK-NEXT:         })
// CHECK-NEXT:     }) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if foo1() != foo2() {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
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
