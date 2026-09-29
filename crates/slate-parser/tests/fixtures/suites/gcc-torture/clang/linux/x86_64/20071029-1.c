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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 f: @type[[TYPE1:[0-9]+]];
// DEFAULT-NEXT:         field1 s: array<i8, 56>;
// DEFAULT-NEXT:         field2 a: i64;
// DEFAULT-NEXT:     } [size=56, align=8, offsets=[0, 0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE1]] = struct {
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
// DEFAULT-NEXT:     type @type[[TYPE_T:[0-9]+]] T = @type[[TYPE0]];
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: i32 [storage=static] = const<i32>(11) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_test:[0-9]+]] @test(%[[VALUE_t:[0-9]+]] t: ptr<@type[[TYPE0]]>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(field0(field0(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_t]]))))), read<i32>(%[[VALUE1]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field1(field0(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_t]]))))), const<i32>(0)), ne<i32>(read<i32>(field2(field0(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_t]]))))), const<i32>(0))), ne<i32>(read<i32>(field3(field0(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_t]]))))), const<i32>(0))), ne<i32>(read<i32>(field4(field0(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_t]]))))), const<i32>(0))), ne<i32>(read<i32>(field5(field0(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_t]]))))), const<i32>(0))), ne<i32>(read<i32>(field6(field0(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_t]]))))), const<i32>(0))), ne<i32>(read<i32>(field7(field0(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_t]]))))), const<i32>(0))), ne<i64>(read<i64>(field8(field0(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_t]]))))), const<i64>(0))), ne<i64>(read<i64>(field9(field0(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_t]]))))), const<i64>(0))), ne<i32>(read<i32>(field10(field0(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_t]]))))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%[[VALUE_i]]), const<i32>(20))
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_i_2:[0-9]+]] i: i32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_t_2:[0-9]+]] t: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:         label %[[VALUE_again:[0-9]+]] again:
// DEFAULT-NEXT:             let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:             let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE3]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:             write<@type[[TYPE0]]>(%[[VALUE_t_2]], copy<@type[[TYPE0]], reason=assign>(read<@type[[TYPE0]]>(compound_literal %[[VALUE5:[0-9]+]] [storage=automatic] = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = aggregate<@type[[TYPE1]], zero_fill=false>(field0 = read<i32>(%[[VALUE4]]), field1 = const<i32>(0), field2 = const<i32>(0), field3 = const<i32>(0), field4 = const<i32>(0), field5 = const<i32>(0), field6 = const<i32>(0), field7 = const<i32>(0), field8 = widen<i64, reason=assign>(const<i32>(0)), field9 = widen<i64, reason=assign>(const<i32>(0)), field10 = const<i32>(0))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE0]]>) -> void>(%[[VALUE_test]], addr_of<ptr<@type[[TYPE0]]>>(%[[VALUE_t_2]]));
// DEFAULT-NEXT:         goto %[[VALUE_again]];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_t1:[0-9]+]] t1: ptr<@type[[TYPE0]]> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_t2:[0-9]+]] t2: ptr<@type[[TYPE0]]> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_cnt:[0-9]+]] cnt: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         write<ptr<@type[[TYPE0]]>>(%[[VALUE_t1]], null<ptr<@type[[TYPE0]]>>);
// DEFAULT-NEXT:         label %[[VALUE_loop:[0-9]+]] loop:
// DEFAULT-NEXT:             write<ptr<@type[[TYPE0]]>>(%[[VALUE_t2]], read<ptr<@type[[TYPE0]]>>(%[[VALUE_t1]]));
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_cnt]]);
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_cnt]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE0]]>>(%[[VALUE_t1]], addr_of<ptr<@type[[TYPE0]]>>(compound_literal %[[VALUE8:[0-9]+]] [storage=automatic] = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = aggregate<@type[[TYPE1]], zero_fill=true>(field8 = widen<i64, reason=assign>(read<i32>(%[[VALUE6]]))))));
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%[[VALUE_cnt]]), const<i32>(3))
// DEFAULT-NEXT:             goto %[[VALUE_loop]];
// DEFAULT-NEXT:         if logical_or<bool>(ne<ptr<@type[[TYPE0]]>>(read<ptr<@type[[TYPE0]]>>(%[[VALUE_t1]]), read<ptr<@type[[TYPE0]]>>(%[[VALUE_t2]])), ne<i64>(read<i64>(field8(field0(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_t1]]))))), widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_foo]], const<i32>(10));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
