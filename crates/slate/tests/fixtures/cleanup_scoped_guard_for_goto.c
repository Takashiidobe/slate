#include <stdio.h>

struct guard {
  int *lock;
};

static void guard_release(struct guard *g) {
  if (g->lock)
    (*g->lock)--;
}

static struct guard guard_take(int *lock, int ok) {
  struct guard g = {ok ? lock : 0};
  if (g.lock)
    (*g.lock)++;
  return g;
}

#define scoped_guard_label(target, ok, label)                                    \
  for (struct guard scope __attribute__((cleanup(guard_release))) =            \
           guard_take(target, ok);                                             \
       scope.lock || !1; ({ goto label; }))                                    \
    if (0) {                                                                   \
    label:                                                                     \
      break;                                                                   \
    } else

static int set_value(int *lock, int *slot, int value, int ok) {
  int ret = -22;
  scoped_guard_label(lock, ok, done_set) {
    *slot = value;
    ret = 0;
  }
  return ret;
}

static int read_value(int *lock, int *slot) {
  scoped_guard_label(lock, 1, done_read) { return *slot + *lock * 100; }
  return -1;
}

static char order[32];
static int order_len;

static void note(char *c) { order[order_len++] = *c; }

static int nested(int *outer_lock, int *inner_lock, int mode) {
  scoped_guard_label(outer_lock, 1, done_outer) {
    char a __attribute__((cleanup(note))) = 'a';
    scoped_guard_label(inner_lock, 1, done_inner) {
      char b __attribute__((cleanup(note))) = 'b';
      if (mode == 0)
        return *outer_lock * 10 + *inner_lock;
      if (mode == 1)
        goto out;
      if (mode == 2)
        break;
    }
    order[order_len++] = '-';
  }
  return -1;
out:
  order[order_len++] = '!';
  return -2;
}

static int loop_guards(int *counter, int n) {
  int total = 0;
  for (int i = 0; i < n; i++) {
    scoped_guard_label(counter, 1, done_loop) {
      char c __attribute__((cleanup(note))) = 'c';
      if (i == 1)
        continue;
      if (i == 3)
        goto stop;
      total += *counter;
    }
  }
stop:
  return total;
}

int main(void) {
  int outer = 0, inner = 0;
  for (int mode = 0; mode < 3; mode++) {
    order_len = 0;
    int r = nested(&outer, &inner, mode);
    order[order_len] = 0;
    printf("%d %d %d %s\n", r, outer, inner, order);
  }
  order_len = 0;
  int t = loop_guards(&outer, 5);
  order[order_len] = 0;
  printf("%d %d %s\n", t, outer, order);
  int lock = 0, slot = 0;
  int a = set_value(&lock, &slot, 7, 1);
  printf("%d %d %d\n", a, lock, slot);
  int b = set_value(&lock, &slot, 9, 0);
  printf("%d %d %d\n", b, lock, slot);
  int c = read_value(&lock, &slot);
  printf("%d %d\n", c, lock);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: mod __slate_cleanup {
// CHECK-NEXT:     pub struct Guard<F: FnMut()>(pub F);
// CHECK-NEXT:     impl<F: FnMut()> Drop for Guard<F> {
// CHECK-NEXT:         fn drop(&mut self) {
// CHECK-NEXT:             (self.0)()
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct guard {
// CHECK-NEXT:     lock: *mut i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe impl<T> Sync for __SlateAlign16<T> {}
// CHECK-NEXT: fn guard_release(g: *mut guard) {
// CHECK-NEXT:     if !unsafe { (*g).lock }.is_null() {
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: *mut i32 = unsafe { (*g).lock };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_0]] = *[[V_0]] - 1;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn guard_take(lock: *mut i32, ok: i32) -> guard {
// CHECK-NEXT:     let g: guard = guard {
// CHECK-NEXT:         lock: if ok != 0 { lock } else { core::ptr::null_mut::<i32>() },
// CHECK-NEXT:     };
// CHECK-NEXT:     if !g.lock.is_null() {
// CHECK-NEXT:         let [[V_0]]: *mut i32 = g.lock;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_0]] = *[[V_0]] + 1;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     return g;
// CHECK-NEXT: }
// CHECK-NEXT: fn set_value(lock: *mut i32, slot: &mut i32, value: i32, ok: i32) -> i32 {
// CHECK-NEXT:     let mut scope: core::mem::MaybeUninit<guard> = core::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_0:__slate_slot_[0-9]+]]: *mut guard = core::ptr::addr_of_mut!(scope) as *mut guard;
// CHECK-NEXT:     let mut ret: i32 = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     let __slate_cleanup_armed_0 = core::cell::Cell::new(false);
// CHECK-NEXT:     let __slate_cleanup_0 = __slate_cleanup::Guard(|| {
// CHECK-NEXT:         if __slate_cleanup_armed_0.get() {
// CHECK-NEXT:             unsafe { guard_release([[SLATE_SLOT_0]] as *mut _) };
// CHECK-NEXT:         }
// CHECK-NEXT:     });
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         ret = -22;
// CHECK-NEXT:         core::ptr::write([[SLATE_SLOT_0]], guard_take(lock, ok));
// CHECK-NEXT:         __slate_cleanup_armed_0.set(true);
// CHECK-NEXT:         if !(*[[SLATE_SLOT_0]]).lock.is_null() || false {
// CHECK-NEXT:             if true {
// CHECK-NEXT:                 *(slot as *mut i32) = value;
// CHECK-NEXT:                 ret = 0;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         __slate_cleanup_armed_0.set(false);
// CHECK-NEXT:         guard_release([[SLATE_SLOT_0]] as *mut _);
// CHECK-NEXT:         return ret;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn read_value(lock: *mut i32, slot: &i32) -> i32 {
// CHECK-NEXT:     let mut scope: core::mem::MaybeUninit<guard> = core::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_0]]: *mut guard = core::ptr::addr_of_mut!(scope) as *mut guard;
// CHECK-NEXT:     let __slate_cleanup_armed_0 = core::cell::Cell::new(false);
// CHECK-NEXT:     let __slate_cleanup_0 = __slate_cleanup::Guard(|| {
// CHECK-NEXT:         if __slate_cleanup_armed_0.get() {
// CHECK-NEXT:             unsafe { guard_release([[SLATE_SLOT_0]] as *mut _) };
// CHECK-NEXT:         }
// CHECK-NEXT:     });
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::ptr::write([[SLATE_SLOT_0]], guard_take(lock, 1i32));
// CHECK-NEXT:         __slate_cleanup_armed_0.set(true);
// CHECK-NEXT:         if !(*[[SLATE_SLOT_0]]).lock.is_null() || false {
// CHECK-NEXT:             if true {
// CHECK-NEXT:                 return *(slot as *const i32) + *lock * 100;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         __slate_cleanup_armed_0.set(false);
// CHECK-NEXT:         guard_release([[SLATE_SLOT_0]] as *mut _);
// CHECK-NEXT:         return -1;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: static mut order: __SlateAlign16<[i8; 32]> = __SlateAlign16(unsafe {
// CHECK-NEXT:     core::mem::zeroed()
// CHECK-NEXT: });
// CHECK-NEXT: static mut order_len: i32 = unsafe { core::mem::zeroed() };
// CHECK-NEXT: fn note(c: *mut i8) {
// CHECK-NEXT:     let [[V_0]]: i32 = unsafe { order_len };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         order_len = [[V_0]] + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         order.0[[[V_0]] as usize] = *c;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn nested(outer_lock: *mut i32, inner_lock: *mut i32, mode: i32) -> i32 {
// CHECK-NEXT:     let mut b: core::mem::MaybeUninit<i8> = core::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_1:__slate_slot_[0-9]+]]: *mut i8 = core::ptr::addr_of_mut!(b) as *mut i8;
// CHECK-NEXT:     let mut scope: core::mem::MaybeUninit<guard> = core::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_2:__slate_slot_[0-9]+]]: *mut guard = core::ptr::addr_of_mut!(scope) as *mut guard;
// CHECK-NEXT:     let mut a: core::mem::MaybeUninit<i8> = core::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_3:__slate_slot_[0-9]+]]: *mut i8 = core::ptr::addr_of_mut!(a) as *mut i8;
// CHECK-NEXT:     let mut scope: core::mem::MaybeUninit<guard> = core::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_4:__slate_slot_[0-9]+]]: *mut guard = core::ptr::addr_of_mut!(scope) as *mut guard;
// CHECK-NEXT:     let __slate_cleanup_armed_7 = core::cell::Cell::new(false);
// CHECK-NEXT:     let __slate_cleanup_7 = __slate_cleanup::Guard(|| {
// CHECK-NEXT:         if __slate_cleanup_armed_7.get() {
// CHECK-NEXT:             unsafe { guard_release([[SLATE_SLOT_4]] as *mut _) };
// CHECK-NEXT:         }
// CHECK-NEXT:     });
// CHECK-NEXT:     let __slate_cleanup_armed_6 = core::cell::Cell::new(false);
// CHECK-NEXT:     let __slate_cleanup_6 = __slate_cleanup::Guard(|| {
// CHECK-NEXT:         if __slate_cleanup_armed_6.get() {
// CHECK-NEXT:             unsafe { note([[SLATE_SLOT_3]] as *mut _) };
// CHECK-NEXT:         }
// CHECK-NEXT:     });
// CHECK-NEXT:     let __slate_cleanup_armed_5 = core::cell::Cell::new(false);
// CHECK-NEXT:     let __slate_cleanup_5 = __slate_cleanup::Guard(|| {
// CHECK-NEXT:         if __slate_cleanup_armed_5.get() {
// CHECK-NEXT:             unsafe { guard_release([[SLATE_SLOT_2]] as *mut _) };
// CHECK-NEXT:         }
// CHECK-NEXT:     });
// CHECK-NEXT:     let __slate_cleanup_armed_4 = core::cell::Cell::new(false);
// CHECK-NEXT:     let __slate_cleanup_4 = __slate_cleanup::Guard(|| {
// CHECK-NEXT:         if __slate_cleanup_armed_4.get() {
// CHECK-NEXT:             unsafe { note([[SLATE_SLOT_1]] as *mut _) };
// CHECK-NEXT:         }
// CHECK-NEXT:     });
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::ptr::write([[SLATE_SLOT_4]], guard_take(outer_lock, 1i32));
// CHECK-NEXT:         __slate_cleanup_armed_7.set(true);
// CHECK-NEXT:         if !(*[[SLATE_SLOT_4]]).lock.is_null() || false {
// CHECK-NEXT:             if true {
// CHECK-NEXT:                 core::ptr::write([[SLATE_SLOT_3]], 97i8);
// CHECK-NEXT:                 __slate_cleanup_armed_6.set(true);
// CHECK-NEXT:                 core::ptr::write([[SLATE_SLOT_2]], guard_take(inner_lock, 1i32));
// CHECK-NEXT:                 __slate_cleanup_armed_5.set(true);
// CHECK-NEXT:                 if !(*[[SLATE_SLOT_2]]).lock.is_null() || false {
// CHECK-NEXT:                     if true {
// CHECK-NEXT:                         core::ptr::write([[SLATE_SLOT_1]], 98i8);
// CHECK-NEXT:                         __slate_cleanup_armed_4.set(true);
// CHECK-NEXT:                         if mode == 0 {
// CHECK-NEXT:                             return *outer_lock * 10 + *inner_lock;
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             if mode == 1 {
// CHECK-NEXT:                                 __slate_cleanup_armed_4.set(false);
// CHECK-NEXT:                                 note([[SLATE_SLOT_1]] as *mut _);
// CHECK-NEXT:                                 __slate_cleanup_armed_5.set(false);
// CHECK-NEXT:                                 guard_release([[SLATE_SLOT_2]] as *mut _);
// CHECK-NEXT:                                 __slate_cleanup_armed_6.set(false);
// CHECK-NEXT:                                 note([[SLATE_SLOT_3]] as *mut _);
// CHECK-NEXT:                                 __slate_cleanup_armed_7.set(false);
// CHECK-NEXT:                                 guard_release([[SLATE_SLOT_4]] as *mut _);
// CHECK-NEXT:                                 let [[V_1:__v[0-9]+]]: i32 = order_len;
// CHECK-NEXT:                                 order_len = [[V_1]] + 1;
// CHECK-NEXT:                                 order.0[[[V_1]] as usize] = 33;
// CHECK-NEXT:                                 return -2;
// CHECK-NEXT:                             } else {
// CHECK-NEXT:                                 if mode == 2 {
// CHECK-NEXT:                                     __slate_cleanup_armed_4.set(false);
// CHECK-NEXT:                                     note([[SLATE_SLOT_1]] as *mut _);
// CHECK-NEXT:                                 } else {
// CHECK-NEXT:                                     __slate_cleanup_armed_4.set(false);
// CHECK-NEXT:                                     note([[SLATE_SLOT_1]] as *mut _);
// CHECK-NEXT:                                 }
// CHECK-NEXT:                             }
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 __slate_cleanup_armed_5.set(false);
// CHECK-NEXT:                 guard_release([[SLATE_SLOT_2]] as *mut _);
// CHECK-NEXT:                 let [[V_2:__v[0-9]+]]: i32 = order_len;
// CHECK-NEXT:                 order_len = [[V_2]] + 1;
// CHECK-NEXT:                 order.0[[[V_2]] as usize] = 45;
// CHECK-NEXT:                 __slate_cleanup_armed_6.set(false);
// CHECK-NEXT:                 note([[SLATE_SLOT_3]] as *mut _);
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         __slate_cleanup_armed_7.set(false);
// CHECK-NEXT:         guard_release([[SLATE_SLOT_4]] as *mut _);
// CHECK-NEXT:         return -1;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn loop_guards(counter: *mut i32, n: i32) -> i32 {
// CHECK-NEXT:     let mut c: core::mem::MaybeUninit<i8> = core::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_1]]: *mut i8 = core::ptr::addr_of_mut!(c) as *mut i8;
// CHECK-NEXT:     let mut scope: core::mem::MaybeUninit<guard> = core::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_2]]: *mut guard = core::ptr::addr_of_mut!(scope) as *mut guard;
// CHECK-NEXT:     let mut i: i32 = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     let mut total: i32 = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     let __slate_cleanup_armed_5 = core::cell::Cell::new(false);
// CHECK-NEXT:     let __slate_cleanup_5 = __slate_cleanup::Guard(|| {
// CHECK-NEXT:         if __slate_cleanup_armed_5.get() {
// CHECK-NEXT:             unsafe { guard_release([[SLATE_SLOT_2]] as *mut _) };
// CHECK-NEXT:         }
// CHECK-NEXT:     });
// CHECK-NEXT:     let __slate_cleanup_armed_4 = core::cell::Cell::new(false);
// CHECK-NEXT:     let __slate_cleanup_4 = __slate_cleanup::Guard(|| {
// CHECK-NEXT:         if __slate_cleanup_armed_4.get() {
// CHECK-NEXT:             unsafe { note([[SLATE_SLOT_1]] as *mut _) };
// CHECK-NEXT:         }
// CHECK-NEXT:     });
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         total = 0;
// CHECK-NEXT:         i = 0;
// CHECK-NEXT:         '[[JOIN_0:__join_[0-9]+]]: {
// CHECK-NEXT:             loop {
// CHECK-NEXT:                 if i < n {
// CHECK-NEXT:                     core::ptr::write([[SLATE_SLOT_2]], guard_take(counter, 1i32));
// CHECK-NEXT:                     __slate_cleanup_armed_5.set(true);
// CHECK-NEXT:                     if !(*[[SLATE_SLOT_2]]).lock.is_null() || false {
// CHECK-NEXT:                         if true {
// CHECK-NEXT:                             core::ptr::write([[SLATE_SLOT_1]], 99i8);
// CHECK-NEXT:                             __slate_cleanup_armed_4.set(true);
// CHECK-NEXT:                             if i == 1 {
// CHECK-NEXT:                                 __slate_cleanup_armed_4.set(false);
// CHECK-NEXT:                                 note([[SLATE_SLOT_1]] as *mut _);
// CHECK-NEXT:                             } else {
// CHECK-NEXT:                                 if i == 3 {
// CHECK-NEXT:                                     break;
// CHECK-NEXT:                                 } else {
// CHECK-NEXT:                                     total += *counter;
// CHECK-NEXT:                                     __slate_cleanup_armed_4.set(false);
// CHECK-NEXT:                                     note([[SLATE_SLOT_1]] as *mut _);
// CHECK-NEXT:                                 }
// CHECK-NEXT:                             }
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                     __slate_cleanup_armed_5.set(false);
// CHECK-NEXT:                     guard_release([[SLATE_SLOT_2]] as *mut _);
// CHECK-NEXT:                     i += 1;
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     break '[[JOIN_0]];
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             __slate_cleanup_armed_4.set(false);
// CHECK-NEXT:             note([[SLATE_SLOT_1]] as *mut _);
// CHECK-NEXT:             __slate_cleanup_armed_5.set(false);
// CHECK-NEXT:             guard_release([[SLATE_SLOT_2]] as *mut _);
// CHECK-NEXT:         }
// CHECK-NEXT:         return total;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut outer: i32 = 0;
// CHECK-NEXT:     let mut inner: i32 = 0;
// CHECK-NEXT:     for mode in 0i32..3i32 {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             order_len = 0;
// CHECK-NEXT:         }
// CHECK-NEXT:         let r: i32 = nested(
// CHECK-NEXT:             core::ptr::addr_of_mut!(outer),
// CHECK-NEXT:             core::ptr::addr_of_mut!(inner),
// CHECK-NEXT:             mode,
// CHECK-NEXT:         );
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             order.0[order_len as usize] = 0;
// CHECK-NEXT:         }
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(
// CHECK-NEXT:                 c"%d %d %d %s\n".as_ptr(),
// CHECK-NEXT:                 r,
// CHECK-NEXT:                 outer,
// CHECK-NEXT:                 inner,
// CHECK-NEXT:                 core::ptr::addr_of_mut!(order.0) as *mut i8,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         order_len = 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     let t: i32 = loop_guards(core::ptr::addr_of_mut!(outer), 5i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         order.0[order_len as usize] = 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%d %d %s\n".as_ptr(),
// CHECK-NEXT:             t,
// CHECK-NEXT:             outer,
// CHECK-NEXT:             core::ptr::addr_of_mut!(order.0) as *mut i8,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let mut lock: i32 = 0;
// CHECK-NEXT:     let mut slot: i32 = 0;
// CHECK-NEXT:     let a: i32 = set_value(
// CHECK-NEXT:         core::ptr::addr_of_mut!(lock),
// CHECK-NEXT:         unsafe { &mut (*core::ptr::addr_of_mut!(slot)) },
// CHECK-NEXT:         7i32,
// CHECK-NEXT:         1i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{} {} {}", a, lock, slot);
// CHECK-NEXT:     let b: i32 = set_value(
// CHECK-NEXT:         core::ptr::addr_of_mut!(lock),
// CHECK-NEXT:         unsafe { &mut (*core::ptr::addr_of_mut!(slot)) },
// CHECK-NEXT:         9i32,
// CHECK-NEXT:         0i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{} {} {}", b, lock, slot);
// CHECK-NEXT:     let c: i32 = read_value(
// CHECK-NEXT:         core::ptr::addr_of_mut!(lock),
// CHECK-NEXT:         unsafe { &(*core::ptr::addr_of_mut!(slot)) },
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{} {}", c, lock);
// CHECK-NEXT:     0
// CHECK-NEXT: }
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
