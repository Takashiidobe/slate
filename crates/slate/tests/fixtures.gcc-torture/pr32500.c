extern void abort(void);
extern void exit(int);
void        foo(int) __attribute__((noinline));
void        bar(void) __attribute__((noinline));

/* Make sure foo is not inlined or considered pure/const.  */
int  x;
void foo(int i) { x = i; }
void bar(void) { exit(0); }

int main(int argc, char *argv[]) {
  int i;
  int numbers[4] = {0xdead, 0xbeef, 0x1337, 0x4242};

  for (i = 1; i <= 12; i++) {
    if (i <= 4)
      foo(numbers[i - 1]);
    else if (i >= 7 && i <= 9)
      bar();
  }

  abort();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: /// Make sure foo is not inlined or considered pure/const.
// CHECK-NEXT: static mut x: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut i: i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         x = i;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar() {
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main(mut argc: i32, mut argv: *mut *mut i8) -> i32 {
// CHECK-NEXT:     let mut i: i32 = 1 as i32;
// CHECK-NEXT:     let mut numbers: __SlateAlign16<[i32; 4]> =
// CHECK-NEXT:         __SlateAlign16([57005 as i32, 48879 as i32, 4919 as i32, 16962 as i32]);
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i <= (12 as i32) {
// CHECK-NEXT:         if i <= (4 as i32) {
// CHECK-NEXT:             foo(unsafe {
// CHECK-NEXT:                 *unsafe { (numbers.0.as_mut_ptr() as *mut i32).offset((i - (1 as i32)) as isize) }
// CHECK-NEXT:             });
// CHECK-NEXT:         } else {
// CHECK-NEXT:             if i >= (7 as i32) && i <= (9 as i32) {
// CHECK-NEXT:                 bar();
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:         i = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { abort() };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main(argc as _, argv as _) }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
