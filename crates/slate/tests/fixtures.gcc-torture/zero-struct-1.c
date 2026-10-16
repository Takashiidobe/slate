struct g {};
char  y[3];
char *f  = &y[0];
char *ff = &y[0];
void  h(void) {
  struct g t;
  *((struct g *)(f++)) = *((struct g *)(ff++));
  *((struct g *)(f++)) = (struct g){};
  t                    = *((struct g *)(ff++));
}

void abort(void);

int main(void) {
  h();
  if (f != &y[2])
    abort();
  if (ff != &y[2])
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct g {}
// CHECK-NEXT: static mut y: [i8; 3] = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut f: *mut i8 = unsafe {
// CHECK-NEXT:     unsafe { std::ptr::addr_of_mut!(y) as *mut i8 }.offset((0 as i32) as isize)
// CHECK-NEXT: };
// CHECK-NEXT: static mut ff: *mut i8 = unsafe {
// CHECK-NEXT:     unsafe { std::ptr::addr_of_mut!(y) as *mut i8 }.offset((0 as i32) as isize)
// CHECK-NEXT: };
// CHECK-NEXT: fn h() {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: g = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut t: g = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: *mut i8 = unsafe { ff };
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: *mut i8 = unsafe { [[V_0]].offset((1 as i32) as isize) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         ff = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: *mut i8 = unsafe { f };
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: *mut i8 = unsafe { [[V_2]].offset((1 as i32) as isize) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         f = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *([[V_2]] as *mut g) = unsafe { *([[V_0]] as *mut g) };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: *mut i8 = unsafe { f };
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: *mut i8 = unsafe { [[V_4]].offset((1 as i32) as isize) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         f = [[V_5]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *([[V_4]] as *mut g) = unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_0]] = g {};
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_6:__v[0-9]+]]: *mut i8 = unsafe { ff };
// CHECK-NEXT:     let mut [[V_7:__v[0-9]+]]: *mut i8 = unsafe { [[V_6]].offset((1 as i32) as isize) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         ff = [[V_7]];
// CHECK-NEXT:     }
// CHECK-NEXT:     t = unsafe { *([[V_6]] as *mut g) };
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     h();
// CHECK-NEXT:     if (unsafe { f })
// CHECK-NEXT:         != unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(y) as *mut i8 }.offset((2 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { ff })
// CHECK-NEXT:         != unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(y) as *mut i8 }.offset((2 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
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
