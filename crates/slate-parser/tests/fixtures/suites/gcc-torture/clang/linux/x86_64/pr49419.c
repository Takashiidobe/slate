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
// DEFAULT-NEXT:     type @type0 S = struct {
// DEFAULT-NEXT:         field0 w: i32;
// DEFAULT-NEXT:         field1 x: i32;
// DEFAULT-NEXT:         field2 y: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     global %2 t: ptr<@type0> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @foo(%4 n: i32, %5 f: i32, %6 s: ptr<i32>, %7 m: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8 x: i32 [storage=automatic];
// DEFAULT-NEXT:         let %9 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %10 a: i32 [storage=automatic];
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%4), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         for %15
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%8, read<i32>(%4));
// DEFAULT-NEXT:                 write<i32>(%9, const<i32>(0));
// DEFAULT-NEXT:             condition: logical_and<bool>(eq<i32>(read<i32>(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(read<ptr<@type0>>(%2), read<i32>(%8))))), read<i32>(%5)), lt<i32>(read<i32>(%9), read<i32>(%7)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %17: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:                 let %18: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%17), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%9, read<i32>(%18));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(%8, read<i32>(field1(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(read<ptr<@type0>>(%2), read<i32>(%8))))));
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%9), read<i32>(%7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i32>(%10, add<i32, overflow=ub>(read<i32>(%9), const<i32>(1)));
// DEFAULT-NEXT:         for %16
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%8, read<i32>(%4));
// DEFAULT-NEXT:             condition: gt<i32>(read<i32>(%9), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %19: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:                 let %20: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%19), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%9, read<i32>(%20));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%6), read<i32>(%9))), read<i32>(field2(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(read<ptr<@type0>>(%2), read<i32>(%8))))));
// DEFAULT-NEXT:                     write<i32>(%8, read<i32>(field1(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(read<ptr<@type0>>(%2), read<i32>(%8))))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%6), const<i32>(0))), read<i32>(%8));
// DEFAULT-NEXT:         return read<i32>(%10);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %12 s: array<i32, 3> [storage=automatic];
// DEFAULT-NEXT:         let %13 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %14 buf: array<@type0, 3> [storage=automatic] [align=16] = aggregate<array<@type0, 3>, zero_fill=false>(index0 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(1), field2 = const<i32>(2)), index1 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(0), field1 = const<i32>(0), field2 = const<i32>(0)), index2 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(0), field1 = const<i32>(0), field2 = const<i32>(0)));
// DEFAULT-NEXT:         write<ptr<@type0>>(%2, array_decay<ptr<@type0>, length=Some(3)>(%14));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32, ptr<i32>, i32) -> i32>(%3, const<i32>(0), const<i32>(1), array_decay<ptr<i32>, length=Some(3)>(%12), const<i32>(3)), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%12), const<i32>(0)))), const<i32>(1)), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%12), const<i32>(1)))), const<i32>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
