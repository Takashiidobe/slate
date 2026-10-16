void abort(void);
void exit(int);

int f(char *p) {}

int main(void) {
  char  c;
  char  c2;
  int   i   = 0;
  char *pc  = &c;
  char *pc2 = &c2;
  int  *pi  = &i;

  *pc2  = 1;
  *pi   = 1;
  *pc2 &= *pi;
  f(pc2);
  *pc2  = 1;
  *pc2 &= *pi;
  if (*pc2 != 1)
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
// CHECK-NEXT: fn f(mut p: *mut i8) -> i32 {
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut c2: i8 = 0 as i8;
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut pc2: *mut i8 = std::ptr::addr_of_mut!(c2);
// CHECK-NEXT:     let mut pi: *mut i32 = std::ptr::addr_of_mut!(i);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *pc2 = (1 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *pi = 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i8 = unsafe { *pc2 };
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: i8 = (([[V_1]] as i32) & unsafe { *pi }) as i8;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *pc2 = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT:     f(pc2);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *pc2 = (1 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: i8 = unsafe { *pc2 };
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: i8 = (([[V_3]] as i32) & unsafe { *pi }) as i8;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *pc2 = [[V_4]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((unsafe { *pc2 }) as i32) != (1 as i32) {
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
