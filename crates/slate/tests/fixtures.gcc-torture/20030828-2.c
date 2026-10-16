void abort(void);
void exit(int);

struct rtx_def {
  int code;
};

int main(void) {
  int             tmp[2];
  struct rtx_def *r, s;
  int            *p, *q;

  /* The alias analyzer was creating the same memory tag for r, p and q
     because 'struct rtx_def *' is type-compatible with 'int *'.  However,
     the alias set of 'int[2]' is not the same as 'int *', so variable
     'tmp' was deemed not aliased with anything.  */
  r       = &s;
  r->code = 39;

  /* If 'r' wasn't declared, then q and tmp would have had the same memory
     tag.  */
  p      = tmp;
  q      = p + 1;
  *q     = 0;
  tmp[1] = 39;
  if (*q != 39)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct rtx_def {
// CHECK-NEXT:     code: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut tmp: [i32; 2] = [0 as i32; 2];
// CHECK-NEXT:     let mut r: *mut rtx_def = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut s: rtx_def = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut p: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut q: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     // The alias analyzer was creating the same memory tag for r, p and q
// CHECK-NEXT:     // because 'struct rtx_def *' is type-compatible with 'int *'.  However,
// CHECK-NEXT:     // the alias set of 'int[2]' is not the same as 'int *', so variable
// CHECK-NEXT:     // 'tmp' was deemed not aliased with anything.
// CHECK-NEXT:     r = std::ptr::addr_of_mut!(s);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*r).code = 39 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     // If 'r' wasn't declared, then q and tmp would have had the same memory
// CHECK-NEXT:     // tag.
// CHECK-NEXT:     p = tmp.as_mut_ptr() as *mut i32;
// CHECK-NEXT:     q = unsafe { p.offset((1 as i32) as isize) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *q = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (tmp.as_mut_ptr() as *mut i32).offset((1 as i32) as isize) } = 39 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { *q }) != (39 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
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
