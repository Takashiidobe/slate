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
// DEFAULT-NEXT:     type @type0 Pair = struct {
// DEFAULT-NEXT:         field0 lo: i32;
// DEFAULT-NEXT:         field1 hi: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type1 Node = struct {
// DEFAULT-NEXT:         field0 val: i32;
// DEFAULT-NEXT:         field1 next: ptr<@type1>;
// DEFAULT-NEXT:         field2 prev: ptr<@type1>;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     type @type2 Tree = struct {
// DEFAULT-NEXT:         field0 val: i32;
// DEFAULT-NEXT:         field1 parent: ptr<@type2>;
// DEFAULT-NEXT:         field2 left: ptr<@type2>;
// DEFAULT-NEXT:         field3 right: ptr<@type2>;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16, 24]];
// DEFAULT-NEXT:     type @type3 Buf = struct {
// DEFAULT-NEXT:         field0 data: array<i8, 8>;
// DEFAULT-NEXT:         field1 cursor: ptr<i8>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type4 Pun = union {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:         field1 f: f32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     global %38 g_ptr: ptr<i32> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %52 .str52: array<i8, 25> [storage=static] = code_units<array<i8, 25>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%48 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @add_in_place(%2 a: ptr<i32>, %3 b: ptr<i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %53: ptr<i32> [synthetic] = read<ptr<i32>>(%2);
// DEFAULT-NEXT:         let %54: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%53)));
// DEFAULT-NEXT:         let %55: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%54), read<i32>(deref(read<ptr<i32>>(%3))));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%53)), read<i32>(%55));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @alias_same_object() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 x: i32 [storage=automatic] = const<i32>(5);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, ptr<i32>) -> void>(%1, addr_of<ptr<i32>>(%5), addr_of<ptr<i32>>(%5));
// DEFAULT-NEXT:         return read<i32>(%5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @pair_lo(%8 p: ptr<@type0>) -> ptr<i32> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return addr_of<ptr<i32>>(field0(deref(read<ptr<@type0>>(%8))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @pair_hi(%10 p: ptr<@type0>) -> ptr<i32> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return addr_of<ptr<i32>>(field1(deref(read<ptr<@type0>>(%10))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @alias_struct_fields() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %12 pair: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(2));
// DEFAULT-NEXT:         let %13 lo: ptr<i32> [storage=automatic] = call<ptr<i32>, signature=fn(ptr<@type0>) -> ptr<i32>>(%7, addr_of<ptr<@type0>>(%12));
// DEFAULT-NEXT:         let %14 hi: ptr<i32> [storage=automatic] = call<ptr<i32>, signature=fn(ptr<@type0>) -> ptr<i32>>(%9, addr_of<ptr<@type0>>(%12));
// DEFAULT-NEXT:         let %56: ptr<i32> [synthetic] = read<ptr<i32>>(%13);
// DEFAULT-NEXT:         let %57: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%56)));
// DEFAULT-NEXT:         let %58: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%57), read<i32>(deref(read<ptr<i32>>(%14))));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%56)), read<i32>(%58));
// DEFAULT-NEXT:         let %59: ptr<i32> [synthetic] = read<ptr<i32>>(%14);
// DEFAULT-NEXT:         let %60: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%59)));
// DEFAULT-NEXT:         let %61: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%60), read<i32>(deref(read<ptr<i32>>(%13))));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%59)), read<i32>(%61));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(field0(%12)), read<i32>(field1(%12)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @circular_list() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %17 a: @type1 [storage=automatic] = aggregate<@type1, zero_fill=false>(field0 = const<i32>(1), field1 = null<ptr<@type1>>, field2 = null<ptr<@type1>>);
// DEFAULT-NEXT:         let %18 b: @type1 [storage=automatic] = aggregate<@type1, zero_fill=false>(field0 = const<i32>(2), field1 = null<ptr<@type1>>, field2 = null<ptr<@type1>>);
// DEFAULT-NEXT:         let %19 c: @type1 [storage=automatic] = aggregate<@type1, zero_fill=false>(field0 = const<i32>(3), field1 = null<ptr<@type1>>, field2 = null<ptr<@type1>>);
// DEFAULT-NEXT:         write<ptr<@type1>>(field1(%17), addr_of<ptr<@type1>>(%18));
// DEFAULT-NEXT:         write<ptr<@type1>>(field1(%18), addr_of<ptr<@type1>>(%19));
// DEFAULT-NEXT:         write<ptr<@type1>>(field1(%19), addr_of<ptr<@type1>>(%17));
// DEFAULT-NEXT:         write<ptr<@type1>>(field2(%17), addr_of<ptr<@type1>>(%19));
// DEFAULT-NEXT:         write<ptr<@type1>>(field2(%18), addr_of<ptr<@type1>>(%17));
// DEFAULT-NEXT:         write<ptr<@type1>>(field2(%19), addr_of<ptr<@type1>>(%18));
// DEFAULT-NEXT:         let %20 cur: ptr<@type1> [storage=automatic] = addr_of<ptr<@type1>>(%17);
// DEFAULT-NEXT:         let %21 sum: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %49
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %22 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%22), const<i32>(6))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %62: i32 [synthetic] = read<i32>(%22);
// DEFAULT-NEXT:                 let %63: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%62), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%22, read<i32>(%63));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %64: i32 [synthetic] = read<i32>(%21);
// DEFAULT-NEXT:                     let %65: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%64), read<i32>(field0(deref(read<ptr<@type1>>(%20)))));
// DEFAULT-NEXT:                     write<i32>(%21, read<i32>(%65));
// DEFAULT-NEXT:                     write<ptr<@type1>>(%20, read<ptr<@type1>>(field1(deref(read<ptr<@type1>>(%20)))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         let %66: ptr<@type1> [synthetic] = read<ptr<@type1>>(field2(deref(read<ptr<@type1>>(%20))));
// DEFAULT-NEXT:         let %67: i32 [synthetic] = read<i32>(field0(deref(read<ptr<@type1>>(%66))));
// DEFAULT-NEXT:         let %68: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%67), const<i32>(10));
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type1>>(%66))), read<i32>(%68));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%21), read<i32>(field0(%17))), read<i32>(field0(%18))), read<i32>(field0(%19)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @parent_pointer_tree() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %25 root: @type2 [storage=automatic] = aggregate<@type2, zero_fill=false>(field0 = const<i32>(1), field1 = null<ptr<@type2>>, field2 = null<ptr<@type2>>, field3 = null<ptr<@type2>>);
// DEFAULT-NEXT:         let %26 left: @type2 [storage=automatic] = aggregate<@type2, zero_fill=false>(field0 = const<i32>(2), field1 = addr_of<ptr<@type2>>(%25), field2 = null<ptr<@type2>>, field3 = null<ptr<@type2>>);
// DEFAULT-NEXT:         let %27 right: @type2 [storage=automatic] = aggregate<@type2, zero_fill=false>(field0 = const<i32>(3), field1 = addr_of<ptr<@type2>>(%25), field2 = null<ptr<@type2>>, field3 = null<ptr<@type2>>);
// DEFAULT-NEXT:         write<ptr<@type2>>(field2(%25), addr_of<ptr<@type2>>(%26));
// DEFAULT-NEXT:         write<ptr<@type2>>(field3(%25), addr_of<ptr<@type2>>(%27));
// DEFAULT-NEXT:         let %69: ptr<@type2> [synthetic] = read<ptr<@type2>>(field1(%26));
// DEFAULT-NEXT:         let %70: i32 [synthetic] = read<i32>(field0(deref(read<ptr<@type2>>(%69))));
// DEFAULT-NEXT:         let %71: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%70), read<i32>(field0(%26)));
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type2>>(%69))), read<i32>(%71));
// DEFAULT-NEXT:         let %72: ptr<@type2> [synthetic] = read<ptr<@type2>>(field1(%27));
// DEFAULT-NEXT:         let %73: i32 [synthetic] = read<i32>(field0(deref(read<ptr<@type2>>(%72))));
// DEFAULT-NEXT:         let %74: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%73), read<i32>(field0(%27)));
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type2>>(%72))), read<i32>(%74));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(field0(%25)), read<i32>(field0(%26))), read<i32>(field0(%27)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %29 @self_referential_struct() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %30 buf: @type3 [storage=automatic];
// DEFAULT-NEXT:         for %50
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %31 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%31), const<i32>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %75: i32 [synthetic] = read<i32>(%31);
// DEFAULT-NEXT:                 let %76: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%75), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%31, read<i32>(%76));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(8)>(field0(%30)), read<i32>(%31))), truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(const<i32>(97), read<i32>(%31))));
// DEFAULT-NEXT:         write<ptr<i8>>(field1(%30), addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(8)>(field0(%30)), const<i32>(3)))));
// DEFAULT-NEXT:         let %77: ptr<i8> [synthetic] = read<ptr<i8>>(field1(%30));
// DEFAULT-NEXT:         let %78: i8 [synthetic] = read<i8>(deref(read<ptr<i8>>(%77)));
// DEFAULT-NEXT:         let %79: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%78)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(deref(read<ptr<i8>>(%77)), read<i8>(%79));
// DEFAULT-NEXT:         return sub<i32, overflow=ub>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(8)>(field0(%30)), const<i32>(3))))), widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(field1(%30)), const<i32>(0)))))), mul<i32, overflow=ub>(const<i32>(2), const<i32>(97)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %32 @reverse_in_place(%33 lo: ptr<i32>, %34 hi: ptr<i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         while %51 lt<ptr<i32>>(read<ptr<i32>>(%33), read<ptr<i32>>(%34))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %35 t: i32 [storage=automatic] = read<i32>(deref(read<ptr<i32>>(%33)));
// DEFAULT-NEXT:                 write<i32>(deref(read<ptr<i32>>(%33)), read<i32>(deref(read<ptr<i32>>(%34))));
// DEFAULT-NEXT:                 write<i32>(deref(read<ptr<i32>>(%34)), read<i32>(%35));
// DEFAULT-NEXT:                 let %80: ptr<i32> [synthetic] = read<ptr<i32>>(%33);
// DEFAULT-NEXT:                 let %81: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%80), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i32>>(%33, read<ptr<i32>>(%81));
// DEFAULT-NEXT:                 let %82: ptr<i32> [synthetic] = read<ptr<i32>>(%34);
// DEFAULT-NEXT:                 let %83: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=true, element=i32, overflow=ub>(read<ptr<i32>>(%82), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i32>>(%34, read<ptr<i32>>(%83));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %36 @overlapping_array_pointers() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %37 values: array<i32, 5> [storage=automatic] [align=16] = aggregate<array<i32, 5>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2), index2 = const<i32>(3), index3 = const<i32>(4), index4 = const<i32>(5));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, ptr<i32>) -> void>(%32, addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(5)>(%37), const<i32>(0)))), addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(5)>(%37), const<i32>(4)))));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(mul<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(5)>(%37), const<i32>(0)))), const<i32>(10000)), mul<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(5)>(%37), const<i32>(1)))), const<i32>(1000))), mul<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(5)>(%37), const<i32>(2)))), const<i32>(100))), mul<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(5)>(%37), const<i32>(3)))), const<i32>(10))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(5)>(%37), const<i32>(4)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %39 @capture_global(%40 p: ptr<i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<ptr<i32>>(%38, read<ptr<i32>>(%40));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %41 @global_alias_with_local() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %42 x: i32 [storage=automatic] = const<i32>(7);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%39, addr_of<ptr<i32>>(%42));
// DEFAULT-NEXT:         let %84: ptr<i32> [synthetic] = read<ptr<i32>>(%38);
// DEFAULT-NEXT:         let %85: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%84)));
// DEFAULT-NEXT:         let %86: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%85), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%84)), read<i32>(%86));
// DEFAULT-NEXT:         let %87: i32 [synthetic] = read<i32>(%42);
// DEFAULT-NEXT:         let %88: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%87), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%42, read<i32>(%88));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%42), read<i32>(deref(read<ptr<i32>>(%38))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %44 @type_punning() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %45 u: @type4 [storage=automatic];
// DEFAULT-NEXT:         write<f32>(field1(%45), const<f32>(1.0));
// DEFAULT-NEXT:         let %46 bits: i32 [storage=automatic] = read<i32>(field0(%45));
// DEFAULT-NEXT:         let %89: i32 [synthetic] = read<i32>(field0(%45));
// DEFAULT-NEXT:         let %90: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%89), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(field0(%45), read<i32>(%90));
// DEFAULT-NEXT:         return conditional<i32>(eq<i32>(read<i32>(%46), read<i32>(field0(%45))), neg<i32, overflow=ub>(const<i32>(1)), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f32>(field1(%45))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %47 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(25)>(%52)), call<i32, signature=fn() -> i32>(%4), call<i32, signature=fn() -> i32>(%11), call<i32, signature=fn() -> i32>(%16), call<i32, signature=fn() -> i32>(%24), call<i32, signature=fn() -> i32>(%29), call<i32, signature=fn() -> i32>(%36), call<i32, signature=fn() -> i32>(%41), call<i32, signature=fn() -> i32>(%44));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
