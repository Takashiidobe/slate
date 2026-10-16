/* PR target/19005 */
extern void abort(void);

int v, s;

void bar(int a, int b) {
  unsigned char x = v;

  if (!s) {
    if (a != x || b != (unsigned char)(x + 1))
      abort();
  } else if (a != (unsigned char)(x + 1) || b != x)
    abort();
  s ^= 1;
}

int foo(int x) {
  unsigned char a = x, b = x + 1;

  bar(a, b);
  a ^= b;
  b ^= a;
  a ^= b;
  bar(a, b);
  return 0;
}

int main(void) {
  for (v = -10; v < 266; v++)
    foo(v);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // PR target/19005
// CHECK-NEXT: static mut v: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut s: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut a: i32, mut b: i32) {
// CHECK-NEXT:     let mut x: u8 = ((unsafe { v }) as i8) as u8;
// CHECK-NEXT:     if !((unsafe { s }) != (0 as i32)) {
// CHECK-NEXT:         if a != ((x as u32) as i32)
// CHECK-NEXT:             || b != (((((((x as u32) as i32) + (1 as i32)) as i8) as u8) as u32) as i32)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if a != (((((((x as u32) as i32) + (1 as i32)) as i8) as u8) as u32) as i32)
// CHECK-NEXT:             || b != ((x as u32) as i32)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: i32 = unsafe { s };
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] ^ (1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         s = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: i32) -> i32 {
// CHECK-NEXT:     let mut a: u8 = (x as i8) as u8;
// CHECK-NEXT:     let mut b: u8 = ((x + (1 as i32)) as i8) as u8;
// CHECK-NEXT:     bar((a as u32) as i32, (b as u32) as i32);
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: u8 = a;
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: u8 = (((([[V_2]] as u32) as i32) ^ ((b as u32) as i32)) as i8) as u8;
// CHECK-NEXT:     a = [[V_3]];
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: u8 = b;
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: u8 = (((([[V_4]] as u32) as i32) ^ ((a as u32) as i32)) as i8) as u8;
// CHECK-NEXT:     b = [[V_5]];
// CHECK-NEXT:     let mut [[V_6:__v[0-9]+]]: u8 = a;
// CHECK-NEXT:     let mut [[V_7:__v[0-9]+]]: u8 = (((([[V_6]] as u32) as i32) ^ ((b as u32) as i32)) as i8) as u8;
// CHECK-NEXT:     a = [[V_7]];
// CHECK-NEXT:     bar((a as u32) as i32, (b as u32) as i32);
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         v = -(10 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (unsafe { v }) < (266 as i32) {
// CHECK-NEXT:         foo(unsafe { v });
// CHECK-NEXT:         let mut [[V_8:__v[0-9]+]]: i32 = unsafe { v };
// CHECK-NEXT:         let mut [[V_9:__v[0-9]+]]: i32 = [[V_8]] + (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             v = [[V_9]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
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
