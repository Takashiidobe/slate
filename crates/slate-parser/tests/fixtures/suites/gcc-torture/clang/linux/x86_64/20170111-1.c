/* PR rtl-optimization/79032 */
/* Reported by Daniel Cederman <cederman@gaisler.com> */

extern void abort(void);

struct S {
  short          a;
  long long      b;
  short          c;
  char           d;
  unsigned short e;
  long          *f;
};

static long foo(struct S *s) __attribute__((noclone, noinline));

static long foo(struct S *s) {
  long a  = 1;
  a      /= s->e;
  s->f[a]--;
  return a;
}

int main(void) {
  long     val = 1;
  struct S s   = {0, 0, 0, 0, 2, &val};
  val          = foo(&s);
  if (val != 0)
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
// DEFAULT-NEXT:         field0 a: i16;
// DEFAULT-NEXT:         field1 b: i64;
// DEFAULT-NEXT:         field2 c: i16;
// DEFAULT-NEXT:         field3 d: i8;
// DEFAULT-NEXT:         field4 e: u16;
// DEFAULT-NEXT:         field5 f: ptr<i64>;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16, 18, 20, 24]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_s:[0-9]+]] s: ptr<@type[[TYPE_S]]>) -> i64 [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_a]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i64 [synthetic] = div<i64, by_zero=ub, min_by_neg_one=ub>(read<i64>(%[[VALUE0]]), widen<i64, reason=usual_arith>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field4(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_s]]))))))));
// DEFAULT-NEXT:         write<i64>(%[[VALUE_a]], read<i64>(%[[VALUE1]]));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: ptr<i64> [synthetic] = ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(read<ptr<i64>>(field5(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_s]])))), read<i64>(%[[VALUE_a]]));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i64 [synthetic] = read<i64>(deref(read<ptr<i64>>(%[[VALUE2]])));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE3]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(deref(read<ptr<i64>>(%[[VALUE2]])), read<i64>(%[[VALUE4]]));
// DEFAULT-NEXT:         return read<i64>(%[[VALUE_a]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_val:[0-9]+]] val: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_s_2:[0-9]+]] s: @type[[TYPE_S]] [storage=automatic] = aggregate<@type[[TYPE_S]], zero_fill=false>(field0 = truncate<i16, reason=assign, fits=always>(const<i32>(0)), field1 = widen<i64, reason=assign>(const<i32>(0)), field2 = truncate<i16, reason=assign, fits=always>(const<i32>(0)), field3 = truncate<i8, reason=assign, fits=always>(const<i32>(0)), field4 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(2))), field5 = addr_of<ptr<i64>>(%[[VALUE_val]]));
// DEFAULT-NEXT:         write<i64>(%[[VALUE_val]], call<i64, signature=fn(ptr<@type[[TYPE_S]]>) -> i64>(%[[VALUE_foo]], addr_of<ptr<@type[[TYPE_S]]>>(%[[VALUE_s_2]])));
// DEFAULT-NEXT:         call<i64, signature=fn(ptr<@type[[TYPE_S]]>) -> i64>(%[[VALUE_foo]], addr_of<ptr<@type[[TYPE_S]]>>(%[[VALUE_s_2]]));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%[[VALUE_val]]), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
