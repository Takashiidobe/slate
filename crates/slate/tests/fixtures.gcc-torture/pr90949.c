void __attribute__((noipa, noinline)) my_puts(const char *str) {}

void __attribute__((noipa, noinline)) my_free(void *p) {}

struct Node {
  struct Node *child;
};

struct Node space[2] = {};

struct Node *__attribute__((noipa, noinline)) my_malloc(int bytes) {
  return &space[0];
}

void walk(struct Node *module, int cleanup) {
  if (module == 0) {
    return;
  }
  if (!cleanup) {
    my_puts("No cleanup");
  }
  walk(module->child, cleanup);
  if (cleanup) {
    my_free(module);
  }
}

int main() {
  struct Node *node = my_malloc(sizeof(struct Node));
  node->child       = 0;
  walk(node, 1);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: fn my_puts(mut str: *const i8) {}
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn my_free(mut p: *mut ()) {}
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Node {
// CHECK-NEXT:     child: *mut Node,
// CHECK-NEXT: }
// CHECK-NEXT: static mut space: __SlateAlign16<[Node; 2]> = __SlateAlign16({
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: [Node; 2] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: });
// CHECK-NEXT: fn my_malloc(mut bytes: i32) -> *mut Node {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(space.0) as *mut Node }
// CHECK-NEXT:             .offset((0 as i32) as isize)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn walk(mut module: *mut Node, mut cleanup: i32) {
// CHECK-NEXT:     if module == std::ptr::null_mut::<Node>() {
// CHECK-NEXT:         return;
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(cleanup != (0 as i32)) {
// CHECK-NEXT:         my_puts(c"No cleanup".as_ptr() as *const i8);
// CHECK-NEXT:     }
// CHECK-NEXT:     walk(unsafe { (*module).child }, cleanup);
// CHECK-NEXT:     if cleanup != (0 as i32) {
// CHECK-NEXT:         my_free(module as *mut ());
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut node: *mut Node = my_malloc(((8 as u64) as u32) as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*node).child = std::ptr::null_mut::<Node>();
// CHECK-NEXT:     }
// CHECK-NEXT:     walk(node, 1 as i32);
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
