extern void exit(int);
extern void abort(void);

volatile int        a = 1;
volatile int        b = 0;
volatile int        x = 2;
volatile signed int r = 8;

void __attribute__((noinline)) foo(void) { exit(0); }

int main(void) {
  int si1 = a;
  int si2 = b;
  int i;

  for (i = 0; i < 100; ++i) {
    foo();
    if (x == 8)
      i++;
    r += i + si1 % si2;
  }
  abort();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: static mut a: i32 = 1 as i32;
// CHECK-NEXT: static mut b: i32 = 0 as i32;
// CHECK-NEXT: static mut x: i32 = 2 as i32;
// CHECK-NEXT: static mut r: i32 = 8 as i32;
// CHECK-NEXT: fn foo() {
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut si1: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// CHECK-NEXT:     let mut si2: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (100 as i32) {
// CHECK-NEXT:         foo();
// CHECK-NEXT:         if (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) == (8 as i32) {
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:             let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:             i = [[V_2]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] + (i + si1 % si2);
// CHECK-NEXT:         unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), [[V_4]]) };
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: i32 = [[V_5]] + (1 as i32);
// CHECK-NEXT:         i = [[V_6]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { abort() };
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
