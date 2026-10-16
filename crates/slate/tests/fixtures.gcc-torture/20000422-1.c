void abort(void);
void exit(int);

int ops[13] = {11, 12, 46, 3, 2, 2, 3, 2, 1, 3, 2, 1, 2};

int correct[13] = {46, 12, 11, 3, 3, 3, 2, 2, 2, 2, 2, 1, 1};

int num = 13;

int main() {
  int i;

  for (i = 0; i < num; i++) {
    int j;

    for (j = num - 1; j > i; j--) {
      if (ops[j - 1] < ops[j]) {
        int op     = ops[j];
        ops[j]     = ops[j - 1];
        ops[j - 1] = op;
      }
    }
  }

  for (i = 0; i < num; i++)
    if (ops[i] != correct[i])
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
// CHECK-NEXT: static mut ops: __SlateAlign16<[i32; 13]> = __SlateAlign16([
// CHECK-NEXT:     11 as i32,
// CHECK-NEXT:     12 as i32,
// CHECK-NEXT:     46 as i32,
// CHECK-NEXT:     3 as i32,
// CHECK-NEXT:     2 as i32,
// CHECK-NEXT:     2 as i32,
// CHECK-NEXT:     3 as i32,
// CHECK-NEXT:     2 as i32,
// CHECK-NEXT:     1 as i32,
// CHECK-NEXT:     3 as i32,
// CHECK-NEXT:     2 as i32,
// CHECK-NEXT:     1 as i32,
// CHECK-NEXT:     2 as i32,
// CHECK-NEXT: ]);
// CHECK-NEXT: static mut correct: __SlateAlign16<[i32; 13]> = __SlateAlign16([
// CHECK-NEXT:     46 as i32,
// CHECK-NEXT:     12 as i32,
// CHECK-NEXT:     11 as i32,
// CHECK-NEXT:     3 as i32,
// CHECK-NEXT:     3 as i32,
// CHECK-NEXT:     3 as i32,
// CHECK-NEXT:     2 as i32,
// CHECK-NEXT:     2 as i32,
// CHECK-NEXT:     2 as i32,
// CHECK-NEXT:     2 as i32,
// CHECK-NEXT:     2 as i32,
// CHECK-NEXT:     1 as i32,
// CHECK-NEXT:     1 as i32,
// CHECK-NEXT: ]);
// CHECK-NEXT: static mut num: i32 = 13 as i32;
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < unsafe { num } {
// CHECK-NEXT:         let mut j: i32 = (unsafe { num }) - (1 as i32);
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while j > i {
// CHECK-NEXT:             if (unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(ops.0) as *mut i32 }
// CHECK-NEXT:                         .offset((j - (1 as i32)) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:             })
// CHECK-NEXT:                 < unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         unsafe { std::ptr::addr_of_mut!(ops.0) as *mut i32 }
// CHECK-NEXT:                             .offset(j as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:             {
// CHECK-NEXT:                 let mut op: i32 = unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         unsafe { std::ptr::addr_of_mut!(ops.0) as *mut i32 }
// CHECK-NEXT:                             .offset(j as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                 };
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         unsafe { std::ptr::addr_of_mut!(ops.0) as *mut i32 }
// CHECK-NEXT:                             .offset(j as isize)
// CHECK-NEXT:                     } = unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of_mut!(ops.0) as *mut i32 }
// CHECK-NEXT:                                 .offset((j - (1 as i32)) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         unsafe { std::ptr::addr_of_mut!(ops.0) as *mut i32 }
// CHECK-NEXT:                             .offset((j - (1 as i32)) as isize)
// CHECK-NEXT:                     } = op;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i32 = j;
// CHECK-NEXT:             let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] - (1 as i32);
// CHECK-NEXT:             j = [[V_2]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] + (1 as i32);
// CHECK-NEXT:         i = [[V_4]];
// CHECK-NEXT:     }
// CHECK-NEXT:     i = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while i < unsafe { num } {
// CHECK-NEXT:         if (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(ops.0) as *mut i32 }.offset(i as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         })
// CHECK-NEXT:             != unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(correct.0) as *mut i32 }
// CHECK-NEXT:                         .offset(i as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: i32 = [[V_5]] + (1 as i32);
// CHECK-NEXT:         i = [[V_6]];
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
