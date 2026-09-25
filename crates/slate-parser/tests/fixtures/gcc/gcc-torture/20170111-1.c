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
// DEFAULT-NEXT:     type @type0 S = struct {
// DEFAULT-NEXT:         field0 a: i16;
// DEFAULT-NEXT:         field1 b: i64;
// DEFAULT-NEXT:         field2 c: i16;
// DEFAULT-NEXT:         field3 d: i8;
// DEFAULT-NEXT:         field4 e: u16;
// DEFAULT-NEXT:         field5 f: ptr<i64>;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16, 18, 20, 24]];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo(%3 s: ptr<@type0>) -> i64 [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 a: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(1));
// DEFAULT-NEXT:         let %9: i64 [synthetic] = read<i64>(%4);
// DEFAULT-NEXT:         let %10: i64 [synthetic] = div<i64, by_zero=ub, min_by_neg_one=ub>(read<i64>(%9), widen<i64, reason=usual_arith>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field4(deref(read<ptr<@type0>>(%3))))))));
// DEFAULT-NEXT:         write<i64>(%4, read<i64>(%10));
// DEFAULT-NEXT:         let %11: ptr<i64> [synthetic] = ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(read<ptr<i64>>(field5(deref(read<ptr<@type0>>(%3)))), read<i64>(%4));
// DEFAULT-NEXT:         let %12: i64 [synthetic] = read<i64>(deref(read<ptr<i64>>(%11)));
// DEFAULT-NEXT:         let %13: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%12), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(deref(read<ptr<i64>>(%11)), read<i64>(%13));
// DEFAULT-NEXT:         return read<i64>(%4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 val: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(1));
// DEFAULT-NEXT:         let %7 s: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = truncate<i16, reason=assign, fits=always>(const<i32>(0)), field1 = widen<i64, reason=assign>(const<i32>(0)), field2 = truncate<i16, reason=assign, fits=always>(const<i32>(0)), field3 = truncate<i8, reason=assign, fits=always>(const<i32>(0)), field4 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(2))), field5 = addr_of<ptr<i64>>(%6));
// DEFAULT-NEXT:         write<i64>(%6, call<i64, signature=fn(ptr<@type0>) -> i64>(%2, addr_of<ptr<@type0>>(%7)));
// DEFAULT-NEXT:         call<i64, signature=fn(ptr<@type0>) -> i64>(%2, addr_of<ptr<@type0>>(%7));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%6), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
