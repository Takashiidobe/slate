#include <stdio.h>

struct Opaque;

typedef struct Point {
  int  x;
  long y;
} Point;

struct Segment {
  Point          from;
  Point          to;
  char           tag;
  double         weight;
  struct Opaque *extra;
};

struct Node {
  int          value;
  struct Node *next;
};

static long segment_length(struct Segment *s) {
  return (s->to.x - s->from.x) + (s->to.y - s->from.y);
}

static int sum_list(struct Node *node, int count) {
  int total = 0;
  while (count-- > 0) {
    total += node->value;
    node = node->next;
  }
  return total;
}

int main(void) {
  struct Segment s;
  s.from.x = 1;
  s.from.y = 2;
  s.to.x   = 10;
  s.to.y   = 20;
  s.tag    = 'a';
  s.weight = 0.5;
  printf("%ld %c %.2f\n", segment_length(&s), s.tag, s.weight);

  struct Node third;
  third.value = 3;
  third.next  = &third;
  struct Node second;
  second.value = 2;
  second.next  = &third;
  struct Node first;
  first.value = 1;
  first.next  = &second;
  first.next->next->value += 4;
  printf("%d\n", sum_list(&first, 3));
  return 0;
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
// CHECK-NEXT: struct Opaque {}
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Point {
// CHECK-NEXT:     x: i32,
// CHECK-NEXT:     y: i64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Segment {
// CHECK-NEXT:     from: Point,
// CHECK-NEXT:     to: Point,
// CHECK-NEXT:     tag: i8,
// CHECK-NEXT:     weight: f64,
// CHECK-NEXT:     extra: *mut Opaque,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Node {
// CHECK-NEXT:     value: i32,
// CHECK-NEXT:     next: *mut Node,
// CHECK-NEXT: }
// CHECK-NEXT: fn segment_length(s: &Segment) -> i64 {
// CHECK-NEXT:     (((unsafe { (*(s as *const Segment)).to.x })
// CHECK-NEXT:         - unsafe { (*(s as *const Segment)).from.x }) as i64)
// CHECK-NEXT:         + ((unsafe { (*(s as *const Segment)).to.y })
// CHECK-NEXT:             - unsafe { (*(s as *const Segment)).from.y })
// CHECK-NEXT: }
// CHECK-NEXT: fn sum_list(mut node: *mut Node, mut count: i32) -> i32 {
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: i32 = count;
// CHECK-NEXT:         count = [[V_0]] - 1;
// CHECK-NEXT:         if [[V_0]] <= 0 {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         total += unsafe { (*node).value };
// CHECK-NEXT:         node = unsafe { (*node).next };
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut s: Segment = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     s.from.x = 1;
// CHECK-NEXT:     s.from.y = 2;
// CHECK-NEXT:     s.to.x = 10;
// CHECK-NEXT:     s.to.y = 20;
// CHECK-NEXT:     s.tag = 97;
// CHECK-NEXT:     s.weight = 0.5f64;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%ld %c %.2f\n".as_ptr(),
// CHECK-NEXT:             segment_length(&(*std::ptr::addr_of_mut!(s))),
// CHECK-NEXT:             s.tag as i32,
// CHECK-NEXT:             s.weight,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut third: Node = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     third.value = 3;
// CHECK-NEXT:     third.next = std::ptr::addr_of_mut!(third);
// CHECK-NEXT:     let mut second: Node = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     second.value = 2;
// CHECK-NEXT:     second.next = std::ptr::addr_of_mut!(third);
// CHECK-NEXT:     let mut first: Node = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     first.value = 1;
// CHECK-NEXT:     first.next = std::ptr::addr_of_mut!(second);
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: *mut Node = unsafe { (*first.next).next };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*[[V_1]]).value = (*[[V_1]]).value + 4;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("{}", sum_list(std::ptr::addr_of_mut!(first), 3i32));
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
