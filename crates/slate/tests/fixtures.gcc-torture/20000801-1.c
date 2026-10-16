extern void abort(void);
extern void exit(int);

void foo(char *bp, unsigned n) {
  register char  c;
  register char *ep = bp + n;
  register char *sp;

  while (bp < ep) {
    sp     = bp + 3;
    c      = *sp;
    *sp    = *bp;
    *bp++  = c;
    sp     = bp + 1;
    c      = *sp;
    *sp    = *bp;
    *bp++  = c;
    bp    += 2;
  }
}

int main(void) {
  int one = 1;

  if (sizeof(int) != 4 * sizeof(char))
    exit(0);

  foo((char *)&one, sizeof(one));
  foo((char *)&one, sizeof(one));

  if (one != 1)
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
// CHECK-NEXT: fn foo(mut bp: *mut i8, mut n: u32) {
// CHECK-NEXT:     let mut c: i8 = 0 as i8;
// CHECK-NEXT:     let mut ep: *mut i8 = unsafe { bp.offset(n as isize) };
// CHECK-NEXT:     let mut sp: *mut i8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while bp < ep {
// CHECK-NEXT:         sp = unsafe { bp.offset((3 as i32) as isize) };
// CHECK-NEXT:         c = unsafe { *sp };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *sp = unsafe { *bp };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: *mut i8 = bp;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: *mut i8 = unsafe { [[V_1]].offset((1 as i32) as isize) };
// CHECK-NEXT:         bp = [[V_2]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_1]] = c;
// CHECK-NEXT:         }
// CHECK-NEXT:         sp = unsafe { bp.offset((1 as i32) as isize) };
// CHECK-NEXT:         c = unsafe { *sp };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *sp = unsafe { *bp };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: *mut i8 = bp;
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: *mut i8 = unsafe { [[V_3]].offset((1 as i32) as isize) };
// CHECK-NEXT:         bp = [[V_4]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_3]] = c;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: *mut i8 = bp;
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: *mut i8 = unsafe { [[V_5]].offset((2 as i32) as isize) };
// CHECK-NEXT:         bp = [[V_6]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut one: i32 = 1 as i32;
// CHECK-NEXT:     if (4 as u64) != (((4 as i32) as i64) as u64).wrapping_mul(1 as u64) {
// CHECK-NEXT:         unsafe { exit(0 as i32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     foo(std::ptr::addr_of_mut!(one) as *mut i8, (4 as u64) as u32);
// CHECK-NEXT:     foo(std::ptr::addr_of_mut!(one) as *mut i8, (4 as u64) as u32);
// CHECK-NEXT:     if one != (1 as i32) {
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
