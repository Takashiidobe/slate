#include <stddef.h>
#include <stdio.h>

struct Node {
  int value;
  struct Node *next;
};

struct Entry {
  const char *key;
  int *slot;
};

static struct Node *head = NULL;
static const char *last_key = 0;
void *opaque = (void *)0;
static int *table[3] = {NULL, 0, NULL};

static struct Node *find(struct Node *list, int value) {
  for (struct Node *node = list; node != NULL; node = node->next) {
    if (node->value == value)
      return node;
  }
  return NULL;
}

static int length(const struct Node *list) {
  int count = 0;
  while (list) {
    count++;
    list = list->next;
  }
  return count;
}

static const char *name_or_default(const char *name) { return name ? name : "default"; }

int main(void) {
  struct Node c = {3, NULL};
  struct Node b = {2, &c};
  struct Node a = {1, &b};
  int stored = 7;
  struct Entry entries[2] = {{"present", &stored}, {NULL, NULL}};
  int missing = find(&a, 9) == NULL;
  int found = find(&a, 2) != 0;
  head = &a;
  int *cursor = NULL;
  if (!cursor)
    cursor = entries[0].slot;
  printf("%d %d %d %d\n", length(head), missing, found, *cursor);
  printf("%s %s\n", name_or_default(entries[0].key), name_or_default(entries[1].key));
  printf("%d %d %d\n", last_key == NULL, opaque == NULL, table[1] == NULL);
  head = NULL;
  last_key = entries[0].key;
  printf("%d %s\n", length(head), last_key);
  return entries[1].slot == NULL ? 0 : 1;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Node {
// CHECK-NEXT:     value: i32,
// CHECK-NEXT:     next: *mut Node,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Entry {
// CHECK-NEXT:     key: *const i8,
// CHECK-NEXT:     slot: *mut i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut head: *mut Node = std::ptr::null_mut::<Node>();
// CHECK-NEXT: static mut last_key: *const i8 = std::ptr::null::<i8>();
// CHECK-NEXT: static mut opaque: *mut () = std::ptr::null_mut::<()>();
// CHECK-NEXT: static mut table: __SlateAlign16<[*mut i32; 3]> = __SlateAlign16([
// CHECK-NEXT:     std::ptr::null_mut::<i32>(),
// CHECK-NEXT:     std::ptr::null_mut::<i32>(),
// CHECK-NEXT:     std::ptr::null_mut::<i32>(),
// CHECK-NEXT: ]);
// CHECK-NEXT: fn find(mut node: *mut Node, value: i32) -> *mut Node {
// CHECK-NEXT:     while !node.is_null() {
// CHECK-NEXT:         if (unsafe { (*node).value }) == value {
// CHECK-NEXT:             return node;
// CHECK-NEXT:         }
// CHECK-NEXT:         node = unsafe { (*node).next };
// CHECK-NEXT:     }
// CHECK-NEXT:     std::ptr::null_mut::<Node>()
// CHECK-NEXT: }
// CHECK-NEXT: fn length(mut list: *const Node) -> i32 {
// CHECK-NEXT:     let mut count: i32 = 0;
// CHECK-NEXT:     while !list.is_null() {
// CHECK-NEXT:         count += 1;
// CHECK-NEXT:         list = (unsafe { (*list).next }) as *const Node;
// CHECK-NEXT:     }
// CHECK-NEXT:     count
// CHECK-NEXT: }
// CHECK-NEXT: fn name_or_default(name: *const i8) -> *const i8 {
// CHECK-NEXT:     if !name.is_null() { name } else { c"default".as_ptr() }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut c: Node = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: Node = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].value = 3 as i32;
// CHECK-NEXT:         [[T_0]].next = std::ptr::null_mut::<Node>();
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut b: Node = {
// CHECK-NEXT:         let mut [[T_1:__t[0-9]+]]: Node = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]].value = 2 as i32;
// CHECK-NEXT:         [[T_1]].next = std::ptr::addr_of_mut!(c);
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut a: Node = {
// CHECK-NEXT:         let mut [[T_2:__t[0-9]+]]: Node = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_2]].value = 1 as i32;
// CHECK-NEXT:         [[T_2]].next = std::ptr::addr_of_mut!(b);
// CHECK-NEXT:         [[T_2]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut stored: i32 = 7;
// CHECK-NEXT:     let entries: __SlateAlign16<[Entry; 2]> = __SlateAlign16([
// CHECK-NEXT:         Entry {
// CHECK-NEXT:             key: c"present".as_ptr(),
// CHECK-NEXT:             slot: std::ptr::addr_of_mut!(stored),
// CHECK-NEXT:         },
// CHECK-NEXT:         Entry {
// CHECK-NEXT:             key: std::ptr::null::<i8>(),
// CHECK-NEXT:             slot: std::ptr::null_mut::<i32>(),
// CHECK-NEXT:         },
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let missing: i32 = find(std::ptr::addr_of_mut!(a), 9 as i32).is_null() as i32;
// CHECK-NEXT:     let found: i32 = !find(std::ptr::addr_of_mut!(a), 2 as i32).is_null() as i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         head = std::ptr::addr_of_mut!(a);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut cursor: *mut i32 = std::ptr::null_mut::<i32>();
// CHECK-NEXT:     if cursor.is_null() {
// CHECK-NEXT:         cursor = entries.0[0].slot;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {}", length((unsafe { head }) as * const Node), missing, found, unsafe
// CHECK-NEXT:         { * cursor }
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%s %s\n".as_ptr(),
// CHECK-NEXT:             name_or_default(entries.0[0].key),
// CHECK-NEXT:             name_or_default(entries.0[1].key),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}", unsafe { last_key } .is_null() as i32, unsafe { opaque } .is_null()
// CHECK-NEXT:         as i32, unsafe { table.0[1] } .is_null() as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         head = std::ptr::null_mut::<Node>();
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         last_key = entries.0[0].key;
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%d %s\n".as_ptr(), length(head as *const Node), last_key) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     if entries.0[1].slot.is_null() { 0 } else { 1 }
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
