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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     type @type1 tree_node = union {
// DEFAULT-NEXT:         field0 othr: @type3;
// DEFAULT-NEXT:         field1 vec: @type4;
// DEFAULT-NEXT:     } [size=96, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type2 tree_common = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i64;
// DEFAULT-NEXT:         field2 c: i64;
// DEFAULT-NEXT:         field3 p: ptr<void>;
// DEFAULT-NEXT:         field4 d: i32;
// DEFAULT-NEXT:     } [size=40, align=8, offsets=[0, 8, 16, 24, 32]];
// DEFAULT-NEXT:     type @type3 other_tree = struct {
// DEFAULT-NEXT:         field0 common: @type2;
// DEFAULT-NEXT:         field1 arr: array<i32, 14>;
// DEFAULT-NEXT:     } [size=96, align=8, offsets=[0, 40]];
// DEFAULT-NEXT:     type @type4 tree_vec = struct {
// DEFAULT-NEXT:         field0 common: @type2;
// DEFAULT-NEXT:         field1 length: i32;
// DEFAULT-NEXT:         field2 a: array<ptr<@type1>, 1>;
// DEFAULT-NEXT:     } [size=56, align=8, offsets=[0, 40, 48]];
// DEFAULT-NEXT:     global %8 global: @type1 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %2 @malloc(%15 __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %3 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %9 @foo(%10 p: ptr<@type1>, %11 i: i32) -> ptr<@type1> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %12 q: ptr<ptr<@type1>> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<@type1>>(deref(ptr_offset<ptr<ptr<@type1>>, subtract=false, element=ptr<@type1>, overflow=ub>(array_decay<ptr<ptr<@type1>>, length=Some(1)>(field2(field1(deref(read<ptr<@type1>>(%10))))), read<i32>(%11))), null<ptr<@type1>>);
// DEFAULT-NEXT:         write<ptr<ptr<@type1>>>(%12, addr_of<ptr<ptr<@type1>>>(deref(ptr_offset<ptr<ptr<@type1>>, subtract=false, element=ptr<@type1>, overflow=ub>(array_decay<ptr<ptr<@type1>>, length=Some(1)>(field2(field1(deref(read<ptr<@type1>>(%10))))), const<i32>(1)))));
// DEFAULT-NEXT:         write<ptr<@type1>>(deref(read<ptr<ptr<@type1>>>(%12)), addr_of<ptr<@type1>>(%8));
// DEFAULT-NEXT:         return read<ptr<@type1>>(deref(ptr_offset<ptr<ptr<@type1>>, subtract=false, element=ptr<@type1>, overflow=ub>(array_decay<ptr<ptr<@type1>>, length=Some(1)>(field2(field1(deref(read<ptr<@type1>>(%10))))), read<i32>(%11))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %14 p: ptr<@type1> [storage=automatic] = pointer_cast<ptr<@type1>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%2, const<u64>(96)));
// DEFAULT-NEXT:         if ne<ptr<@type1>>(call<ptr<@type1>, signature=fn(ptr<@type1>, i32) -> ptr<@type1>>(%9, read<ptr<@type1>>(%14), const<i32>(1)), addr_of<ptr<@type1>>(%8))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
