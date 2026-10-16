/* Distilled from optimization/863.  */

extern void abort(void);
extern void exit(int);
extern void ok(int);

typedef struct {
  int x, y, z;
} Data;

void find(Data *first, Data *last) {
  int i;
  for (i = (last - first) >> 2; i > 0; --i)
    ok(i);
  abort();
}

void ok(int i) {
  if (i != 1)
    abort();
  exit(0);
}

int main() {
  Data DataList[4];
  find(DataList + 0, DataList + 4);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Distilled from optimization/863.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     x: i32,
// CHECK-NEXT:     y: i32,
// CHECK-NEXT:     z: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn find(mut first: *mut [[SLATERECORD_0]], mut last: *mut [[SLATERECORD_0]]) {
// CHECK-NEXT:     let mut i: i32 =
// CHECK-NEXT:         (((unsafe { last.offset_from(first as *mut [[SLATERECORD_0]]) }) as i64) >> (2 as i32)) as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i > (0 as i32) {
// CHECK-NEXT:         ok(i);
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] - (1 as i32);
// CHECK-NEXT:         i = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { abort() };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ok(mut i: i32) {
// CHECK-NEXT:     if i != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut DataList: __SlateAlign16<[[[SLATERECORD_0]]; 4]> =
// CHECK-NEXT:         __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-NEXT:     find(
// CHECK-NEXT:         unsafe { (DataList.0.as_mut_ptr() as *mut [[SLATERECORD_0]]).offset((0 as i32) as isize) },
// CHECK-NEXT:         unsafe { (DataList.0.as_mut_ptr() as *mut [[SLATERECORD_0]]).offset((4 as i32) as isize) },
// CHECK-NEXT:     );
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
