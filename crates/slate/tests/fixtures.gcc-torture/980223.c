void abort(void);

typedef struct {
  char *addr;
  long  type;
} object;

object bar(object blah) { abort(); }

object foo(object x, object y) {
  object z = *(object *)(x.addr);
  if (z.type & 64) {
    y = *(object *)(z.addr + sizeof(object));
    z = *(object *)(z.addr);
    if (z.type & 64)
      y = bar(y);
  }
  return y;
}

int    nil;
object cons1[2] = {{(char *)&nil, 0}, {(char *)&nil, 0}};
object cons2[2] = {{(char *)&cons1, 64}, {(char *)&nil, 0}};

int main(void) {
  object x     = {(char *)&cons2, 64};
  object y     = {(char *)&nil, 0};
  object three = foo(x, y);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     addr: *mut i8,
// CHECK-NEXT:     r#type: i64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn bar(mut blah: [[SLATERECORD_0]]) -> [[SLATERECORD_0]] {
// CHECK-NEXT:     unsafe { abort() };
// CHECK-NEXT:     return unsafe { std::mem::zeroed() };
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut x: [[SLATERECORD_0]], mut y: [[SLATERECORD_0]]) -> [[SLATERECORD_0]] {
// CHECK-NEXT:     let mut z: [[SLATERECORD_0]] = unsafe { *(x.addr as *mut [[SLATERECORD_0]]) };
// CHECK-NEXT:     if z.r#type & ((64 as i32) as i64) != (0 as i64) {
// CHECK-NEXT:         y = unsafe {
// CHECK-NEXT:             *((unsafe { z.addr.offset((16 as u64) as isize) }) as *mut [[SLATERECORD_0]])
// CHECK-NEXT:         };
// CHECK-NEXT:         z = unsafe { *(z.addr as *mut [[SLATERECORD_0]]) };
// CHECK-NEXT:         if z.r#type & ((64 as i32) as i64) != (0 as i64) {
// CHECK-NEXT:             y = bar(y);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     return y;
// CHECK-NEXT: }
// CHECK-NEXT: static mut nil: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut cons1: __SlateAlign16<[[[SLATERECORD_0]]; 2]> = __SlateAlign16([
// CHECK-NEXT:     [[SLATERECORD_0]] {
// CHECK-NEXT:         addr: (unsafe { std::ptr::addr_of_mut!(nil) }) as *mut i8,
// CHECK-NEXT:         r#type: (0 as i32) as i64,
// CHECK-NEXT:     },
// CHECK-NEXT:     [[SLATERECORD_0]] {
// CHECK-NEXT:         addr: (unsafe { std::ptr::addr_of_mut!(nil) }) as *mut i8,
// CHECK-NEXT:         r#type: (0 as i32) as i64,
// CHECK-NEXT:     },
// CHECK-NEXT: ]);
// CHECK-NEXT: static mut cons2: __SlateAlign16<[[[SLATERECORD_0]]; 2]> = __SlateAlign16([
// CHECK-NEXT:     [[SLATERECORD_0]] {
// CHECK-NEXT:         addr: (unsafe { std::ptr::addr_of_mut!(cons1.0) }) as *mut i8,
// CHECK-NEXT:         r#type: (64 as i32) as i64,
// CHECK-NEXT:     },
// CHECK-NEXT:     [[SLATERECORD_0]] {
// CHECK-NEXT:         addr: (unsafe { std::ptr::addr_of_mut!(nil) }) as *mut i8,
// CHECK-NEXT:         r#type: (0 as i32) as i64,
// CHECK-NEXT:     },
// CHECK-NEXT: ]);
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: [[SLATERECORD_0]] = [[SLATERECORD_0]] {
// CHECK-NEXT:         addr: (unsafe { std::ptr::addr_of_mut!(cons2.0) }) as *mut i8,
// CHECK-NEXT:         r#type: (64 as i32) as i64,
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut y: [[SLATERECORD_0]] = [[SLATERECORD_0]] {
// CHECK-NEXT:         addr: (unsafe { std::ptr::addr_of_mut!(nil) }) as *mut i8,
// CHECK-NEXT:         r#type: (0 as i32) as i64,
// CHECK-NEXT:     };
// CHECK-NEXT:     foo(x, y);
// CHECK-NEXT:     0 as i32
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
