void abort(void);
void exit(int);

int ns_name_skip(unsigned char **x, unsigned char *y) {
  *x = 0;
  return 0;
}

unsigned char a[2];

int dn_skipname(unsigned char *ptr, unsigned char *eom) {
  unsigned char *saveptr = ptr;

  if (ns_name_skip(&ptr, eom) == -1)
    return (-1);
  return (ptr - saveptr);
}

int main(void) {
  if (dn_skipname(&a[0], &a[1]) == 0)
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
// CHECK-NEXT: fn ns_name_skip(mut x: &mut *mut u8, mut y: *mut u8) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(x as *mut *mut u8) = std::ptr::null_mut::<u8>();
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: static mut a: [u8; 2] = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn dn_skipname(mut ptr: *mut u8, mut eom: *mut u8) -> i32 {
// CHECK-NEXT:     let mut saveptr: *mut u8 = ptr;
// CHECK-NEXT:     if ns_name_skip(unsafe { &mut (*std::ptr::addr_of_mut!(ptr)) }, eom) == -(1 as i32) {
// CHECK-NEXT:         return -(1 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     ((unsafe { ptr.offset_from(saveptr as *mut u8) }) as i64) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if dn_skipname(
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(a) as *mut u8 }.offset((0 as i32) as isize)
// CHECK-NEXT:         },
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(a) as *mut u8 }.offset((1 as i32) as isize)
// CHECK-NEXT:         },
// CHECK-NEXT:     ) == (0 as i32)
// CHECK-NEXT:     {
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
