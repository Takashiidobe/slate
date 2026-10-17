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
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut bytes: __SlateAlign16<[u8; 16]> = __SlateAlign16([
// CHECK-NEXT:     ((0 as i32) as i8) as u8,
// CHECK-NEXT:     ((1 as i32) as i8) as u8,
// CHECK-NEXT:     ((2 as i32) as i8) as u8,
// CHECK-NEXT:     ((3 as i32) as i8) as u8,
// CHECK-NEXT:     ((4 as i32) as i8) as u8,
// CHECK-NEXT:     ((5 as i32) as i8) as u8,
// CHECK-NEXT:     ((6 as i32) as i8) as u8,
// CHECK-NEXT:     ((7 as i32) as i8) as u8,
// CHECK-NEXT:     ((8 as i32) as i8) as u8,
// CHECK-NEXT:     ((9 as i32) as i8) as u8,
// CHECK-NEXT:     ((10 as i32) as i8) as u8,
// CHECK-NEXT:     ((11 as i32) as i8) as u8,
// CHECK-NEXT:     ((12 as i32) as i8) as u8,
// CHECK-NEXT:     ((13 as i32) as i8) as u8,
// CHECK-NEXT:     ((14 as i32) as i8) as u8,
// CHECK-NEXT:     ((15 as i32) as i8) as u8,
// CHECK-NEXT: ]);
// CHECK-NEXT: fn at(base: *const (), index: i64) -> u8 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(((unsafe { (base as *const u8).offset(index as isize) }) as *const ())
// CHECK-NEXT:             as *const u8)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let start: *mut () = (unsafe { std::ptr::addr_of_mut!(bytes.0) as *mut u8 })
// CHECK-NEXT:         as *mut ();
// CHECK-NEXT:     let end: *mut () = (unsafe {
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(bytes.0) as *mut u8 }.offset(16 as isize)
// CHECK-NEXT:     }) as *mut ();
// CHECK-NEXT:     let middle: *const () = ((unsafe { (start as *const u8).offset(5 as isize) })
// CHECK-NEXT:         as *mut ()) as *const ();
// CHECK-NEXT:     let mut cursor: *mut () = end;
// CHECK-NEXT:     cursor = (unsafe { (cursor as *const u8).offset(-(3 as isize)) }) as *mut ();
// CHECK-NEXT:     cursor = (unsafe { (cursor as *const u8).offset(-(2 as isize)) }) as *mut ();
// CHECK-NEXT:     cursor = (unsafe { (cursor as *const u8).offset(1 as isize) }) as *mut ();
// CHECK-NEXT:     cursor = (unsafe { (cursor as *const u8).offset(1 as isize) }) as *mut ();
// CHECK-NEXT:     cursor = (unsafe { (cursor as *const u8).offset(-(1 as isize)) }) as *mut ();
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
// CHECK-NEXT:             (at(start as *const (), 7 as i64) as u32) as i32,
// CHECK-NEXT:             (at(middle, -2 as i64) as u32) as i32,
// CHECK-NEXT:             ((unsafe { *(cursor as *mut u8) }) as u32) as i32,
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
