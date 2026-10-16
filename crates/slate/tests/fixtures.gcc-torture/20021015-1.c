/* PR opt/7409.  */

extern void abort(void);

char g_list[] = {'1'};

void g(void *p, char *list, int length, char **elementPtr, char **nextPtr) {
  if (*nextPtr != g_list)
    abort();

  **nextPtr = 0;
}

int main(void) {
  char *list = g_list;
  char *element;
  int   i, length = 100;

  for (i = 0; *list != 0; i++) {
    char *prevList = list;
    g(0, list, length, &element, &list);
    length -= (list - prevList);
  }

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR opt/7409.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut g_list: [i8; 1] = [(49 as i32) as i8];
// CHECK-EMPTY:
// CHECK-NEXT: fn g(
// CHECK-NEXT:     mut p: *mut (),
// CHECK-NEXT:     mut list: *mut i8,
// CHECK-NEXT:     mut length: i32,
// CHECK-NEXT:     mut elementPtr: &*mut i8,
// CHECK-NEXT:     mut nextPtr: &*mut i8,
// CHECK-NEXT: ) {
// CHECK-NEXT:     if (unsafe { *(nextPtr as *const *mut i8) })
// CHECK-NEXT:         != unsafe { std::ptr::addr_of_mut!(g_list) as *mut i8 }
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { *(nextPtr as *const *mut i8) } = (0 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut list: *mut i8 = unsafe { std::ptr::addr_of_mut!(g_list) as *mut i8 };
// CHECK-NEXT:     let mut element: *mut i8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut length: i32 = 100 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while ((unsafe { *list }) as i32) != (0 as i32) {
// CHECK-NEXT:         let mut prevList: *mut i8 = list;
// CHECK-NEXT:         g(
// CHECK-NEXT:             std::ptr::null_mut::<()>(),
// CHECK-NEXT:             list,
// CHECK-NEXT:             length,
// CHECK-NEXT:             unsafe { &(*std::ptr::addr_of_mut!(element)) },
// CHECK-NEXT:             unsafe { &(*std::ptr::addr_of_mut!(list)) },
// CHECK-NEXT:         );
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = length;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 =
// CHECK-NEXT:             (([[V_0]] as i64) - ((unsafe { list.offset_from(prevList as *mut i8) }) as i64)) as i32;
// CHECK-NEXT:         length = [[V_1]];
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         i = [[V_3]];
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
