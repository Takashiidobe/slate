/* PR tree-optimization/50569 */
/* Reported by Paul Koning <pkoning@gcc.gnu.org> */
/* Reduced testcase by Mikael Pettersson <mikpe@it.uu.se> */

struct event {
  struct {
    unsigned int sec;
  } sent __attribute__((packed));
};

void __attribute__((noinline, noclone)) frob_entry(char *buf) {
  struct event event;

  __builtin_memcpy(&event, buf, sizeof(event));
  if (event.sent.sec < 64) {
    event.sent.sec = -1U;
    __builtin_memcpy(buf, &event, sizeof(event));
  }
}

int main(void) {
  union {
    char buf[1 + sizeof(struct event)];
    int  align;
  } u;

  __builtin_memset(&u, 0, sizeof u);

  frob_entry(&u.buf[1]);

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/50569
// CHECK-NEXT: //!
// CHECK-NEXT: //! Reported by Paul Koning <pkoning@gcc.gnu.org>
// CHECK-NEXT: //!
// CHECK-NEXT: //! Reduced testcase by Mikael Pettersson <mikpe@it.uu.se>
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "memcpy"]
// CHECK-NEXT:     fn __builtin_memcpy([[V_0:__v[0-9]+]]: *mut (), [[V_1:__v[0-9]+]]: *const (), [[V_2:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT:     #[link_name = "memset"]
// CHECK-NEXT:     fn __builtin_memset([[V_3:__v[0-9]+]]: *mut (), [[V_4:__v[0-9]+]]: i32, [[V_5:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct event {
// CHECK-NEXT:     sent: [[SLATERECORD_0:__SlateRecord[0-9]+]],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0]] {
// CHECK-NEXT:     sec: u32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[SLATERECORD_1:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     buf: [i8; 5],
// CHECK-NEXT:     align: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn frob_entry(mut buf: *mut i8) {
// CHECK-NEXT:     let mut event: event = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memcpy(
// CHECK-NEXT:             std::ptr::addr_of_mut!(event) as *mut (),
// CHECK-NEXT:             buf as *const (),
// CHECK-NEXT:             4 as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     if event.sent.sec < ((64 as i32) as u32) {
// CHECK-NEXT:         event.sent.sec = (1 as u32).wrapping_neg();
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             __builtin_memcpy(
// CHECK-NEXT:                 buf as *mut (),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(event) as *const (),
// CHECK-NEXT:                 4 as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut u: [[SLATERECORD_1]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe { __builtin_memset(std::ptr::addr_of_mut!(u) as *mut (), 0 as i32, 8 as u64) };
// CHECK-NEXT:     frob_entry(unsafe { unsafe { u.buf.as_mut_ptr() as *mut i8 }.offset((1 as i32) as isize) });
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
