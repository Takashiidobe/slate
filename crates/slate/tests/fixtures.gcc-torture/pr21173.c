void abort(void);

char  q;
void *a[2];

void foo(char *p) {
  int i;
  for (i = 0; i < 2; i++)
    a[i] += p - &q;
}

int main(void) {
  int i;
  foo(&q);
  for (i = 0; i < 2; i++)
    if (a[i])
      abort();
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
// CHECK-NEXT: static mut q: i8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut a: __SlateAlign16<[*mut (); 2]> = __SlateAlign16(unsafe {
// CHECK-NEXT:     std::mem::zeroed()
// CHECK-NEXT: });
// CHECK-NEXT: fn foo(mut p: *mut i8) {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (2 as i32) {
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: *mut *mut () = unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(a.0) as *mut *mut () }.offset(i as isize)
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: *mut () = unsafe { *[[V_0]] };
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: *mut () = (unsafe {
// CHECK-NEXT:             ([[V_1]] as *const u8)
// CHECK-NEXT:                 .offset(
// CHECK-NEXT:                     ((unsafe {
// CHECK-NEXT:                         p.offset_from((unsafe { std::ptr::addr_of_mut!(q) }) as *mut i8)
// CHECK-NEXT:                     }) as i64) as isize,
// CHECK-NEXT:                 )
// CHECK-NEXT:         }) as *mut ();
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_0]] = [[V_2]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] + (1 as i32);
// CHECK-NEXT:         i = [[V_4]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     foo(unsafe { std::ptr::addr_of_mut!(q) });
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < (2 as i32) {
// CHECK-NEXT:         if (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(a.0) as *mut *mut () }.offset(i as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) != std::ptr::null_mut::<()>()
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: i32 = [[V_5]] + (1 as i32);
// CHECK-NEXT:         i = [[V_6]];
// CHECK-NEXT:     }
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
