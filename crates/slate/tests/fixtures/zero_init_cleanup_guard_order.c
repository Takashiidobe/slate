#include <stdio.h>

struct port { int locked; };

static void pause_rx(struct port *p) { printf("lock\n"); p->locked = 1; }
static void continue_rx(struct port *p) { printf("unlock\n"); p->locked = 0; }

typedef struct port *class_pause_t;
static inline void class_pause_destructor(class_pause_t *p) { if (*p) continue_rx(*p); }
static inline class_pause_t class_pause_constructor(struct port *p) { pause_rx(p); return p; }

struct dev { struct port *port; int nak; };

__attribute__((noinline)) int do_send(struct dev *d, unsigned char byte) {
  printf("do_send locked=%d\n", d->port->locked);
  return byte + d->port->locked;
}

int send(struct dev *d, unsigned char byte) {
  int retval;
  class_pause_t guard __attribute__((cleanup(class_pause_destructor), unused)) =
      class_pause_constructor(d->port);
  retval = do_send(d, byte);
  if (0)
    printf("%02x - %x\n", byte, d->nak);
  return retval;
}

int main(void) {
  struct port p = {0};
  struct dev d = {&p, 0};
  printf("%d\n", send(&d, 41));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct port {
// CHECK-NEXT:     locked: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn pause_rx(p: *mut port) {
// CHECK-NEXT:     println!("lock");
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*p).locked = 1;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn continue_rx(p: *mut port) {
// CHECK-NEXT:     println!("unlock");
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*p).locked = 0;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn class_pause_destructor(p: *mut *mut port) {
// CHECK-NEXT:     if !unsafe { *p }.is_null() {
// CHECK-NEXT:         continue_rx(unsafe { *p });
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn class_pause_constructor(p: *mut port) -> *mut port {
// CHECK-NEXT:     pause_rx(p);
// CHECK-NEXT:     p
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct dev {
// CHECK-NEXT:     port: *mut port,
// CHECK-NEXT:     nak: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[inline(never)]
// CHECK-NEXT: fn do_send(d: &dev, byte: u8) -> i32 {
// CHECK-NEXT:     println!("do_send locked={}", unsafe { (*(*d).port).locked });
// CHECK-NEXT:     (byte as i32) + unsafe { (*(*d).port).locked }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn send(d: &dev, byte: u8) -> i32 {
// CHECK-NEXT:     let mut retval: i32 = 0;
// CHECK-NEXT:     let mut guard: *mut port = class_pause_constructor((*d).port);
// CHECK-NEXT:     let __slate_cleanup_ptr_0 = core::ptr::addr_of_mut!(guard) as *mut _;
// CHECK-NEXT:     let __slate_cleanup_0 = ::slate_support::Guard(
// CHECK-NEXT:         &mut || class_pause_destructor(__slate_cleanup_ptr_0 as *mut _),
// CHECK-NEXT:     );
// CHECK-NEXT:     retval = do_send(d, byte);
// CHECK-NEXT:     if false {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::printf(c"%02x - %x\n".as_ptr(), byte as i32, (*d).nak) };
// CHECK-NEXT:         unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     retval
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut p: port = port { locked: 0 };
// CHECK-NEXT:     let d: dev = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: dev = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].port = core::ptr::addr_of_mut!(p);
// CHECK-NEXT:         [[T_0]].nak = 0i32;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     println!("{}", send(&d, 41u8));
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
