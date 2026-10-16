/* PR tree-optimization/36038 */

long long  list[10];
long long  expect[10] = {0, 1, 2, 3, 4, 4, 5, 6, 7, 9};
long long *stack_base;
int        indices[10];
int       *markstack_ptr;

void doit(void) {
  long long *src;
  long long *dst;
  long long *sp   = stack_base + 5;
  int        diff = 2;
  int        shift;
  int        count;

  shift = diff - (markstack_ptr[-1] - markstack_ptr[-2]);
  count = (sp - stack_base) - markstack_ptr[-1] + 2;
  src   = sp;
  dst   = (sp += shift);
  while (--count)
    *dst-- = *src--;
}

int main() {
  int i;
  for (i = 0; i < 10; i++)
    list[i] = i;

  markstack_ptr     = indices + 9;
  markstack_ptr[-1] = 2;
  markstack_ptr[-2] = 1;

  stack_base = list + 2;
  doit();
  if (__builtin_memcmp(expect, list, sizeof(list)))
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/36038
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "memcmp"]
// CHECK-NEXT:     fn __builtin_memcmp([[V_0:__v[0-9]+]]: *const (), [[V_1:__v[0-9]+]]: *const (), [[V_2:__v[0-9]+]]: u64) -> i32;
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut list: __SlateAlign16<[i64; 10]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: static mut expect: __SlateAlign16<[i64; 10]> = __SlateAlign16([
// CHECK-NEXT:     (0 as i32) as i64,
// CHECK-NEXT:     (1 as i32) as i64,
// CHECK-NEXT:     (2 as i32) as i64,
// CHECK-NEXT:     (3 as i32) as i64,
// CHECK-NEXT:     (4 as i32) as i64,
// CHECK-NEXT:     (4 as i32) as i64,
// CHECK-NEXT:     (5 as i32) as i64,
// CHECK-NEXT:     (6 as i32) as i64,
// CHECK-NEXT:     (7 as i32) as i64,
// CHECK-NEXT:     (9 as i32) as i64,
// CHECK-NEXT: ]);
// CHECK-EMPTY:
// CHECK-NEXT: static mut stack_base: *mut i64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut indices: __SlateAlign16<[i32; 10]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: static mut markstack_ptr: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn doit() {
// CHECK-NEXT:     let mut src: *mut i64 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut dst: *mut i64 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut sp: *mut i64 = unsafe { unsafe { stack_base }.offset((5 as i32) as isize) };
// CHECK-NEXT:     let mut diff: i32 = 2 as i32;
// CHECK-NEXT:     let mut shift: i32 = diff
// CHECK-NEXT:         - ((unsafe { *unsafe { unsafe { markstack_ptr }.offset(-(1 as i32) as isize) } })
// CHECK-NEXT:             - unsafe { *unsafe { unsafe { markstack_ptr }.offset(-(2 as i32) as isize) } });
// CHECK-NEXT:     let mut count: i32 = (((unsafe { sp.offset_from((unsafe { stack_base }) as *mut i64) }) as i64)
// CHECK-NEXT:         - ((unsafe { *unsafe { unsafe { markstack_ptr }.offset(-(1 as i32) as isize) } }) as i64)
// CHECK-NEXT:         + ((2 as i32) as i64)) as i32;
// CHECK-NEXT:     src = sp;
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: *mut i64 = sp;
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: *mut i64 = unsafe { [[V_3]].offset(shift as isize) };
// CHECK-NEXT:     sp = [[V_4]];
// CHECK-NEXT:     dst = [[V_4]];
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = count;
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: i32 = [[V_5]] - (1 as i32);
// CHECK-NEXT:         count = [[V_6]];
// CHECK-NEXT:         if !([[V_6]] != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_7:__v[0-9]+]]: *mut i64 = src;
// CHECK-NEXT:         let mut [[V_8:__v[0-9]+]]: *mut i64 = unsafe { [[V_7]].offset(-((1 as i32) as isize)) };
// CHECK-NEXT:         src = [[V_8]];
// CHECK-NEXT:         let mut [[V_9:__v[0-9]+]]: *mut i64 = dst;
// CHECK-NEXT:         let mut [[V_10:__v[0-9]+]]: *mut i64 = unsafe { [[V_9]].offset(-((1 as i32) as isize)) };
// CHECK-NEXT:         dst = [[V_10]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_9]] = unsafe { *[[V_7]] };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < (10 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { unsafe { std::ptr::addr_of_mut!(list.0) as *mut i64 }.offset(i as isize) } =
// CHECK-NEXT:                 i as i64;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_11:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_12:__v[0-9]+]]: i32 = [[V_11]] + (1 as i32);
// CHECK-NEXT:         i = [[V_12]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         markstack_ptr = unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(indices.0) as *mut i32 }.offset((9 as i32) as isize)
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { unsafe { markstack_ptr }.offset(-(1 as i32) as isize) } = 2 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { unsafe { markstack_ptr }.offset(-(2 as i32) as isize) } = 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         stack_base = unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(list.0) as *mut i64 }.offset((2 as i32) as isize)
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     doit();
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         __builtin_memcmp(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(expect.0) as *mut i64 }) as *const (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(list.0) as *mut i64 }) as *const (),
// CHECK-NEXT:             80 as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
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
