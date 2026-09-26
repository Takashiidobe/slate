/* PR middle-end/57748 */
/* { dg-do run } */
/* ICE in expand_assignment:
   misalignp == true, !MEM_P (to_rtx), offset != 0,
   => gcc_assert (TREE_CODE (offset) == INTEGER_CST) */

#include <stdlib.h>

extern void abort(void);

typedef long long V
    __attribute__((vector_size(2 * sizeof(long long)), may_alias));

typedef struct S {
  V a;
  V b[0];
} P __attribute__((aligned(1)));

struct __attribute__((packed)) T {
  char c;
  P    s;
};

void __attribute__((noinline, noclone)) check(struct T *t) {
  if (t->s.b[0][0] != 3 || t->s.b[0][1] != 4)
    abort();
}

int __attribute__((noinline, noclone)) get_i(void) { return 0; }

void __attribute__((noinline, noclone)) foo(P *p) {
  V   a   = {3, 4};
  int i   = get_i();
  p->b[i] = a;
}

int
main() {
  struct T *t = (struct T *)calloc(128, 1);

  foo(&t->s);
  check(t);

  free(t);
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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     type @type1 V = vector<i64, 2>;
// DEFAULT-NEXT:     type @type2 S = struct {
// DEFAULT-NEXT:         field0 a: vector<i64, 2>;
// DEFAULT-NEXT:         field1 b: array<vector<i64, 2>, 0>;
// DEFAULT-NEXT:     } [size=16, align=16, offsets=[0, 16]];
// DEFAULT-NEXT:     type @type3 P = @type2;
// DEFAULT-NEXT:     type @type4 T = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 s: @type2;
// DEFAULT-NEXT:     } [size=17, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     fn %1 @calloc(%17 __nmemb: u64, %18 __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %2 @free(%19 __ptr: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %8 @check(%9 t: ptr<@type4>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(ne<i64>(read<i64>(lane(deref(ptr_offset<ptr<vector<i64, 2>>, subtract=false, element=vector<i64, 2>, overflow=ub>(array_decay<ptr<vector<i64, 2>>, length=Some(0)>(field1(field1(deref(read<ptr<@type4>>(%9))))), const<i32>(0))), const<i32>(0))), widen<i64, reason=usual_arith>(const<i32>(3))), ne<i64>(read<i64>(lane(deref(ptr_offset<ptr<vector<i64, 2>>, subtract=false, element=vector<i64, 2>, overflow=ub>(array_decay<ptr<vector<i64, 2>>, length=Some(0)>(field1(field1(deref(read<ptr<@type4>>(%9))))), const<i32>(0))), const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @get_i() -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @foo(%12 p: ptr<@type2>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %13 a: vector<i64, 2> [storage=automatic] = aggregate<vector<i64, 2>, zero_fill=false>(index0 = widen<i64, reason=assign>(const<i32>(3)), index1 = widen<i64, reason=assign>(const<i32>(4)));
// DEFAULT-NEXT:         let %14 i: i32 [storage=automatic] = call<i32, signature=fn() -> i32>(%10);
// DEFAULT-NEXT:         write<vector<i64, 2>>(deref(ptr_offset<ptr<vector<i64, 2>>, subtract=false, element=vector<i64, 2>, overflow=ub>(array_decay<ptr<vector<i64, 2>>, length=Some(0)>(field1(deref(read<ptr<@type2>>(%12)))), read<i32>(%14))), read<vector<i64, 2>>(%13));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %16 t: ptr<@type4> [storage=automatic] = pointer_cast<ptr<@type4>, reason=explicit>(call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(calloc, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(128))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type2>) -> void>(%11, addr_of<ptr<@type2>>(field1(deref(read<ptr<@type4>>(%16)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type4>) -> void>(%8, read<ptr<@type4>>(%16));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(free, pointer_cast<ptr<void>, reason=arg>(read<ptr<@type4>>(%16)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
