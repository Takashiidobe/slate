
unsigned char a = 6;
int           b, c;

static void fn1() {
  int           i = a > 1 ? 1 : a, j = 6 & (c = a && (b = a));
  int           d = 0, e = a, f = ~c, g = b || a;
  unsigned char h = ~a;
  if (a)
    f = j;
  if (h && g)
    d = a;
  i = -~(f * d * h) + c && (e || i) ^ f;
  if (i != 1)
    __builtin_abort();
}

int main() {
  fn1();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-NEXT: static mut a: u8 = ((6 as i32) as i8) as u8;
// CHECK-NEXT: static mut b: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut c: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn fn1() {
// CHECK-NEXT:     let mut i: i32 = if (((unsafe { a }) as u32) as i32) > (1 as i32) {
// CHECK-NEXT:         1 as i32
// CHECK-NEXT:     } else {
// CHECK-NEXT:         ((unsafe { a }) as u32) as i32
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut j: i32 = 0 as i32;
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: bool;
// CHECK-NEXT:     if (unsafe { a }) != (0 as u8) {
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = ((unsafe { a }) as u32) as i32;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             b = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:         [[V_0]] = [[V_1]] != (0 as i32);
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_0]] = false as bool;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: i32 = [[V_0]] as i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         c = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT:     j = (6 as i32) & [[V_2]];
// CHECK-NEXT:     let mut d: i32 = 0 as i32;
// CHECK-NEXT:     let mut e: i32 = ((unsafe { a }) as u32) as i32;
// CHECK-NEXT:     let mut f: i32 = !unsafe { c };
// CHECK-NEXT:     let mut g: i32 = ((unsafe { b }) != (0 as i32) || (unsafe { a }) != (0 as u8))
// CHECK-NEXT:         as i32;
// CHECK-NEXT:     let mut h: u8 = (!(((unsafe { a }) as u32) as i32) as i8) as u8;
// CHECK-NEXT:     if (unsafe { a }) != (0 as u8) {
// CHECK-NEXT:         f = j;
// CHECK-NEXT:     }
// CHECK-NEXT:     if h != (0 as u8) && g != (0 as i32) {
// CHECK-NEXT:         d = ((unsafe { a }) as u32) as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     i = (-(!(f * d * ((h as u32) as i32))) + unsafe { c } != (0 as i32)
// CHECK-NEXT:         && ((e != (0 as i32) || i != (0 as i32)) as i32) ^ f != (0 as i32)) as i32;
// CHECK-NEXT:     if i != (1 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     fn1();
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
