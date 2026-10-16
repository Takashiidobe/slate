/* PR target/17377
   Bug in code emitted by "return" pattern on CRIS: missing pop of
   forced return address on stack.  */
/* { dg-require-effective-target return_address } */
void abort(void);
void exit(int);

int calls = 0;

void *f(int) __attribute__((__noinline__));
void *f(int i) {
  /* The code does a little brittle song and dance to trig the "return"
     pattern instead of the function epilogue.  This must still be a
     leaf function for the bug to be exposed.  */

  if (calls++ == 0)
    return __builtin_return_address(0);

  switch (i) {
  case 1:
    return f;
  case 0:
    return __builtin_return_address(0);
  }
  return 0;
}

volatile int x;

void *y(int i) __attribute__((__noinline__, __noclone__));
void *y(int i) {
  x = 0;

  /* This must not be a sibling call: the return address must appear
     constant for different calls to this function.  Postincrementing x
     catches otherwise unidentified multiple returns (e.g. through the
     return-address register and then this epilogue popping the address
     stored on stack in "f").  */
  return (char *)f(i) + x++;
}

int main(void) {
  void *v = y(4);
  if (y(1) != f
      /* Can't reasonably check the validity of the return address
         above, but it's not that important: the test-case will probably
         crash on the first call to f with the bug present, or it will
         run wild including returning early (in y or here), so we also
         try and check the number of calls.  */
      || y(0) != v || y(3) != 0 || y(-1) != 0 || calls != 5)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(link_llvm_intrinsics)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "llvm-intrinsic" {
// CHECK-NEXT:     #[link_name = "llvm.returnaddress.p0"]
// CHECK-NEXT:     fn __slate_intrinsic_ded315013ea6523([[ARG_0:arg[0-9]+]]: i32) -> *mut ();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // PR target/17377
// CHECK-NEXT: // Bug in code emitted by "return" pattern on CRIS: missing pop of
// CHECK-NEXT: // forced return address on stack.
// CHECK-NEXT: //
// CHECK-NEXT: // { dg-require-effective-target return_address }
// CHECK-NEXT: static mut calls: i32 = 0 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.f")]
// CHECK-NEXT: extern "C-unwind" fn f(mut i: i32) -> *mut () {
// CHECK-NEXT:     // The code does a little brittle song and dance to trig the "return"
// CHECK-NEXT:     // pattern instead of the function epilogue.  This must still be a
// CHECK-NEXT:     // leaf function for the bug to be exposed.
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i32 = unsafe { calls };
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         calls = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_1]] == (0 as i32) {
// CHECK-NEXT:         return unsafe { __slate_intrinsic_ded315013ea6523(const { ((0 as i32) as u32) as i32 }) };
// CHECK-NEXT:     }
// CHECK-NEXT:     match i {
// CHECK-NEXT:         1 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 std::mem::transmute::<Option<unsafe extern "C-unwind" fn(i32) -> *mut ()>, *mut ()>(
// CHECK-NEXT:                     Some(f),
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         0 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(const { ((0 as i32) as u32) as i32 })
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {}
// CHECK-NEXT:     }
// CHECK-NEXT:     std::ptr::null_mut::<()>()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut x: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn y(mut i: i32) -> *mut () {
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(x), 0 as i32) };
// CHECK-NEXT:     // This must not be a sibling call: the return address must appear
// CHECK-NEXT:     // constant for different calls to this function.  Postincrementing x
// CHECK-NEXT:     // catches otherwise unidentified multiple returns (e.g. through the
// CHECK-NEXT:     // return-address register and then this epilogue popping the address
// CHECK-NEXT:     // stored on stack in "f").
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) };
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] + (1 as i32);
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(x), [[V_4]]) };
// CHECK-NEXT:     (unsafe { (f(i) as *mut i8).offset([[V_3]] as isize) }) as *mut ()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut v: *mut () = y(4 as i32);
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: bool;
// CHECK-NEXT:     if y(1 as i32)
// CHECK-NEXT:         != unsafe {
// CHECK-NEXT:             std::mem::transmute::<Option<unsafe extern "C-unwind" fn(i32) -> *mut ()>, *mut ()>(
// CHECK-NEXT:                 Some(f),
// CHECK-NEXT:             )
// CHECK-NEXT:         }
// CHECK-NEXT:     {
// CHECK-NEXT:         [[V_5]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_5]] = y(0 as i32) != v;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_6:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_5]] {
// CHECK-NEXT:         [[V_6]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_6]] = y(3 as i32) != std::ptr::null_mut::<()>();
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_7:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_6]] {
// CHECK-NEXT:         [[V_7]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_7]] = y(-(1 as i32)) != std::ptr::null_mut::<()>();
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_7]] || (unsafe { calls }) != (5 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     // Can't reasonably check the validity of the return address
// CHECK-NEXT:     // above, but it's not that important: the test-case will probably
// CHECK-NEXT:     // crash on the first call to f with the bug present, or it will
// CHECK-NEXT:     // run wild including returning early (in y or here), so we also
// CHECK-NEXT:     // try and check the number of calls.
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
