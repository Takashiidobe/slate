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


// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type[[TYPE_Pair:[0-9]+]] Pair = struct {
// DEFAULT-NEXT:         field0 lo: i32;
// DEFAULT-NEXT:         field1 hi: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_Node:[0-9]+]] Node = struct {
// DEFAULT-NEXT:         field0 val: i32;
// DEFAULT-NEXT:         field1 next: ptr<@type[[TYPE_Node]]>;
// DEFAULT-NEXT:         field2 prev: ptr<@type[[TYPE_Node]]>;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     type @type[[TYPE_Tree:[0-9]+]] Tree = struct {
// DEFAULT-NEXT:         field0 val: i32;
// DEFAULT-NEXT:         field1 parent: ptr<@type[[TYPE_Tree]]>;
// DEFAULT-NEXT:         field2 left: ptr<@type[[TYPE_Tree]]>;
// DEFAULT-NEXT:         field3 right: ptr<@type[[TYPE_Tree]]>;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16, 24]];
// DEFAULT-NEXT:     type @type[[TYPE_Buf:[0-9]+]] Buf = struct {
// DEFAULT-NEXT:         field0 data: array<i8, 8>;
// DEFAULT-NEXT:         field1 cursor: ptr<i8>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_Pun:[0-9]+]] Pun = union {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:         field1 f: f32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     global %[[VALUE_g_ptr:[0-9]+]] g_ptr: ptr<i32> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 25> [storage=static] = code_units<array<i8, 25>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_add_in_place:[0-9]+]] @add_in_place(%[[VALUE_a:[0-9]+]] a: ptr<i32>, %[[VALUE_b:[0-9]+]] b: ptr<i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_a]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%[[VALUE0]])));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), read<i32>(deref(read<ptr<i32>>(%[[VALUE_b]]))));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE0]])), read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alias_same_object:[0-9]+]] @alias_same_object() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: i32 [storage=automatic] = const<i32>(5);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, ptr<i32>) -> void>(%[[VALUE_add_in_place]], addr_of<ptr<i32>>(%[[VALUE_x]]), addr_of<ptr<i32>>(%[[VALUE_x]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_pair_lo:[0-9]+]] @pair_lo(%[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_Pair]]>) -> ptr<i32> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return addr_of<ptr<i32>>(field0(deref(read<ptr<@type[[TYPE_Pair]]>>(%[[VALUE_p]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_pair_hi:[0-9]+]] @pair_hi(%[[VALUE_p_2:[0-9]+]] p: ptr<@type[[TYPE_Pair]]>) -> ptr<i32> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return addr_of<ptr<i32>>(field1(deref(read<ptr<@type[[TYPE_Pair]]>>(%[[VALUE_p_2]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alias_struct_fields:[0-9]+]] @alias_struct_fields() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_pair:[0-9]+]] pair: @type[[TYPE_Pair]] [storage=automatic] = aggregate<@type[[TYPE_Pair]], zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(2));
// DEFAULT-NEXT:         let %[[VALUE_lo:[0-9]+]] lo: ptr<i32> [storage=automatic] = call<ptr<i32>, signature=fn(ptr<@type[[TYPE_Pair]]>) -> ptr<i32>>(%[[VALUE_pair_lo]], addr_of<ptr<@type[[TYPE_Pair]]>>(%[[VALUE_pair]]));
// DEFAULT-NEXT:         let %[[VALUE_hi:[0-9]+]] hi: ptr<i32> [storage=automatic] = call<ptr<i32>, signature=fn(ptr<@type[[TYPE_Pair]]>) -> ptr<i32>>(%[[VALUE_pair_hi]], addr_of<ptr<@type[[TYPE_Pair]]>>(%[[VALUE_pair]]));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_lo]]);
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%[[VALUE3]])));
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), read<i32>(deref(read<ptr<i32>>(%[[VALUE_hi]]))));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE3]])), read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_hi]]);
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%[[VALUE6]])));
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE7]]), read<i32>(deref(read<ptr<i32>>(%[[VALUE_lo]]))));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE6]])), read<i32>(%[[VALUE8]]));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(field0(%[[VALUE_pair]])), read<i32>(field1(%[[VALUE_pair]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_circular_list:[0-9]+]] @circular_list() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_a_2:[0-9]+]] a: @type[[TYPE_Node]] [storage=automatic] = aggregate<@type[[TYPE_Node]], zero_fill=false>(field0 = const<i32>(1), field1 = null<ptr<@type[[TYPE_Node]]>>, field2 = null<ptr<@type[[TYPE_Node]]>>);
// DEFAULT-NEXT:         let %[[VALUE_b_2:[0-9]+]] b: @type[[TYPE_Node]] [storage=automatic] = aggregate<@type[[TYPE_Node]], zero_fill=false>(field0 = const<i32>(2), field1 = null<ptr<@type[[TYPE_Node]]>>, field2 = null<ptr<@type[[TYPE_Node]]>>);
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: @type[[TYPE_Node]] [storage=automatic] = aggregate<@type[[TYPE_Node]], zero_fill=false>(field0 = const<i32>(3), field1 = null<ptr<@type[[TYPE_Node]]>>, field2 = null<ptr<@type[[TYPE_Node]]>>);
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_Node]]>>(field1(%[[VALUE_a_2]]), addr_of<ptr<@type[[TYPE_Node]]>>(%[[VALUE_b_2]]));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_Node]]>>(field1(%[[VALUE_b_2]]), addr_of<ptr<@type[[TYPE_Node]]>>(%[[VALUE_c]]));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_Node]]>>(field1(%[[VALUE_c]]), addr_of<ptr<@type[[TYPE_Node]]>>(%[[VALUE_a_2]]));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_Node]]>>(field2(%[[VALUE_a_2]]), addr_of<ptr<@type[[TYPE_Node]]>>(%[[VALUE_c]]));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_Node]]>>(field2(%[[VALUE_b_2]]), addr_of<ptr<@type[[TYPE_Node]]>>(%[[VALUE_a_2]]));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_Node]]>>(field2(%[[VALUE_c]]), addr_of<ptr<@type[[TYPE_Node]]>>(%[[VALUE_b_2]]));
// DEFAULT-NEXT:         let %[[VALUE_cur:[0-9]+]] cur: ptr<@type[[TYPE_Node]]> [storage=automatic] = addr_of<ptr<@type[[TYPE_Node]]>>(%[[VALUE_a_2]]);
// DEFAULT-NEXT:         let %[[VALUE_sum:[0-9]+]] sum: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %[[VALUE9:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(6))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE10:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE11:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE10]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE11]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE12:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_sum]]);
// DEFAULT-NEXT:                     let %[[VALUE13:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE12]]), read<i32>(field0(deref(read<ptr<@type[[TYPE_Node]]>>(%[[VALUE_cur]])))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_sum]], read<i32>(%[[VALUE13]]));
// DEFAULT-NEXT:                     write<ptr<@type[[TYPE_Node]]>>(%[[VALUE_cur]], read<ptr<@type[[TYPE_Node]]>>(field1(deref(read<ptr<@type[[TYPE_Node]]>>(%[[VALUE_cur]])))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: ptr<@type[[TYPE_Node]]> [synthetic] = read<ptr<@type[[TYPE_Node]]>>(field2(deref(read<ptr<@type[[TYPE_Node]]>>(%[[VALUE_cur]]))));
// DEFAULT-NEXT:         let %[[VALUE15:[0-9]+]]: i32 [synthetic] = read<i32>(field0(deref(read<ptr<@type[[TYPE_Node]]>>(%[[VALUE14]]))));
// DEFAULT-NEXT:         let %[[VALUE16:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE15]]), const<i32>(10));
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type[[TYPE_Node]]>>(%[[VALUE14]]))), read<i32>(%[[VALUE16]]));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%[[VALUE_sum]]), read<i32>(field0(%[[VALUE_a_2]]))), read<i32>(field0(%[[VALUE_b_2]]))), read<i32>(field0(%[[VALUE_c]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_parent_pointer_tree:[0-9]+]] @parent_pointer_tree() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_root:[0-9]+]] root: @type[[TYPE_Tree]] [storage=automatic] = aggregate<@type[[TYPE_Tree]], zero_fill=false>(field0 = const<i32>(1), field1 = null<ptr<@type[[TYPE_Tree]]>>, field2 = null<ptr<@type[[TYPE_Tree]]>>, field3 = null<ptr<@type[[TYPE_Tree]]>>);
// DEFAULT-NEXT:         let %[[VALUE_left:[0-9]+]] left: @type[[TYPE_Tree]] [storage=automatic] = aggregate<@type[[TYPE_Tree]], zero_fill=false>(field0 = const<i32>(2), field1 = addr_of<ptr<@type[[TYPE_Tree]]>>(%[[VALUE_root]]), field2 = null<ptr<@type[[TYPE_Tree]]>>, field3 = null<ptr<@type[[TYPE_Tree]]>>);
// DEFAULT-NEXT:         let %[[VALUE_right:[0-9]+]] right: @type[[TYPE_Tree]] [storage=automatic] = aggregate<@type[[TYPE_Tree]], zero_fill=false>(field0 = const<i32>(3), field1 = addr_of<ptr<@type[[TYPE_Tree]]>>(%[[VALUE_root]]), field2 = null<ptr<@type[[TYPE_Tree]]>>, field3 = null<ptr<@type[[TYPE_Tree]]>>);
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_Tree]]>>(field2(%[[VALUE_root]]), addr_of<ptr<@type[[TYPE_Tree]]>>(%[[VALUE_left]]));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_Tree]]>>(field3(%[[VALUE_root]]), addr_of<ptr<@type[[TYPE_Tree]]>>(%[[VALUE_right]]));
// DEFAULT-NEXT:         let %[[VALUE17:[0-9]+]]: ptr<@type[[TYPE_Tree]]> [synthetic] = read<ptr<@type[[TYPE_Tree]]>>(field1(%[[VALUE_left]]));
// DEFAULT-NEXT:         let %[[VALUE18:[0-9]+]]: i32 [synthetic] = read<i32>(field0(deref(read<ptr<@type[[TYPE_Tree]]>>(%[[VALUE17]]))));
// DEFAULT-NEXT:         let %[[VALUE19:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE18]]), read<i32>(field0(%[[VALUE_left]])));
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type[[TYPE_Tree]]>>(%[[VALUE17]]))), read<i32>(%[[VALUE19]]));
// DEFAULT-NEXT:         let %[[VALUE20:[0-9]+]]: ptr<@type[[TYPE_Tree]]> [synthetic] = read<ptr<@type[[TYPE_Tree]]>>(field1(%[[VALUE_right]]));
// DEFAULT-NEXT:         let %[[VALUE21:[0-9]+]]: i32 [synthetic] = read<i32>(field0(deref(read<ptr<@type[[TYPE_Tree]]>>(%[[VALUE20]]))));
// DEFAULT-NEXT:         let %[[VALUE22:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE21]]), read<i32>(field0(%[[VALUE_right]])));
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type[[TYPE_Tree]]>>(%[[VALUE20]]))), read<i32>(%[[VALUE22]]));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(field0(%[[VALUE_root]])), read<i32>(field0(%[[VALUE_left]]))), read<i32>(field0(%[[VALUE_right]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_self_referential_struct:[0-9]+]] @self_referential_struct() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_buf:[0-9]+]] buf: @type[[TYPE_Buf]] [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE23:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE24:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE25:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE24]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE25]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(8)>(field0(%[[VALUE_buf]])), read<i32>(%[[VALUE_i_2]]))), truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(const<i32>(97), read<i32>(%[[VALUE_i_2]]))));
// DEFAULT-NEXT:         write<ptr<i8>>(field1(%[[VALUE_buf]]), addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(8)>(field0(%[[VALUE_buf]])), const<i32>(3)))));
// DEFAULT-NEXT:         let %[[VALUE26:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(field1(%[[VALUE_buf]]));
// DEFAULT-NEXT:         let %[[VALUE27:[0-9]+]]: i8 [synthetic] = read<i8>(deref(read<ptr<i8>>(%[[VALUE26]])));
// DEFAULT-NEXT:         let %[[VALUE28:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE27]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(deref(read<ptr<i8>>(%[[VALUE26]])), read<i8>(%[[VALUE28]]));
// DEFAULT-NEXT:         return sub<i32, overflow=ub>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(8)>(field0(%[[VALUE_buf]])), const<i32>(3))))), widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(field1(%[[VALUE_buf]])), const<i32>(0)))))), mul<i32, overflow=ub>(const<i32>(2), const<i32>(97)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_reverse_in_place:[0-9]+]] @reverse_in_place(%[[VALUE_lo_2:[0-9]+]] lo: ptr<i32>, %[[VALUE_hi_2:[0-9]+]] hi: ptr<i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         while %[[VALUE29:[0-9]+]] lt<ptr<i32>>(read<ptr<i32>>(%[[VALUE_lo_2]]), read<ptr<i32>>(%[[VALUE_hi_2]]))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_t:[0-9]+]] t: i32 [storage=automatic] = read<i32>(deref(read<ptr<i32>>(%[[VALUE_lo_2]])));
// DEFAULT-NEXT:                 write<i32>(deref(read<ptr<i32>>(%[[VALUE_lo_2]])), read<i32>(deref(read<ptr<i32>>(%[[VALUE_hi_2]]))));
// DEFAULT-NEXT:                 write<i32>(deref(read<ptr<i32>>(%[[VALUE_hi_2]])), read<i32>(%[[VALUE_t]]));
// DEFAULT-NEXT:                 let %[[VALUE30:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_lo_2]]);
// DEFAULT-NEXT:                 let %[[VALUE31:[0-9]+]]: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE30]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i32>>(%[[VALUE_lo_2]], read<ptr<i32>>(%[[VALUE31]]));
// DEFAULT-NEXT:                 let %[[VALUE32:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_hi_2]]);
// DEFAULT-NEXT:                 let %[[VALUE33:[0-9]+]]: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=true, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE32]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i32>>(%[[VALUE_hi_2]], read<ptr<i32>>(%[[VALUE33]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_overlapping_array_pointers:[0-9]+]] @overlapping_array_pointers() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_values:[0-9]+]] values: array<i32, 5> [storage=automatic] [align=16] = aggregate<array<i32, 5>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2), index2 = const<i32>(3), index3 = const<i32>(4), index4 = const<i32>(5));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, ptr<i32>) -> void>(%[[VALUE_reverse_in_place]], addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(5)>(%[[VALUE_values]]), const<i32>(0)))), addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(5)>(%[[VALUE_values]]), const<i32>(4)))));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(mul<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>,
// DEFAULT-SAME: length=Some(5)>(%[[VALUE_values]]), const<i32>(0)))), const<i32>(10000)), mul<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false,
// DEFAULT-SAME: element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(5)>(%[[VALUE_values]]), const<i32>(1)))), const<i32>(1000))), mul<i32,
// DEFAULT-SAME: overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(5)>(%[[VALUE_values]]),
// DEFAULT-SAME: const<i32>(2)))), const<i32>(100))), mul<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>,
// DEFAULT-SAME: length=Some(5)>(%[[VALUE_values]]), const<i32>(3)))), const<i32>(10))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32,
// DEFAULT-SAME: overflow=ub>(array_decay<ptr<i32>, length=Some(5)>(%[[VALUE_values]]), const<i32>(4)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_capture_global:[0-9]+]] @capture_global(%[[VALUE_p_3:[0-9]+]] p: ptr<i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_g_ptr]], read<ptr<i32>>(%[[VALUE_p_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_global_alias_with_local:[0-9]+]] @global_alias_with_local() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_2:[0-9]+]] x: i32 [storage=automatic] = const<i32>(7);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%[[VALUE_capture_global]], addr_of<ptr<i32>>(%[[VALUE_x_2]]));
// DEFAULT-NEXT:         let %[[VALUE34:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_g_ptr]]);
// DEFAULT-NEXT:         let %[[VALUE35:[0-9]+]]: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%[[VALUE34]])));
// DEFAULT-NEXT:         let %[[VALUE36:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE35]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE34]])), read<i32>(%[[VALUE36]]));
// DEFAULT-NEXT:         let %[[VALUE37:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x_2]]);
// DEFAULT-NEXT:         let %[[VALUE38:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE37]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_x_2]], read<i32>(%[[VALUE38]]));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%[[VALUE_x_2]]), read<i32>(deref(read<ptr<i32>>(%[[VALUE_g_ptr]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_type_punning:[0-9]+]] @type_punning() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_u:[0-9]+]] u: @type[[TYPE_Pun]] [storage=automatic];
// DEFAULT-NEXT:         write<f32>(field1(%[[VALUE_u]]), const<f32>(1.0));
// DEFAULT-NEXT:         let %[[VALUE_bits:[0-9]+]] bits: i32 [storage=automatic] = read<i32>(field0(%[[VALUE_u]]));
// DEFAULT-NEXT:         let %[[VALUE39:[0-9]+]]: i32 [synthetic] = read<i32>(field0(%[[VALUE_u]]));
// DEFAULT-NEXT:         let %[[VALUE40:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE39]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(field0(%[[VALUE_u]]), read<i32>(%[[VALUE40]]));
// DEFAULT-NEXT:         return conditional<i32>(eq<i32>(read<i32>(%[[VALUE_bits]]), read<i32>(field0(%[[VALUE_u]]))), neg<i32, overflow=ub>(const<i32>(1)), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f32>(field1(%[[VALUE_u]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(25)>(%[[VALUE_str]])), call<i32, signature=fn() -> i32>(%[[VALUE_alias_same_object]]), call<i32, signature=fn() -> i32>(%[[VALUE_alias_struct_fields]]), call<i32, signature=fn() -> i32>(%[[VALUE_circular_list]]), call<i32, signature=fn() -> i32>(%[[VALUE_parent_pointer_tree]]), call<i32, signature=fn() -> i32>(%[[VALUE_self_referential_struct]]), call<i32, signature=fn() -> i32>(%[[VALUE_overlapping_array_pointers]]), call<i32, signature=fn() -> i32>(%[[VALUE_global_alias_with_local]]), call<i32, signature=fn() -> i32>(%[[VALUE_type_punning]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
