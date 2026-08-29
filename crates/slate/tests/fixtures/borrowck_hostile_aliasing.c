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
  struct Pair pair = {1, 2};
  int        *lo   = pair_lo(&pair);
  int        *hi   = pair_hi(&pair);
  *lo += *hi;
  *hi += *lo;
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
    cur = cur->next;
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

  left.parent->val += left.val;
  right.parent->val += right.val;
  return root.val + left.val + right.val;
}

struct Buf {
  char  data[8];
  char *cursor;
};

static int self_referential_struct(void) {
  struct Buf buf;
  for (int i = 0; i < 8; i++) buf.data[i] = (char)('a' + i);
  buf.cursor = &buf.data[3];
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
  x += 1;
  return x + *g_ptr;
}

union Pun {
  int   i;
  float f;
};

static int type_punning(void) {
  union Pun u;
  u.f = 1.0f;
  int bits = u.i;
  u.i += 1;
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
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct Buf {
// LOWERING-NEXT:     data: [i8; 8],
// LOWERING-NEXT:     cursor: *mut i8,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct Node {
// LOWERING-NEXT:     val: i32,
// LOWERING-NEXT:     next: *mut Node,
// LOWERING-NEXT:     prev: *mut Node,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct Pair {
// LOWERING-NEXT:     lo: i32,
// LOWERING-NEXT:     hi: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: union Pun {
// LOWERING-NEXT:     i: i32,
// LOWERING-NEXT:     f: f32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
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
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut g_ptr: *mut i32 = std::ptr::null_mut();
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn add_in_place(arg5: *mut i32, arg6: *mut i32) {
// LOWERING-NEXT:     let mut a: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut b: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     a = arg5;
// LOWERING-NEXT:     b = arg6;
// LOWERING-NEXT:     let _v0: *mut i32 = b;
// LOWERING-NEXT:     let _v1: i32 = unsafe { *_v0 };
// LOWERING-NEXT:     let _v2: *mut i32 = a;
// LOWERING-NEXT:     let _v3: i32 = unsafe { *_v2 };
// LOWERING-NEXT:     let _v4: i32 = _v3 + _v1;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *_v2 = _v4;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn alias_same_object() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut x: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 5;
// LOWERING-NEXT:     x = _v0;
// LOWERING-NEXT:     add_in_place(std::ptr::addr_of_mut!(x), std::ptr::addr_of_mut!(x));
// LOWERING-NEXT:     let _v1: i32 = x;
// LOWERING-NEXT:     __retval = _v1;
// LOWERING-NEXT:     let _v2: i32 = __retval;
// LOWERING-NEXT:     return _v2;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn pair_lo(arg4: *mut Pair) -> *mut i32 {
// LOWERING-NEXT:     let mut p: *mut Pair = std::ptr::null_mut();
// LOWERING-NEXT:     let mut __retval: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     p = arg4;
// LOWERING-NEXT:     let _v0: *mut Pair = p;
// LOWERING-NEXT:     __retval = unsafe { std::ptr::addr_of_mut!((*_v0).lo) };
// LOWERING-NEXT:     let _v1: *mut i32 = __retval;
// LOWERING-NEXT:     return _v1;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn pair_hi(arg3: *mut Pair) -> *mut i32 {
// LOWERING-NEXT:     let mut p: *mut Pair = std::ptr::null_mut();
// LOWERING-NEXT:     let mut __retval: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     p = arg3;
// LOWERING-NEXT:     let _v0: *mut Pair = p;
// LOWERING-NEXT:     __retval = unsafe { std::ptr::addr_of_mut!((*_v0).hi) };
// LOWERING-NEXT:     let _v1: *mut i32 = __retval;
// LOWERING-NEXT:     return _v1;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn alias_struct_fields() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut pair: Pair = Pair { lo: 0, hi: 0 };
// LOWERING-NEXT:     let mut lo: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut hi: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     pair = Pair { lo: 1, hi: 2 };
// LOWERING-NEXT:     let _v0: *mut i32 = pair_lo(std::ptr::addr_of_mut!(pair));
// LOWERING-NEXT:     lo = _v0;
// LOWERING-NEXT:     let _v1: *mut i32 = pair_hi(std::ptr::addr_of_mut!(pair));
// LOWERING-NEXT:     hi = _v1;
// LOWERING-NEXT:     let _v2: *mut i32 = hi;
// LOWERING-NEXT:     let _v3: i32 = unsafe { *_v2 };
// LOWERING-NEXT:     let _v4: *mut i32 = lo;
// LOWERING-NEXT:     let _v5: i32 = unsafe { *_v4 };
// LOWERING-NEXT:     let _v6: i32 = _v5 + _v3;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *_v4 = _v6;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v7: *mut i32 = lo;
// LOWERING-NEXT:     let _v8: i32 = unsafe { *_v7 };
// LOWERING-NEXT:     let _v9: *mut i32 = hi;
// LOWERING-NEXT:     let _v10: i32 = unsafe { *_v9 };
// LOWERING-NEXT:     let _v11: i32 = _v10 + _v8;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *_v9 = _v11;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v12: i32 = pair.lo;
// LOWERING-NEXT:     let _v13: i32 = pair.hi;
// LOWERING-NEXT:     let _v14: i32 = _v12 + _v13;
// LOWERING-NEXT:     __retval = _v14;
// LOWERING-NEXT:     let _v15: i32 = __retval;
// LOWERING-NEXT:     return _v15;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn circular_list() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut a: Node = Node { val: 0, next: std::ptr::null_mut(), prev: std::ptr::null_mut() };
// LOWERING-NEXT:     let mut b: Node = Node { val: 0, next: std::ptr::null_mut(), prev: std::ptr::null_mut() };
// LOWERING-NEXT:     let mut c: Node = Node { val: 0, next: std::ptr::null_mut(), prev: std::ptr::null_mut() };
// LOWERING-NEXT:     let mut cur: *mut Node = std::ptr::null_mut();
// LOWERING-NEXT:     let mut sum: i32 = 0;
// LOWERING-NEXT:     a = Node { val: 1, next: std::ptr::null_mut(), prev: std::ptr::null_mut() };
// LOWERING-NEXT:     b = Node { val: 2, next: std::ptr::null_mut(), prev: std::ptr::null_mut() };
// LOWERING-NEXT:     c = Node { val: 3, next: std::ptr::null_mut(), prev: std::ptr::null_mut() };
// LOWERING-NEXT:     a.next = std::ptr::addr_of_mut!(b);
// LOWERING-NEXT:     b.next = std::ptr::addr_of_mut!(c);
// LOWERING-NEXT:     c.next = std::ptr::addr_of_mut!(a);
// LOWERING-NEXT:     a.prev = std::ptr::addr_of_mut!(c);
// LOWERING-NEXT:     b.prev = std::ptr::addr_of_mut!(a);
// LOWERING-NEXT:     c.prev = std::ptr::addr_of_mut!(b);
// LOWERING-NEXT:     cur = std::ptr::addr_of_mut!(a);
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     sum = _v0;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let _v1: i32 = 0;
// LOWERING-NEXT:         i = _v1;
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let _v2: i32 = i;
// LOWERING-NEXT:             let _v3: i32 = 6;
// LOWERING-NEXT:             let _v4: bool = _v2 < _v3;
// LOWERING-NEXT:             if !_v4 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let _v5: *mut Node = cur;
// LOWERING-NEXT:                 let _v6: i32 = unsafe { (*_v5).val };
// LOWERING-NEXT:                 let _v7: i32 = sum;
// LOWERING-NEXT:                 let _v8: i32 = _v7 + _v6;
// LOWERING-NEXT:                 sum = _v8;
// LOWERING-NEXT:                 let _v9: *mut Node = cur;
// LOWERING-NEXT:                 let _v10: *mut Node = unsafe { (*_v9).next };
// LOWERING-NEXT:                 cur = _v10;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let _v11: i32 = i;
// LOWERING-NEXT:             let _v12: i32 = _v11 + 1;
// LOWERING-NEXT:             i = _v12;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v13: i32 = 10;
// LOWERING-NEXT:     let _v14: *mut Node = cur;
// LOWERING-NEXT:     let _v15: *mut Node = unsafe { (*_v14).prev };
// LOWERING-NEXT:     let _v16: i32 = unsafe { (*_v15).val };
// LOWERING-NEXT:     let _v17: i32 = _v16 + _v13;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*_v15).val = _v17;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v18: i32 = sum;
// LOWERING-NEXT:     let _v19: i32 = a.val;
// LOWERING-NEXT:     let _v20: i32 = _v18 + _v19;
// LOWERING-NEXT:     let _v21: i32 = b.val;
// LOWERING-NEXT:     let _v22: i32 = _v20 + _v21;
// LOWERING-NEXT:     let _v23: i32 = c.val;
// LOWERING-NEXT:     let _v24: i32 = _v22 + _v23;
// LOWERING-NEXT:     __retval = _v24;
// LOWERING-NEXT:     let _v25: i32 = __retval;
// LOWERING-NEXT:     return _v25;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn parent_pointer_tree() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut root: Tree = Tree { val: 0, parent: std::ptr::null_mut(), left: std::ptr::null_mut(), right: std::ptr::null_mut() };
// LOWERING-NEXT:     let mut left: Tree = Tree { val: 0, parent: std::ptr::null_mut(), left: std::ptr::null_mut(), right: std::ptr::null_mut() };
// LOWERING-NEXT:     let mut right: Tree = Tree { val: 0, parent: std::ptr::null_mut(), left: std::ptr::null_mut(), right: std::ptr::null_mut() };
// LOWERING-NEXT:     root = Tree { val: 1, parent: std::ptr::null_mut(), left: std::ptr::null_mut(), right: std::ptr::null_mut() };
// LOWERING-NEXT:     let _v0: i32 = 2;
// LOWERING-NEXT:     left.val = _v0;
// LOWERING-NEXT:     left.parent = std::ptr::addr_of_mut!(root);
// LOWERING-NEXT:     let _v1: *mut Tree = std::ptr::null_mut();
// LOWERING-NEXT:     left.left = _v1;
// LOWERING-NEXT:     let _v2: *mut Tree = std::ptr::null_mut();
// LOWERING-NEXT:     left.right = _v2;
// LOWERING-NEXT:     let _v3: i32 = 3;
// LOWERING-NEXT:     right.val = _v3;
// LOWERING-NEXT:     right.parent = std::ptr::addr_of_mut!(root);
// LOWERING-NEXT:     let _v4: *mut Tree = std::ptr::null_mut();
// LOWERING-NEXT:     right.left = _v4;
// LOWERING-NEXT:     let _v5: *mut Tree = std::ptr::null_mut();
// LOWERING-NEXT:     right.right = _v5;
// LOWERING-NEXT:     root.left = std::ptr::addr_of_mut!(left);
// LOWERING-NEXT:     root.right = std::ptr::addr_of_mut!(right);
// LOWERING-NEXT:     let _v6: i32 = left.val;
// LOWERING-NEXT:     let _v7: *mut Tree = left.parent;
// LOWERING-NEXT:     let _v8: i32 = unsafe { (*_v7).val };
// LOWERING-NEXT:     let _v9: i32 = _v8 + _v6;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*_v7).val = _v9;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v10: i32 = right.val;
// LOWERING-NEXT:     let _v11: *mut Tree = right.parent;
// LOWERING-NEXT:     let _v12: i32 = unsafe { (*_v11).val };
// LOWERING-NEXT:     let _v13: i32 = _v12 + _v10;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*_v11).val = _v13;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v14: i32 = root.val;
// LOWERING-NEXT:     let _v15: i32 = left.val;
// LOWERING-NEXT:     let _v16: i32 = _v14 + _v15;
// LOWERING-NEXT:     let _v17: i32 = right.val;
// LOWERING-NEXT:     let _v18: i32 = _v16 + _v17;
// LOWERING-NEXT:     __retval = _v18;
// LOWERING-NEXT:     let _v19: i32 = __retval;
// LOWERING-NEXT:     return _v19;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn self_referential_struct() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut buf: Buf = Buf { data: [0; 8], cursor: std::ptr::null_mut() };
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let _v0: i32 = 0;
// LOWERING-NEXT:         i = _v0;
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let _v1: i32 = i;
// LOWERING-NEXT:             let _v2: i32 = 8;
// LOWERING-NEXT:             let _v3: bool = _v1 < _v2;
// LOWERING-NEXT:             if !_v3 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let _v4: i32 = 97;
// LOWERING-NEXT:             let _v5: i32 = i;
// LOWERING-NEXT:             let _v6: i32 = _v4 + _v5;
// LOWERING-NEXT:             let _v7: i8 = _v6 as i8;
// LOWERING-NEXT:             let _v8: i32 = i;
// LOWERING-NEXT:             let _v9: i64 = _v8 as i64;
// LOWERING-NEXT:             buf.data[(_v9 as usize)] = _v7;
// LOWERING-NEXT:             let _v10: i32 = i;
// LOWERING-NEXT:             let _v11: i32 = _v10 + 1;
// LOWERING-NEXT:             i = _v11;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v12: i64 = 3;
// LOWERING-NEXT:     buf.cursor = std::ptr::addr_of_mut!(buf.data[(_v12 as usize)]);
// LOWERING-NEXT:     let _v13: i32 = 1;
// LOWERING-NEXT:     let _v14: *mut i8 = buf.cursor;
// LOWERING-NEXT:     let _v15: i8 = unsafe { *_v14 };
// LOWERING-NEXT:     let _v16: i32 = _v15 as i32;
// LOWERING-NEXT:     let _v17: i32 = _v16 + _v13;
// LOWERING-NEXT:     let _v18: i8 = _v17 as i8;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *_v14 = _v18;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v19: i64 = 3;
// LOWERING-NEXT:     let _v20: i8 = buf.data[(_v19 as usize)];
// LOWERING-NEXT:     let _v21: i32 = _v20 as i32;
// LOWERING-NEXT:     let _v22: i64 = 0;
// LOWERING-NEXT:     let _v23: *mut i8 = buf.cursor;
// LOWERING-NEXT:     let _v24: *mut i8 = unsafe { _v23.add(0) };
// LOWERING-NEXT:     let _v25: i8 = unsafe { *_v24 };
// LOWERING-NEXT:     let _v26: i32 = _v25 as i32;
// LOWERING-NEXT:     let _v27: i32 = _v21 + _v26;
// LOWERING-NEXT:     let _v28: i32 = 2;
// LOWERING-NEXT:     let _v29: i32 = 97;
// LOWERING-NEXT:     let _v30: i32 = _v28 * _v29;
// LOWERING-NEXT:     let _v31: i32 = _v27 - _v30;
// LOWERING-NEXT:     __retval = _v31;
// LOWERING-NEXT:     let _v32: i32 = __retval;
// LOWERING-NEXT:     return _v32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn reverse_in_place(arg1: *mut i32, arg2: *mut i32) {
// LOWERING-NEXT:     let mut lo: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut hi: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     lo = arg1;
// LOWERING-NEXT:     hi = arg2;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let _v0: *mut i32 = lo;
// LOWERING-NEXT:             let _v1: *mut i32 = hi;
// LOWERING-NEXT:             let _v2: bool = _v0 < _v1;
// LOWERING-NEXT:             if !_v2 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let mut t: i32 = 0;
// LOWERING-NEXT:                 let _v3: *mut i32 = lo;
// LOWERING-NEXT:                 let _v4: i32 = unsafe { *_v3 };
// LOWERING-NEXT:                 t = _v4;
// LOWERING-NEXT:                 let _v5: *mut i32 = hi;
// LOWERING-NEXT:                 let _v6: i32 = unsafe { *_v5 };
// LOWERING-NEXT:                 let _v7: *mut i32 = lo;
// LOWERING-NEXT:                 unsafe {
// LOWERING-NEXT:                     *_v7 = _v6;
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 let _v8: i32 = t;
// LOWERING-NEXT:                 let _v9: *mut i32 = hi;
// LOWERING-NEXT:                 unsafe {
// LOWERING-NEXT:                     *_v9 = _v8;
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 let _v10: *mut i32 = lo;
// LOWERING-NEXT:                 let _v11: i32 = 1;
// LOWERING-NEXT:                 let _v12: *mut i32 = unsafe { _v10.add(1) };
// LOWERING-NEXT:                 lo = _v12;
// LOWERING-NEXT:                 let _v13: *mut i32 = hi;
// LOWERING-NEXT:                 let _v14: i32 = -1;
// LOWERING-NEXT:                 let _v15: *mut i32 = unsafe { _v13.offset(_v14 as isize) };
// LOWERING-NEXT:                 hi = _v15;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn overlapping_array_pointers() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut values: aligned::Aligned<aligned::A16, [i32; 5]> = aligned::Aligned([0; 5]);
// LOWERING-NEXT:     *values = [1, 2, 3, 4, 5];
// LOWERING-NEXT:     let _v0: i64 = 0;
// LOWERING-NEXT:     let _v1: i64 = 4;
// LOWERING-NEXT:     reverse_in_place(std::ptr::addr_of_mut!(values[(_v0 as usize)]), std::ptr::addr_of_mut!(values[(_v1 as usize)]));
// LOWERING-NEXT:     let _v2: i64 = 0;
// LOWERING-NEXT:     let _v3: i32 = values[(_v2 as usize)];
// LOWERING-NEXT:     let _v4: i32 = 10000;
// LOWERING-NEXT:     let _v5: i32 = _v3 * _v4;
// LOWERING-NEXT:     let _v6: i64 = 1;
// LOWERING-NEXT:     let _v7: i32 = values[(_v6 as usize)];
// LOWERING-NEXT:     let _v8: i32 = 1000;
// LOWERING-NEXT:     let _v9: i32 = _v7 * _v8;
// LOWERING-NEXT:     let _v10: i32 = _v5 + _v9;
// LOWERING-NEXT:     let _v11: i64 = 2;
// LOWERING-NEXT:     let _v12: i32 = values[(_v11 as usize)];
// LOWERING-NEXT:     let _v13: i32 = 100;
// LOWERING-NEXT:     let _v14: i32 = _v12 * _v13;
// LOWERING-NEXT:     let _v15: i32 = _v10 + _v14;
// LOWERING-NEXT:     let _v16: i64 = 3;
// LOWERING-NEXT:     let _v17: i32 = values[(_v16 as usize)];
// LOWERING-NEXT:     let _v18: i32 = 10;
// LOWERING-NEXT:     let _v19: i32 = _v17 * _v18;
// LOWERING-NEXT:     let _v20: i32 = _v15 + _v19;
// LOWERING-NEXT:     let _v21: i64 = 4;
// LOWERING-NEXT:     let _v22: i32 = values[(_v21 as usize)];
// LOWERING-NEXT:     let _v23: i32 = _v20 + _v22;
// LOWERING-NEXT:     __retval = _v23;
// LOWERING-NEXT:     let _v24: i32 = __retval;
// LOWERING-NEXT:     return _v24;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn capture_global(arg0: *mut i32) {
// LOWERING-NEXT:     let mut p: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     p = arg0;
// LOWERING-NEXT:     let _v0: *mut i32 = p;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         g_ptr = _v0;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn global_alias_with_local() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut x: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 7;
// LOWERING-NEXT:     x = _v0;
// LOWERING-NEXT:     capture_global(std::ptr::addr_of_mut!(x));
// LOWERING-NEXT:     let _v1: i32 = 1;
// LOWERING-NEXT:     let _v2: *mut i32 = unsafe { g_ptr };
// LOWERING-NEXT:     let _v3: i32 = unsafe { *_v2 };
// LOWERING-NEXT:     let _v4: i32 = _v3 + _v1;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *_v2 = _v4;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v5: i32 = 1;
// LOWERING-NEXT:     let _v6: i32 = x;
// LOWERING-NEXT:     let _v7: i32 = _v6 + _v5;
// LOWERING-NEXT:     x = _v7;
// LOWERING-NEXT:     let _v8: i32 = x;
// LOWERING-NEXT:     let _v9: *mut i32 = unsafe { g_ptr };
// LOWERING-NEXT:     let _v10: i32 = unsafe { *_v9 };
// LOWERING-NEXT:     let _v11: i32 = _v8 + _v10;
// LOWERING-NEXT:     __retval = _v11;
// LOWERING-NEXT:     let _v12: i32 = __retval;
// LOWERING-NEXT:     return _v12;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn type_punning() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut u: Pun = Pun { i: 0 };
// LOWERING-NEXT:     let mut bits: i32 = 0;
// LOWERING-NEXT:     let _v0: f32 = 1.0;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         u.f = _v0;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v1: i32 = unsafe { u.i };
// LOWERING-NEXT:     bits = _v1;
// LOWERING-NEXT:     let _v2: i32 = 1;
// LOWERING-NEXT:     let _v3: i32 = unsafe { u.i };
// LOWERING-NEXT:     let _v4: i32 = _v3 + _v2;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         u.i = _v4;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v5: i32 = bits;
// LOWERING-NEXT:     let _v6: i32 = unsafe { u.i };
// LOWERING-NEXT:     let _v7: bool = _v5 == _v6;
// LOWERING-NEXT:     let _v8: i32 = if _v7 {
// LOWERING-NEXT:         let _v9: i32 = -1;
// LOWERING-NEXT:         _v9
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v10: f32 = unsafe { u.f };
// LOWERING-NEXT:         let _v11: i32 = _v10 as i32;
// LOWERING-NEXT:         _v11
// LOWERING-NEXT:     };
// LOWERING-NEXT:     __retval = _v8;
// LOWERING-NEXT:     let _v12: i32 = __retval;
// LOWERING-NEXT:     return _v12;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = b"%d %d %d %d %d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: i32 = alias_same_object();
// LOWERING-NEXT:     let _v3: i32 = alias_struct_fields();
// LOWERING-NEXT:     let _v4: i32 = circular_list();
// LOWERING-NEXT:     let _v5: i32 = parent_pointer_tree();
// LOWERING-NEXT:     let _v6: i32 = self_referential_struct();
// LOWERING-NEXT:     let _v7: i32 = overlapping_array_pointers();
// LOWERING-NEXT:     let _v8: i32 = global_alias_with_local();
// LOWERING-NEXT:     let _v9: i32 = type_punning();
// LOWERING-NEXT:     let _v10: i32 = unsafe { printf(_v1 as *const i8, _v2, _v3, _v4, _v5, _v6, _v7, _v8, _v9) };
// LOWERING-NEXT:     let _v11: i32 = 0;
// LOWERING-NEXT:     __retval = _v11;
// LOWERING-NEXT:     let _v12: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v12 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct Buf {
// REWRITES-NEXT:     data: [i8; 8],
// REWRITES-NEXT:     cursor: *mut i8,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct Node {
// REWRITES-NEXT:     val: i32,
// REWRITES-NEXT:     next: *mut Node,
// REWRITES-NEXT:     prev: *mut Node,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct Pair {
// REWRITES-NEXT:     lo: i32,
// REWRITES-NEXT:     hi: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: union Pun {
// REWRITES-NEXT:     i: i32,
// REWRITES-NEXT:     f: f32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
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
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut g_ptr: *mut i32 = std::ptr::null_mut();
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn add_in_place(arg5: &mut i32, arg6: &i32) {
// REWRITES-NEXT: let _v2: *mut i32 = arg5;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *_v2 = (unsafe { *_v2 }) + unsafe { *((arg6 as *const i32) as *mut i32) };
// REWRITES-NEXT: }
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn alias_same_object() -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut x: i32 = 0;
// REWRITES-NEXT: x = 5;
// REWRITES-NEXT: add_in_place(unsafe { &mut (*std::ptr::addr_of_mut!(x)) }, unsafe { &(*std::ptr::addr_of_mut!(x)) });
// REWRITES-NEXT: __retval = x;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn pair_lo(arg4: &Pair) -> *mut i32 {
// REWRITES-NEXT: let mut __retval: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT: __retval = unsafe { std::ptr::addr_of_mut!((*((arg4 as *const Pair) as *mut Pair)).lo) };
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn pair_hi(arg3: &Pair) -> *mut i32 {
// REWRITES-NEXT: let mut __retval: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT: __retval = unsafe { std::ptr::addr_of_mut!((*((arg3 as *const Pair) as *mut Pair)).hi) };
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn alias_struct_fields() -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut pair: Pair = Pair { lo: 1, hi: 2 };
// REWRITES-NEXT: let mut lo: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT: let mut hi: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT: lo = pair_lo(unsafe { &(*std::ptr::addr_of_mut!(pair)) });
// REWRITES-NEXT: hi = pair_hi(unsafe { &(*std::ptr::addr_of_mut!(pair)) });
// REWRITES-NEXT: let _v4: *mut i32 = lo;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *_v4 = (unsafe { *_v4 }) + unsafe { *hi };
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v9: *mut i32 = hi;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *_v9 = (unsafe { *_v9 }) + unsafe { *lo };
// REWRITES-NEXT: }
// REWRITES-NEXT: __retval = pair.lo + pair.hi;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn circular_list() -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut a: Node = Node { val: 1, next: std::ptr::null_mut(), prev: std::ptr::null_mut() };
// REWRITES-NEXT: let mut b: Node = Node { val: 2, next: std::ptr::null_mut(), prev: std::ptr::null_mut() };
// REWRITES-NEXT: let mut c: Node = Node { val: 3, next: std::ptr::null_mut(), prev: std::ptr::null_mut() };
// REWRITES-NEXT: let mut cur: *mut Node = std::ptr::null_mut();
// REWRITES-NEXT: let mut sum: i32 = 0;
// REWRITES-NEXT: a.next = std::ptr::addr_of_mut!(b);
// REWRITES-NEXT: b.next = std::ptr::addr_of_mut!(c);
// REWRITES-NEXT: c.next = std::ptr::addr_of_mut!(a);
// REWRITES-NEXT: a.prev = std::ptr::addr_of_mut!(c);
// REWRITES-NEXT: b.prev = std::ptr::addr_of_mut!(a);
// REWRITES-NEXT: c.prev = std::ptr::addr_of_mut!(b);
// REWRITES-NEXT: cur = std::ptr::addr_of_mut!(a);
// REWRITES-NEXT: sum = 0;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut i: i32 = 0;
// REWRITES-NEXT:         i = 0;
// REWRITES-NEXT:         loop {
// REWRITES-NEXT:                     let _v3: i32 = 6;
// REWRITES-NEXT:                     if !(i < _v3) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     {
// REWRITES-NEXT:                                     sum = sum + unsafe { (*cur).val };
// REWRITES-NEXT:                                     cur = unsafe { (*cur).next };
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     i = i + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v13: i32 = 10;
// REWRITES-NEXT: let _v15: *mut Node = unsafe { (*cur).prev };
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         (*_v15).val = (unsafe { (*_v15).val }) + _v13;
// REWRITES-NEXT: }
// REWRITES-NEXT: __retval = sum + a.val + b.val + c.val;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn parent_pointer_tree() -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut root: Tree = Tree { val: 1, parent: std::ptr::null_mut(), left: std::ptr::null_mut(), right: std::ptr::null_mut() };
// REWRITES-NEXT: let mut left: Tree = Tree { val: 0, parent: std::ptr::null_mut(), left: std::ptr::null_mut(), right: std::ptr::null_mut() };
// REWRITES-NEXT: let mut right: Tree = Tree { val: 0, parent: std::ptr::null_mut(), left: std::ptr::null_mut(), right: std::ptr::null_mut() };
// REWRITES-NEXT: left.val = 2;
// REWRITES-NEXT: left.parent = std::ptr::addr_of_mut!(root);
// REWRITES-NEXT: left.left = std::ptr::null_mut();
// REWRITES-NEXT: left.right = std::ptr::null_mut();
// REWRITES-NEXT: right.val = 3;
// REWRITES-NEXT: right.parent = std::ptr::addr_of_mut!(root);
// REWRITES-NEXT: right.left = std::ptr::null_mut();
// REWRITES-NEXT: right.right = std::ptr::null_mut();
// REWRITES-NEXT: root.left = std::ptr::addr_of_mut!(left);
// REWRITES-NEXT: root.right = std::ptr::addr_of_mut!(right);
// REWRITES-NEXT: let _v7: *mut Tree = left.parent;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         (*_v7).val = (unsafe { (*_v7).val }) + left.val;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v11: *mut Tree = right.parent;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         (*_v11).val = (unsafe { (*_v11).val }) + right.val;
// REWRITES-NEXT: }
// REWRITES-NEXT: __retval = root.val + left.val + right.val;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn self_referential_struct() -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut buf: Buf = Buf { data: [0; 8], cursor: std::ptr::null_mut() };
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut i: i32 = 0;
// REWRITES-NEXT:         i = 0;
// REWRITES-NEXT:         loop {
// REWRITES-NEXT:                     let _v2: i32 = 8;
// REWRITES-NEXT:                     if !(i < _v2) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     let _v4: i32 = 97;
// REWRITES-NEXT:                     buf.data[((i as i64) as usize)] = (_v4 + i) as i8;
// REWRITES-NEXT:                     i = i + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v12: i64 = 3;
// REWRITES-NEXT: buf.cursor = std::ptr::addr_of_mut!(buf.data[(_v12 as usize)]);
// REWRITES-NEXT: let _v13: i32 = 1;
// REWRITES-NEXT: let _v14: *mut i8 = buf.cursor;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *_v14 = (((unsafe { *_v14 }) as i32) + _v13) as i8;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v19: i64 = 3;
// REWRITES-NEXT: let _v21: i32 = buf.data[(_v19 as usize)] as i32;
// REWRITES-NEXT: let _v22: i64 = 0;
// REWRITES-NEXT: let _v23: *mut i8 = buf.cursor;
// REWRITES-NEXT: let _v24: *mut i8 = unsafe { _v23.add(0) };
// REWRITES-NEXT: let _v28: i32 = 2;
// REWRITES-NEXT: let _v29: i32 = 97;
// REWRITES-NEXT: __retval = _v21 + ((unsafe { *_v24 }) as i32) - _v28 * _v29;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn reverse_in_place(arg1: *mut i32, arg2: *mut i32) {
// REWRITES-NEXT: let mut lo: *mut i32 = arg1;
// REWRITES-NEXT: let mut hi: *mut i32 = arg2;
// REWRITES-NEXT: loop {
// REWRITES-NEXT:         if !(lo < hi) {
// REWRITES-NEXT:                     break;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         {
// REWRITES-NEXT:                     let mut t: i32 = 0;
// REWRITES-NEXT:                     t = unsafe { *lo };
// REWRITES-NEXT:                     unsafe {
// REWRITES-NEXT:                                     *lo = unsafe { *hi };
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     unsafe {
// REWRITES-NEXT:                                     *hi = t;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     let _v10: *mut i32 = lo;
// REWRITES-NEXT:                     let _v11: i32 = 1;
// REWRITES-NEXT:                     lo = unsafe { _v10.add(1) };
// REWRITES-NEXT:                     let _v13: *mut i32 = hi;
// REWRITES-NEXT:                     let _v14: i32 = -1;
// REWRITES-NEXT:                     hi = unsafe { _v13.offset(_v14 as isize) };
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn overlapping_array_pointers() -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut values: aligned::Aligned<aligned::A16, [i32; 5]> = aligned::Aligned([0; 5]);
// REWRITES-NEXT: *values = [1, 2, 3, 4, 5];
// REWRITES-NEXT: let _v0: i64 = 0;
// REWRITES-NEXT: let _v1: i64 = 4;
// REWRITES-NEXT: reverse_in_place(std::ptr::addr_of_mut!(values[(_v0 as usize)]), std::ptr::addr_of_mut!(values[(_v1 as usize)]));
// REWRITES-NEXT: let _v2: i64 = 0;
// REWRITES-NEXT: let _v4: i32 = 10000;
// REWRITES-NEXT: let _v6: i64 = 1;
// REWRITES-NEXT: let _v8: i32 = 1000;
// REWRITES-NEXT: let _v11: i64 = 2;
// REWRITES-NEXT: let _v13: i32 = 100;
// REWRITES-NEXT: let _v16: i64 = 3;
// REWRITES-NEXT: let _v18: i32 = 10;
// REWRITES-NEXT: let _v21: i64 = 4;
// REWRITES-NEXT: __retval = values[(_v2 as usize)] * _v4 + values[(_v6 as usize)] * _v8 + values[(_v11 as usize)] * _v13 + values[(_v16 as usize)] * _v18 + values[(_v21 as usize)];
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn capture_global(arg0: &i32) {
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         g_ptr = (arg0 as *const i32) as *mut i32;
// REWRITES-NEXT: }
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn global_alias_with_local() -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut x: i32 = 0;
// REWRITES-NEXT: x = 7;
// REWRITES-NEXT: capture_global(unsafe { &(*std::ptr::addr_of_mut!(x)) });
// REWRITES-NEXT: let _v1: i32 = 1;
// REWRITES-NEXT: let _v2: *mut i32 = unsafe { g_ptr };
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *_v2 = (unsafe { *_v2 }) + _v1;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v5: i32 = 1;
// REWRITES-NEXT: x = x + _v5;
// REWRITES-NEXT: __retval = x + unsafe { *unsafe { g_ptr } };
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn type_punning() -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut u: Pun = Pun { i: 0 };
// REWRITES-NEXT: let mut bits: i32 = 0;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         u.f = 1.0;
// REWRITES-NEXT: }
// REWRITES-NEXT: bits = unsafe { u.i };
// REWRITES-NEXT: let _v2: i32 = 1;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         u.i = (unsafe { u.i }) + _v2;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v8: i32 = if bits == unsafe { u.i } {
// REWRITES-NEXT:         let _v9: i32 = -1;
// REWRITES-NEXT:     _v9
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v11: i32 = (unsafe { u.f }) as i32;
// REWRITES-NEXT:     _v11
// REWRITES-NEXT: };
// REWRITES-NEXT: __retval = _v8;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: *mut i8 = b"%d %d %d %d %d %d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: i32 = alias_same_object();
// REWRITES-NEXT: let _v3: i32 = alias_struct_fields();
// REWRITES-NEXT: let _v4: i32 = circular_list();
// REWRITES-NEXT: let _v5: i32 = parent_pointer_tree();
// REWRITES-NEXT: let _v6: i32 = self_referential_struct();
// REWRITES-NEXT: let _v7: i32 = overlapping_array_pointers();
// REWRITES-NEXT: let _v8: i32 = global_alias_with_local();
// REWRITES-NEXT: let _v9: i32 = type_punning();
// REWRITES-NEXT: let _v10: i32 = unsafe { printf(_v1 as *const i8, _v2, _v3, _v4, _v5, _v6, _v7, _v8, _v9) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
