#include <stdio.h>

enum Err { ERR_NONE = 0, ERR_BAD = 5 };

static int first(void) {
  struct Rec {
    const char *text;
    enum Err    code;
  };
  struct Rec items[] = {{"a", ERR_NONE}, {"bb", ERR_BAD}};
  int        total   = 0;
  for (unsigned i = 0; i < sizeof(items) / sizeof(struct Rec); i++)
    total += (int)items[i].code + (int)items[i].text[0];
  return total;
}

static int second(void) {
  struct Rec {
    unsigned long n;
    const char   *text;
    enum Err      code;
  };
  struct Rec items[] = {{5, "x", ERR_NONE}, {6, "y", ERR_BAD}};
  int        total   = 0;
  for (unsigned i = 0; i < sizeof(items) / sizeof(struct Rec); i++)
    total += (int)items[i].n + (int)items[i].code + (int)items[i].text[0];
  return total;
}

int main(void) {
  printf("%d %d\n", first(), second());
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Rec__6 {
// CHECK-NEXT:     text: *const i8,
// CHECK-NEXT:     code: u32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Rec__18 {
// CHECK-NEXT:     n: u64,
// CHECK-NEXT:     text: *const i8,
// CHECK-NEXT:     code: u32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn first() -> i32 {
// CHECK-NEXT:     let mut items: __SlateAlign16<[Rec__6; 2]> = __SlateAlign16([
// CHECK-NEXT:         {
// CHECK-NEXT:             let mut [[T_0:__t[0-9]+]]: Rec__6 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_0]].text = c"a".as_ptr();
// CHECK-NEXT:             [[T_0]].code = 0u32;
// CHECK-NEXT:             [[T_0]]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:             let mut [[T_1:__t[0-9]+]]: Rec__6 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_1]].text = c"bb".as_ptr();
// CHECK-NEXT:             [[T_1]].code = 5u32;
// CHECK-NEXT:             [[T_1]]
// CHECK-NEXT:         },
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     let mut i: u32 = 0;
// CHECK-NEXT:     while (i as u64) < 32 / 16 {
// CHECK-NEXT:         total
// CHECK-NEXT:             += ((items.0[i as usize].code as u32) as i32)
// CHECK-NEXT:                 + ((unsafe { *items.0[i as usize].text.add(0) }) as i32);
// CHECK-NEXT:         i = i.wrapping_add(1);
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn second() -> i32 {
// CHECK-NEXT:     let mut items: __SlateAlign16<[Rec__18; 2]> = __SlateAlign16([
// CHECK-NEXT:         {
// CHECK-NEXT:             let mut [[T_0]]: Rec__18 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_0]].n = 5u64;
// CHECK-NEXT:             [[T_0]].text = c"x".as_ptr();
// CHECK-NEXT:             [[T_0]].code = 0u32;
// CHECK-NEXT:             [[T_0]]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:             let mut [[T_1]]: Rec__18 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_1]].n = 6u64;
// CHECK-NEXT:             [[T_1]].text = c"y".as_ptr();
// CHECK-NEXT:             [[T_1]].code = 5u32;
// CHECK-NEXT:             [[T_1]]
// CHECK-NEXT:         },
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     let mut i: u32 = 0;
// CHECK-NEXT:     while (i as u64) < 48 / 24 {
// CHECK-NEXT:         total
// CHECK-NEXT:             += ((items.0[i as usize].n as u32) as i32)
// CHECK-NEXT:                 + ((items.0[i as usize].code as u32) as i32)
// CHECK-NEXT:                 + ((unsafe { *items.0[i as usize].text.add(0) }) as i32);
// CHECK-NEXT:         i = i.wrapping_add(1);
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{} {}", first(), second());
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
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
