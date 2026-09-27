/* { dg-do run } */

struct S {
  int *p;
  int *q;
};

int **__attribute__((noinline, noclone, pure)) foo(struct S *s) {
  int tem;
  __asm__("" : "=g"(tem) : "g"(s->p));
  return &s->q;
}

int main() {
  struct S s;
  int      i = 1, j = 2;
  int    **x;
  s.p = &i;
  s.q = &j;
  x   = foo(&s);
  **x = 7;
  if (j != 7)
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
// DEFAULT-NEXT:         field0 p: ptr<i32>;
// DEFAULT-NEXT:         field1 q: ptr<i32>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     fn %1 @foo(%2 s: ptr<@type0>) -> ptr<ptr<i32>> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 tem: i32 [storage=automatic];
// DEFAULT-NEXT:         asm "" [dialect=att] {
// DEFAULT-NEXT:             out 0 "=g" place<i32>(%3);
// DEFAULT-NEXT:             in 1 "g" read<ptr<i32>>(field0(deref(read<ptr<@type0>>(%2))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return addr_of<ptr<ptr<i32>>>(field1(deref(read<ptr<@type0>>(%2))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %5 s: @type0 [storage=automatic];
// DEFAULT-NEXT:         let %6 i: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         let %7 j: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:         let %8 x: ptr<ptr<i32>> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i32>>(field0(%5), addr_of<ptr<i32>>(%6));
// DEFAULT-NEXT:         write<ptr<i32>>(field1(%5), addr_of<ptr<i32>>(%7));
// DEFAULT-NEXT:         write<ptr<ptr<i32>>>(%8, call<ptr<ptr<i32>>, signature=fn(ptr<@type0>) -> ptr<ptr<i32>>>(%1, addr_of<ptr<@type0>>(%5)));
// DEFAULT-NEXT:         call<ptr<ptr<i32>>, signature=fn(ptr<@type0>) -> ptr<ptr<i32>>>(%1, addr_of<ptr<@type0>>(%5));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(deref(read<ptr<ptr<i32>>>(%8)))), const<i32>(7));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%7), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
