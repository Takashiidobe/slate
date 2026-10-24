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

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn add_in_place(a: &mut i32, b: &i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *a = *a + *(b as *const i32);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn alias_same_object() -> i32 {
// CHECK-NEXT:     let mut x: i32 = 5;
// CHECK-NEXT:     add_in_place(
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(x)) },
// CHECK-NEXT:         unsafe { &(*std::ptr::addr_of_mut!(x)) },
// CHECK-NEXT:     );
// CHECK-NEXT:     x
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Pair {
// CHECK-NEXT:     lo: i32,
// CHECK-NEXT:     hi: i32,
// CHECK-NEXT: }
// CHECK-NEXT: fn pair_lo(p: &mut Pair) -> *mut i32 {
// CHECK-NEXT:     unsafe { std::ptr::addr_of_mut!((* (p as * mut Pair)).lo) }
// CHECK-NEXT: }
// CHECK-NEXT: fn pair_hi(p: &mut Pair) -> *mut i32 {
// CHECK-NEXT:     unsafe { std::ptr::addr_of_mut!((* (p as * mut Pair)).hi) }
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn alias_struct_fields() -> i32 {
// CHECK-NEXT:     let mut pair: Pair = Pair { lo: 1, hi: 2 };
// CHECK-NEXT:     let lo: *mut i32 = pair_lo(unsafe { &mut (*std::ptr::addr_of_mut!(pair)) });
// CHECK-NEXT:     let hi: *mut i32 = pair_hi(unsafe { &mut (*std::ptr::addr_of_mut!(pair)) });
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *lo = *lo + *hi;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *hi = *hi + *lo;
// CHECK-NEXT:     }
// CHECK-NEXT:     pair.lo + pair.hi
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Node {
// CHECK-NEXT:     val: i32,
// CHECK-NEXT:     next: *mut Node,
// CHECK-NEXT:     prev: *mut Node,
// CHECK-NEXT: }
// CHECK-NEXT: fn circular_list() -> i32 {
// CHECK-NEXT:     let mut a: Node = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: Node = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].val = 1i32;
// CHECK-NEXT:         [[T_0]].next = std::ptr::null_mut::<Node>();
// CHECK-NEXT:         [[T_0]].prev = std::ptr::null_mut::<Node>();
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut b: Node = {
// CHECK-NEXT:         let mut [[T_1:__t[0-9]+]]: Node = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]].val = 2i32;
// CHECK-NEXT:         [[T_1]].next = std::ptr::null_mut::<Node>();
// CHECK-NEXT:         [[T_1]].prev = std::ptr::null_mut::<Node>();
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut c: Node = {
// CHECK-NEXT:         let mut [[T_2:__t[0-9]+]]: Node = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_2]].val = 3i32;
// CHECK-NEXT:         [[T_2]].next = std::ptr::null_mut::<Node>();
// CHECK-NEXT:         [[T_2]].prev = std::ptr::null_mut::<Node>();
// CHECK-NEXT:         [[T_2]]
// CHECK-NEXT:     };
// CHECK-NEXT:     a.next = std::ptr::addr_of_mut!(b);
// CHECK-NEXT:     b.next = std::ptr::addr_of_mut!(c);
// CHECK-NEXT:     c.next = std::ptr::addr_of_mut!(a);
// CHECK-NEXT:     a.prev = std::ptr::addr_of_mut!(c);
// CHECK-NEXT:     b.prev = std::ptr::addr_of_mut!(a);
// CHECK-NEXT:     c.prev = std::ptr::addr_of_mut!(b);
// CHECK-NEXT:     let mut cur: *mut Node = std::ptr::addr_of_mut!(a);
// CHECK-NEXT:     let mut sum: i32 = 0;
// CHECK-NEXT:     for i in 0i32..6i32 {
// CHECK-NEXT:         sum += unsafe { (*cur).val };
// CHECK-NEXT:         cur = unsafe { (*cur).next };
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: *mut Node = unsafe { (*cur).prev };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*[[V_0]]).val = (*[[V_0]]).val + 10;
// CHECK-NEXT:     }
// CHECK-NEXT:     sum + a.val + b.val + c.val
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Tree {
// CHECK-NEXT:     val: i32,
// CHECK-NEXT:     parent: *mut Tree,
// CHECK-NEXT:     left: *mut Tree,
// CHECK-NEXT:     right: *mut Tree,
// CHECK-NEXT: }
// CHECK-NEXT: fn parent_pointer_tree() -> i32 {
// CHECK-NEXT:     let mut root: Tree = {
// CHECK-NEXT:         let mut [[T_0]]: Tree = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].val = 1i32;
// CHECK-NEXT:         [[T_0]].parent = std::ptr::null_mut::<Tree>();
// CHECK-NEXT:         [[T_0]].left = std::ptr::null_mut::<Tree>();
// CHECK-NEXT:         [[T_0]].right = std::ptr::null_mut::<Tree>();
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut left: Tree = {
// CHECK-NEXT:         let mut [[T_1]]: Tree = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]].val = 2i32;
// CHECK-NEXT:         [[T_1]].parent = std::ptr::addr_of_mut!(root);
// CHECK-NEXT:         [[T_1]].left = std::ptr::null_mut::<Tree>();
// CHECK-NEXT:         [[T_1]].right = std::ptr::null_mut::<Tree>();
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut right: Tree = {
// CHECK-NEXT:         let mut [[T_2]]: Tree = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_2]].val = 3i32;
// CHECK-NEXT:         [[T_2]].parent = std::ptr::addr_of_mut!(root);
// CHECK-NEXT:         [[T_2]].left = std::ptr::null_mut::<Tree>();
// CHECK-NEXT:         [[T_2]].right = std::ptr::null_mut::<Tree>();
// CHECK-NEXT:         [[T_2]]
// CHECK-NEXT:     };
// CHECK-NEXT:     root.left = std::ptr::addr_of_mut!(left);
// CHECK-NEXT:     root.right = std::ptr::addr_of_mut!(right);
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: *mut Tree = left.parent;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*[[V_1]]).val = (*[[V_1]]).val + left.val;
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_2:__v[0-9]+]]: *mut Tree = right.parent;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*[[V_2]]).val = (*[[V_2]]).val + right.val;
// CHECK-NEXT:     }
// CHECK-NEXT:     root.val + left.val + right.val
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Buf {
// CHECK-NEXT:     data: [i8; 8],
// CHECK-NEXT:     cursor: *mut i8,
// CHECK-NEXT: }
// CHECK-NEXT: fn self_referential_struct() -> i32 {
// CHECK-NEXT:     let mut buf: Buf = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     for i in 0i32..8i32 {
// CHECK-NEXT:         buf.data[i as usize] = (97 + i) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     buf.cursor = unsafe { (buf.data.as_mut_ptr() as *mut i8).add(3) };
// CHECK-NEXT:     let [[V_3:__v[0-9]+]]: *mut i8 = buf.cursor;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_3]] = ((*[[V_3]] as i32) + 1) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     (buf.data[3] as i32) + ((unsafe { *buf.cursor.add(0) }) as i32) - 2 * 97
// CHECK-NEXT: }
// CHECK-NEXT: fn reverse_in_place(mut lo: *mut i32, mut hi: *mut i32) {
// CHECK-NEXT:     while lo < hi {
// CHECK-NEXT:         let t: i32 = unsafe { *lo };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *lo = *hi;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *hi = t;
// CHECK-NEXT:         }
// CHECK-NEXT:         let [[V_4:__v[0-9]+]]: *mut i32 = lo;
// CHECK-NEXT:         lo = unsafe { [[V_4]].add(1) };
// CHECK-NEXT:         let [[V_5:__v[0-9]+]]: *mut i32 = hi;
// CHECK-NEXT:         hi = unsafe { [[V_5]].sub(1) };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn overlapping_array_pointers() -> i32 {
// CHECK-NEXT:     let mut values: __SlateAlign16<[i32; 5]> = __SlateAlign16([
// CHECK-NEXT:         1i32, 2i32, 3i32, 4i32, 5i32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     reverse_in_place(
// CHECK-NEXT:         unsafe { (values.0.as_mut_ptr() as *mut i32).add(0) },
// CHECK-NEXT:         unsafe { (values.0.as_mut_ptr() as *mut i32).add(4) },
// CHECK-NEXT:     );
// CHECK-NEXT:     values.0[0] * 10000 + values.0[1] * 1000 + values.0[2] * 100 + values.0[3] * 10
// CHECK-NEXT:         + values.0[4]
// CHECK-NEXT: }
// CHECK-NEXT: static mut g_ptr: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn capture_global(p: *mut i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         g_ptr = p;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn global_alias_with_local() -> i32 {
// CHECK-NEXT:     let mut x: i32 = 7;
// CHECK-NEXT:     capture_global(std::ptr::addr_of_mut!(x));
// CHECK-NEXT:     let [[V_6:__v[0-9]+]]: *mut i32 = unsafe { g_ptr };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_6]] = *[[V_6]] + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     x += 1;
// CHECK-NEXT:     x + unsafe { *g_ptr }
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union Pun {
// CHECK-NEXT:     i: i32,
// CHECK-NEXT:     f: f32,
// CHECK-NEXT: }
// CHECK-NEXT: fn type_punning() -> i32 {
// CHECK-NEXT:     let mut u: Pun = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         u.f = 1.0f32;
// CHECK-NEXT:     }
// CHECK-NEXT:     let bits: i32 = unsafe { u.i };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         u.i = u.i + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     if bits == unsafe { u.i } { -1 } else { (unsafe { u.f }) as i32 }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {} {} {} {} {}", alias_same_object(), alias_struct_fields(),
// CHECK-NEXT:         circular_list(), parent_pointer_tree(), self_referential_struct(),
// CHECK-NEXT:         overlapping_array_pointers(), global_alias_with_local(), type_punning()
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
