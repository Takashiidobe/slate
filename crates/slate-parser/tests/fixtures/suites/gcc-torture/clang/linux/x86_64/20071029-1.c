extern void exit(int);
extern void abort(void);

typedef union {
  struct {
    int      f1, f2, f3, f4, f5, f6, f7, f8;
    long int f9, f10;
    int      f11;
  } f;
  char     s[56];
  long int a;
} T;

__attribute__((noinline)) void test(T *t) {
  static int i = 11;
  if (t->f.f1 != i++)
    abort();
  if (t->f.f2 || t->f.f3 || t->f.f4 || t->f.f5 || t->f.f6 || t->f.f7 ||
      t->f.f8 || t->f.f9 || t->f.f10 || t->f.f11)
    abort();
  if (i == 20)
    exit(0);
}

__attribute__((noinline)) void foo(int i) {
  T t;
again:
  t = (T){{++i, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
  test(&t);
  goto again;
}

int main(void) {
  T  *t1, *t2;
  int cnt = 0;
  t1      = (T *)0;
loop:
  t2 = t1;
  t1 = &(T){.f.f9 = cnt++};
  if (cnt < 3)
    goto loop;
  if (t1 != t2 || t1->f.f9 != 2)
    abort();
  foo(10);
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
// DEFAULT-NEXT:     type @type0 = union {
// DEFAULT-NEXT:         field0 f: @type1;
// DEFAULT-NEXT:         field1 s: array<i8, 56>;
// DEFAULT-NEXT:         field2 a: i64;
// DEFAULT-NEXT:     } [size=56, align=8, offsets=[0, 0, 0]];
// DEFAULT-NEXT:     type @type1 = struct {
// DEFAULT-NEXT:         field0 f1: i32;
// DEFAULT-NEXT:         field1 f2: i32;
// DEFAULT-NEXT:         field2 f3: i32;
// DEFAULT-NEXT:         field3 f4: i32;
// DEFAULT-NEXT:         field4 f5: i32;
// DEFAULT-NEXT:         field5 f6: i32;
// DEFAULT-NEXT:         field6 f7: i32;
// DEFAULT-NEXT:         field7 f8: i32;
// DEFAULT-NEXT:         field8 f9: i64;
// DEFAULT-NEXT:         field9 f10: i64;
// DEFAULT-NEXT:         field10 f11: i32;
// DEFAULT-NEXT:     } [size=56, align=8, offsets=[0, 4, 8, 12, 16, 20, 24, 28, 32, 40, 48]];
// DEFAULT-NEXT:     type @type2 T = @type0;
// DEFAULT-NEXT:     global %7 i: i32 [storage=static] = const<i32>(11) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @exit(%17 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %5 @test(%6 t: ptr<@type0>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %20: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:         let %21: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%20), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%7, read<i32>(%21));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(field0(field0(deref(read<ptr<@type0>>(%6))))), read<i32>(%20))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field1(field0(deref(read<ptr<@type0>>(%6))))), const<i32>(0)), ne<i32>(read<i32>(field2(field0(deref(read<ptr<@type0>>(%6))))), const<i32>(0))), ne<i32>(read<i32>(field3(field0(deref(read<ptr<@type0>>(%6))))), const<i32>(0))), ne<i32>(read<i32>(field4(field0(deref(read<ptr<@type0>>(%6))))), const<i32>(0))), ne<i32>(read<i32>(field5(field0(deref(read<ptr<@type0>>(%6))))), const<i32>(0))), ne<i32>(read<i32>(field6(field0(deref(read<ptr<@type0>>(%6))))), const<i32>(0))), ne<i32>(read<i32>(field7(field0(deref(read<ptr<@type0>>(%6))))), const<i32>(0))), ne<i64>(read<i64>(field8(field0(deref(read<ptr<@type0>>(%6))))), const<i64>(0))), ne<i64>(read<i64>(field9(field0(deref(read<ptr<@type0>>(%6))))), const<i64>(0))), ne<i32>(read<i32>(field10(field0(deref(read<ptr<@type0>>(%6))))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%7), const<i32>(20))
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%0, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @foo(%10 i: i32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %11 t: @type0 [storage=automatic];
// DEFAULT-NEXT:         label %9 again:
// DEFAULT-NEXT:             let %22: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:             let %23: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%22), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%10, read<i32>(%23));
// DEFAULT-NEXT:             write<@type0>(%11, copy<@type0, reason=assign>(read<@type0>(compound_literal %18 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = aggregate<@type1, zero_fill=false>(field0 = read<i32>(%23), field1 = const<i32>(0), field2 = const<i32>(0), field3 = const<i32>(0), field4 = const<i32>(0), field5 = const<i32>(0), field6 = const<i32>(0), field7 = const<i32>(0), field8 = widen<i64, reason=assign>(const<i32>(0)), field9 = widen<i64, reason=assign>(const<i32>(0)), field10 = const<i32>(0))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>) -> void>(%5, addr_of<ptr<@type0>>(%11));
// DEFAULT-NEXT:         goto %9;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %14 t1: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:         let %15 t2: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:         let %16 cnt: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         write<ptr<@type0>>(%14, null<ptr<@type0>>);
// DEFAULT-NEXT:         label %13 loop:
// DEFAULT-NEXT:             write<ptr<@type0>>(%15, read<ptr<@type0>>(%14));
// DEFAULT-NEXT:         let %24: i32 [synthetic] = read<i32>(%16);
// DEFAULT-NEXT:         let %25: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%24), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%16, read<i32>(%25));
// DEFAULT-NEXT:         write<ptr<@type0>>(%14, addr_of<ptr<@type0>>(compound_literal %19 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = aggregate<@type1, zero_fill=true>(field8 = widen<i64, reason=assign>(read<i32>(%24))))));
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%16), const<i32>(3))
// DEFAULT-NEXT:             goto %13;
// DEFAULT-NEXT:         if logical_or<bool>(ne<ptr<@type0>>(read<ptr<@type0>>(%14), read<ptr<@type0>>(%15)), ne<i64>(read<i64>(field8(field0(deref(read<ptr<@type0>>(%14))))), widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%8, const<i32>(10));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
