void abort(void);
void exit(int);

int *foo(int *x, int b) {

  *(x++) = 55;
  if (b)
    *(x++) = b;

  return x;
}

int main(void) {
  int a[5];

  __builtin_memset(a, 1, sizeof(a));

  if (foo(a, 0) - a != 1 || a[0] != 55 || a[1] != a[4])
    abort();

  __builtin_memset(a, 1, sizeof(a));

  if (foo(a, 2) - a != 2 || a[0] != 55 || a[1] != 2)
    abort();

  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT:     #[link_name = "memset"]
// CHECK-NEXT:     fn __builtin_memset([[V_1:__v[0-9]+]]: *mut (), [[V_2:__v[0-9]+]]: i32, [[V_3:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn foo(mut x: *mut i32, mut b: i32) -> *mut i32 {
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: *mut i32 = x;
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: *mut i32 = unsafe { [[V_4]].offset((1 as i32) as isize) };
// CHECK-NEXT:     x = [[V_5]];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_4]] = 55 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if b != (0 as i32) {
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: *mut i32 = x;
// CHECK-NEXT:         let mut [[V_7:__v[0-9]+]]: *mut i32 = unsafe { [[V_6]].offset((1 as i32) as isize) };
// CHECK-NEXT:         x = [[V_7]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_6]] = b;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: __SlateAlign16<[i32; 5]> = __SlateAlign16([0 as i32; 5]);
// CHECK-NEXT:     unsafe { __builtin_memset(a.0.as_mut_ptr() as *mut (), 1 as i32, 20 as u64) };
// CHECK-NEXT:     if ((unsafe {
// CHECK-NEXT:         foo(a.0.as_mut_ptr() as *mut i32, 0 as i32)
// CHECK-NEXT:             .offset_from(a.0.as_mut_ptr() as *mut i32)
// CHECK-NEXT:     }) as i64) != ((1 as i32) as i64)
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             *unsafe { (a.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) }
// CHECK-NEXT:         }) != (55 as i32)
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             *unsafe { (a.0.as_mut_ptr() as *mut i32).offset((1 as i32) as isize) }
// CHECK-NEXT:         })
// CHECK-NEXT:             != unsafe {
// CHECK-NEXT:                 *unsafe { (a.0.as_mut_ptr() as *mut i32).offset((4 as i32) as isize) }
// CHECK-NEXT:             }
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { __builtin_memset(a.0.as_mut_ptr() as *mut (), 1 as i32, 20 as u64) };
// CHECK-NEXT:     if ((unsafe {
// CHECK-NEXT:         foo(a.0.as_mut_ptr() as *mut i32, 2 as i32)
// CHECK-NEXT:             .offset_from(a.0.as_mut_ptr() as *mut i32)
// CHECK-NEXT:     }) as i64) != ((2 as i32) as i64)
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             *unsafe { (a.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) }
// CHECK-NEXT:         }) != (55 as i32)
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             *unsafe { (a.0.as_mut_ptr() as *mut i32).offset((1 as i32) as isize) }
// CHECK-NEXT:         }) != (2 as i32)
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
