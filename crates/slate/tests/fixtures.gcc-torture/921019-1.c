void abort(void);
void exit(int);

void *foo[] = {(void *)&("X"[0])};

int main(void) {
  if (((char *)foo[0])[0] != 'X')
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
// CHECK-NEXT: static mut foo: [*mut (); 1] = [
// CHECK-NEXT:     (unsafe { (b"X\0".as_ptr() as *mut i8).offset((0 as i32) as isize) }) as *mut (),
// CHECK-NEXT: ];
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if ((unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             ((unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(foo) as *mut *mut () }
// CHECK-NEXT:                         .offset((0 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:             }) as *mut i8)
// CHECK-NEXT:                 .offset((0 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     }) as i32) != (88 as i32)
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
