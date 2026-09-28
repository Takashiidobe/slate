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
// DEFAULT-NEXT:     global %39 g_ptr: ptr<i32> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %53 .str53: array<i8, 25> [storage=static] = code_units<array<i8, 25>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%49 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @add_in_place(%3 a: ptr<i32>, %4 b: ptr<i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %54: ptr<i32> [synthetic] = read<ptr<i32>>(%3);
// DEFAULT-NEXT:         let %55: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%54)));
// DEFAULT-NEXT:         let %56: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%55), read<i32>(deref(read<ptr<i32>>(%4))));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%54)), read<i32>(%56));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @alias_same_object() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %6 x: i32 [storage=automatic] = const<i32>(5);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, ptr<i32>) -> void>(%2, addr_of<ptr<i32>>(%6), addr_of<ptr<i32>>(%6));
// DEFAULT-NEXT:         return read<i32>(%6);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @pair_lo(%9 p: ptr<@type0>) -> ptr<i32> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return addr_of<ptr<i32>>(field0(deref(read<ptr<@type0>>(%9))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @pair_hi(%11 p: ptr<@type0>) -> ptr<i32> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return addr_of<ptr<i32>>(field1(deref(read<ptr<@type0>>(%11))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @alias_struct_fields() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %13 pair: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(2));
// DEFAULT-NEXT:         let %14 lo: ptr<i32> [storage=automatic] = call<ptr<i32>, signature=fn(ptr<@type0>) -> ptr<i32>>(%8, addr_of<ptr<@type0>>(%13));
// DEFAULT-NEXT:         let %15 hi: ptr<i32> [storage=automatic] = call<ptr<i32>, signature=fn(ptr<@type0>) -> ptr<i32>>(%10, addr_of<ptr<@type0>>(%13));
// DEFAULT-NEXT:         let %57: ptr<i32> [synthetic] = read<ptr<i32>>(%14);
// DEFAULT-NEXT:         let %58: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%57)));
// DEFAULT-NEXT:         let %59: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%58), read<i32>(deref(read<ptr<i32>>(%15))));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%57)), read<i32>(%59));
// DEFAULT-NEXT:         let %60: ptr<i32> [synthetic] = read<ptr<i32>>(%15);
// DEFAULT-NEXT:         let %61: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%60)));
// DEFAULT-NEXT:         let %62: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%61), read<i32>(deref(read<ptr<i32>>(%14))));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%60)), read<i32>(%62));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(field0(%13)), read<i32>(field1(%13)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @circular_list() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %18 a: @type1 [storage=automatic] = aggregate<@type1, zero_fill=false>(field0 = const<i32>(1), field1 = null<ptr<@type1>>, field2 = null<ptr<@type1>>);
// DEFAULT-NEXT:         let %19 b: @type1 [storage=automatic] = aggregate<@type1, zero_fill=false>(field0 = const<i32>(2), field1 = null<ptr<@type1>>, field2 = null<ptr<@type1>>);
// DEFAULT-NEXT:         let %20 c: @type1 [storage=automatic] = aggregate<@type1, zero_fill=false>(field0 = const<i32>(3), field1 = null<ptr<@type1>>, field2 = null<ptr<@type1>>);
// DEFAULT-NEXT:         write<ptr<@type1>>(field1(%18), addr_of<ptr<@type1>>(%19));
// DEFAULT-NEXT:         write<ptr<@type1>>(field1(%19), addr_of<ptr<@type1>>(%20));
// DEFAULT-NEXT:         write<ptr<@type1>>(field1(%20), addr_of<ptr<@type1>>(%18));
// DEFAULT-NEXT:         write<ptr<@type1>>(field2(%18), addr_of<ptr<@type1>>(%20));
// DEFAULT-NEXT:         write<ptr<@type1>>(field2(%19), addr_of<ptr<@type1>>(%18));
// DEFAULT-NEXT:         write<ptr<@type1>>(field2(%20), addr_of<ptr<@type1>>(%19));
// DEFAULT-NEXT:         let %21 cur: ptr<@type1> [storage=automatic] = addr_of<ptr<@type1>>(%18);
// DEFAULT-NEXT:         let %22 sum: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %50
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %23 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%23), const<i32>(6))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %63: i32 [synthetic] = read<i32>(%23);
// DEFAULT-NEXT:                 let %64: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%63), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%23, read<i32>(%64));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %65: i32 [synthetic] = read<i32>(%22);
// DEFAULT-NEXT:                     let %66: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%65), read<i32>(field0(deref(read<ptr<@type1>>(%21)))));
// DEFAULT-NEXT:                     write<i32>(%22, read<i32>(%66));
// DEFAULT-NEXT:                     write<ptr<@type1>>(%21, read<ptr<@type1>>(field1(deref(read<ptr<@type1>>(%21)))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         let %67: ptr<@type1> [synthetic] = read<ptr<@type1>>(field2(deref(read<ptr<@type1>>(%21))));
// DEFAULT-NEXT:         let %68: i32 [synthetic] = read<i32>(field0(deref(read<ptr<@type1>>(%67))));
// DEFAULT-NEXT:         let %69: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%68), const<i32>(10));
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type1>>(%67))), read<i32>(%69));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%22), read<i32>(field0(%18))), read<i32>(field0(%19))), read<i32>(field0(%20)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @parent_pointer_tree() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %26 root: @type2 [storage=automatic] = aggregate<@type2, zero_fill=false>(field0 = const<i32>(1), field1 = null<ptr<@type2>>, field2 = null<ptr<@type2>>, field3 = null<ptr<@type2>>);
// DEFAULT-NEXT:         let %27 left: @type2 [storage=automatic] = aggregate<@type2, zero_fill=false>(field0 = const<i32>(2), field1 = addr_of<ptr<@type2>>(%26), field2 = null<ptr<@type2>>, field3 = null<ptr<@type2>>);
// DEFAULT-NEXT:         let %28 right: @type2 [storage=automatic] = aggregate<@type2, zero_fill=false>(field0 = const<i32>(3), field1 = addr_of<ptr<@type2>>(%26), field2 = null<ptr<@type2>>, field3 = null<ptr<@type2>>);
// DEFAULT-NEXT:         write<ptr<@type2>>(field2(%26), addr_of<ptr<@type2>>(%27));
// DEFAULT-NEXT:         write<ptr<@type2>>(field3(%26), addr_of<ptr<@type2>>(%28));
// DEFAULT-NEXT:         let %70: ptr<@type2> [synthetic] = read<ptr<@type2>>(field1(%27));
// DEFAULT-NEXT:         let %71: i32 [synthetic] = read<i32>(field0(deref(read<ptr<@type2>>(%70))));
// DEFAULT-NEXT:         let %72: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%71), read<i32>(field0(%27)));
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type2>>(%70))), read<i32>(%72));
// DEFAULT-NEXT:         let %73: ptr<@type2> [synthetic] = read<ptr<@type2>>(field1(%28));
// DEFAULT-NEXT:         let %74: i32 [synthetic] = read<i32>(field0(deref(read<ptr<@type2>>(%73))));
// DEFAULT-NEXT:         let %75: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%74), read<i32>(field0(%28)));
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type2>>(%73))), read<i32>(%75));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(field0(%26)), read<i32>(field0(%27))), read<i32>(field0(%28)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %30 @self_referential_struct() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %31 buf: @type3 [storage=automatic];
// DEFAULT-NEXT:         for %51
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %32 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%32), const<i32>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %76: i32 [synthetic] = read<i32>(%32);
// DEFAULT-NEXT:                 let %77: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%76), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%32, read<i32>(%77));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(8)>(field0(%31)), read<i32>(%32))), truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(const<i32>(97), read<i32>(%32))));
// DEFAULT-NEXT:         write<ptr<i8>>(field1(%31), addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(8)>(field0(%31)), const<i32>(3)))));
// DEFAULT-NEXT:         let %78: ptr<i8> [synthetic] = read<ptr<i8>>(field1(%31));
// DEFAULT-NEXT:         let %79: i8 [synthetic] = read<i8>(deref(read<ptr<i8>>(%78)));
// DEFAULT-NEXT:         let %80: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%79)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(deref(read<ptr<i8>>(%78)), read<i8>(%80));
// DEFAULT-NEXT:         return sub<i32, overflow=ub>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(8)>(field0(%31)), const<i32>(3))))), widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(field1(%31)), const<i32>(0)))))), mul<i32, overflow=ub>(const<i32>(2), const<i32>(97)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %33 @reverse_in_place(%34 lo: ptr<i32>, %35 hi: ptr<i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         while %52 lt<ptr<i32>>(read<ptr<i32>>(%34), read<ptr<i32>>(%35))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %36 t: i32 [storage=automatic] = read<i32>(deref(read<ptr<i32>>(%34)));
// DEFAULT-NEXT:                 write<i32>(deref(read<ptr<i32>>(%34)), read<i32>(deref(read<ptr<i32>>(%35))));
// DEFAULT-NEXT:                 write<i32>(deref(read<ptr<i32>>(%35)), read<i32>(%36));
// DEFAULT-NEXT:                 let %81: ptr<i32> [synthetic] = read<ptr<i32>>(%34);
// DEFAULT-NEXT:                 let %82: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%81), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i32>>(%34, read<ptr<i32>>(%82));
// DEFAULT-NEXT:                 let %83: ptr<i32> [synthetic] = read<ptr<i32>>(%35);
// DEFAULT-NEXT:                 let %84: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=true, element=i32, overflow=ub>(read<ptr<i32>>(%83), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i32>>(%35, read<ptr<i32>>(%84));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %37 @overlapping_array_pointers() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %38 values: array<i32, 5> [storage=automatic] [align=16] = aggregate<array<i32, 5>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2), index2 = const<i32>(3), index3 = const<i32>(4), index4 = const<i32>(5));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, ptr<i32>) -> void>(%33, addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(5)>(%38), const<i32>(0)))), addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(5)>(%38), const<i32>(4)))));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(mul<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(5)>(%38), const<i32>(0)))), const<i32>(10000)), mul<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(5)>(%38), const<i32>(1)))), const<i32>(1000))), mul<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(5)>(%38), const<i32>(2)))), const<i32>(100))), mul<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(5)>(%38), const<i32>(3)))), const<i32>(10))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(5)>(%38), const<i32>(4)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %40 @capture_global(%41 p: ptr<i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<ptr<i32>>(%39, read<ptr<i32>>(%41));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %42 @global_alias_with_local() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %43 x: i32 [storage=automatic] = const<i32>(7);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%40, addr_of<ptr<i32>>(%43));
// DEFAULT-NEXT:         let %85: ptr<i32> [synthetic] = read<ptr<i32>>(%39);
// DEFAULT-NEXT:         let %86: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%85)));
// DEFAULT-NEXT:         let %87: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%86), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%85)), read<i32>(%87));
// DEFAULT-NEXT:         let %88: i32 [synthetic] = read<i32>(%43);
// DEFAULT-NEXT:         let %89: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%88), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%43, read<i32>(%89));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%43), read<i32>(deref(read<ptr<i32>>(%39))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %45 @type_punning() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %46 u: @type4 [storage=automatic];
// DEFAULT-NEXT:         write<f32>(field1(%46), const<f32>(1.0));
// DEFAULT-NEXT:         let %47 bits: i32 [storage=automatic] = read<i32>(field0(%46));
// DEFAULT-NEXT:         let %90: i32 [synthetic] = read<i32>(field0(%46));
// DEFAULT-NEXT:         let %91: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%90), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(field0(%46), read<i32>(%91));
// DEFAULT-NEXT:         return conditional<i32>(eq<i32>(read<i32>(%47), read<i32>(field0(%46))), neg<i32, overflow=ub>(const<i32>(1)), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f32>(field1(%46))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %48 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(25)>(%53)), call<i32, signature=fn() -> i32>(%5), call<i32, signature=fn() -> i32>(%12), call<i32, signature=fn() -> i32>(%17), call<i32, signature=fn() -> i32>(%25), call<i32, signature=fn() -> i32>(%30), call<i32, signature=fn() -> i32>(%37), call<i32, signature=fn() -> i32>(%42), call<i32, signature=fn() -> i32>(%45));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
