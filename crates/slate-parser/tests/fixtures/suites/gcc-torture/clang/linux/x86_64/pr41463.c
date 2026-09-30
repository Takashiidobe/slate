#include <stdlib.h>

union tree_node;

struct tree_common {
  int   a;
  long  b;
  long  c;
  void *p;
  int   d;
};

struct other_tree {
  struct tree_common common;
  int                arr[14];
};

struct tree_vec {
  struct tree_common common;
  int                length;
  union tree_node   *a[1];
};

union tree_node {
  struct other_tree othr;
  struct tree_vec   vec;
};

union tree_node global;

union tree_node *__attribute__((noinline)) foo(union tree_node *p, int i) {
  union tree_node **q;
  p->vec.a[i] = (union tree_node *)0;
  q           = &p->vec.a[1];
  *q          = &global;
  return p->vec.a[i];
}

extern void  abort(void);
extern void *malloc(__SIZE_TYPE__);

int main() {
  union tree_node *p = malloc(sizeof(union tree_node));
  if (foo(p, 1) != &global)
    abort();
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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_tree_node:[0-9]+]] tree_node = union {
// DEFAULT-NEXT:         field0 othr: @type[[TYPE_other_tree:[0-9]+]];
// DEFAULT-NEXT:         field1 vec: @type[[TYPE_tree_vec:[0-9]+]];
// DEFAULT-NEXT:     } [size=96, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_tree_common:[0-9]+]] tree_common = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i64;
// DEFAULT-NEXT:         field2 c: i64;
// DEFAULT-NEXT:         field3 p: ptr<void>;
// DEFAULT-NEXT:         field4 d: i32;
// DEFAULT-NEXT:     } [size=40, align=8, offsets=[0, 8, 16, 24, 32]];
// DEFAULT-NEXT:     type @type[[TYPE_other_tree]] other_tree = struct {
// DEFAULT-NEXT:         field0 common: @type[[TYPE_tree_common]];
// DEFAULT-NEXT:         field1 arr: array<i32, 14>;
// DEFAULT-NEXT:     } [size=96, align=8, offsets=[0, 40]];
// DEFAULT-NEXT:     type @type[[TYPE_tree_vec]] tree_vec = struct {
// DEFAULT-NEXT:         field0 common: @type[[TYPE_tree_common]];
// DEFAULT-NEXT:         field1 length: i32;
// DEFAULT-NEXT:         field2 a: array<ptr<@type[[TYPE_tree_node]]>, 1>;
// DEFAULT-NEXT:     } [size=56, align=8, offsets=[0, 40, 48]];
// DEFAULT-NEXT:     global %[[VALUE_global:[0-9]+]] global: @type[[TYPE_tree_node]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_malloc:[0-9]+]] @malloc(%[[VALUE___size:[0-9]+]] __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_tree_node]]>, %[[VALUE_i:[0-9]+]] i: i32) -> ptr<@type[[TYPE_tree_node]]> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_q:[0-9]+]] q: ptr<ptr<@type[[TYPE_tree_node]]>> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_tree_node]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_tree_node]]>>, subtract=false, element=ptr<@type[[TYPE_tree_node]]>, overflow=ub>(array_decay<ptr<ptr<@type[[TYPE_tree_node]]>>, length=Some(1)>(field2(field1(deref(read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE_p]]))))), read<i32>(%[[VALUE_i]]))), null<ptr<@type[[TYPE_tree_node]]>>);
// DEFAULT-NEXT:         write<ptr<ptr<@type[[TYPE_tree_node]]>>>(%[[VALUE_q]], addr_of<ptr<ptr<@type[[TYPE_tree_node]]>>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_tree_node]]>>, subtract=false, element=ptr<@type[[TYPE_tree_node]]>, overflow=ub>(array_decay<ptr<ptr<@type[[TYPE_tree_node]]>>, length=Some(1)>(field2(field1(deref(read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE_p]]))))), const<i32>(1)))));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_tree_node]]>>(deref(read<ptr<ptr<@type[[TYPE_tree_node]]>>>(%[[VALUE_q]])), addr_of<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE_global]]));
// DEFAULT-NEXT:         return read<ptr<@type[[TYPE_tree_node]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_tree_node]]>>, subtract=false, element=ptr<@type[[TYPE_tree_node]]>, overflow=ub>(array_decay<ptr<ptr<@type[[TYPE_tree_node]]>>, length=Some(1)>(field2(field1(deref(read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE_p]]))))), read<i32>(%[[VALUE_i]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_p_2:[0-9]+]] p: ptr<@type[[TYPE_tree_node]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_tree_node]]>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_malloc]], const<u64>(96)));
// DEFAULT-NEXT:         if ne<ptr<@type[[TYPE_tree_node]]>>(call<ptr<@type[[TYPE_tree_node]]>, signature=fn(ptr<@type[[TYPE_tree_node]]>, i32) -> ptr<@type[[TYPE_tree_node]]>>(%[[VALUE_foo]], read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE_p_2]]), const<i32>(1)), addr_of<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE_global]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
