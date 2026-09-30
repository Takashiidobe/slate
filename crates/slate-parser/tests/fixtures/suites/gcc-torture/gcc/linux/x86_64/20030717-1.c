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
// DEFAULT-NEXT:     type @type[[TYPE_A:[0-9]+]] A = struct {
// DEFAULT-NEXT:         field0 a1: u16;
// DEFAULT-NEXT:         field1 a2: u64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_B:[0-9]+]] B = struct {
// DEFAULT-NEXT:         field0 b1: i32;
// DEFAULT-NEXT:         field1 b2: i32;
// DEFAULT-NEXT:         field2 b3: i32;
// DEFAULT-NEXT:         field3 b4: i32;
// DEFAULT-NEXT:         field4 b5: i32;
// DEFAULT-NEXT:     } [size=20, align=4, offsets=[0, 4, 8, 12, 16]];
// DEFAULT-NEXT:     type @type[[TYPE_C:[0-9]+]] C = struct {
// DEFAULT-NEXT:         field0 c1: array<@type[[TYPE_B]], 1>;
// DEFAULT-NEXT:         field1 c2: i32;
// DEFAULT-NEXT:         field2 c3: i32;
// DEFAULT-NEXT:     } [size=28, align=4, offsets=[0, 20, 24]];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(lt<i32>(read<i32>(%[[VALUE_x]]), const<i32>(0)), neg<i32, overflow=ub>(read<i32>(%[[VALUE_x]])), read<i32>(%[[VALUE_x]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_x_2:[0-9]+]] x: ptr<@type[[TYPE_C]]>, %[[VALUE_y:[0-9]+]] y: ptr<@type[[TYPE_A]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: i32 [storage=automatic] = read<i32>(field2(deref(read<ptr<@type[[TYPE_C]]>>(%[[VALUE_x_2]]))));
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: i32 [storage=automatic] [const] = shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field0(deref(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_y]])))))), const<i32>(9));
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: u64 [storage=automatic] [const] = read<u64>(field1(deref(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_y]]))));
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: i32 [storage=automatic] = read<i32>(%[[VALUE_a]]);
// DEFAULT-NEXT:         let %[[VALUE_e:[0-9]+]] e: u64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_f:[0-9]+]] f: u64 [storage=automatic];
// DEFAULT-NEXT:         write<u64>(%[[VALUE_f]], reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo]], reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=unknown>(sub<u64, overflow=wrap>(read<u64>(%[[VALUE_c]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(field3(deref(ptr_offset<ptr<@type[[TYPE_B]]>, subtract=false, element=@type[[TYPE_B]], overflow=ub>(array_decay<ptr<@type[[TYPE_B]]>, length=Some(1)>(field0(deref(read<ptr<@type[[TYPE_C]]>>(%[[VALUE_x_2]])))), read<i32>(%[[VALUE_d]]))))))))))))));
// DEFAULT-NEXT:         do %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if le<i32>(read<i32>(%[[VALUE_d]]), const<i32>(0))
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_d]], read<i32>(field1(deref(read<ptr<@type[[TYPE_C]]>>(%[[VALUE_x_2]])))));
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_d]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_d]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_e]], reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo]], reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=unknown>(sub<u64, overflow=wrap>(read<u64>(%[[VALUE_c]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(field3(deref(ptr_offset<ptr<@type[[TYPE_B]]>, subtract=false, element=@type[[TYPE_B]], overflow=ub>(array_decay<ptr<@type[[TYPE_B]]>, length=Some(1)>(field0(deref(read<ptr<@type[[TYPE_C]]>>(%[[VALUE_x_2]])))), read<i32>(%[[VALUE_d]]))))))))))))));
// DEFAULT-NEXT:                 if lt<u64>(read<u64>(%[[VALUE_e]]), read<u64>(%[[VALUE_f]]))
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE_d]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(read<i32>(%[[VALUE_d]]), read<i32>(field2(deref(read<ptr<@type[[TYPE_C]]>>(%[[VALUE_x_2]])))));
// DEFAULT-NEXT:         write<i32>(field3(deref(ptr_offset<ptr<@type[[TYPE_B]]>, subtract=false, element=@type[[TYPE_B]], overflow=ub>(array_decay<ptr<@type[[TYPE_B]]>, length=Some(1)>(field0(deref(read<ptr<@type[[TYPE_C]]>>(%[[VALUE_x_2]])))), read<i32>(%[[VALUE_a]])))), reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(read<u64>(%[[VALUE_c]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_b]])))))));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_a]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_memset:[0-9]+]] @__builtin_memset(%[[VALUE3:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE4:[0-9]+]] <unnamed>: i32, %[[VALUE5:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a_2:[0-9]+]] a: @type[[TYPE_A]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_b_2:[0-9]+]] b: @type[[TYPE_C]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_c_2:[0-9]+]] c: i32 [storage=automatic];
// DEFAULT-NEXT:         write<u16>(field0(%[[VALUE_a_2]]), reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(512))));
// DEFAULT-NEXT:         write<u64>(field1(%[[VALUE_a_2]]), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(4242))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE___builtin_memset]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE_C]]>>(%[[VALUE_b_2]])), const<i32>(0), const<u64>(28));
// DEFAULT-NEXT:         write<i32>(field2(deref(ptr_offset<ptr<@type[[TYPE_B]]>, subtract=false, element=@type[[TYPE_B]], overflow=ub>(array_decay<ptr<@type[[TYPE_B]]>, length=Some(1)>(field0(%[[VALUE_b_2]])), const<i32>(0)))), const<i32>(424242));
// DEFAULT-NEXT:         write<i32>(field1(%[[VALUE_b_2]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_c_2]], call<i32, signature=fn(ptr<@type[[TYPE_C]]>, ptr<@type[[TYPE_A]]>) -> i32>(%[[VALUE_bar]], addr_of<ptr<@type[[TYPE_C]]>>(%[[VALUE_b_2]]), addr_of<ptr<@type[[TYPE_A]]>>(%[[VALUE_a_2]])));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
