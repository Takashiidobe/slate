#include <stdio.h>

struct node {
  unsigned next;
  int value;
};

static struct node nodes[4] = {{1, 10}, {2, 20}, {3, 30}, {0, 40}};
static struct node *table[4] = {&nodes[0], &nodes[1], &nodes[2], &nodes[3]};

static void unlink_after(struct node *target) {
  struct node *prev, *cur = table[0];
  unsigned i = 0;
  if (cur == target)
    return;
  for (;;) {
    prev = cur;
    i = cur->next;
    cur = table[i];
    if (cur == target) {
      prev->value = cur->value + 1;
      break;
    }
  }
}

int main(void) {
  unlink_after(&nodes[2]);
  printf("%d %d %d %d\n", nodes[0].value, nodes[1].value, nodes[2].value, nodes[3].value);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct node {
// CHECK-NEXT:     next: u32,
// CHECK-NEXT:     value: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut nodes: __SlateAlign16<[node; 4]> = __SlateAlign16([
// CHECK-NEXT:     node { next: 1u32, value: 10i32 },
// CHECK-NEXT:     node { next: 2u32, value: 20i32 },
// CHECK-NEXT:     node { next: 3u32, value: 30i32 },
// CHECK-NEXT:     node { next: 0u32, value: 40i32 },
// CHECK-NEXT: ]);
// CHECK-NEXT: static mut table: __SlateAlign16<[*mut node; 4]> = __SlateAlign16([
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(nodes.0) as *mut node }.offset(0i32 as isize)
// CHECK-NEXT:     },
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(nodes.0) as *mut node }.offset(1i32 as isize)
// CHECK-NEXT:     },
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(nodes.0) as *mut node }.offset(2i32 as isize)
// CHECK-NEXT:     },
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(nodes.0) as *mut node }.offset(3i32 as isize)
// CHECK-NEXT:     },
// CHECK-NEXT: ]);
// CHECK-NEXT: fn unlink_after(target: *mut node) {
// CHECK-NEXT:     let mut cur: *mut node = unsafe { table.0[0] };
// CHECK-NEXT:     let mut i: u32 = 0;
// CHECK-NEXT:     if cur == target {
// CHECK-NEXT:         return;
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let prev: *mut node = cur;
// CHECK-NEXT:         i = unsafe { (*cur).next };
// CHECK-NEXT:         cur = unsafe { table.0[i as usize] };
// CHECK-NEXT:         if cur == target {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*prev).value = (*cur).value + 1;
// CHECK-NEXT:             }
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unlink_after(unsafe { (std::ptr::addr_of_mut!(nodes.0) as *mut node).add(2) });
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {}", unsafe { nodes.0[0].value }, unsafe { nodes.0[1].value }, unsafe {
// CHECK-NEXT:         nodes.0[2].value }, unsafe { nodes.0[3].value }
// CHECK-NEXT:     );
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
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
