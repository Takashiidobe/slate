/* PR middle-end/52419 */
/* { dg-do run } */

extern void abort(void);

typedef long long V
    __attribute__((vector_size(2 * sizeof(long long)), may_alias));

typedef struct S {
  V b;
} P __attribute__((aligned(1)));

struct __attribute__((packed)) T {
  char c;
  P    s;
};

__attribute__((noinline, noclone)) void foo(P *p) { p->b[1] = 5; }

int
main() {
  V        a = {3, 4};
  struct T t;

  t.s.b = a;
  foo(&t.s);

  if (t.s.b[0] != 3 || t.s.b[1] != 5)
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
// DEFAULT-NEXT:     type @type0 V = vector<i64, 2>;
// DEFAULT-NEXT:     type @type1 S = struct {
// DEFAULT-NEXT:         field0 b: vector<i64, 2>;
// DEFAULT-NEXT:     } [size=16, align=16, offsets=[0]];
// DEFAULT-NEXT:     type @type2 P = @type1;
// DEFAULT-NEXT:     type @type3 T = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 s: @type1;
// DEFAULT-NEXT:     } [size=17, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %5 @foo(%6 p: ptr<@type1>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i64>(lane(field0(deref(read<ptr<@type1>>(%6))), const<i32>(1)), widen<i64, reason=assign>(const<i32>(5)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %8 a: vector<i64, 2> [storage=automatic] = aggregate<vector<i64, 2>, zero_fill=false>(index0 = widen<i64, reason=assign>(const<i32>(3)), index1 = widen<i64, reason=assign>(const<i32>(4)));
// DEFAULT-NEXT:         let %9 t: @type3 [storage=automatic];
// DEFAULT-NEXT:         write<vector<i64, 2>>(field0(field1(%9)), read<vector<i64, 2>>(%8));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type1>) -> void>(%5, addr_of<ptr<@type1>>(field1(%9)));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i64>(read<i64>(lane(field0(field1(%9)), const<i32>(0))), widen<i64, reason=usual_arith>(const<i32>(3))), ne<i64>(read<i64>(lane(field0(field1(%9)), const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(5))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
