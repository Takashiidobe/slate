/* PR target/11087
   This testcase was miscompiled on ppc64, because basic_induction_var called
   convert_modes, yet did not expect it to emit any new instructions.
   Those were emitted at the end of the function and destroyed during life
   analysis, while the program used uninitialized pseudos created by
   convert_modes.  */

struct A {
  unsigned short a1;
  unsigned long  a2;
};

struct B {
  int b1, b2, b3, b4, b5;
};

struct C {
  struct B c1[1];
  int      c2, c3;
};

static int foo(int x) { return x < 0 ? -x : x; }

int bar(struct C *x, struct A *y) {
  int                 a = x->c3;
  const int           b = y->a1 >> 9;
  const unsigned long c = y->a2;
  int                 d = a;
  unsigned long       e, f;

  f = foo(c - x->c1[d].b4);
  do {
    if (d <= 0)
      d = x->c2;
    d--;

    e = foo(c - x->c1[d].b4);
    if (e < f)
      a = d;
  } while (d != x->c3);
  x->c1[a].b4 = c + b;
  return a;
}

int main() {
  struct A a;
  struct C b;
  int      c;

  a.a1 = 512;
  a.a2 = 4242;
  __builtin_memset(&b, 0, sizeof(b));
  b.c1[0].b3 = 424242;
  b.c2       = 1;
  c          = bar(&b, &a);
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
// DEFAULT-NEXT:     type @type0 A = struct {
// DEFAULT-NEXT:         field0 a1: u16;
// DEFAULT-NEXT:         field1 a2: u64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type1 B = struct {
// DEFAULT-NEXT:         field0 b1: i32;
// DEFAULT-NEXT:         field1 b2: i32;
// DEFAULT-NEXT:         field2 b3: i32;
// DEFAULT-NEXT:         field3 b4: i32;
// DEFAULT-NEXT:         field4 b5: i32;
// DEFAULT-NEXT:     } [size=20, align=4, offsets=[0, 4, 8, 12, 16]];
// DEFAULT-NEXT:     type @type2 C = struct {
// DEFAULT-NEXT:         field0 c1: array<@type1, 1>;
// DEFAULT-NEXT:         field1 c2: i32;
// DEFAULT-NEXT:         field2 c3: i32;
// DEFAULT-NEXT:     } [size=28, align=4, offsets=[0, 20, 24]];
// DEFAULT-NEXT:     fn %3 @foo(%4 x: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(lt<i32>(read<i32>(%4), const<i32>(0)), neg<i32, overflow=ub>(read<i32>(%4)), read<i32>(%4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @bar(%6 x: ptr<@type2>, %7 y: ptr<@type0>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8 a: i32 [storage=automatic] = read<i32>(field2(deref(read<ptr<@type2>>(%6))));
// DEFAULT-NEXT:         let %9 b: i32 [storage=automatic] [const] = shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field0(deref(read<ptr<@type0>>(%7)))))), const<i32>(9));
// DEFAULT-NEXT:         let %10 c: u64 [storage=automatic] [const] = read<u64>(field1(deref(read<ptr<@type0>>(%7))));
// DEFAULT-NEXT:         let %11 d: i32 [storage=automatic] = read<i32>(%8);
// DEFAULT-NEXT:         let %12 e: u64 [storage=automatic];
// DEFAULT-NEXT:         let %13 f: u64 [storage=automatic];
// DEFAULT-NEXT:         write<u64>(%13, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(call<i32, signature=fn(i32) -> i32>(%3, reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=unknown>(sub<u64, overflow=wrap>(read<u64>(%10), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(field3(deref(ptr_offset<ptr<@type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<@type1>, length=Some(1)>(field0(deref(read<ptr<@type2>>(%6)))), read<i32>(%11))))))))))))));
// DEFAULT-NEXT:         reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(call<i32, signature=fn(i32) -> i32>(%3, reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=unknown>(sub<u64, overflow=wrap>(read<u64>(%10), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(field3(deref(ptr_offset<ptr<@type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<@type1>, length=Some(1)>(field0(deref(read<ptr<@type2>>(%6)))), read<i32>(%11)))))))))))));
// DEFAULT-NEXT:         do %18
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if le<i32>(read<i32>(%11), const<i32>(0))
// DEFAULT-NEXT:                     write<i32>(%11, read<i32>(field1(deref(read<ptr<@type2>>(%6)))));
// DEFAULT-NEXT:                 let %19: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:                 let %20: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%19), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%11, read<i32>(%20));
// DEFAULT-NEXT:                 write<u64>(%12, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(call<i32, signature=fn(i32) -> i32>(%3, reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=unknown>(sub<u64, overflow=wrap>(read<u64>(%10), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(field3(deref(ptr_offset<ptr<@type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<@type1>, length=Some(1)>(field0(deref(read<ptr<@type2>>(%6)))), read<i32>(%11))))))))))))));
// DEFAULT-NEXT:                 reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(call<i32, signature=fn(i32) -> i32>(%3, reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=unknown>(sub<u64, overflow=wrap>(read<u64>(%10), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(field3(deref(ptr_offset<ptr<@type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<@type1>, length=Some(1)>(field0(deref(read<ptr<@type2>>(%6)))), read<i32>(%11)))))))))))));
// DEFAULT-NEXT:                 if lt<u64>(read<u64>(%12), read<u64>(%13))
// DEFAULT-NEXT:                     write<i32>(%8, read<i32>(%11));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(read<i32>(%11), read<i32>(field2(deref(read<ptr<@type2>>(%6)))));
// DEFAULT-NEXT:         write<i32>(field3(deref(ptr_offset<ptr<@type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<@type1>, length=Some(1)>(field0(deref(read<ptr<@type2>>(%6)))), read<i32>(%8)))), reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(read<u64>(%10), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%9)))))));
// DEFAULT-NEXT:         return read<i32>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %15 a: @type0 [storage=automatic];
// DEFAULT-NEXT:         let %16 b: @type2 [storage=automatic];
// DEFAULT-NEXT:         let %17 c: i32 [storage=automatic];
// DEFAULT-NEXT:         write<u16>(field0(%15), reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(512))));
// DEFAULT-NEXT:         write<u64>(field1(%15), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(4242))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(__builtin_memset, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type2>>(%16)), const<i32>(0), const<u64>(28));
// DEFAULT-NEXT:         write<i32>(field2(deref(ptr_offset<ptr<@type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<@type1>, length=Some(1)>(field0(%16)), const<i32>(0)))), const<i32>(424242));
// DEFAULT-NEXT:         write<i32>(field1(%16), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%17, call<i32, signature=fn(ptr<@type2>, ptr<@type0>) -> i32>(%5, addr_of<ptr<@type2>>(%16), addr_of<ptr<@type0>>(%15)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type2>, ptr<@type0>) -> i32>(%5, addr_of<ptr<@type2>>(%16), addr_of<ptr<@type0>>(%15));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
