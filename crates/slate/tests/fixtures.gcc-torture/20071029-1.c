extern void exit(int);
extern void abort(void);

typedef union {
  struct {
    int      f1, f2, f3, f4, f5, f6, f7, f8;
    long int f9, f10;
    int      f11;
  } f;
  char     s[56];
  long int a;
} T;

__attribute__((noinline)) void test(T *t) {
  static int i = 11;
  if (t->f.f1 != i++)
    abort();
  if (t->f.f2 || t->f.f3 || t->f.f4 || t->f.f5 || t->f.f6 || t->f.f7 ||
      t->f.f8 || t->f.f9 || t->f.f10 || t->f.f11)
    abort();
  if (i == 20)
    exit(0);
}

__attribute__((noinline)) void foo(int i) {
  T t;
again:
  t = (T){{++i, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
  test(&t);
  goto again;
}

int main(void) {
  T  *t1, *t2;
  int cnt = 0;
  t1      = (T *)0;
loop:
  t2 = t1;
  t1 = &(T){.f.f9 = cnt++};
  if (cnt < 3)
    goto loop;
  if (t1 != t2 || t1->f.f9 != 2)
    abort();
  foo(10);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     f: [[SLATERECORD_1:__SlateRecord[0-9]+]],
// CHECK-NEXT:     s: [i8; 56],
// CHECK-NEXT:     a: i64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_1]] {
// CHECK-NEXT:     f1: i32,
// CHECK-NEXT:     f2: i32,
// CHECK-NEXT:     f3: i32,
// CHECK-NEXT:     f4: i32,
// CHECK-NEXT:     f5: i32,
// CHECK-NEXT:     f6: i32,
// CHECK-NEXT:     f7: i32,
// CHECK-NEXT:     f8: i32,
// CHECK-NEXT:     f9: i64,
// CHECK-NEXT:     f10: i64,
// CHECK-NEXT:     f11: i32,
// CHECK-NEXT: }
// CHECK-NEXT: fn test(mut t: &[[SLATERECORD_0]]) {
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i32 = unsafe { i };
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         i = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { (*(t as *const [[SLATERECORD_0]])).f.f1 }) != [[V_1]] {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { (*(t as *const [[SLATERECORD_0]])).f.f2 }) != (0 as i32)
// CHECK-NEXT:         || (unsafe { (*(t as *const [[SLATERECORD_0]])).f.f3 }) != (0 as i32)
// CHECK-NEXT:         || (unsafe { (*(t as *const [[SLATERECORD_0]])).f.f4 }) != (0 as i32)
// CHECK-NEXT:         || (unsafe { (*(t as *const [[SLATERECORD_0]])).f.f5 }) != (0 as i32)
// CHECK-NEXT:         || (unsafe { (*(t as *const [[SLATERECORD_0]])).f.f6 }) != (0 as i32)
// CHECK-NEXT:         || (unsafe { (*(t as *const [[SLATERECORD_0]])).f.f7 }) != (0 as i32)
// CHECK-NEXT:         || (unsafe { (*(t as *const [[SLATERECORD_0]])).f.f8 }) != (0 as i32)
// CHECK-NEXT:         || (unsafe { (*(t as *const [[SLATERECORD_0]])).f.f9 }) != (0 as i64)
// CHECK-NEXT:         || (unsafe { (*(t as *const [[SLATERECORD_0]])).f.f10 }) != (0 as i64)
// CHECK-NEXT:         || (unsafe { (*(t as *const [[SLATERECORD_0]])).f.f11 }) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { i }) == (20 as i32) {
// CHECK-NEXT:         unsafe { exit(0 as i32) };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: static mut i: i32 = 11 as i32;
// CHECK-NEXT: fn foo(mut [[I_0:i_[0-9]+]]: i32) {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_STORAGE_0:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_0:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_0]])
// CHECK-NEXT:         as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_1:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_1:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_1]])
// CHECK-NEXT:         as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_2:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<[[SLATERECORD_0]]> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_2:__slate_slot_[0-9]+]]: *mut [[SLATERECORD_0]] = std::ptr::addr_of_mut!([[SLATE_STORAGE_2]])
// CHECK-NEXT:         as *mut [[SLATERECORD_0]];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             std::ptr::write([[SLATE_SLOT_1]], [[I_0]]);
// CHECK-NEXT:             std::ptr::write([[SLATE_SLOT_0]], *[[SLATE_SLOT_1]] + (1 as i32));
// CHECK-NEXT:             [[I_0]] = *[[SLATE_SLOT_0]];
// CHECK-NEXT:             *[[SLATE_SLOT_2]] = unsafe {
// CHECK-NEXT:                 *{
// CHECK-NEXT:                     [[SLATE_COMPOUND_0]] = {
// CHECK-NEXT:                         let mut [[T_0:__t[0-9]+]]: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                         [[T_0]].f = [[SLATERECORD_1]] {
// CHECK-NEXT:                             f1: *[[SLATE_SLOT_0]],
// CHECK-NEXT:                             f2: 0 as i32,
// CHECK-NEXT:                             f3: 0 as i32,
// CHECK-NEXT:                             f4: 0 as i32,
// CHECK-NEXT:                             f5: 0 as i32,
// CHECK-NEXT:                             f6: 0 as i32,
// CHECK-NEXT:                             f7: 0 as i32,
// CHECK-NEXT:                             f8: 0 as i32,
// CHECK-NEXT:                             f9: (0 as i32) as i64,
// CHECK-NEXT:                             f10: (0 as i32) as i64,
// CHECK-NEXT:                             f11: 0 as i32,
// CHECK-NEXT:                         };
// CHECK-NEXT:                         [[T_0]]
// CHECK-NEXT:                     };
// CHECK-NEXT:                     std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:                 }
// CHECK-NEXT:             };
// CHECK-NEXT:             test(unsafe { &(*std::ptr::addr_of_mut!(* [[SLATE_SLOT_2]])) });
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_1:__slate_compound_[0-9]+]]: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_STORAGE_3:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_3:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_3]])
// CHECK-NEXT:         as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_4:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_4:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_4]])
// CHECK-NEXT:         as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_5:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_5:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_5]])
// CHECK-NEXT:         as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_6:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*mut [[SLATERECORD_0]]> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_6:__slate_slot_[0-9]+]]: *mut *mut [[SLATERECORD_0]] = std::ptr::addr_of_mut!(
// CHECK-NEXT:         [[SLATE_STORAGE_6]]
// CHECK-NEXT:     ) as *mut *mut [[SLATERECORD_0]];
// CHECK-NEXT:     let mut [[SLATE_STORAGE_7:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*mut [[SLATERECORD_0]]> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_7:__slate_slot_[0-9]+]]: *mut *mut [[SLATERECORD_0]] = std::ptr::addr_of_mut!(
// CHECK-NEXT:         [[SLATE_STORAGE_7]]
// CHECK-NEXT:     ) as *mut *mut [[SLATERECORD_0]];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::write([[SLATE_SLOT_5]], 0 as i32);
// CHECK-NEXT:         *[[SLATE_SLOT_7]] = std::ptr::null_mut::<[[SLATERECORD_0]]>();
// CHECK-NEXT:         loop {
// CHECK-NEXT:             *[[SLATE_SLOT_6]] = *[[SLATE_SLOT_7]];
// CHECK-NEXT:             std::ptr::write([[SLATE_SLOT_4]], *[[SLATE_SLOT_5]]);
// CHECK-NEXT:             std::ptr::write([[SLATE_SLOT_3]], *[[SLATE_SLOT_4]] + (1 as i32));
// CHECK-NEXT:             *[[SLATE_SLOT_5]] = *[[SLATE_SLOT_3]];
// CHECK-NEXT:             *[[SLATE_SLOT_7]] = unsafe {
// CHECK-NEXT:                 std::ptr::addr_of_mut!(
// CHECK-NEXT:                     * { [[SLATE_COMPOUND_1]] = { let mut [[T_0]] : [[SLATERECORD_0]] = unsafe {
// CHECK-NEXT:                     std::mem::zeroed() }; [[T_0]].f = { let mut [[T_1:__t[0-9]+]] : [[SLATERECORD_1]] =
// CHECK-NEXT:                     unsafe { std::mem::zeroed() }; [[T_1]].f9 = * [[SLATE_SLOT_4]] as i64;
// CHECK-NEXT:                     [[T_1]] }; [[T_0]] }; std::ptr::addr_of_mut!([[SLATE_COMPOUND_1]]) }
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:             if !(*[[SLATE_SLOT_5]] < (3 as i32)) {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         if *[[SLATE_SLOT_7]] != *[[SLATE_SLOT_6]]
// CHECK-NEXT:             || (unsafe { (*(*[[SLATE_SLOT_7]])).f.f9 }) != ((2 as i32) as i64)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         foo(10 as i32);
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
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
// SLATE-FILECHECK-END
