/* Test C23 auto.  Valid code, execution tests.  Based on auto-type-1.c.  */
/* { dg-do run } */
/* { dg-options "-std=c23 -pedantic-errors" } */

extern void abort (void);
extern void exit (int);

auto i = 1;
extern int i;
auto c = (char) 1;
extern char c;
static auto u = 10U;
extern unsigned int u;
const auto ll = 1LL;
extern const long long ll;

int
main (void)
{
  if (i != 1 || c != 1 || u != 10U)
    abort ();
  auto ai = i;
  int *aip = &ai;
  if (ai != 1)
    abort ();
  auto p = (int (*) [++i]) 0;
  if (i != 2)
    abort ();
  if (sizeof (*p) != 2 * sizeof (int))
    abort ();
  int vla[u][u];
  int (*vp)[u] = &vla[0];
  auto vpp = ++vp;
  if (vp != &vla[1])
    abort ();
  exit (0);
}

// SLATE-FILECHECK-STD DEFAULT c23
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
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_u:[0-9]+]] u: u32 [storage=static] = const<u32>(10) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_ll:[0-9]+]] ll: i64 [storage=static] [const] = const<i64>(1) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(%[[VALUE_i]]), const<i32>(1)), ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_c]])), const<i32>(1))), ne<u32>(read<u32>(%[[VALUE_u]]), const<u32>(10)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_ai:[0-9]+]] ai: i32 [storage=automatic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:         let %[[VALUE_aip:[0-9]+]] aip: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(%[[VALUE_ai]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_ai]]), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<vla<i32, %[[VALUE1:[0-9]+]]>> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:         let %[[VALUE1]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE3]])));
// DEFAULT-NEXT:         write<ptr<vla<i32, %[[VALUE1]]>>>(%[[VALUE_p]], null<ptr<vla<i32, %[[VALUE1]]>>>);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_i]]), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(mul<u64, overflow=wrap>(read<u64>(%[[VALUE1]]), const<u64>(4)), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), const<u64>(4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: u64 [synthetic] = widen<u64, reason=assign>(read<u32>(%[[VALUE_u]]));
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: u64 [synthetic] = widen<u64, reason=assign>(read<u32>(%[[VALUE_u]]));
// DEFAULT-NEXT:         let %[[VALUE_vla:[0-9]+]] vla: vla<vla<i32, %[[VALUE5]]>, %[[VALUE4]]> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: u64 [synthetic] = widen<u64, reason=assign>(read<u32>(%[[VALUE_u]]));
// DEFAULT-NEXT:         let %[[VALUE_vp:[0-9]+]] vp: ptr<vla<i32, %[[VALUE6]]>> [storage=automatic] = pointer_cast<ptr<vla<i32, %[[VALUE6]]>>, reason=assign>(addr_of<ptr<vla<i32, %[[VALUE5]]>>>(deref(ptr_offset<ptr<vla<i32, %[[VALUE5]]>>, subtract=false, element=vla<i32, %[[VALUE5]]>, overflow=ub>(array_decay<ptr<vla<i32, %[[VALUE5]]>>, length=None>(%[[VALUE_vla]]), const<i32>(0)))));
// DEFAULT-NEXT:         let %[[VALUE_vpp:[0-9]+]] vpp: ptr<vla<i32, %[[VALUE6]]>> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: ptr<vla<i32, %[[VALUE6]]>> [synthetic] = read<ptr<vla<i32, %[[VALUE6]]>>>(%[[VALUE_vp]]);
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: ptr<vla<i32, %[[VALUE6]]>> [synthetic] = ptr_offset<ptr<vla<i32, %[[VALUE6]]>>, subtract=false, element=vla<i32, %[[VALUE6]]>, overflow=ub>(read<ptr<vla<i32, %[[VALUE6]]>>>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<vla<i32, %[[VALUE6]]>>>(%[[VALUE_vp]], read<ptr<vla<i32, %[[VALUE6]]>>>(%[[VALUE8]]));
// DEFAULT-NEXT:         write<ptr<vla<i32, %[[VALUE6]]>>>(%[[VALUE_vpp]], read<ptr<vla<i32, %[[VALUE6]]>>>(%[[VALUE8]]));
// DEFAULT-NEXT:         if ne<ptr<vla<i32, %[[VALUE6]]>>>(read<ptr<vla<i32, %[[VALUE6]]>>>(%[[VALUE_vp]]), pointer_cast<ptr<vla<i32, %[[VALUE6]]>>, reason=usual_arith>(addr_of<ptr<vla<i32, %[[VALUE5]]>>>(deref(ptr_offset<ptr<vla<i32, %[[VALUE5]]>>, subtract=false, element=vla<i32, %[[VALUE5]]>, overflow=ub>(array_decay<ptr<vla<i32, %[[VALUE5]]>>, length=None>(%[[VALUE_vla]]), const<i32>(1))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
