/* { dg-do run } */
/* { dg-skip-if "asm operand has impossible constraints" { hppa*-*-* } } */
/* { dg-additional-options "-fstack-protector" { target fstack_protector } } */
/* { dg-additional-options "-fPIC" { target fpic } } */

struct S {
  int *l, *u;
};
int a[3];

__attribute__((noipa)) struct S foo(void) {
  int     *p = a, *q = a + 1;
  struct S s;
  asm volatile("" : "+g"(p), "+g"(q) : : "memory");
  s.l = p;
  s.u = q;
  a[0]++;
  return s;
}

__attribute__((noipa)) void bar(struct S *x) {
  asm volatile("" : : "g"(x) : "memory");
  if (x->l != a || x->u != a + 1)
    __builtin_abort();
  a[1]++;
}

__attribute__((noipa)) int baz(int *x, int *y) {
  int r = -1;
  asm volatile("" : "+g"(r) : "g"(x), "g"(y) : "memory");
  a[2]++;
  return r;
}

__attribute__((noipa)) void quux(void) { asm volatile("" : : : "memory"); }

__attribute__((noipa)) void qux(void) {
  struct S v = foo();
  struct S w;
  struct S x = foo();
  int      y = 0;

  w.l = x.l;
  w.u = x.u;
  if (baz(x.l, v.l) > 0) {
    w.l = v.l;
    y   = 1;
    quux();
  }
  if (baz(x.u, v.u) < 0) {
    w.u = v.u;
    y   = 1;
  }
  if (y)
    bar(&w);
}

int
main() {
  qux();
  if (a[0] != 2 || a[1] != 1 || a[2] != 2)
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
// DEFAULT-NEXT:         field0 l: ptr<i32>;
// DEFAULT-NEXT:         field1 u: ptr<i32>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %1 a: array<i32, 3> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo() -> @type0 [linkage=external] [abi=sysv64() -> coerce<i64, i64>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 p: ptr<i32> [storage=automatic] = array_decay<ptr<i32>, length=Some(3)>(%1);
// DEFAULT-NEXT:         let %4 q: ptr<i32> [storage=automatic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%1), const<i32>(1));
// DEFAULT-NEXT:         let %5 s: @type0 [storage=automatic];
// DEFAULT-NEXT:         asm volatile "" [dialect=att] [options=nostack] {
// DEFAULT-NEXT:             inlateout 0 "g" [reg | mem | imm] -> reg width 64 place<ptr<i32>>(%3);
// DEFAULT-NEXT:             inlateout 1 "g" [reg | mem | imm] -> reg width 64 place<ptr<i32>>(%4);
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<ptr<i32>>(field0(%5), read<ptr<i32>>(%3));
// DEFAULT-NEXT:         write<ptr<i32>>(field1(%5), read<ptr<i32>>(%4));
// DEFAULT-NEXT:         let %20: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%1), const<i32>(0));
// DEFAULT-NEXT:         let %21: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%20)));
// DEFAULT-NEXT:         let %22: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%21), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%20)), read<i32>(%22));
// DEFAULT-NEXT:         return copy<@type0, reason=return>(read<@type0>(%5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %6 @bar(%7 x: ptr<@type0>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         asm volatile "" [dialect=att] [options=nostack] {
// DEFAULT-NEXT:             in 0 "g" [reg | mem | imm] -> reg width 64 read<ptr<@type0>>(%7);
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if logical_or<bool>(ne<ptr<i32>>(read<ptr<i32>>(field0(deref(read<ptr<@type0>>(%7)))), array_decay<ptr<i32>, length=Some(3)>(%1)), ne<ptr<i32>>(read<ptr<i32>>(field1(deref(read<ptr<@type0>>(%7)))), ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%1), const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%19);
// DEFAULT-NEXT:         let %23: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%1), const<i32>(1));
// DEFAULT-NEXT:         let %24: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%23)));
// DEFAULT-NEXT:         let %25: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%24), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%23)), read<i32>(%25));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @baz(%9 x: ptr<i32>, %10 y: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %11 r: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         asm volatile "" [dialect=att] [options=nostack] {
// DEFAULT-NEXT:             inlateout 0 "g" [reg | mem | imm] -> reg width 32 place<i32>(%11);
// DEFAULT-NEXT:             in 1 "g" [reg | mem | imm] -> reg width 64 read<ptr<i32>>(%9);
// DEFAULT-NEXT:             in 2 "g" [reg | mem | imm] -> reg width 64 read<ptr<i32>>(%10);
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %26: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%1), const<i32>(2));
// DEFAULT-NEXT:         let %27: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%26)));
// DEFAULT-NEXT:         let %28: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%27), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%26)), read<i32>(%28));
// DEFAULT-NEXT:         return read<i32>(%11);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @quux() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         asm volatile "" [dialect=att] [options=nostack] {
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @qux() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %14 v: @type0 [storage=automatic] = copy<@type0, reason=assign>(call<@type0, signature=fn() -> @type0, abi=sysv64() -> coerce<i64, i64>>(%2));
// DEFAULT-NEXT:         let %15 w: @type0 [storage=automatic];
// DEFAULT-NEXT:         let %16 x: @type0 [storage=automatic] = copy<@type0, reason=assign>(call<@type0, signature=fn() -> @type0, abi=sysv64() -> coerce<i64, i64>>(%2));
// DEFAULT-NEXT:         let %17 y: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         write<ptr<i32>>(field0(%15), read<ptr<i32>>(field0(%16)));
// DEFAULT-NEXT:         write<ptr<i32>>(field1(%15), read<ptr<i32>>(field1(%16)));
// DEFAULT-NEXT:         if gt<i32>(call<i32, signature=fn(ptr<i32>, ptr<i32>) -> i32>(%8, read<ptr<i32>>(field0(%16)), read<ptr<i32>>(field0(%14))), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<ptr<i32>>(field0(%15), read<ptr<i32>>(field0(%14)));
// DEFAULT-NEXT:                 write<i32>(%17, const<i32>(1));
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%12);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if lt<i32>(call<i32, signature=fn(ptr<i32>, ptr<i32>) -> i32>(%8, read<ptr<i32>>(field1(%16)), read<ptr<i32>>(field1(%14))), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<ptr<i32>>(field1(%15), read<ptr<i32>>(field1(%14)));
// DEFAULT-NEXT:                 write<i32>(%17, const<i32>(1));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%17), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type0>) -> void>(%6, addr_of<ptr<@type0>>(%15));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%1), const<i32>(0)))), const<i32>(2)), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%1), const<i32>(1)))), const<i32>(1))), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%1), const<i32>(2)))), const<i32>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%19);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
