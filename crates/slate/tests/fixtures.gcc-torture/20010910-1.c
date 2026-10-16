/* Test case contributed by Ingo Rohloff <rohloff@in.tum.de>.
   Code distilled from Linux kernel.  */

/* Compile this program with a gcc-2.95.2 using
   "gcc -O2" and run it. The result will be that
   rx_ring[1].next == 0   (it should be == 14)
   and
   ep.skbuff[4] == 5      (it should be 0)
*/

extern void abort(void);

struct epic_rx_desc {
  unsigned int next;
};

struct epic_private {
  struct epic_rx_desc *rx_ring;
  unsigned int         rx_skbuff[5];
};

static void epic_init_ring(struct epic_private *ep) {
  int i;

  for (i = 0; i < 5; i++) {
    ep->rx_ring[i].next = 10 + (i + 1) * 2;
    ep->rx_skbuff[i]    = 0;
  }
  ep->rx_ring[i - 1].next = 10;
}

static int check_rx_ring[5] = {12, 14, 16, 18, 10};

int main(void) {
  struct epic_private ep;
  struct epic_rx_desc rx_ring[5];
  int                 i;

  for (i = 0; i < 5; i++) {
    rx_ring[i].next = 0;
    ep.rx_skbuff[i] = 5;
  }

  ep.rx_ring = rx_ring;
  epic_init_ring(&ep);

  for (i = 0; i < 5; i++) {
    if (rx_ring[i].next != check_rx_ring[i])
      abort();
    if (ep.rx_skbuff[i] != 0)
      abort();
  }
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Test case contributed by Ingo Rohloff <rohloff@in.tum.de>.
// CHECK-NEXT: //! Code distilled from Linux kernel.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // Compile this program with a gcc-2.95.2 using
// CHECK-NEXT: // "gcc -O2" and run it. The result will be that
// CHECK-NEXT: // rx_ring[1].next == 0   (it should be == 14)
// CHECK-NEXT: // and
// CHECK-NEXT: // ep.skbuff[4] == 5      (it should be 0)
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct epic_rx_desc {
// CHECK-NEXT:     next: u32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct epic_private {
// CHECK-NEXT:     rx_ring: *mut epic_rx_desc,
// CHECK-NEXT:     rx_skbuff: [u32; 5],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn epic_init_ring(mut ep: &mut epic_private) {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (5 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*unsafe { unsafe { (*(ep as *mut epic_private)).rx_ring }.offset(i as isize) }).next =
// CHECK-NEXT:                 ((10 as i32) + (i + (1 as i32)) * (2 as i32)) as u32;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { (*(ep as *mut epic_private)).rx_skbuff.as_mut_ptr() as *mut u32 }
// CHECK-NEXT:                     .offset(i as isize)
// CHECK-NEXT:             } = (0 as i32) as u32;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         i = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe {
// CHECK-NEXT:             unsafe { (*(ep as *mut epic_private)).rx_ring }.offset((i - (1 as i32)) as isize)
// CHECK-NEXT:         })
// CHECK-NEXT:         .next = (10 as i32) as u32;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut check_rx_ring: __SlateAlign16<[i32; 5]> =
// CHECK-NEXT:     __SlateAlign16([12 as i32, 14 as i32, 16 as i32, 18 as i32, 10 as i32]);
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut ep: epic_private = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut rx_ring: __SlateAlign16<[epic_rx_desc; 5]> =
// CHECK-NEXT:         __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < (5 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*unsafe { (rx_ring.0.as_mut_ptr() as *mut epic_rx_desc).offset(i as isize) }).next =
// CHECK-NEXT:                 (0 as i32) as u32;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (ep.rx_skbuff.as_mut_ptr() as *mut u32).offset(i as isize) } =
// CHECK-NEXT:                 (5 as i32) as u32;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         i = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     ep.rx_ring = rx_ring.0.as_mut_ptr() as *mut epic_rx_desc;
// CHECK-NEXT:     epic_init_ring(unsafe { &mut (*std::ptr::addr_of_mut!(ep)) });
// CHECK-NEXT:     i = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while i < (5 as i32) {
// CHECK-NEXT:         if (unsafe {
// CHECK-NEXT:             (*unsafe { (rx_ring.0.as_mut_ptr() as *mut epic_rx_desc).offset(i as isize) }).next
// CHECK-NEXT:         }) != ((unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(check_rx_ring.0) as *mut i32 }.offset(i as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as u32)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         if (unsafe { *unsafe { (ep.rx_skbuff.as_mut_ptr() as *mut u32).offset(i as isize) } })
// CHECK-NEXT:             != ((0 as i32) as u32)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = [[V_4]] + (1 as i32);
// CHECK-NEXT:         i = [[V_5]];
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
