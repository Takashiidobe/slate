#include <stddef.h>
#include <stdio.h>

#pragma pack(3)
struct IgnoredBadAlign {
  char a;
  int  b;
};
#pragma pack()

#pragma pack(push, lbl, 1)
struct NamedPushed {
  char a;
  int  b;
};
#pragma pack(pop, lbl)

struct AfterNamedPop {
  char a;
  int  b;
};

#pragma pack(pop)
struct AfterExtraPop {
  char a;
  int  b;
};

#pragma pack(push, 1)
struct __attribute__((aligned(16))) PackedButAligned {
  char a;
  int  b;
  char c;
};
#pragma pack(pop)

int main(void) {
  printf("%d %d %d %d %d %d %d\n", (int)sizeof(struct IgnoredBadAlign),
         (int)sizeof(struct NamedPushed), (int)sizeof(struct AfterNamedPop),
         (int)sizeof(struct AfterExtraPop),
         (int)sizeof(struct PackedButAligned),
         (int)_Alignof(struct PackedButAligned),
         (int)offsetof(struct PackedButAligned, b));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct IgnoredBadAlign {
// CHECK-NEXT:     a: i8,
// CHECK-NEXT:     b: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct NamedPushed {
// CHECK-NEXT:     a: i8,
// CHECK-NEXT:     b: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct AfterNamedPop {
// CHECK-NEXT:     a: i8,
// CHECK-NEXT:     b: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct AfterExtraPop {
// CHECK-NEXT:     a: i8,
// CHECK-NEXT:     b: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct PackedButAligned {
// CHECK-NEXT:     a: std::mem::MaybeUninit<[u8; 1]>,
// CHECK-NEXT:     b: std::mem::MaybeUninit<[u8; 4]>,
// CHECK-NEXT:     c: std::mem::MaybeUninit<[u8; 1]>,
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {} {} {} {}", std::mem::size_of::< IgnoredBadAlign > () as i32,
// CHECK-NEXT:         std::mem::size_of::< NamedPushed > () as i32, std::mem::size_of::< AfterNamedPop
// CHECK-NEXT:         > () as i32, std::mem::size_of::< AfterExtraPop > () as i32, std::mem::size_of::<
// CHECK-NEXT:         PackedButAligned > () as i32, std::mem::align_of::< PackedButAligned > () as i32,
// CHECK-NEXT:         1i32
// CHECK-NEXT:     );
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
