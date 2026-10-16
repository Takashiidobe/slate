/* PR middle-end/13400 */
/* The following test used to fail at run-time with a write to read-only
   memory, caused by if-conversion converting a conditional write into an
   unconditional write.  */

typedef struct {
  int  c, l;
  char ch[3];
} pstr;
const pstr        ao = {2, 2, "OK"};
const pstr *const a  = &ao;

void test1(void) {
  if (a->ch[a->l]) {
    ((char *)a->ch)[a->l] = 0;
  }
}

void test2(void) {
  if (a->ch[a->l]) {
    ((char *)a->ch)[a->l] = -1;
  }
}

void test3(void) {
  if (a->ch[a->l]) {
    ((char *)a->ch)[a->l] = 1;
  }
}

int main(void) {
  test1();
  test2();
  test3();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR middle-end/13400
// CHECK-NEXT: //!
// CHECK-NEXT: //! The following test used to fail at run-time with a write to read-only
// CHECK-NEXT: //! memory, caused by if-conversion converting a conditional write into an
// CHECK-NEXT: //! unconditional write.
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     c: i32,
// CHECK-NEXT:     l: i32,
// CHECK-NEXT:     ch: [i8; 3],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut ao: [[SLATERECORD_0]] = [[SLATERECORD_0]] {
// CHECK-NEXT:     c: 2 as i32,
// CHECK-NEXT:     l: 2 as i32,
// CHECK-NEXT:     ch: [79 as i8, 75 as i8, 0 as i8],
// CHECK-NEXT: };
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: *const [[SLATERECORD_0]] = unsafe { std::ptr::addr_of!(ao) };
// CHECK-EMPTY:
// CHECK-NEXT: fn test1() {
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { (*unsafe { a }).ch.as_ptr() as *const i8 }
// CHECK-NEXT:                 .offset((unsafe { (*unsafe { a }).l }) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     }) != (0 as i8)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 ((unsafe { (*unsafe { a }).ch.as_ptr() as *const i8 }) as *mut i8)
// CHECK-NEXT:                     .offset((unsafe { (*unsafe { a }).l }) as isize)
// CHECK-NEXT:             } = (0 as i32) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test2() {
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { (*unsafe { a }).ch.as_ptr() as *const i8 }
// CHECK-NEXT:                 .offset((unsafe { (*unsafe { a }).l }) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     }) != (0 as i8)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 ((unsafe { (*unsafe { a }).ch.as_ptr() as *const i8 }) as *mut i8)
// CHECK-NEXT:                     .offset((unsafe { (*unsafe { a }).l }) as isize)
// CHECK-NEXT:             } = -(1 as i32) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test3() {
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { (*unsafe { a }).ch.as_ptr() as *const i8 }
// CHECK-NEXT:                 .offset((unsafe { (*unsafe { a }).l }) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     }) != (0 as i8)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 ((unsafe { (*unsafe { a }).ch.as_ptr() as *const i8 }) as *mut i8)
// CHECK-NEXT:                     .offset((unsafe { (*unsafe { a }).l }) as isize)
// CHECK-NEXT:             } = (1 as i32) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     test1();
// CHECK-NEXT:     test2();
// CHECK-NEXT:     test3();
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
