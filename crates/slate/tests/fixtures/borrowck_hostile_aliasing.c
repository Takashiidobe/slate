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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING: #![feature(c_variadic)]
// COMMON-LOWERING-NEXT: #![allow(
// COMMON-LOWERING-NEXT:     dead_code,
// COMMON-LOWERING-NEXT:     unused,
// COMMON-LOWERING-NEXT:     non_camel_case_types,
// COMMON-LOWERING-NEXT:     non_snake_case,
// COMMON-LOWERING-NEXT:     non_upper_case_globals,
// COMMON-LOWERING-NEXT:     arithmetic_overflow,
// COMMON-LOWERING-NEXT:     unconditional_panic,
// COMMON-LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-LOWERING-NEXT:     unused_comparisons
// COMMON-LOWERING-NEXT: )]
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct Buf {
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct Node {
// COMMON-LOWERING-NEXT:     val: i32,
// COMMON-LOWERING-NEXT:     next: *mut Node,
// COMMON-LOWERING-NEXT:     prev: *mut Node,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct Pair {
// COMMON-LOWERING-NEXT:     lo: i32,
// COMMON-LOWERING-NEXT:     hi: i32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: union Pun {
// COMMON-LOWERING-NEXT:     i: i32,
// COMMON-LOWERING-NEXT:     f: f32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct Tree {
// COMMON-LOWERING-NEXT:     val: i32,
// COMMON-LOWERING-NEXT:     parent: *mut Tree,
// COMMON-LOWERING-NEXT:     left: *mut Tree,
// COMMON-LOWERING-NEXT:     right: *mut Tree,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: static mut g_ptr: *mut i32 = std::ptr::null_mut();
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = alias_same_object();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = alias_struct_fields();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = circular_list();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = parent_pointer_tree();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = self_referential_struct();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = overlapping_array_pointers();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = global_alias_with_local();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = type_punning();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         printf(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn alias_same_object() -> i32 {
// COMMON-LOWERING-NEXT:     let mut x: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 5;
// COMMON-LOWERING-NEXT:     x = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     add_in_place(std::ptr::addr_of_mut!(x), std::ptr::addr_of_mut!(x));
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = x;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn alias_struct_fields() -> i32 {
// COMMON-LOWERING-NEXT:     let mut pair: Pair = Pair { lo: 0, hi: 0 };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: Pair = Pair { lo: 1, hi: 2 };
// COMMON-LOWERING-NEXT:     pair = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = pair_lo(std::ptr::addr_of_mut!(pair));
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = pair_hi(std::ptr::addr_of_mut!(pair));
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = pair.lo;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = pair.hi;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn circular_list() -> i32 {
// COMMON-LOWERING-NEXT:     let mut a: Node = Node {
// COMMON-LOWERING-NEXT:         val: 0,
// COMMON-LOWERING-NEXT:         next: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:         prev: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let mut b: Node = Node {
// COMMON-LOWERING-NEXT:         val: 0,
// COMMON-LOWERING-NEXT:         next: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:         prev: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let mut c: Node = Node {
// COMMON-LOWERING-NEXT:         val: 0,
// COMMON-LOWERING-NEXT:         next: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:         prev: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let mut cur: *mut Node = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let mut sum: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: Node = Node {
// COMMON-LOWERING-NEXT:         val: 1,
// COMMON-LOWERING-NEXT:         next: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:         prev: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     a = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: Node = Node {
// COMMON-LOWERING-NEXT:         val: 2,
// COMMON-LOWERING-NEXT:         next: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:         prev: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     b = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: Node = Node {
// COMMON-LOWERING-NEXT:         val: 3,
// COMMON-LOWERING-NEXT:         next: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:         prev: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     c = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     a.next = std::ptr::addr_of_mut!(b);
// COMMON-LOWERING-NEXT:     b.next = std::ptr::addr_of_mut!(c);
// COMMON-LOWERING-NEXT:     c.next = std::ptr::addr_of_mut!(a);
// COMMON-LOWERING-NEXT:     a.prev = std::ptr::addr_of_mut!(c);
// COMMON-LOWERING-NEXT:     b.prev = std::ptr::addr_of_mut!(a);
// COMMON-LOWERING-NEXT:     c.prev = std::ptr::addr_of_mut!(b);
// COMMON-LOWERING-NEXT:     cur = std::ptr::addr_of_mut!(a);
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     sum = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let mut i: i32 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 6;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut Node = cur;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).val };
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = sum;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 sum = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut Node = cur;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut Node = unsafe { (*{{__v[0-9]+}}).next };
// COMMON-LOWERING-NEXT:                 cur = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:             i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 10;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut Node = cur;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut Node = unsafe { (*{{__v[0-9]+}}).prev };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).val };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         (*{{__v[0-9]+}}).val = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = sum;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = a.val;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = b.val;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = c.val;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn parent_pointer_tree() -> i32 {
// COMMON-LOWERING-NEXT:     let mut root: Tree = Tree {
// COMMON-LOWERING-NEXT:         val: 0,
// COMMON-LOWERING-NEXT:         parent: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:         left: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:         right: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let mut left: Tree = Tree {
// COMMON-LOWERING-NEXT:         val: 0,
// COMMON-LOWERING-NEXT:         parent: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:         left: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:         right: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let mut right: Tree = Tree {
// COMMON-LOWERING-NEXT:         val: 0,
// COMMON-LOWERING-NEXT:         parent: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:         left: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:         right: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: Tree = Tree {
// COMMON-LOWERING-NEXT:         val: 1,
// COMMON-LOWERING-NEXT:         parent: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:         left: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:         right: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     root = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 2;
// COMMON-LOWERING-NEXT:     left.val = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     left.parent = std::ptr::addr_of_mut!(root);
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut Tree = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     left.left = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut Tree = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     left.right = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:     right.val = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     right.parent = std::ptr::addr_of_mut!(root);
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut Tree = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     right.left = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut Tree = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     right.right = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     root.left = std::ptr::addr_of_mut!(left);
// COMMON-LOWERING-NEXT:     root.right = std::ptr::addr_of_mut!(right);
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = left.val;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut Tree = left.parent;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).val };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         (*{{__v[0-9]+}}).val = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = right.val;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut Tree = right.parent;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).val };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         (*{{__v[0-9]+}}).val = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = root.val;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = left.val;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = right.val;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn self_referential_struct() -> i32 {
// COMMON-LOWERING-NEXT:     let mut buf: Buf = Buf {
// COMMON-LOWERING-NEXT:         data: [0; 8],
// COMMON-LOWERING-NEXT:         cursor: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let mut i: i32 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 8;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 97;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:             buf.data[({{__v[0-9]+}} as usize)] = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:             i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 3;
// COMMON-LOWERING-NEXT:     buf.cursor = std::ptr::addr_of_mut!(buf.data[({{__v[0-9]+}} as usize)]);
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 2;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 97;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} - {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn overlapping_array_pointers() -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: [i32; 5] = [1, 2, 3, 4, 5];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 4;
// COMMON-LOWERING-NEXT:     reverse_in_place(
// COMMON-LOWERING-NEXT:         std::ptr::addr_of_mut!(values[({{__v[0-9]+}} as usize)]),
// COMMON-LOWERING-NEXT:         std::ptr::addr_of_mut!(values[({{__v[0-9]+}} as usize)]),
// COMMON-LOWERING-NEXT:     );
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = values[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 10000;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = values[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1000;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = values[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 100;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = values[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 10;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 4;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = values[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn global_alias_with_local() -> i32 {
// COMMON-LOWERING-NEXT:     let mut x: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 7;
// COMMON-LOWERING-NEXT:     x = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     capture_global(std::ptr::addr_of_mut!(x));
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = unsafe { g_ptr };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = x;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     x = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = x;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = unsafe { g_ptr };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn type_punning() -> i32 {
// COMMON-LOWERING-NEXT:     let mut u: Pun = unsafe { std::mem::zeroed::<Pun>() };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 1.0;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         u.f = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { u.i };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { u.i };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         u.i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { u.i };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = -1;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: f32 = unsafe { u.f };
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn add_in_place({{arg[0-9]+}}: *mut i32, {{arg[0-9]+}}: *mut i32) {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *{{arg[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *{{arg[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         *{{arg[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     return;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn pair_lo({{arg[0-9]+}}: *mut Pair) -> *mut i32 {
// COMMON-LOWERING-NEXT:     return unsafe { std::ptr::addr_of_mut!((*{{arg[0-9]+}}).lo) };
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn pair_hi({{arg[0-9]+}}: *mut Pair) -> *mut i32 {
// COMMON-LOWERING-NEXT:     return unsafe { std::ptr::addr_of_mut!((*{{arg[0-9]+}}).hi) };
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn reverse_in_place({{arg[0-9]+}}: *mut i32, {{arg[0-9]+}}: *mut i32) {
// COMMON-LOWERING-NEXT:     let mut lo: *mut i32 = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let mut hi: *mut i32 = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     lo = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     hi = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: *mut i32 = lo;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: *mut i32 = hi;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut i32 = lo;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut i32 = hi;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut i32 = lo;
// COMMON-LOWERING-NEXT:                 unsafe {
// COMMON-LOWERING-NEXT:                     *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut i32 = hi;
// COMMON-LOWERING-NEXT:                 unsafe {
// COMMON-LOWERING-NEXT:                     *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut i32 = lo;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(1) };
// COMMON-LOWERING-NEXT:                 lo = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut i32 = hi;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = -1;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// COMMON-LOWERING-NEXT:                 hi = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     return;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn capture_global({{arg[0-9]+}}: *mut i32) {
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         g_ptr = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     return;
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     data: [i8; 8],
// LOWERING-X86_64-GNU-NEXT:     cursor: *mut i8,
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d %d %d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: i8 = {{__v[0-9]+}} as i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = buf.cursor;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = {{__v[0-9]+}} as i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = buf.data[({{__v[0-9]+}} as usize)];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = buf.cursor;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(0) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-X86_64-GNU-NEXT:     let mut values: aligned::Aligned<aligned::A16, [i32; 5]> = aligned::Aligned([0; 5]);
// LOWERING-X86_64-GNU-NEXT:     *values = {{__v[0-9]+}};
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     data: [u8; 8],
// LOWERING-AARCH64-GNU-NEXT:     cursor: *mut u8,
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d %d %d %d %d %d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: u8 = {{__v[0-9]+}} as u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = buf.cursor;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = {{__v[0-9]+}} as u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = buf.data[({{__v[0-9]+}} as usize)];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = buf.cursor;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(0) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-AARCH64-GNU-NEXT:     let mut values: [i32; 5] = [0; 5];
// LOWERING-AARCH64-GNU-NEXT:     values = {{__v[0-9]+}};
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES: #![feature(c_variadic)]
// COMMON-REWRITES-NEXT: #![allow(
// COMMON-REWRITES-NEXT:     dead_code,
// COMMON-REWRITES-NEXT:     unused,
// COMMON-REWRITES-NEXT:     non_camel_case_types,
// COMMON-REWRITES-NEXT:     non_snake_case,
// COMMON-REWRITES-NEXT:     non_upper_case_globals,
// COMMON-REWRITES-NEXT:     arithmetic_overflow,
// COMMON-REWRITES-NEXT:     unconditional_panic,
// COMMON-REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-REWRITES-NEXT:     unused_comparisons
// COMMON-REWRITES-NEXT: )]
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct Buf {
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct Node {
// COMMON-REWRITES-NEXT:     val: i32,
// COMMON-REWRITES-NEXT:     next: *mut Node,
// COMMON-REWRITES-NEXT:     prev: *mut Node,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct Pair {
// COMMON-REWRITES-NEXT:     lo: i32,
// COMMON-REWRITES-NEXT:     hi: i32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: union Pun {
// COMMON-REWRITES-NEXT:     i: i32,
// COMMON-REWRITES-NEXT:     f: f32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct Tree {
// COMMON-REWRITES-NEXT:     val: i32,
// COMMON-REWRITES-NEXT:     parent: *mut Tree,
// COMMON-REWRITES-NEXT:     left: *mut Tree,
// COMMON-REWRITES-NEXT:     right: *mut Tree,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: static mut g_ptr: *mut i32 = std::ptr::null_mut();
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%d %d %d %d %d %d %d %d\n".as_ptr(),
// COMMON-REWRITES-NEXT:             alias_same_object(),
// COMMON-REWRITES-NEXT:             alias_struct_fields(),
// COMMON-REWRITES-NEXT:             circular_list(),
// COMMON-REWRITES-NEXT:             parent_pointer_tree(),
// COMMON-REWRITES-NEXT:             self_referential_struct(),
// COMMON-REWRITES-NEXT:             overlapping_array_pointers(),
// COMMON-REWRITES-NEXT:             global_alias_with_local(),
// COMMON-REWRITES-NEXT:             type_punning(),
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn alias_same_object() -> i32 {
// COMMON-REWRITES-NEXT:     let mut x: i32 = 0;
// COMMON-REWRITES-NEXT:     x = 5;
// COMMON-REWRITES-NEXT:     add_in_place(unsafe { &mut (*std::ptr::addr_of_mut!(x)) }, unsafe {
// COMMON-REWRITES-NEXT:         &(*std::ptr::addr_of_mut!(x))
// COMMON-REWRITES-NEXT:     });
// COMMON-REWRITES-NEXT:     x
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn alias_struct_fields() -> i32 {
// COMMON-REWRITES-NEXT:     let mut pair: Pair = Pair { lo: 0, hi: 0 };
// COMMON-REWRITES-NEXT:     pair = Pair { lo: 1, hi: 2 };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = pair_lo(unsafe { &mut (*std::ptr::addr_of_mut!(pair)) });
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = pair_hi(unsafe { &mut (*std::ptr::addr_of_mut!(pair)) });
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         *{{__v[0-9]+}} = (unsafe { *{{__v[0-9]+}} }) + unsafe { *{{__v[0-9]+}} };
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         *{{__v[0-9]+}} = (unsafe { *{{__v[0-9]+}} }) + unsafe { *{{__v[0-9]+}} };
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     pair.lo + pair.hi
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn circular_list() -> i32 {
// COMMON-REWRITES-NEXT:     let mut a: Node = Node {
// COMMON-REWRITES-NEXT:         val: 1,
// COMMON-REWRITES-NEXT:         next: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:         prev: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let mut b: Node = Node {
// COMMON-REWRITES-NEXT:         val: 2,
// COMMON-REWRITES-NEXT:         next: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:         prev: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let mut c: Node = Node {
// COMMON-REWRITES-NEXT:         val: 3,
// COMMON-REWRITES-NEXT:         next: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:         prev: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let mut cur: *mut Node = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     let mut sum: i32 = 0;
// COMMON-REWRITES-NEXT:     a.next = std::ptr::addr_of_mut!(b);
// COMMON-REWRITES-NEXT:     b.next = std::ptr::addr_of_mut!(c);
// COMMON-REWRITES-NEXT:     c.next = std::ptr::addr_of_mut!(a);
// COMMON-REWRITES-NEXT:     a.prev = std::ptr::addr_of_mut!(c);
// COMMON-REWRITES-NEXT:     b.prev = std::ptr::addr_of_mut!(a);
// COMMON-REWRITES-NEXT:     c.prev = std::ptr::addr_of_mut!(b);
// COMMON-REWRITES-NEXT:     cur = std::ptr::addr_of_mut!(a);
// COMMON-REWRITES-NEXT:     for i in 0..6 {
// COMMON-REWRITES-NEXT:         sum += unsafe { (*cur).val };
// COMMON-REWRITES-NEXT:         cur = unsafe { (*cur).next };
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut Node = unsafe { (*cur).prev };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         (*{{__v[0-9]+}}).val = (unsafe { (*{{__v[0-9]+}}).val }) + 10;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     sum + a.val + b.val + c.val
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn parent_pointer_tree() -> i32 {
// COMMON-REWRITES-NEXT:     let mut root: Tree = Tree {
// COMMON-REWRITES-NEXT:         val: 0,
// COMMON-REWRITES-NEXT:         parent: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:         left: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:         right: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let mut left: Tree = Tree {
// COMMON-REWRITES-NEXT:         val: 0,
// COMMON-REWRITES-NEXT:         parent: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:         left: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:         right: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let mut right: Tree = Tree {
// COMMON-REWRITES-NEXT:         val: 0,
// COMMON-REWRITES-NEXT:         parent: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:         left: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:         right: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     root = Tree {
// COMMON-REWRITES-NEXT:         val: 1,
// COMMON-REWRITES-NEXT:         parent: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:         left: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:         right: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     left.val = 2;
// COMMON-REWRITES-NEXT:     left.parent = std::ptr::addr_of_mut!(root);
// COMMON-REWRITES-NEXT:     left.left = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     left.right = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     right.val = 3;
// COMMON-REWRITES-NEXT:     right.parent = std::ptr::addr_of_mut!(root);
// COMMON-REWRITES-NEXT:     right.left = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     right.right = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     root.left = std::ptr::addr_of_mut!(left);
// COMMON-REWRITES-NEXT:     root.right = std::ptr::addr_of_mut!(right);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut Tree = left.parent;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         (*{{__v[0-9]+}}).val = (unsafe { (*{{__v[0-9]+}}).val }) + left.val;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut Tree = right.parent;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         (*{{__v[0-9]+}}).val = (unsafe { (*{{__v[0-9]+}}).val }) + right.val;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     root.val + left.val + right.val
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn self_referential_struct() -> i32 {
// COMMON-REWRITES-NEXT:     let mut buf: Buf = Buf {
// COMMON-REWRITES-NEXT:         data: [0; 8],
// COMMON-REWRITES-NEXT:         cursor: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     for i in 0..8 {
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     buf.cursor = std::ptr::addr_of_mut!(buf.data[3]);
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = buf.data[3] as i32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 97;
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}} + ((unsafe { *{{__v[0-9]+}} }) as i32) - 2 * {{__v[0-9]+}}
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn overlapping_array_pointers() -> i32 {
// COMMON-REWRITES-NEXT:     reverse_in_place(
// COMMON-REWRITES-NEXT:         std::ptr::addr_of_mut!(values[0]),
// COMMON-REWRITES-NEXT:         std::ptr::addr_of_mut!(values[4]),
// COMMON-REWRITES-NEXT:     );
// COMMON-REWRITES-NEXT:     values[0] * 10000 + values[1] * 1000 + values[2] * 100 + values[3] * 10 + values[4]
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn global_alias_with_local() -> i32 {
// COMMON-REWRITES-NEXT:     let mut x: i32 = 0;
// COMMON-REWRITES-NEXT:     x = 7;
// COMMON-REWRITES-NEXT:     capture_global(std::ptr::addr_of_mut!(x));
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = unsafe { g_ptr };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         *{{__v[0-9]+}} = (unsafe { *{{__v[0-9]+}} }) + 1;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     x += 1;
// COMMON-REWRITES-NEXT:     x + unsafe { *unsafe { g_ptr } }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn type_punning() -> i32 {
// COMMON-REWRITES-NEXT:     let mut u: Pun = unsafe { std::mem::zeroed::<Pun>() };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         u.f = 1.0;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { u.i };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         u.i = (unsafe { u.i }) + 1;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} == unsafe { u.i } {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = -1;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = (unsafe { u.f }) as i32;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}}
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn add_in_place({{arg[0-9]+}}: &mut i32, {{arg[0-9]+}}: &i32) {
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         *({{arg[0-9]+}} as *mut i32) = (unsafe { *({{arg[0-9]+}} as *mut i32) }) + unsafe { *({{arg[0-9]+}} as *const i32) };
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     return;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn pair_lo({{arg[0-9]+}}: &mut Pair) -> *mut i32 {
// COMMON-REWRITES-NEXT:     unsafe { std::ptr::addr_of_mut!((*({{arg[0-9]+}} as *mut Pair)).lo) }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn pair_hi({{arg[0-9]+}}: &mut Pair) -> *mut i32 {
// COMMON-REWRITES-NEXT:     unsafe { std::ptr::addr_of_mut!((*({{arg[0-9]+}} as *mut Pair)).hi) }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn reverse_in_place(mut lo: *mut i32, mut hi: *mut i32) {
// COMMON-REWRITES-NEXT:     while lo < hi {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { *lo };
// COMMON-REWRITES-NEXT:         unsafe {
// COMMON-REWRITES-NEXT:             *lo = unsafe { *hi };
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         unsafe {
// COMMON-REWRITES-NEXT:             *hi = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut i32 = lo;
// COMMON-REWRITES-NEXT:         lo = unsafe { {{__v[0-9]+}}.add(1) };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut i32 = hi;
// COMMON-REWRITES-NEXT:         hi = unsafe { {{__v[0-9]+}}.offset((-1 as i32) as isize) };
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     return;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn capture_global({{arg[0-9]+}}: *mut i32) {
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         g_ptr = {{arg[0-9]+}};
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     return;
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT:     data: [i8; 8],
// REWRITES-X86_64-GNU-NEXT:     cursor: *mut i8,
// REWRITES-X86_64-GNU-NEXT:         buf.data[((i as i64) as usize)] = (97 + i) as i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = buf.cursor;
// REWRITES-X86_64-GNU-NEXT:         *{{__v[0-9]+}} = (((unsafe { *{{__v[0-9]+}} }) as i32) + 1) as i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = buf.cursor;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(0) };
// REWRITES-X86_64-GNU-NEXT:     let mut values: aligned::Aligned<aligned::A16, [i32; 5]> = aligned::Aligned([0; 5]);
// REWRITES-X86_64-GNU-NEXT:     *values = [1, 2, 3, 4, 5];
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT:     data: [u8; 8],
// REWRITES-AARCH64-GNU-NEXT:     cursor: *mut u8,
// REWRITES-AARCH64-GNU-NEXT:         buf.data[((i as i64) as usize)] = (97 + i) as u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = buf.cursor;
// REWRITES-AARCH64-GNU-NEXT:         *{{__v[0-9]+}} = (((unsafe { *{{__v[0-9]+}} }) as i32) + 1) as u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = buf.cursor;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(0) };
// REWRITES-AARCH64-GNU-NEXT:     let mut values: [i32; 5] = [1, 2, 3, 4, 5];
// SLATE-FILECHECK-END rewrites-aarch64-gnu
