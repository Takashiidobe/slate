extern void  abort(void);
typedef long GLint;
void         aglChoosePixelFormat(const GLint *);

void find(const int *alistp) {
  const int *blist;
  int        list[32];
  if (alistp)
    blist = alistp;
  else {
    list[3] = 42;
    blist   = list;
  }
  aglChoosePixelFormat((GLint *)blist);
}

void aglChoosePixelFormat(const GLint *a) {
  int *b = (int *)a;
  if (b[3] != 42)
    abort();
}

int main(void) {
  find(0);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn find(mut alistp: *const i32) {
// CHECK-NEXT:     let mut blist: *const i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut list: __SlateAlign16<[i32; 32]> = __SlateAlign16([0 as i32; 32]);
// CHECK-NEXT:     if alistp != std::ptr::null::<i32>() {
// CHECK-NEXT:         blist = alistp;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (list.0.as_mut_ptr() as *mut i32).offset((3 as i32) as isize) } = 42
// CHECK-NEXT:                 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         blist = list.0.as_mut_ptr() as *const i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     aglChoosePixelFormat(blist as *const i64);
// CHECK-NEXT: }
// CHECK-NEXT: fn aglChoosePixelFormat(mut a: *const i64) {
// CHECK-NEXT:     let mut b: *mut i32 = a as *mut i32;
// CHECK-NEXT:     if (unsafe { *unsafe { b.offset((3 as i32) as isize) } }) != (42 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     find(std::ptr::null::<i32>());
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
