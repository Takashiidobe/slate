#include <stddef.h>
#include <stdio.h>

static unsigned char bytes[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

static unsigned char at(const void *base, long index) {
  return *(const unsigned char *)(base + index);
}

int main(void) {
  void       *start  = bytes;
  void       *end    = bytes + sizeof bytes;
  const void *middle = start + 5;
  void       *cursor = end;
  cursor             = cursor - 3;
  cursor -= 2;
  cursor += 1;
  cursor++;
  --cursor;

  ptrdiff_t span     = end - start;
  ptrdiff_t back     = start - end;
  ptrdiff_t offset   = middle - (const void *)start;
  ptrdiff_t position = cursor - start;

  printf("%td %td %td %td\n", span, back, offset, position);
  printf("%u %u %u\n", at(start, 7), at(middle, -2), *(unsigned char *)cursor);
  return (int)(cursor - middle);
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut bytes: __SlateAlign16<[u8; 16]> = __SlateAlign16([
// CHECK-NEXT:     0u8, 1u8, 2u8, 3u8, 4u8, 5u8, 6u8, 7u8, 8u8, 9u8, 10u8, 11u8, 12u8, 13u8, 14u8, 15u8,
// CHECK-NEXT: ]);
// CHECK-NEXT: fn at(base: *const (), index: i64) -> u8 {
// CHECK-NEXT:     unsafe { *(((base as *const u8).offset(index as isize) as *const ()) as *const u8) }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let start: *mut () = (unsafe { std::ptr::addr_of_mut!(bytes.0) as *mut u8 })
// CHECK-NEXT:         as *mut ();
// CHECK-NEXT:     let end: *mut () = (unsafe { (std::ptr::addr_of_mut!(bytes.0) as *mut u8).add(16) })
// CHECK-NEXT:         as *mut ();
// CHECK-NEXT:     let middle: *const () = ((unsafe { (start as *const u8).add(5) }) as *mut ())
// CHECK-NEXT:         as *const ();
// CHECK-NEXT:     let mut cursor: *mut () = end;
// CHECK-NEXT:     cursor = (unsafe { (cursor as *const u8).sub(3) }) as *mut ();
// CHECK-NEXT:     cursor = (unsafe { (cursor as *const u8).sub(2) }) as *mut ();
// CHECK-NEXT:     cursor = (unsafe { (cursor as *const u8).add(1) }) as *mut ();
// CHECK-NEXT:     cursor = (unsafe { (cursor as *const u8).add(1) }) as *mut ();
// CHECK-NEXT:     cursor = (unsafe { (cursor as *const u8).sub(1) }) as *mut ();
// CHECK-NEXT:     let span: i64 = (unsafe { (end as *const u8).offset_from(start as *const u8) })
// CHECK-NEXT:         as i64;
// CHECK-NEXT:     let back: i64 = (unsafe { (start as *const u8).offset_from(end as *const u8) })
// CHECK-NEXT:         as i64;
// CHECK-NEXT:     let offset: i64 = (unsafe { (middle as *const u8).offset_from(start as *const u8) })
// CHECK-NEXT:         as i64;
// CHECK-NEXT:     let position: i64 = (unsafe {
// CHECK-NEXT:         (cursor as *const u8).offset_from(start as *const u8)
// CHECK-NEXT:     }) as i64;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%td %td %td %td\n".as_ptr(), span, back, offset, position) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%u %u %u\n".as_ptr(),
// CHECK-NEXT:             (at(start as *const (), 7i64) as u32) as i32,
// CHECK-NEXT:             (at(middle, -2i64) as u32) as i32,
// CHECK-NEXT:             (*(cursor as *mut u8) as u32) as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     ((unsafe { (cursor as *const u8).offset_from(middle as *const u8) }) as i64) as i32
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
