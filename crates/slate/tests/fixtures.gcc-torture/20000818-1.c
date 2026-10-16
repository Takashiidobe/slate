/* Copyright (C) 2000  Free Software Foundation.

   by Manfred Hollstein <manfredh@redhat.com>  */

void abort(void);
void exit(int);

void *temporary_obstack;

static int  input(void);
static int  ISALNUM(int ch);
static void obstack_1grow(void **ptr, int ch);

int yylex(void);
int main(void);

int main(void) {
  int ch = yylex();

  exit(0);
}

int yylex(void) {
  int ch;

#ifndef WORK_AROUND
  for (;;) {
    ch = input();
    if (ISALNUM(ch))
      obstack_1grow(&temporary_obstack, ch);
    else if (ch != '_')
      break;
  }
#else
  do {
    ch = input();
    if (ISALNUM(ch))
      obstack_1grow(&temporary_obstack, ch);
  } while (ch == '_');
#endif

  return ch;
}

static int input(void) { return 0; }

static int ISALNUM(int ch) {
  return ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') ||
          (ch >= '0' && ch <= '0'));
}

static void obstack_1grow(void **ptr, int ch) {}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Copyright (C) 2000  Free Software Foundation.
// CHECK-NEXT: //!
// CHECK-NEXT: //! by Manfred Hollstein <manfredh@redhat.com>
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut temporary_obstack: *mut () = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     yylex();
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn yylex() -> i32 {
// CHECK-NEXT:     let mut ch: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         ch = input();
// CHECK-NEXT:         if ISALNUM(ch) != (0 as i32) {
// CHECK-NEXT:             obstack_1grow(unsafe { std::ptr::addr_of_mut!(temporary_obstack) }, ch);
// CHECK-NEXT:         } else {
// CHECK-NEXT:             if ch != (95 as i32) {
// CHECK-NEXT:                 break '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     ch
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn input() -> i32 {
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ISALNUM(mut ch: i32) -> i32 {
// CHECK-NEXT:     (ch >= (65 as i32) && ch <= (90 as i32)
// CHECK-NEXT:         || ch >= (97 as i32) && ch <= (122 as i32)
// CHECK-NEXT:         || ch >= (48 as i32) && ch <= (48 as i32)) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn obstack_1grow(mut ptr: *mut *mut (), mut ch: i32) {}
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
