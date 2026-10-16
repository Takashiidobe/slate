void abort(void);
void exit(int);

short int         i = -1;
const char *const wordlist[207];

const char *const *foo(void) {
  register const char *const *wordptr = &wordlist[207u + i];
  return wordptr;
}

int main() {
  if (foo() != &wordlist[206])
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
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut i: i16 = -(1 as i32) as i16;
// CHECK-NEXT: static mut wordlist: __SlateAlign16<[*const i8; 207]> = __SlateAlign16(unsafe {
// CHECK-NEXT:     std::mem::zeroed()
// CHECK-NEXT: });
// CHECK-NEXT: fn foo() -> *const *const i8 {
// CHECK-NEXT:     let mut wordptr: *const *const i8 = unsafe {
// CHECK-NEXT:         unsafe { std::ptr::addr_of!(wordlist.0) as *const *const i8 }
// CHECK-NEXT:             .offset((207 as u32).wrapping_add(((unsafe { i }) as i32) as u32) as isize)
// CHECK-NEXT:     };
// CHECK-NEXT:     wordptr
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if foo()
// CHECK-NEXT:         != unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of!(wordlist.0) as *const *const i8 }
// CHECK-NEXT:                 .offset((206 as i32) as isize)
// CHECK-NEXT:         }
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
