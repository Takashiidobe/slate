#include <stdio.h>

static void add_in_place(int *a, int *b) { *a += *b; }

static int alias_same_object(void) {
  int x = 5;
  add_in_place(&x, &x);
  return x;
}

struct Pair {
  int lo;
  int hi;
};

static int *pair_lo(struct Pair *p) { return &p->lo; }
static int *pair_hi(struct Pair *p) { return &p->hi; }

static int alias_struct_fields(void) {
  struct Pair pair  = {1, 2};
  int        *lo    = pair_lo(&pair);
  int        *hi    = pair_hi(&pair);
  *lo              += *hi;
  *hi              += *lo;
  return pair.lo + pair.hi;
}

struct Node {
  int          val;
  struct Node *next;
  struct Node *prev;
};

static int circular_list(void) {
  struct Node a = {1, 0, 0};
  struct Node b = {2, 0, 0};
  struct Node c = {3, 0, 0};

  a.next = &b;
  b.next = &c;
  c.next = &a;
  a.prev = &c;
  b.prev = &a;
  c.prev = &b;

  struct Node *cur = &a;
  int          sum = 0;
  for (int i = 0; i < 6; i++) {
    sum += cur->val;
    cur  = cur->next;
  }
  cur->prev->val += 10;
  return sum + a.val + b.val + c.val;
}

struct Tree {
  int          val;
  struct Tree *parent;
  struct Tree *left;
  struct Tree *right;
};

static int parent_pointer_tree(void) {
  struct Tree root  = {1, 0, 0, 0};
  struct Tree left  = {2, &root, 0, 0};
  struct Tree right = {3, &root, 0, 0};
  root.left         = &left;
  root.right        = &right;

  left.parent->val  += left.val;
  right.parent->val += right.val;
  return root.val + left.val + right.val;
}

struct Buf {
  char  data[8];
  char *cursor;
};

static int self_referential_struct(void) {
  struct Buf buf;
  for (int i = 0; i < 8; i++)
    buf.data[i] = (char)('a' + i);
  buf.cursor   = &buf.data[3];
  *buf.cursor += 1;
  return buf.data[3] + buf.cursor[0] - 2 * 'a';
}

static void reverse_in_place(int *lo, int *hi) {
  while (lo < hi) {
    int t = *lo;
    *lo   = *hi;
    *hi   = t;
    lo++;
    hi--;
  }
}

static int overlapping_array_pointers(void) {
  int values[5] = {1, 2, 3, 4, 5};
  reverse_in_place(&values[0], &values[4]);
  return values[0] * 10000 + values[1] * 1000 + values[2] * 100 +
         values[3] * 10 + values[4];
}

static int *g_ptr;

static void capture_global(int *p) { g_ptr = p; }

static int global_alias_with_local(void) {
  int x = 7;
  capture_global(&x);
  *g_ptr += 1;
  x      += 1;
  return x + *g_ptr;
}

union Pun {
  int   i;
  float f;
};

static int type_punning(void) {
  union Pun u;
  u.f       = 1.0f;
  int bits  = u.i;
  u.i      += 1;
  return (bits == u.i) ? -1 : (int)u.f;
}

int main(void) {
  printf("%d %d %d %d %d %d %d %d\n", alias_same_object(),
         alias_struct_fields(), circular_list(), parent_pointer_tree(),
         self_referential_struct(), overlapping_array_pointers(),
         global_alias_with_local(), type_punning());
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(
// LOWERING-NEXT:     dead_code,
// LOWERING-NEXT:     unused,
// LOWERING-NEXT:     non_camel_case_types,
// LOWERING-NEXT:     non_snake_case,
// LOWERING-NEXT:     non_upper_case_globals,
// LOWERING-NEXT:     arithmetic_overflow,
// LOWERING-NEXT:     unconditional_panic,
// LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-NEXT:     unused_comparisons
// LOWERING-NEXT: )]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct Buf {
// LOWERING-NEXT:     data: [i8; 8],
// LOWERING-NEXT:     cursor: *mut i8,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct Node {
// LOWERING-NEXT:     val: i32,
// LOWERING-NEXT:     next: *mut Node,
// LOWERING-NEXT:     prev: *mut Node,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct Pair {
// LOWERING-NEXT:     lo: i32,
// LOWERING-NEXT:     hi: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: union Pun {
// LOWERING-NEXT:     i: i32,
// LOWERING-NEXT:     f: f32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct Tree {
// LOWERING-NEXT:     val: i32,
// LOWERING-NEXT:     parent: *mut Tree,
// LOWERING-NEXT:     left: *mut Tree,
// LOWERING-NEXT:     right: *mut Tree,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut g_ptr: *mut i32 = std::ptr::null_mut();
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d %d %d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = alias_same_object();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = alias_struct_fields();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = circular_list();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = parent_pointer_tree();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = self_referential_struct();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = overlapping_array_pointers();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = global_alias_with_local();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = type_punning();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         printf(
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn alias_same_object() -> i32 {
// LOWERING-NEXT:     let mut x: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 5;
// LOWERING-NEXT:     x = {{_v[0-9]+}};
// LOWERING-NEXT:     add_in_place(std::ptr::addr_of_mut!(x), std::ptr::addr_of_mut!(x));
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = x;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn alias_struct_fields() -> i32 {
// LOWERING-NEXT:     let mut pair: Pair = Pair { lo: 0, hi: 0 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: Pair = Pair { lo: 1, hi: 2 };
// LOWERING-NEXT:     pair = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = pair_lo(std::ptr::addr_of_mut!(pair));
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = pair_hi(std::ptr::addr_of_mut!(pair));
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = pair.lo;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = pair.hi;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn circular_list() -> i32 {
// LOWERING-NEXT:     let mut a: Node = Node {
// LOWERING-NEXT:         val: 0,
// LOWERING-NEXT:         next: std::ptr::null_mut(),
// LOWERING-NEXT:         prev: std::ptr::null_mut(),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut b: Node = Node {
// LOWERING-NEXT:         val: 0,
// LOWERING-NEXT:         next: std::ptr::null_mut(),
// LOWERING-NEXT:         prev: std::ptr::null_mut(),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut c: Node = Node {
// LOWERING-NEXT:         val: 0,
// LOWERING-NEXT:         next: std::ptr::null_mut(),
// LOWERING-NEXT:         prev: std::ptr::null_mut(),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut cur: *mut Node = std::ptr::null_mut();
// LOWERING-NEXT:     let mut sum: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: Node = Node {
// LOWERING-NEXT:         val: 1,
// LOWERING-NEXT:         next: std::ptr::null_mut(),
// LOWERING-NEXT:         prev: std::ptr::null_mut(),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     a = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: Node = Node {
// LOWERING-NEXT:         val: 2,
// LOWERING-NEXT:         next: std::ptr::null_mut(),
// LOWERING-NEXT:         prev: std::ptr::null_mut(),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     b = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: Node = Node {
// LOWERING-NEXT:         val: 3,
// LOWERING-NEXT:         next: std::ptr::null_mut(),
// LOWERING-NEXT:         prev: std::ptr::null_mut(),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     c = {{_v[0-9]+}};
// LOWERING-NEXT:     a.next = std::ptr::addr_of_mut!(b);
// LOWERING-NEXT:     b.next = std::ptr::addr_of_mut!(c);
// LOWERING-NEXT:     c.next = std::ptr::addr_of_mut!(a);
// LOWERING-NEXT:     a.prev = std::ptr::addr_of_mut!(c);
// LOWERING-NEXT:     b.prev = std::ptr::addr_of_mut!(a);
// LOWERING-NEXT:     c.prev = std::ptr::addr_of_mut!(b);
// LOWERING-NEXT:     cur = std::ptr::addr_of_mut!(a);
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     sum = {{_v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         i = {{_v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 6;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut Node = cur;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { (*{{_v[0-9]+}}).val };
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = sum;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                 sum = {{_v[0-9]+}};
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut Node = cur;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut Node = unsafe { (*{{_v[0-9]+}}).next };
// LOWERING-NEXT:                 cur = {{_v[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:             i = {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut Node = cur;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut Node = unsafe { (*{{_v[0-9]+}}).prev };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { (*{{_v[0-9]+}}).val };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*{{_v[0-9]+}}).val = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = sum;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = a.val;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = b.val;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = c.val;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn parent_pointer_tree() -> i32 {
// LOWERING-NEXT:     let mut root: Tree = Tree {
// LOWERING-NEXT:         val: 0,
// LOWERING-NEXT:         parent: std::ptr::null_mut(),
// LOWERING-NEXT:         left: std::ptr::null_mut(),
// LOWERING-NEXT:         right: std::ptr::null_mut(),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut left: Tree = Tree {
// LOWERING-NEXT:         val: 0,
// LOWERING-NEXT:         parent: std::ptr::null_mut(),
// LOWERING-NEXT:         left: std::ptr::null_mut(),
// LOWERING-NEXT:         right: std::ptr::null_mut(),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut right: Tree = Tree {
// LOWERING-NEXT:         val: 0,
// LOWERING-NEXT:         parent: std::ptr::null_mut(),
// LOWERING-NEXT:         left: std::ptr::null_mut(),
// LOWERING-NEXT:         right: std::ptr::null_mut(),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: Tree = Tree {
// LOWERING-NEXT:         val: 1,
// LOWERING-NEXT:         parent: std::ptr::null_mut(),
// LOWERING-NEXT:         left: std::ptr::null_mut(),
// LOWERING-NEXT:         right: std::ptr::null_mut(),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     root = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:     left.val = {{_v[0-9]+}};
// LOWERING-NEXT:     left.parent = std::ptr::addr_of_mut!(root);
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut Tree = std::ptr::null_mut();
// LOWERING-NEXT:     left.left = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut Tree = std::ptr::null_mut();
// LOWERING-NEXT:     left.right = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     right.val = {{_v[0-9]+}};
// LOWERING-NEXT:     right.parent = std::ptr::addr_of_mut!(root);
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut Tree = std::ptr::null_mut();
// LOWERING-NEXT:     right.left = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut Tree = std::ptr::null_mut();
// LOWERING-NEXT:     right.right = {{_v[0-9]+}};
// LOWERING-NEXT:     root.left = std::ptr::addr_of_mut!(left);
// LOWERING-NEXT:     root.right = std::ptr::addr_of_mut!(right);
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = left.val;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut Tree = left.parent;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { (*{{_v[0-9]+}}).val };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*{{_v[0-9]+}}).val = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = right.val;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut Tree = right.parent;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { (*{{_v[0-9]+}}).val };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*{{_v[0-9]+}}).val = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = root.val;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = left.val;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = right.val;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn self_referential_struct() -> i32 {
// LOWERING-NEXT:     let mut buf: Buf = Buf {
// LOWERING-NEXT:         data: [0; 8],
// LOWERING-NEXT:         cursor: std::ptr::null_mut(),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         i = {{_v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 8;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 97;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:             let {{_v[0-9]+}}: i8 = {{_v[0-9]+}} as i8;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:             buf.data[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:             i = {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     buf.cursor = std::ptr::addr_of_mut!(buf.data[({{_v[0-9]+}} as usize)]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = buf.cursor;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = {{_v[0-9]+}} as i8;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = buf.data[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = buf.cursor;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.add(0) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 97;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn overlapping_array_pointers() -> i32 {
// LOWERING-NEXT:     let mut values: aligned::Aligned<aligned::A16, [i32; 5]> = aligned::Aligned([0; 5]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: [i32; 5] = [1, 2, 3, 4, 5];
// LOWERING-NEXT:     *values = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 4;
// LOWERING-NEXT:     reverse_in_place(
// LOWERING-NEXT:         std::ptr::addr_of_mut!(values[({{_v[0-9]+}} as usize)]),
// LOWERING-NEXT:         std::ptr::addr_of_mut!(values[({{_v[0-9]+}} as usize)]),
// LOWERING-NEXT:     );
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = values[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 10000;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = values[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1000;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = values[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 100;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = values[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = values[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn global_alias_with_local() -> i32 {
// LOWERING-NEXT:     let mut x: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 7;
// LOWERING-NEXT:     x = {{_v[0-9]+}};
// LOWERING-NEXT:     capture_global(std::ptr::addr_of_mut!(x));
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { g_ptr };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = x;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     x = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = x;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { g_ptr };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn type_punning() -> i32 {
// LOWERING-NEXT:     let mut u: Pun = unsafe { std::mem::zeroed::<Pun>() };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 1.0;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         u.f = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { u.i };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { u.i };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         u.i = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { u.i };
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = -1;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: f32 = unsafe { u.f };
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn add_in_place({{arg[0-9]+}}: *mut i32, {{arg[0-9]+}}: *mut i32) {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{arg[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{arg[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{arg[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn pair_lo({{arg[0-9]+}}: *mut Pair) -> *mut i32 {
// LOWERING-NEXT:     return unsafe { std::ptr::addr_of_mut!((*{{arg[0-9]+}}).lo) };
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn pair_hi({{arg[0-9]+}}: *mut Pair) -> *mut i32 {
// LOWERING-NEXT:     return unsafe { std::ptr::addr_of_mut!((*{{arg[0-9]+}}).hi) };
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn reverse_in_place({{arg[0-9]+}}: *mut i32, {{arg[0-9]+}}: *mut i32) {
// LOWERING-NEXT:     let mut lo: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut hi: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     lo = {{arg[0-9]+}};
// LOWERING-NEXT:     hi = {{arg[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut i32 = lo;
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut i32 = hi;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut i32 = lo;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut i32 = hi;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut i32 = lo;
// LOWERING-NEXT:                 unsafe {
// LOWERING-NEXT:                     *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut i32 = hi;
// LOWERING-NEXT:                 unsafe {
// LOWERING-NEXT:                     *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut i32 = lo;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(1) };
// LOWERING-NEXT:                 lo = {{_v[0-9]+}};
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut i32 = hi;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = -1;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.offset({{_v[0-9]+}} as isize) };
// LOWERING-NEXT:                 hi = {{_v[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn capture_global({{arg[0-9]+}}: *mut i32) {
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         g_ptr = {{arg[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(
// REWRITES-NEXT:     dead_code,
// REWRITES-NEXT:     unused,
// REWRITES-NEXT:     non_camel_case_types,
// REWRITES-NEXT:     non_snake_case,
// REWRITES-NEXT:     non_upper_case_globals,
// REWRITES-NEXT:     arithmetic_overflow,
// REWRITES-NEXT:     unconditional_panic,
// REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-NEXT:     unused_comparisons
// REWRITES-NEXT: )]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct Buf {
// REWRITES-NEXT:     data: [i8; 8],
// REWRITES-NEXT:     cursor: *mut i8,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct Node {
// REWRITES-NEXT:     val: i32,
// REWRITES-NEXT:     next: *mut Node,
// REWRITES-NEXT:     prev: *mut Node,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct Pair {
// REWRITES-NEXT:     lo: i32,
// REWRITES-NEXT:     hi: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: union Pun {
// REWRITES-NEXT:     i: i32,
// REWRITES-NEXT:     f: f32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct Tree {
// REWRITES-NEXT:     val: i32,
// REWRITES-NEXT:     parent: *mut Tree,
// REWRITES-NEXT:     left: *mut Tree,
// REWRITES-NEXT:     right: *mut Tree,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut g_ptr: *mut i32 = std::ptr::null_mut();
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%d %d %d %d %d %d %d %d\n".as_ptr(),
// REWRITES-NEXT:             alias_same_object(),
// REWRITES-NEXT:             alias_struct_fields(),
// REWRITES-NEXT:             circular_list(),
// REWRITES-NEXT:             parent_pointer_tree(),
// REWRITES-NEXT:             self_referential_struct(),
// REWRITES-NEXT:             overlapping_array_pointers(),
// REWRITES-NEXT:             global_alias_with_local(),
// REWRITES-NEXT:             type_punning(),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn alias_same_object() -> i32 {
// REWRITES-NEXT:     let mut x: i32 = 0;
// REWRITES-NEXT:     x = 5;
// REWRITES-NEXT:     add_in_place(unsafe { &mut (*std::ptr::addr_of_mut!(x)) }, unsafe {
// REWRITES-NEXT:         &(*std::ptr::addr_of_mut!(x))
// REWRITES-NEXT:     });
// REWRITES-NEXT:     x
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn alias_struct_fields() -> i32 {
// REWRITES-NEXT:     let mut pair: Pair = Pair { lo: 0, hi: 0 };
// REWRITES-NEXT:     pair = Pair { lo: 1, hi: 2 };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = pair_lo(unsafe { &mut (*std::ptr::addr_of_mut!(pair)) });
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = pair_hi(unsafe { &mut (*std::ptr::addr_of_mut!(pair)) });
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         *{{_v[0-9]+}} = (unsafe { *{{_v[0-9]+}} }) + unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         *{{_v[0-9]+}} = (unsafe { *{{_v[0-9]+}} }) + unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     pair.lo + pair.hi
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn circular_list() -> i32 {
// REWRITES-NEXT:     let mut a: Node = Node {
// REWRITES-NEXT:         val: 1,
// REWRITES-NEXT:         next: std::ptr::null_mut(),
// REWRITES-NEXT:         prev: std::ptr::null_mut(),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut b: Node = Node {
// REWRITES-NEXT:         val: 2,
// REWRITES-NEXT:         next: std::ptr::null_mut(),
// REWRITES-NEXT:         prev: std::ptr::null_mut(),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut c: Node = Node {
// REWRITES-NEXT:         val: 3,
// REWRITES-NEXT:         next: std::ptr::null_mut(),
// REWRITES-NEXT:         prev: std::ptr::null_mut(),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut cur: *mut Node = std::ptr::null_mut();
// REWRITES-NEXT:     let mut sum: i32 = 0;
// REWRITES-NEXT:     a.next = std::ptr::addr_of_mut!(b);
// REWRITES-NEXT:     b.next = std::ptr::addr_of_mut!(c);
// REWRITES-NEXT:     c.next = std::ptr::addr_of_mut!(a);
// REWRITES-NEXT:     a.prev = std::ptr::addr_of_mut!(c);
// REWRITES-NEXT:     b.prev = std::ptr::addr_of_mut!(a);
// REWRITES-NEXT:     c.prev = std::ptr::addr_of_mut!(b);
// REWRITES-NEXT:     cur = std::ptr::addr_of_mut!(a);
// REWRITES-NEXT:     for i in 0..6 {
// REWRITES-NEXT:         sum += unsafe { (*cur).val };
// REWRITES-NEXT:         cur = unsafe { (*cur).next };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut Node = unsafe { (*cur).prev };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*{{_v[0-9]+}}).val = (unsafe { (*{{_v[0-9]+}}).val }) + 10;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     sum + a.val + b.val + c.val
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn parent_pointer_tree() -> i32 {
// REWRITES-NEXT:     let mut root: Tree = Tree {
// REWRITES-NEXT:         val: 0,
// REWRITES-NEXT:         parent: std::ptr::null_mut(),
// REWRITES-NEXT:         left: std::ptr::null_mut(),
// REWRITES-NEXT:         right: std::ptr::null_mut(),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut left: Tree = Tree {
// REWRITES-NEXT:         val: 0,
// REWRITES-NEXT:         parent: std::ptr::null_mut(),
// REWRITES-NEXT:         left: std::ptr::null_mut(),
// REWRITES-NEXT:         right: std::ptr::null_mut(),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut right: Tree = Tree {
// REWRITES-NEXT:         val: 0,
// REWRITES-NEXT:         parent: std::ptr::null_mut(),
// REWRITES-NEXT:         left: std::ptr::null_mut(),
// REWRITES-NEXT:         right: std::ptr::null_mut(),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     root = Tree {
// REWRITES-NEXT:         val: 1,
// REWRITES-NEXT:         parent: std::ptr::null_mut(),
// REWRITES-NEXT:         left: std::ptr::null_mut(),
// REWRITES-NEXT:         right: std::ptr::null_mut(),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     left.val = 2;
// REWRITES-NEXT:     left.parent = std::ptr::addr_of_mut!(root);
// REWRITES-NEXT:     left.left = std::ptr::null_mut();
// REWRITES-NEXT:     left.right = std::ptr::null_mut();
// REWRITES-NEXT:     right.val = 3;
// REWRITES-NEXT:     right.parent = std::ptr::addr_of_mut!(root);
// REWRITES-NEXT:     right.left = std::ptr::null_mut();
// REWRITES-NEXT:     right.right = std::ptr::null_mut();
// REWRITES-NEXT:     root.left = std::ptr::addr_of_mut!(left);
// REWRITES-NEXT:     root.right = std::ptr::addr_of_mut!(right);
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut Tree = left.parent;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*{{_v[0-9]+}}).val = (unsafe { (*{{_v[0-9]+}}).val }) + left.val;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut Tree = right.parent;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*{{_v[0-9]+}}).val = (unsafe { (*{{_v[0-9]+}}).val }) + right.val;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     root.val + left.val + right.val
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn self_referential_struct() -> i32 {
// REWRITES-NEXT:     let mut buf: Buf = Buf {
// REWRITES-NEXT:         data: [0; 8],
// REWRITES-NEXT:         cursor: std::ptr::null_mut(),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     for i in 0..8 {
// REWRITES-NEXT:         buf.data[((i as i64) as usize)] = (97 + i) as i8;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     buf.cursor = std::ptr::addr_of_mut!(buf.data[3]);
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = buf.cursor;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         *{{_v[0-9]+}} = (((unsafe { *{{_v[0-9]+}} }) as i32) + 1) as i8;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = buf.data[3] as i32;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = buf.cursor;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.add(0) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 97;
// REWRITES-NEXT:     {{_v[0-9]+}} + ((unsafe { *{{_v[0-9]+}} }) as i32) - 2 * {{_v[0-9]+}}
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn overlapping_array_pointers() -> i32 {
// REWRITES-NEXT:     let mut values: aligned::Aligned<aligned::A16, [i32; 5]> = aligned::Aligned([0; 5]);
// REWRITES-NEXT:     *values = [1, 2, 3, 4, 5];
// REWRITES-NEXT:     reverse_in_place(
// REWRITES-NEXT:         std::ptr::addr_of_mut!(values[0]),
// REWRITES-NEXT:         std::ptr::addr_of_mut!(values[4]),
// REWRITES-NEXT:     );
// REWRITES-NEXT:     values[0] * 10000 + values[1] * 1000 + values[2] * 100 + values[3] * 10 + values[4]
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn global_alias_with_local() -> i32 {
// REWRITES-NEXT:     let mut x: i32 = 0;
// REWRITES-NEXT:     x = 7;
// REWRITES-NEXT:     capture_global(std::ptr::addr_of_mut!(x));
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { g_ptr };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         *{{_v[0-9]+}} = (unsafe { *{{_v[0-9]+}} }) + 1;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     x += 1;
// REWRITES-NEXT:     x + unsafe { *unsafe { g_ptr } }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn type_punning() -> i32 {
// REWRITES-NEXT:     let mut u: Pun = unsafe { std::mem::zeroed::<Pun>() };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         u.f = 1.0;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { u.i };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         u.i = (unsafe { u.i }) + 1;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} == unsafe { u.i } {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = -1;
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     } else {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = (unsafe { u.f }) as i32;
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     };
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn add_in_place({{arg[0-9]+}}: &mut i32, {{arg[0-9]+}}: &i32) {
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         *({{arg[0-9]+}} as *mut i32) = (unsafe { *({{arg[0-9]+}} as *mut i32) }) + unsafe { *({{arg[0-9]+}} as *const i32) };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn pair_lo({{arg[0-9]+}}: &mut Pair) -> *mut i32 {
// REWRITES-NEXT:     unsafe { std::ptr::addr_of_mut!((*({{arg[0-9]+}} as *mut Pair)).lo) }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn pair_hi({{arg[0-9]+}}: &mut Pair) -> *mut i32 {
// REWRITES-NEXT:     unsafe { std::ptr::addr_of_mut!((*({{arg[0-9]+}} as *mut Pair)).hi) }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn reverse_in_place(mut lo: *mut i32, mut hi: *mut i32) {
// REWRITES-NEXT:     while lo < hi {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = unsafe { *lo };
// REWRITES-NEXT:         unsafe {
// REWRITES-NEXT:             *lo = unsafe { *hi };
// REWRITES-NEXT:         }
// REWRITES-NEXT:         unsafe {
// REWRITES-NEXT:             *hi = {{_v[0-9]+}};
// REWRITES-NEXT:         }
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut i32 = lo;
// REWRITES-NEXT:         lo = unsafe { {{_v[0-9]+}}.add(1) };
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut i32 = hi;
// REWRITES-NEXT:         hi = unsafe { {{_v[0-9]+}}.offset((-1 as i32) as isize) };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn capture_global({{arg[0-9]+}}: *mut i32) {
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         g_ptr = {{arg[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
