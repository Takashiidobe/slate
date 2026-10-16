/* powerpc64-linux gcc miscompiled this due to rs6000.c:expand_block_move
   not setting mem aliasing info correctly for the code implementing the
   structure assignment.  */

struct termios {
  unsigned int  a;
  unsigned int  b;
  unsigned int  c;
  unsigned int  d;
  unsigned char pad[28];
};

struct tty_driver {
  unsigned char  pad1[38];
  struct termios t __attribute__((aligned(8)));
};

static struct termios    zero_t;
static struct tty_driver pty;

void ini(void) {
  pty.t   = zero_t;
  pty.t.a = 1;
  pty.t.b = 2;
  pty.t.c = 3;
  pty.t.d = 4;
}

int main(void) {
  extern void abort(void);

  ini();
  if (pty.t.a != 1 || pty.t.b != 2 || pty.t.c != 3 || pty.t.d != 4)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! powerpc64-linux gcc miscompiled this due to rs6000.c:expand_block_move
// CHECK-NEXT: //! not setting mem aliasing info correctly for the code implementing the
// CHECK-NEXT: //! structure assignment.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct termios {
// CHECK-NEXT:     a: u32,
// CHECK-NEXT:     b: u32,
// CHECK-NEXT:     c: u32,
// CHECK-NEXT:     d: u32,
// CHECK-NEXT:     pad: [u8; 28],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(8))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct tty_driver {
// CHECK-NEXT:     pad1: [u8; 38],
// CHECK-NEXT:     t: termios,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut zero_t: termios = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut pty: tty_driver = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn ini() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         pty.t = unsafe { zero_t };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         pty.t.a = (1 as i32) as u32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         pty.t.b = (2 as i32) as u32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         pty.t.c = (3 as i32) as u32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         pty.t.d = (4 as i32) as u32;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     ini();
// CHECK-NEXT:     if (unsafe { pty.t.a }) != ((1 as i32) as u32)
// CHECK-NEXT:         || (unsafe { pty.t.b }) != ((2 as i32) as u32)
// CHECK-NEXT:         || (unsafe { pty.t.c }) != ((3 as i32) as u32)
// CHECK-NEXT:         || (unsafe { pty.t.d }) != ((4 as i32) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
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
