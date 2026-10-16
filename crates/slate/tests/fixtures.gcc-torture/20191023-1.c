/* PR tree-optimization/92131 */
/* Testcase by Armin Rigo <arigo@tunes.org> */

long b, c, d, e, f, i;
char g, h, j, k;
int *aa;

static void error(void) __attribute__((noipa));
static void error(void) { __builtin_abort(); }

static void see_me_here(void) __attribute__((noipa));
static void see_me_here(void) {}

static void aaa(void) __attribute__((noipa));
static void aaa(void) {}

static void a(void) __attribute__((noipa));
static void a(void) {
  long am, ao;
  if (aa == 0) {
    aaa();
    if (j)
      goto ay;
  }
  return;
ay:
  aaa();
  if (k) {
    aaa();
    goto az;
  }
  return;
az:
  if (i)
    if (g)
      if (h)
        if (e)
          goto bd;
  return;
bd:
  am = 0;
  while (am < e) {
    switch (c) {
    case 8:
      goto bh;
    case 4:
      return;
    }
  bh:
    if (am >= 0)
      b = -am;
    ao = am + b;
    f  = ao & 7;
    if (f == 0)
      see_me_here();
    if (ao >= 0)
      am++;
    else
      error();
  }
}

int main(void) {
  j++;
  k++;
  i++;
  g++;
  h++;
  e = 1;
  a();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/92131
// CHECK-NEXT: //!
// CHECK-NEXT: //! Testcase by Armin Rigo <arigo@tunes.org>
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: i64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: i64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut d: i64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut e: i64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut f: i64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut i: i64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut g: i8 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut h: i8 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut j: i8 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut k: i8 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut aa: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn error() {
// CHECK-NEXT:     unsafe { __builtin_abort() };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn see_me_here() {}
// CHECK-EMPTY:
// CHECK-NEXT: fn aaa() {}
// CHECK-EMPTY:
// CHECK-NEXT: fn a() {
// CHECK-NEXT:     let mut [[SLATE_STORAGE_0:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_0:__slate_slot_[0-9]+]]: *mut i64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_0]]) as *mut i64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_1:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_1:__slate_slot_[0-9]+]]: *mut i64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_1]]) as *mut i64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_2:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_2:__slate_slot_[0-9]+]]: *mut i64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_2]]) as *mut i64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_3:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_3:__slate_slot_[0-9]+]]: *mut i64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_3]]) as *mut i64;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         '__slate_dispatch: {
// CHECK-NEXT:             if (unsafe { aa }) == std::ptr::null_mut::<i32>() {
// CHECK-NEXT:                 aaa();
// CHECK-NEXT:                 if (unsafe { j }) != (0 as i8) {
// CHECK-NEXT:                     aaa();
// CHECK-NEXT:                     if (unsafe { k }) != (0 as i8) {
// CHECK-NEXT:                         aaa();
// CHECK-NEXT:                         if (unsafe { i }) != (0 as i64) {
// CHECK-NEXT:                             if (unsafe { g }) != (0 as i8) {
// CHECK-NEXT:                                 if (unsafe { h }) != (0 as i8) {
// CHECK-NEXT:                                     if (unsafe { e }) != (0 as i64) {
// CHECK-NEXT:                                         *[[SLATE_SLOT_3]] = (0 as i32) as i64;
// CHECK-NEXT:                                         loop {
// CHECK-NEXT:                                             if *[[SLATE_SLOT_3]] < unsafe { e } {
// CHECK-NEXT:                                                 let [[T_0:__t[0-9]+]]: i64 = unsafe { c };
// CHECK-NEXT:                                                 if [[T_0]] == (8 as i64) {
// CHECK-NEXT:                                                 } else {
// CHECK-NEXT:                                                     if [[T_0]] == (4 as i64) {
// CHECK-NEXT:                                                         return;
// CHECK-NEXT:                                                     }
// CHECK-NEXT:                                                 }
// CHECK-NEXT:                                                 if *[[SLATE_SLOT_3]] >= ((0 as i32) as i64) {
// CHECK-NEXT:                                                     unsafe {
// CHECK-NEXT:                                                         b = -(*[[SLATE_SLOT_3]]);
// CHECK-NEXT:                                                     }
// CHECK-NEXT:                                                 }
// CHECK-NEXT:                                                 *[[SLATE_SLOT_2]] = *[[SLATE_SLOT_3]] + unsafe { b };
// CHECK-NEXT:                                                 unsafe {
// CHECK-NEXT:                                                     f = *[[SLATE_SLOT_2]] & ((7 as i32) as i64);
// CHECK-NEXT:                                                 }
// CHECK-NEXT:                                                 if (unsafe { f }) == ((0 as i32) as i64) {
// CHECK-NEXT:                                                     see_me_here();
// CHECK-NEXT:                                                 }
// CHECK-NEXT:                                                 if *[[SLATE_SLOT_2]] >= ((0 as i32) as i64) {
// CHECK-NEXT:                                                     std::ptr::write(
// CHECK-NEXT:                                                         [[SLATE_SLOT_1]],
// CHECK-NEXT:                                                         *[[SLATE_SLOT_3]],
// CHECK-NEXT:                                                     );
// CHECK-NEXT:                                                     std::ptr::write(
// CHECK-NEXT:                                                         [[SLATE_SLOT_0]],
// CHECK-NEXT:                                                         *[[SLATE_SLOT_1]] + ((1 as i32) as i64),
// CHECK-NEXT:                                                     );
// CHECK-NEXT:                                                     *[[SLATE_SLOT_3]] = *[[SLATE_SLOT_0]];
// CHECK-NEXT:                                                 } else {
// CHECK-NEXT:                                                     error();
// CHECK-NEXT:                                                 }
// CHECK-NEXT:                                             } else {
// CHECK-NEXT:                                                 break;
// CHECK-NEXT:                                             }
// CHECK-NEXT:                                         }
// CHECK-NEXT:                                         break '__slate_dispatch;
// CHECK-NEXT:                                     }
// CHECK-NEXT:                                 }
// CHECK-NEXT:                             }
// CHECK-NEXT:                         }
// CHECK-NEXT:                         return;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         return;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             return;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: i8 = unsafe { j };
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i8 = (([[V_0]] as i32) + (1 as i32)) as i8;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         j = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: i8 = unsafe { k };
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: i8 = (([[V_2]] as i32) + (1 as i32)) as i8;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         k = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: i64 = unsafe { i };
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: i64 = [[V_4]] + ((1 as i32) as i64);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         i = [[V_5]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_6:__v[0-9]+]]: i8 = unsafe { g };
// CHECK-NEXT:     let mut [[V_7:__v[0-9]+]]: i8 = (([[V_6]] as i32) + (1 as i32)) as i8;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         g = [[V_7]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_8:__v[0-9]+]]: i8 = unsafe { h };
// CHECK-NEXT:     let mut [[V_9:__v[0-9]+]]: i8 = (([[V_8]] as i32) + (1 as i32)) as i8;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         h = [[V_9]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         e = (1 as i32) as i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     a();
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
