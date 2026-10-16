extern void abort(void);
extern void exit(int);

int main(void) {
  int   a, b, c;
  int  *pa, *pb, *pc;
  int **ppa, **ppb, **ppc;
  int   i, j, k, x, y, z;

  a   = 10;
  b   = 20;
  c   = 30;
  pa  = &a;
  pb  = &b;
  pc  = &c;
  ppa = &pa;
  ppb = &pb;
  ppc = &pc;
  x   = 0;
  y   = 0;
  z   = 0;

  for (i = 0; i < 10; i++) {
    if (pa == &a)
      pa = &b;
    else
      pa = &a;
    while ((*pa)--) {
      x++;
      if ((*pa) < 3)
        break;
      else
        pa = &b;
    }
    x++;
    pa = &b;
  }

  if ((*pa) != -5 || (*pb) != -5 || x != 43)
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
// CHECK-NEXT:     let mut a: i32 = 10 as i32;
// CHECK-NEXT:     let mut b: i32 = 20 as i32;
// CHECK-NEXT:     let mut c: i32 = 30 as i32;
// CHECK-NEXT:     let mut pa: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut pb: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut pc: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut ppa: *mut *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut ppb: *mut *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut ppc: *mut *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut x: i32 = 0 as i32;
// CHECK-NEXT:     pa = std::ptr::addr_of_mut!(a);
// CHECK-NEXT:     pb = std::ptr::addr_of_mut!(b);
// CHECK-NEXT:     pc = std::ptr::addr_of_mut!(c);
// CHECK-NEXT:     ppa = std::ptr::addr_of_mut!(pa);
// CHECK-NEXT:     ppb = std::ptr::addr_of_mut!(pb);
// CHECK-NEXT:     ppc = std::ptr::addr_of_mut!(pc);
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (10 as i32) {
// CHECK-NEXT:         if pa == std::ptr::addr_of_mut!(a) {
// CHECK-NEXT:             pa = std::ptr::addr_of_mut!(b);
// CHECK-NEXT:         } else {
// CHECK-NEXT:             pa = std::ptr::addr_of_mut!(a);
// CHECK-NEXT:         }
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i32 = unsafe { *pa };
// CHECK-NEXT:             let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] - (1 as i32);
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *pa = [[V_2]];
// CHECK-NEXT:             }
// CHECK-NEXT:             if !([[V_1]] != (0 as i32)) {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_3:__v[0-9]+]]: i32 = x;
// CHECK-NEXT:             let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] + (1 as i32);
// CHECK-NEXT:             x = [[V_4]];
// CHECK-NEXT:             if (unsafe { *pa }) < (3 as i32) {
// CHECK-NEXT:                 break '[[SLATE_BREAK_1]];
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 pa = std::ptr::addr_of_mut!(b);
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = x;
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: i32 = [[V_5]] + (1 as i32);
// CHECK-NEXT:         x = [[V_6]];
// CHECK-NEXT:         pa = std::ptr::addr_of_mut!(b);
// CHECK-NEXT:         let mut [[V_7:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_8:__v[0-9]+]]: i32 = [[V_7]] + (1 as i32);
// CHECK-NEXT:         i = [[V_8]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { *pa }) != -(5 as i32) || (unsafe { *pb }) != -(5 as i32)
// CHECK-NEXT:         || x != (43 as i32)
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
