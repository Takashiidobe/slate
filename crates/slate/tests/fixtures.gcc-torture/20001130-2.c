void abort(void);
void exit(int);

static int which_alternative = 3;

static const char *i960_output_ldconst(void);

static const char *output_25(void) {
  switch (which_alternative) {
  case 0:
    return "mov	%1,%0";
  case 1:
    return i960_output_ldconst();
  case 2:
    return "ld	%1,%0";
  case 3:
    return "st	%1,%0";
  }
}

static const char *i960_output_ldconst(void) { return "foo"; }
int                main(void) {
  const char *s = output_25();
  if (s[0] != 's')
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
// CHECK-NEXT: static mut which_alternative: i32 = 3 as i32;
// CHECK-NEXT: fn output_25() -> *const i8 {
// CHECK-NEXT:     match unsafe { which_alternative } {
// CHECK-NEXT:         0 => {
// CHECK-NEXT:             return c"mov\t%1,%0".as_ptr() as *const i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         1 => {
// CHECK-NEXT:             return i960_output_ldconst();
// CHECK-NEXT:         }
// CHECK-NEXT:         2 => {
// CHECK-NEXT:             return c"ld\t%1,%0".as_ptr() as *const i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         3 => {
// CHECK-NEXT:             return c"st\t%1,%0".as_ptr() as *const i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {}
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn i960_output_ldconst() -> *const i8 {
// CHECK-NEXT:     c"foo".as_ptr() as *const i8
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut s: *const i8 = output_25();
// CHECK-NEXT:     if ((unsafe { *unsafe { s.offset((0 as i32) as isize) } }) as i32) != (115 as i32) {
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
