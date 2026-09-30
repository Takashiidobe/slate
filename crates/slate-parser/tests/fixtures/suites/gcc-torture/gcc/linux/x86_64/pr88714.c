/* PR bootstrap/88714 */

struct S {
  int  a, b, c;
  int *d;
};
struct T {
  int *e, *f, *g;
}   *t = 0;
int *o = 0;

__attribute__((noipa)) void bar(int *x, int y, int z, int w) {
  if (w == -1) {
    if (x != 0 || y != 0 || z != 0)
      __builtin_abort();
  } else if (w != 0 || x != t->g || y != 0 || z != 12)
    __builtin_abort();
}

__attribute__((noipa)) void foo(struct S *x, struct S *y, int *z, int w) {
  *o = w;
  if (w)
    bar(0, 0, 0, -1);
  x->d = z;
  if (y->d)
    y->c = y->c + y->d[0];
  bar(t->g, 0, y->c, 0);
}

int main() {
  int      a[4] = {8, 9, 10, 11};
  struct S s    = {1, 2, 3, &a[0]};
  struct T u    = {0, 0, &a[3]};
  o             = &a[2];
  t             = &u;
  foo(&s, &s, &a[1], 5);
  if (s.c != 12 || s.d != &a[1])
    __builtin_abort();
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
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:         field2 c: i32;
// DEFAULT-NEXT:         field3 d: ptr<i32>;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 4, 8, 16]];
// DEFAULT-NEXT:     type @type[[TYPE_T:[0-9]+]] T = struct {
// DEFAULT-NEXT:         field0 e: ptr<i32>;
// DEFAULT-NEXT:         field1 f: ptr<i32>;
// DEFAULT-NEXT:         field2 g: ptr<i32>;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     global %[[VALUE_t:[0-9]+]] t: ptr<@type[[TYPE_T]]> [storage=static] = null<ptr<@type[[TYPE_T]]>> [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_o:[0-9]+]] o: ptr<i32> [storage=static] = null<ptr<i32>> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_x:[0-9]+]] x: ptr<i32>, %[[VALUE_y:[0-9]+]] y: i32, %[[VALUE_z:[0-9]+]] z: i32, %[[VALUE_w:[0-9]+]] w: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%[[VALUE_w]]), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(ne<ptr<i32>>(read<ptr<i32>>(%[[VALUE_x]]), null<ptr<i32>>), ne<i32>(read<i32>(%[[VALUE_y]]), const<i32>(0))), ne<i32>(read<i32>(%[[VALUE_z]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(%[[VALUE_w]]), const<i32>(0)), ne<ptr<i32>>(read<ptr<i32>>(%[[VALUE_x]]), read<ptr<i32>>(field2(deref(read<ptr<@type[[TYPE_T]]>>(%[[VALUE_t]])))))), ne<i32>(read<i32>(%[[VALUE_y]]), const<i32>(0))), ne<i32>(read<i32>(%[[VALUE_z]]), const<i32>(12)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x_2:[0-9]+]] x: ptr<@type[[TYPE_S]]>, %[[VALUE_y_2:[0-9]+]] y: ptr<@type[[TYPE_S]]>, %[[VALUE_z_2:[0-9]+]] z: ptr<i32>, %[[VALUE_w_2:[0-9]+]] w: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE_o]])), read<i32>(%[[VALUE_w_2]]));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_w_2]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i32>, i32, i32, i32) -> void>(%[[VALUE_bar]], null<ptr<i32>>, const<i32>(0), const<i32>(0), neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         write<ptr<i32>>(field3(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_x_2]]))), read<ptr<i32>>(%[[VALUE_z_2]]));
// DEFAULT-NEXT:         if ne<ptr<i32>>(read<ptr<i32>>(field3(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_y_2]])))), null<ptr<i32>>)
// DEFAULT-NEXT:             write<i32>(field2(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_y_2]]))), add<i32, overflow=ub>(read<i32>(field2(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_y_2]])))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(field3(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_y_2]])))), const<i32>(0))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, i32, i32, i32) -> void>(%[[VALUE_bar]], read<ptr<i32>>(field2(deref(read<ptr<@type[[TYPE_T]]>>(%[[VALUE_t]])))), const<i32>(0), read<i32>(field2(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_y_2]])))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: array<i32, 4> [storage=automatic] [align=16] = aggregate<array<i32, 4>, zero_fill=false>(index0 = const<i32>(8), index1 = const<i32>(9), index2 = const<i32>(10), index3 = const<i32>(11));
// DEFAULT-NEXT:         let %[[VALUE_s:[0-9]+]] s: @type[[TYPE_S]] [storage=automatic] = aggregate<@type[[TYPE_S]], zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(2), field2 = const<i32>(3), field3 = addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_a]]), const<i32>(0)))));
// DEFAULT-NEXT:         let %[[VALUE_u:[0-9]+]] u: @type[[TYPE_T]] [storage=automatic] = aggregate<@type[[TYPE_T]], zero_fill=false>(field0 = null<ptr<i32>>, field1 = null<ptr<i32>>, field2 = addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_a]]), const<i32>(3)))));
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_o]], addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_a]]), const<i32>(2)))));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_T]]>>(%[[VALUE_t]], addr_of<ptr<@type[[TYPE_T]]>>(%[[VALUE_u]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S]]>, ptr<@type[[TYPE_S]]>, ptr<i32>, i32) -> void>(%[[VALUE_foo]], addr_of<ptr<@type[[TYPE_S]]>>(%[[VALUE_s]]), addr_of<ptr<@type[[TYPE_S]]>>(%[[VALUE_s]]), addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_a]]), const<i32>(1)))), const<i32>(5));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(field2(%[[VALUE_s]])), const<i32>(12)), ne<ptr<i32>>(read<ptr<i32>>(field3(%[[VALUE_s]])), addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_a]]), const<i32>(1))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
