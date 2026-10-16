void abort(void);
void exit(int);

void __attribute__((noinline)) foo(int *p, int d1, int d2, int d3, short count,
                                   int s1, int s2, int s3, int s4, int s5) {
  int n = count;
  while (n--) {
    *p++ = s1;
    *p++ = s2;
    *p++ = s3;
    *p++ = s4;
    *p++ = s5;
  }
}

int main() {
  int x[10], i;

  foo(x, 0, 0, 0, 2, 100, 200, 300, 400, 500);
  for (i = 0; i < 10; i++)
    if (x[i] != (i % 5 + 1) * 100)
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
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn foo(
// CHECK-NEXT:     mut p: *mut i32,
// CHECK-NEXT:     mut d1: i32,
// CHECK-NEXT:     mut d2: i32,
// CHECK-NEXT:     mut d3: i32,
// CHECK-NEXT:     mut count: i16,
// CHECK-NEXT:     mut s1: i32,
// CHECK-NEXT:     mut s2: i32,
// CHECK-NEXT:     mut s3: i32,
// CHECK-NEXT:     mut s4: i32,
// CHECK-NEXT:     mut s5: i32,
// CHECK-NEXT: ) {
// CHECK-NEXT:     let mut n: i32 = count as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] - (1 as i32);
// CHECK-NEXT:         n = [[V_2]];
// CHECK-NEXT:         if !([[V_1]] != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: *mut i32 = p;
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: *mut i32 = unsafe { [[V_3]].offset((1 as i32) as isize) };
// CHECK-NEXT:         p = [[V_4]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_3]] = s1;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: *mut i32 = p;
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: *mut i32 = unsafe { [[V_5]].offset((1 as i32) as isize) };
// CHECK-NEXT:         p = [[V_6]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_5]] = s2;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_7:__v[0-9]+]]: *mut i32 = p;
// CHECK-NEXT:         let mut [[V_8:__v[0-9]+]]: *mut i32 = unsafe { [[V_7]].offset((1 as i32) as isize) };
// CHECK-NEXT:         p = [[V_8]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_7]] = s3;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_9:__v[0-9]+]]: *mut i32 = p;
// CHECK-NEXT:         let mut [[V_10:__v[0-9]+]]: *mut i32 = unsafe { [[V_9]].offset((1 as i32) as isize) };
// CHECK-NEXT:         p = [[V_10]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_9]] = s4;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_11:__v[0-9]+]]: *mut i32 = p;
// CHECK-NEXT:         let mut [[V_12:__v[0-9]+]]: *mut i32 = unsafe { [[V_11]].offset((1 as i32) as isize) };
// CHECK-NEXT:         p = [[V_12]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_11]] = s5;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: __SlateAlign16<[i32; 10]> = __SlateAlign16([0 as i32; 10]);
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     foo(
// CHECK-NEXT:         x.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:         0 as i32,
// CHECK-NEXT:         0 as i32,
// CHECK-NEXT:         0 as i32,
// CHECK-NEXT:         (2 as i32) as i16,
// CHECK-NEXT:         100 as i32,
// CHECK-NEXT:         200 as i32,
// CHECK-NEXT:         300 as i32,
// CHECK-NEXT:         400 as i32,
// CHECK-NEXT:         500 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < (10 as i32) {
// CHECK-NEXT:         if (unsafe { *unsafe { (x.0.as_mut_ptr() as *mut i32).offset(i as isize) } })
// CHECK-NEXT:             != (i % (5 as i32) + (1 as i32)) * (100 as i32)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_13:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_14:__v[0-9]+]]: i32 = [[V_13]] + (1 as i32);
// CHECK-NEXT:         i = [[V_14]];
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
