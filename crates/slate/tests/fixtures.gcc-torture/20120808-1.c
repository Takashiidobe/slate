extern void exit(int);
extern void abort(void);

volatile int i;
unsigned char *volatile cp;
unsigned char d[32] = {0};

int main(void) {
  unsigned char  c[32] = {0};
  unsigned char *p     = d + i;
  int            j;
  for (j = 0; j < 30; j++) {
    int x = 0xff;
    int y = *++p;
    switch (j) {
    case 1:
      x ^= 2;
      break;
    case 2:
      x ^= 4;
      break;
    case 25:
      x ^= 1;
      break;
    default:
      break;
    }
    c[j] = y | x;
    cp   = p;
  }
  if (c[0] != 0xff || c[1] != 0xfd || c[2] != 0xfb || c[3] != 0xff ||
      c[4] != 0xff || c[25] != 0xfe || cp != d + 30)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut i: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut cp: *mut u8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut d: __SlateAlign16<[u8; 32]> = __SlateAlign16({
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: [u8; 32] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]][0] = ((0 as i32) as i8) as u8;
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: });
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut c: __SlateAlign16<[u8; 32]> = __SlateAlign16({
// CHECK-NEXT:         let mut [[T_0]]: [u8; 32] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]][0] = ((0 as i32) as i8) as u8;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     });
// CHECK-NEXT:     let mut p: *mut u8 = unsafe {
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(d.0) as *mut u8 }
// CHECK-NEXT:             .offset((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(i)) }) as isize)
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut j: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while j < (30 as i32) {
// CHECK-NEXT:         let mut x: i32 = 255 as i32;
// CHECK-NEXT:         let mut y: i32 = 0 as i32;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: *mut u8 = p;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: *mut u8 = unsafe { [[V_1]].offset((1 as i32) as isize) };
// CHECK-NEXT:         p = [[V_2]];
// CHECK-NEXT:         y = ((unsafe { *[[V_2]] }) as u32) as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: {
// CHECK-NEXT:             match j {
// CHECK-NEXT:                 1 => {
// CHECK-NEXT:                     let mut [[V_3:__v[0-9]+]]: i32 = x;
// CHECK-NEXT:                     let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] ^ (2 as i32);
// CHECK-NEXT:                     x = [[V_4]];
// CHECK-NEXT:                 }
// CHECK-NEXT:                 2 => {
// CHECK-NEXT:                     let mut [[V_5:__v[0-9]+]]: i32 = x;
// CHECK-NEXT:                     let mut [[V_6:__v[0-9]+]]: i32 = [[V_5]] ^ (4 as i32);
// CHECK-NEXT:                     x = [[V_6]];
// CHECK-NEXT:                 }
// CHECK-NEXT:                 25 => {
// CHECK-NEXT:                     let mut [[V_7:__v[0-9]+]]: i32 = x;
// CHECK-NEXT:                     let mut [[V_8:__v[0-9]+]]: i32 = [[V_7]] ^ (1 as i32);
// CHECK-NEXT:                     x = [[V_8]];
// CHECK-NEXT:                 }
// CHECK-NEXT:                 _ => {}
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (c.0.as_mut_ptr() as *mut u8).offset(j as isize) } = ((y | x)
// CHECK-NEXT:                 as i8) as u8;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(cp), p) };
// CHECK-NEXT:         let mut [[V_9:__v[0-9]+]]: i32 = j;
// CHECK-NEXT:         let mut [[V_10:__v[0-9]+]]: i32 = [[V_9]] + (1 as i32);
// CHECK-NEXT:         j = [[V_10]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if (((unsafe {
// CHECK-NEXT:         *unsafe { (c.0.as_mut_ptr() as *mut u8).offset((0 as i32) as isize) }
// CHECK-NEXT:     }) as u32) as i32) != (255 as i32)
// CHECK-NEXT:         || (((unsafe {
// CHECK-NEXT:             *unsafe { (c.0.as_mut_ptr() as *mut u8).offset((1 as i32) as isize) }
// CHECK-NEXT:         }) as u32) as i32) != (253 as i32)
// CHECK-NEXT:         || (((unsafe {
// CHECK-NEXT:             *unsafe { (c.0.as_mut_ptr() as *mut u8).offset((2 as i32) as isize) }
// CHECK-NEXT:         }) as u32) as i32) != (251 as i32)
// CHECK-NEXT:         || (((unsafe {
// CHECK-NEXT:             *unsafe { (c.0.as_mut_ptr() as *mut u8).offset((3 as i32) as isize) }
// CHECK-NEXT:         }) as u32) as i32) != (255 as i32)
// CHECK-NEXT:         || (((unsafe {
// CHECK-NEXT:             *unsafe { (c.0.as_mut_ptr() as *mut u8).offset((4 as i32) as isize) }
// CHECK-NEXT:         }) as u32) as i32) != (255 as i32)
// CHECK-NEXT:         || (((unsafe {
// CHECK-NEXT:             *unsafe { (c.0.as_mut_ptr() as *mut u8).offset((25 as i32) as isize) }
// CHECK-NEXT:         }) as u32) as i32) != (254 as i32)
// CHECK-NEXT:         || (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(cp)) })
// CHECK-NEXT:             != unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(d.0) as *mut u8 }
// CHECK-NEXT:                     .offset((30 as i32) as isize)
// CHECK-NEXT:             }
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
