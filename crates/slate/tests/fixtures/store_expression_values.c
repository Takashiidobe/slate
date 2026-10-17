#include <stdio.h>

struct Node {
  int value;
  struct Node *next;
};

static int calls;
static int slots[6];

static int idx(void) {
  calls += 1;
  return calls % 6;
}

static int *slot(void) {
  calls += 10;
  return &slots[2];
}

static int produce(void) {
  calls += 100;
  return 7;
}

static int twice(int value) { return value * 2; }

int main(void) {
  int a, b, c;
  a = b = c = produce();
  int d = (a += 3) * 2;
  int e = twice(b = 11);
  slots[idx()] += 5;
  slots[idx()] = slots[0] + 1;
  int f = (*slot() = 40) + 2;
  int g = (*slot() += 1);
  const char *text = "store";
  const char *cursor = text;
  char ch;
  int length = 0;
  while ((ch = *cursor++)) {
    length += ch == 'o';
  }
  int buffer[4] = {0};
  int *out = buffer;
  for (int i = 1; i <= 3; i++) {
    *out++ = i * i;
  }
  struct Node second = {2, 0};
  struct Node first = {1, &second};
  struct Node *node = &first;
  int total = 0;
  while ((node = node->next) != 0) {
    total += (node->value *= 5);
  }
  int flag = 0;
  int picked = (flag = a > 5) ? (b -= 1) : (c += 1);
  unsigned char narrow;
  int widened = (narrow = 300) + 1;
  printf("%d %d %d %d %d %d\n", a, b, c, d, e, calls);
  printf("%d %d %d %d %d %d %d\n", slots[0], slots[1], slots[2], slots[3], f, g, length);
  printf("%d %d %d %d %d %d\n", buffer[0], buffer[1], buffer[2], (int)(out - buffer), total, second.value);
  printf("%d %d %d %d\n", flag, picked, narrow, widened);
  return (a + b + c) & 0x7f;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Node {
// CHECK-NEXT:     value: i32,
// CHECK-NEXT:     next: *mut Node,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut calls: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut slots: __SlateAlign16<[i32; 6]> = __SlateAlign16(unsafe {
// CHECK-NEXT:     std::mem::zeroed()
// CHECK-NEXT: });
// CHECK-NEXT: fn idx() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         calls = (unsafe { calls }) + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     (unsafe { calls }) % 6
// CHECK-NEXT: }
// CHECK-NEXT: fn slot() -> *mut i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         calls = (unsafe { calls }) + 10;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { unsafe { std::ptr::addr_of_mut!(slots.0) as *mut i32 }.offset(2 as isize) }
// CHECK-NEXT: }
// CHECK-NEXT: fn produce() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         calls = (unsafe { calls }) + 100;
// CHECK-NEXT:     }
// CHECK-NEXT:     7
// CHECK-NEXT: }
// CHECK-NEXT: fn twice(value: i32) -> i32 {
// CHECK-NEXT:     value * 2
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: i32 = 0;
// CHECK-NEXT:     let mut b: i32 = 0;
// CHECK-NEXT:     let mut c: i32 = 0;
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: i32 = produce();
// CHECK-NEXT:     c = [[V_0]];
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: i32 = [[V_0]];
// CHECK-NEXT:     b = [[V_1]];
// CHECK-NEXT:     a = [[V_1]];
// CHECK-NEXT:     let mut d: i32 = 0;
// CHECK-NEXT:     let [[V_2:__v[0-9]+]]: i32 = a + 3;
// CHECK-NEXT:     a = [[V_2]];
// CHECK-NEXT:     d = [[V_2]] * 2;
// CHECK-NEXT:     let mut e: i32 = 0;
// CHECK-NEXT:     b = 11;
// CHECK-NEXT:     e = twice(11 as i32);
// CHECK-NEXT:     let [[V_3:__v[0-9]+]]: *mut i32 = unsafe {
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(slots.0) as *mut i32 }.offset(idx() as isize)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_3]] = (unsafe { *[[V_3]] }) + 5;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slots.0[idx() as usize] = (unsafe { slots.0[0] }) + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     let f: i32 = 40 + 2;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *slot() = 40;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut g: i32 = 0;
// CHECK-NEXT:     let [[V_4:__v[0-9]+]]: *mut i32 = slot();
// CHECK-NEXT:     let [[V_5:__v[0-9]+]]: i32 = (unsafe { *[[V_4]] }) + 1;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_4]] = [[V_5]];
// CHECK-NEXT:     }
// CHECK-NEXT:     g = [[V_5]];
// CHECK-NEXT:     let text: *const i8 = c"store".as_ptr();
// CHECK-NEXT:     let mut cursor: *const i8 = text;
// CHECK-NEXT:     let mut ch: i8 = 0;
// CHECK-NEXT:     let mut length: i32 = 0;
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let [[V_6:__v[0-9]+]]: *const i8 = cursor;
// CHECK-NEXT:         cursor = unsafe { [[V_6]].offset(1 as isize) };
// CHECK-NEXT:         let [[V_7:__v[0-9]+]]: i8 = unsafe { *[[V_6]] };
// CHECK-NEXT:         ch = [[V_7]];
// CHECK-NEXT:         if !([[V_7]] != 0) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         length += ((ch as i32) == 111) as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut buffer: __SlateAlign16<[i32; 4]> = __SlateAlign16({
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: [i32; 4] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]][0] = 0 as i32;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     });
// CHECK-NEXT:     let mut out: *mut i32 = buffer.0.as_mut_ptr() as *mut i32;
// CHECK-NEXT:     let mut i: i32 = 1;
// CHECK-NEXT:     while i <= 3 {
// CHECK-NEXT:         let [[V_8:__v[0-9]+]]: *mut i32 = out;
// CHECK-NEXT:         out = unsafe { [[V_8]].offset(1 as isize) };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_8]] = i * i;
// CHECK-NEXT:         }
// CHECK-NEXT:         i += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut second: Node = {
// CHECK-NEXT:         let mut [[T_1:__t[0-9]+]]: Node = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]].value = 2 as i32;
// CHECK-NEXT:         [[T_1]].next = std::ptr::null_mut::<Node>();
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut first: Node = {
// CHECK-NEXT:         let mut [[T_2:__t[0-9]+]]: Node = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_2]].value = 1 as i32;
// CHECK-NEXT:         [[T_2]].next = std::ptr::addr_of_mut!(second);
// CHECK-NEXT:         [[T_2]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut node: *mut Node = std::ptr::addr_of_mut!(first);
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let [[V_9:__v[0-9]+]]: *mut Node = unsafe { (*node).next };
// CHECK-NEXT:         node = [[V_9]];
// CHECK-NEXT:         if !([[V_9]] != std::ptr::null_mut::<Node>()) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         let [[V_10:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:         let [[V_11:__v[0-9]+]]: i32 = (unsafe { (*node).value }) * 5;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*node).value = [[V_11]];
// CHECK-NEXT:         }
// CHECK-NEXT:         total = [[V_10]] + [[V_11]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut flag: i32 = 0;
// CHECK-NEXT:     let mut picked: i32 = 0;
// CHECK-NEXT:     let [[V_12:__v[0-9]+]]: i32 = (a > 5) as i32;
// CHECK-NEXT:     flag = [[V_12]];
// CHECK-NEXT:     let mut [[V_13:__v[0-9]+]]: i32;
// CHECK-NEXT:     if [[V_12]] != 0 {
// CHECK-NEXT:         let [[V_14:__v[0-9]+]]: i32 = b - 1;
// CHECK-NEXT:         b = [[V_14]];
// CHECK-NEXT:         [[V_13]] = [[V_14]];
// CHECK-NEXT:     } else {
// CHECK-NEXT:         let [[V_15:__v[0-9]+]]: i32 = c + 1;
// CHECK-NEXT:         c = [[V_15]];
// CHECK-NEXT:         [[V_13]] = [[V_15]];
// CHECK-NEXT:     }
// CHECK-NEXT:     picked = [[V_13]];
// CHECK-NEXT:     let narrow: u8 = ((300 as i32) as i8) as u8;
// CHECK-NEXT:     let widened: i32 = (((((300 as i32) as i8) as u8) as u32) as i32) + 1;
// CHECK-NEXT:     println!("{} {} {} {} {} {}", a, b, c, d, e, unsafe { calls });
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {} {} {} {}", unsafe { slots.0[0] }, unsafe { slots.0[1] }, unsafe {
// CHECK-NEXT:         slots.0[2] }, unsafe { slots.0[3] }, f, g, length
// CHECK-NEXT:     );
// CHECK-NEXT:     let __slate_printf_arg3 = ((unsafe {
// CHECK-NEXT:         out.offset_from(buffer.0.as_mut_ptr() as *mut i32)
// CHECK-NEXT:     }) as i64) as i32;
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {} {} {}", buffer.0[0], buffer.0[1], buffer.0[2], __slate_printf_arg3,
// CHECK-NEXT:         total, second.value
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{} {} {} {}", flag, picked, (narrow as u32) as i32, widened);
// CHECK-NEXT:     a + b + c & 127
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
