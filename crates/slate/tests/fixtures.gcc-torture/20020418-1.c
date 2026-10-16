/* ifcvt accidentally deletes a referenced label while generating
   conditional traps on machines having such patterns */

void exit(int);

struct foo {
  int a;
};

void gcc_crash(struct foo *p) {
  if (__builtin_expect(p->a < 52, 0))
    __builtin_trap();
top:
  p->a++;
  if (p->a >= 62)
    goto top;
}

int main(void) {
  struct foo x;

  x.a = 53;
  gcc_crash(&x);

  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(link_llvm_intrinsics)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! ifcvt accidentally deletes a referenced label while generating
// CHECK-NEXT: //! conditional traps on machines having such patterns
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT:     fn __builtin_expect([[V_1:__v[0-9]+]]: i64, [[V_2:__v[0-9]+]]: i64) -> i64;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "llvm-intrinsic" {
// CHECK-NEXT:     #[link_name = "llvm.trap"]
// CHECK-NEXT:     fn __slate_intrinsic_46d8fd79c9d1f86c();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct foo {
// CHECK-NEXT:     a: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn gcc_crash(mut p: *mut foo) {
// CHECK-NEXT:     let mut [[SLATE_STORAGE_0:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_0:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_0]]) as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_1:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_1:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_1]]) as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_2:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*mut foo> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_2:__slate_slot_[0-9]+]]: *mut *mut foo =
// CHECK-NEXT:         std::ptr::addr_of_mut!([[SLATE_STORAGE_2]]) as *mut *mut foo;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         '[[JOIN_0:__join_[0-9]+]]: {
// CHECK-NEXT:             if (((unsafe { (*p).a }) < (52 as i32)) as i64) != (0 as i64) {
// CHECK-NEXT:                 unsafe { __slate_intrinsic_46d8fd79c9d1f86c() };
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         loop {
// CHECK-NEXT:             std::ptr::write([[SLATE_SLOT_2]], p);
// CHECK-NEXT:             std::ptr::write([[SLATE_SLOT_1]], unsafe { (*(*[[SLATE_SLOT_2]])).a });
// CHECK-NEXT:             std::ptr::write([[SLATE_SLOT_0]], *[[SLATE_SLOT_1]] + (1 as i32));
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*(*[[SLATE_SLOT_2]])).a = *[[SLATE_SLOT_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             if !((unsafe { (*p).a }) >= (62 as i32)) {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: foo = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     x.a = 53 as i32;
// CHECK-NEXT:     gcc_crash(std::ptr::addr_of_mut!(x));
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
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
