/* PR tree-optimization/49419 */

extern void abort(void);

struct S {
  int w, x, y;
} *t;

int foo(int n, int f, int *s, int m) {
  int x, i, a;
  if (n == -1)
    return 0;
  for (x = n, i = 0; t[x].w == f && i < m; i++)
    x = t[x].x;
  if (i == m)
    abort();
  a = i + 1;
  for (x = n; i > 0; i--) {
    s[i] = t[x].y;
    x    = t[x].x;
  }
  s[0] = x;
  return a;
}

int main(void) {
  int      s[3], i;
  struct S buf[3] = {{1, 1, 2}, {0, 0, 0}, {0, 0, 0}};
  t               = buf;
  if (foo(0, 1, s, 3) != 2)
    abort();
  if (s[0] != 1 || s[1] != 2)
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
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 w: i32;
// DEFAULT-NEXT:         field1 x: i32;
// DEFAULT-NEXT:         field2 y: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     global %[[VALUE_t:[0-9]+]] t: ptr<@type[[TYPE_S]]> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_n:[0-9]+]] n: i32, %[[VALUE_f:[0-9]+]] f: i32, %[[VALUE_s:[0-9]+]] s: ptr<i32>, %[[VALUE_m:[0-9]+]] m: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: i32 [storage=automatic];
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%[[VALUE_n]]), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE_n]]));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: logical_and<bool>(eq<i32>(read<i32>(field0(deref(ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false, element=@type[[TYPE_S]], overflow=ub>(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_t]]), read<i32>(%[[VALUE_x]]))))), read<i32>(%[[VALUE_f]])), lt<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_m]])))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x]], read<i32>(field1(deref(ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false, element=@type[[TYPE_S]], overflow=ub>(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_t]]), read<i32>(%[[VALUE_x]]))))));
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_m]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_a]], add<i32, overflow=ub>(read<i32>(%[[VALUE_i]]), const<i32>(1)));
// DEFAULT-NEXT:         for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE_n]]));
// DEFAULT-NEXT:             condition: gt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_s]]), read<i32>(%[[VALUE_i]]))), read<i32>(field2(deref(ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false, element=@type[[TYPE_S]], overflow=ub>(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_t]]), read<i32>(%[[VALUE_x]]))))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_x]], read<i32>(field1(deref(ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false, element=@type[[TYPE_S]], overflow=ub>(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_t]]), read<i32>(%[[VALUE_x]]))))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_s]]), const<i32>(0))), read<i32>(%[[VALUE_x]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_a]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_s_2:[0-9]+]] s: array<i32, 3> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_buf:[0-9]+]] buf: array<@type[[TYPE_S]], 3> [storage=automatic] [align=16] = aggregate<array<@type[[TYPE_S]], 3>, zero_fill=false>(index0 = aggregate<@type[[TYPE_S]], zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(1), field2 = const<i32>(2)), index1 = aggregate<@type[[TYPE_S]], zero_fill=false>(field0 = const<i32>(0), field1 = const<i32>(0), field2 = const<i32>(0)), index2 = aggregate<@type[[TYPE_S]], zero_fill=false>(field0 = const<i32>(0), field1 = const<i32>(0), field2 = const<i32>(0)));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_S]]>>(%[[VALUE_t]], array_decay<ptr<@type[[TYPE_S]]>, length=Some(3)>(%[[VALUE_buf]]));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32, ptr<i32>, i32) -> i32>(%[[VALUE_foo]], const<i32>(0), const<i32>(1), array_decay<ptr<i32>, length=Some(3)>(%[[VALUE_s_2]]), const<i32>(3)), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%[[VALUE_s_2]]), const<i32>(0)))), const<i32>(1)), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%[[VALUE_s_2]]), const<i32>(1)))), const<i32>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
