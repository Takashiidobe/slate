void abort(void);
void exit(int);

struct blah {
  int m1, m2;
};

void die(struct blah arg) {
  int         i;
  struct blah buf[1];

  for (i = 0; i < 1; buf[i++] = arg)
    ;
  if (buf[0].m1 != 1) {
    abort();
  }
}

int main() {
  struct blah s = {1, 2};

  die(s);
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct blah {
// CHECK-NEXT:     m1: i32,
// CHECK-NEXT:     m2: i32,
// CHECK-NEXT: }
// CHECK-NEXT: fn die(mut arg: blah) {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut buf: [blah; 1] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (1 as i32) {
// CHECK-NEXT:         {}
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:         i = [[V_2]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (buf.as_mut_ptr() as *mut blah).offset([[V_1]] as isize) } = arg;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         (*unsafe { (buf.as_mut_ptr() as *mut blah).offset((0 as i32) as isize) }).m1
// CHECK-NEXT:     }) != (1 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut s: blah = blah { m1: 1 as i32, m2: 2 as i32 };
// CHECK-NEXT:     die(s);
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
