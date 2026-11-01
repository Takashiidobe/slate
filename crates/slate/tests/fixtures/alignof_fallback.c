#include <stdalign.h>
#include <stdio.h>

struct Pair {
  int  left;
  char right;
};

struct Packed {
  char tag;
  int  value;
} __attribute__((packed));

struct Raised {
  int value;
} __attribute__((aligned(32)));

struct Holder {
  long double wide;
  char        tag;
};

union Either {
  char   small;
  double large;
};

typedef int aligned_int __attribute__((aligned(16)));

static char table[_Alignof(struct Pair)];
static const unsigned long global_align = _Alignof(struct Pair) * 2;

static int f(void) { return 7; }

static int classify(unsigned long align) {
  switch (align) {
  case _Alignof(char):
    return 1;
  case _Alignof(short):
    return 2;
  default:
    return 0;
  }
}

static void report_folded(void) {
  int declared __attribute__((aligned(64))) = 0;
  struct Packed packed = {1, 2};
  printf("%d\n", (int)__alignof__(void));
  printf("%d\n", (int)(__alignof__(f) > 0));
  printf("%d\n", (int)_Alignof(long double));
  printf("%d\n", (int)_Alignof(_Atomic int));
  printf("%d\n", (int)_Alignof(aligned_int));
  printf("%d\n", (int)__alignof__(long long));
  printf("%d\n", (int)__alignof__(double));
  printf("%d\n", (int)_Alignof(int[]));
  printf("%d\n", (int)_Alignof(struct Holder));
  printf("%d\n", (int)__alignof__(declared));
  printf("%d\n", (int)__alignof__(packed.value));
  printf("%d\n", declared + packed.value);
}

static void report_kept(void) {
  int   values[5] = {0};
  char *text = "abc";
  struct Pair pair = {1, 'x'};
  printf("%d\n", (int)_Alignof(char));
  printf("%d\n", (int)_Alignof(short));
  printf("%d\n", (int)_Alignof(int));
  printf("%d\n", (int)_Alignof(long));
  printf("%d\n", (int)_Alignof(long long));
  printf("%d\n", (int)_Alignof(float));
  printf("%d\n", (int)_Alignof(double));
  printf("%d\n", (int)_Alignof(void *));
  printf("%d\n", (int)_Alignof(int[4]));
  printf("%d\n", (int)alignof(struct Pair));
  printf("%d\n", (int)_Alignof(struct Packed));
  printf("%d\n", (int)_Alignof(struct Raised));
  printf("%d\n", (int)_Alignof(union Either));
  printf("%d\n", (int)__alignof__(struct Pair));
  printf("%d\n", (int)__alignof__(values));
  printf("%d\n", (int)__alignof__(text));
  printf("%d\n", (int)__alignof__(pair.left));
  printf("%d\n", values[0] + text[0] + pair.left);
}

int main(void) {
  report_folded();
  report_kept();
  printf("%d\n", (int)sizeof(table));
  printf("%lu\n", global_align);
  printf("%d\n", classify(_Alignof(char)));
  printf("%d\n", classify(_Alignof(short)));
  printf("%d\n", classify(_Alignof(struct Raised)));
  return f() - 7;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Pair {
// CHECK-NEXT:     left: i32,
// CHECK-NEXT:     right: i8,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Packed {
// CHECK-NEXT:     tag: i8,
// CHECK-NEXT:     value: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(32))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Raised {
// CHECK-NEXT:     value: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union Either {
// CHECK-NEXT:     small: i8,
// CHECK-NEXT:     large: f64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: #[repr(C, align(64))]
// CHECK-NEXT: struct __SlateAlign64<T>(T);
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: static mut table: [i8; 4] = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut global_align: u64 = (std::mem::align_of::<Pair>() as u64).wrapping_mul(2u64);
// CHECK-NEXT: fn f() -> i32 {
// CHECK-NEXT:     7
// CHECK-NEXT: }
// CHECK-NEXT: fn classify(align: u64) -> i32 {
// CHECK-NEXT:     match align {
// CHECK-NEXT:         1 => {
// CHECK-NEXT:             return 1;
// CHECK-NEXT:         }
// CHECK-NEXT:         2 => {
// CHECK-NEXT:             return 2;
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {
// CHECK-NEXT:             return 0;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn report_folded() {
// CHECK-NEXT:     let declared: __SlateAlign64<i32> = __SlateAlign64(0i32);
// CHECK-NEXT:     let packed: Packed = Packed { tag: 1, value: 2 };
// CHECK-NEXT:     println!("{}", 1i32);
// CHECK-NEXT:     println!("{}", true as i32);
// CHECK-NEXT:     println!("{}", 16i32);
// CHECK-NEXT:     println!("{}", 4i32);
// CHECK-NEXT:     println!("{}", 16i32);
// CHECK-NEXT:     println!("{}", std::mem::align_of::< i64 > () as i32);
// CHECK-NEXT:     println!("{}", std::mem::align_of::< f64 > () as i32);
// CHECK-NEXT:     println!("{}", 4i32);
// CHECK-NEXT:     println!("{}", 16i32);
// CHECK-NEXT:     println!("{}", 64i32);
// CHECK-NEXT:     println!("{}", 1i32);
// CHECK-NEXT:     println!("{}", declared.0 + packed.value);
// CHECK-NEXT: }
// CHECK-NEXT: fn report_kept() {
// CHECK-NEXT:     let values: __SlateAlign16<[i32; 5]> = __SlateAlign16({
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: [i32; 5] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]][0] = 0i32;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     });
// CHECK-NEXT:     let text: *mut i8 = c"abc".as_ptr() as *mut i8;
// CHECK-NEXT:     let pair: Pair = {
// CHECK-NEXT:         let mut [[T_1:__t[0-9]+]]: Pair = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]].left = 1i32;
// CHECK-NEXT:         [[T_1]].right = 120i8;
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     println!("{}", std::mem::align_of::< i8 > () as i32);
// CHECK-NEXT:     println!("{}", std::mem::align_of::< i16 > () as i32);
// CHECK-NEXT:     println!("{}", std::mem::align_of::< i32 > () as i32);
// CHECK-NEXT:     println!("{}", std::mem::align_of::< i64 > () as i32);
// CHECK-NEXT:     println!("{}", std::mem::align_of::< i64 > () as i32);
// CHECK-NEXT:     println!("{}", std::mem::align_of::< f32 > () as i32);
// CHECK-NEXT:     println!("{}", std::mem::align_of::< f64 > () as i32);
// CHECK-NEXT:     println!("{}", std::mem::align_of::<* mut () > () as i32);
// CHECK-NEXT:     println!("{}", std::mem::align_of::< [i32; 4] > () as i32);
// CHECK-NEXT:     println!("{}", std::mem::align_of::< Pair > () as i32);
// CHECK-NEXT:     println!("{}", std::mem::align_of::< Packed > () as i32);
// CHECK-NEXT:     println!("{}", std::mem::align_of::< Raised > () as i32);
// CHECK-NEXT:     println!("{}", std::mem::align_of::< Either > () as i32);
// CHECK-NEXT:     println!("{}", std::mem::align_of::< Pair > () as i32);
// CHECK-NEXT:     println!("{}", std::mem::align_of::< [i32; 5] > () as i32);
// CHECK-NEXT:     println!("{}", std::mem::align_of::<* mut i8 > () as i32);
// CHECK-NEXT:     println!("{}", std::mem::align_of::< i32 > () as i32);
// CHECK-NEXT:     println!("{}", values.0[0] + ((unsafe { * text.add(0) }) as i32) + pair.left);
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     report_folded();
// CHECK-NEXT:     report_kept();
// CHECK-NEXT:     println!("{}", std::mem::size_of::< [i8; 4] > () as i32);
// CHECK-NEXT:     unsafe { slate_support::printf(c"%lu\n".as_ptr(), global_align) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!("{}", classify(std::mem::align_of::< i8 > () as u64));
// CHECK-NEXT:     println!("{}", classify(std::mem::align_of::< i16 > () as u64));
// CHECK-NEXT:     println!("{}", classify(std::mem::align_of::< Raised > () as u64));
// CHECK-NEXT:     f() - 7
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
