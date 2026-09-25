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
// DEFAULT-NEXT:     type @type0 S = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:         field2 c: i32;
// DEFAULT-NEXT:         field3 d: ptr<i32>;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 4, 8, 16]];
// DEFAULT-NEXT:     type @type1 T = struct {
// DEFAULT-NEXT:         field0 e: ptr<i32>;
// DEFAULT-NEXT:         field1 f: ptr<i32>;
// DEFAULT-NEXT:         field2 g: ptr<i32>;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     global %2 t: ptr<@type1> [storage=static] = null<ptr<@type1>> [linkage=external];
// DEFAULT-NEXT:     global %3 o: ptr<i32> [storage=static] = null<ptr<i32>> [linkage=external];
// DEFAULT-NEXT:     fn %4 @bar(%5 x: ptr<i32>, %6 y: i32, %7 z: i32, %8 w: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%8), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(ne<ptr<i32>>(read<ptr<i32>>(%5), null<ptr<i32>>), ne<i32>(read<i32>(%6), const<i32>(0))), ne<i32>(read<i32>(%7), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(%8), const<i32>(0)), ne<ptr<i32>>(read<ptr<i32>>(%5), read<ptr<i32>>(field2(deref(read<ptr<@type1>>(%2)))))), ne<i32>(read<i32>(%6), const<i32>(0))), ne<i32>(read<i32>(%7), const<i32>(12)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @foo(%10 x: ptr<@type0>, %11 y: ptr<@type0>, %12 z: ptr<i32>, %13 w: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%3)), read<i32>(%13));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%13), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i32>, i32, i32, i32) -> void>(%4, null<ptr<i32>>, const<i32>(0), const<i32>(0), neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         write<ptr<i32>>(field3(deref(read<ptr<@type0>>(%10))), read<ptr<i32>>(%12));
// DEFAULT-NEXT:         if ne<ptr<i32>>(read<ptr<i32>>(field3(deref(read<ptr<@type0>>(%11)))), null<ptr<i32>>)
// DEFAULT-NEXT:             write<i32>(field2(deref(read<ptr<@type0>>(%11))), add<i32, overflow=ub>(read<i32>(field2(deref(read<ptr<@type0>>(%11)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(field3(deref(read<ptr<@type0>>(%11)))), const<i32>(0))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, i32, i32, i32) -> void>(%4, read<ptr<i32>>(field2(deref(read<ptr<@type1>>(%2)))), const<i32>(0), read<i32>(field2(deref(read<ptr<@type0>>(%11)))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %15 a: array<i32, 4> [storage=automatic] = aggregate<array<i32, 4>, zero_fill=false>(index0 = const<i32>(8), index1 = const<i32>(9), index2 = const<i32>(10), index3 = const<i32>(11));
// DEFAULT-NEXT:         let %16 s: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(2), field2 = const<i32>(3), field3 = addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%15), const<i32>(0)))));
// DEFAULT-NEXT:         let %17 u: @type1 [storage=automatic] = aggregate<@type1, zero_fill=false>(field0 = null<ptr<i32>>, field1 = null<ptr<i32>>, field2 = addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%15), const<i32>(3)))));
// DEFAULT-NEXT:         write<ptr<i32>>(%3, addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%15), const<i32>(2)))));
// DEFAULT-NEXT:         write<ptr<@type1>>(%2, addr_of<ptr<@type1>>(%17));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>, ptr<@type0>, ptr<i32>, i32) -> void>(%9, addr_of<ptr<@type0>>(%16), addr_of<ptr<@type0>>(%16), addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%15), const<i32>(1)))), const<i32>(5));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(field2(%16)), const<i32>(12)), ne<ptr<i32>>(read<ptr<i32>>(field3(%16)), addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%15), const<i32>(1))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
