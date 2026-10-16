extern void              abort(void);
extern void              exit(int);
typedef __UINTPTR_TYPE__ uintptr_t;
int                      main(void) {
  int       a = 0;
  int      *p;
  uintptr_t b;
  b = (uintptr_t)(p = &(int[]){0, 1, 2}[++a]);
  if (a != 1 || *p != 1 || *(int *)b != 1)
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
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: [i32; 3] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut a: i32 = 0 as i32;
// CHECK-NEXT:     let mut p: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut b: u64 = 0 as u64;
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i32 = a;
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:     a = [[V_2]];
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: *mut i32 = unsafe {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*{
// CHECK-NEXT:                 [[SLATE_COMPOUND_0]] = [0 as i32, 1 as i32, 2 as i32];
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:             })
// CHECK-NEXT:                 .as_mut_ptr() as *mut i32
// CHECK-NEXT:         }
// CHECK-NEXT:             .offset([[V_2]] as isize)
// CHECK-NEXT:     };
// CHECK-NEXT:     p = [[V_3]];
// CHECK-NEXT:     b = [[V_3]] as u64;
// CHECK-NEXT:     if a != (1 as i32) || (unsafe { *p }) != (1 as i32)
// CHECK-NEXT:         || (unsafe { *(b as *mut i32) }) != (1 as i32)
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
