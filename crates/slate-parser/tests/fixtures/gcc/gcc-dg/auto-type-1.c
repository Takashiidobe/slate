/* Test __auto_type.  Test correct uses.  */
/* { dg-do run } */
/* { dg-options "" } */

extern void abort (void);
extern void exit (int);

__auto_type i = 1;
extern int i;
__auto_type c = (char) 1;
extern char c;
static __auto_type u = 10U;
extern unsigned int u;
const __auto_type ll = 1LL;
extern const long long ll;

int
main (void)
{
  if (i != 1 || c != 1 || u != 10U)
    abort ();
  __auto_type ai = i;
  int *aip = &ai;
  if (ai != 1)
    abort ();
  __auto_type p = (int (*) [++i]) 0;
  if (i != 2)
    abort ();
  if (sizeof (*p) != 2 * sizeof (int))
    abort ();
  int vla[u][u];
  int (*vp)[u] = &vla[0];
  __auto_type vpp = ++vp;
  if (vp != &vla[1])
    abort ();
  exit (0);
}

// SLATE-FILECHECK-FLAVOR gcc
// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     global %2 i: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %3 c: i8 [storage=static] = truncate<i8, reason=explicit, fits=always>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %4 u: u32 [storage=static] = const<u32>(10) [linkage=internal];
// DEFAULT-NEXT:     global %5 ll: i64 [storage=static] [const] = const<i64>(1) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%13 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(%2), const<i32>(1)), ne<i32>(widen<i32, reason=promotion>(read<i8>(%3)), const<i32>(1))), ne<u32>(read<u32>(%4), const<u32>(10)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %7 ai: i32 [storage=automatic] = read<i32>(%2);
// DEFAULT-NEXT:         let %8 aip: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(%7);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%7), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %9 p: ptr<vla<i32, %14>> [storage=automatic];
// DEFAULT-NEXT:         let %18: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %19: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%18), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%19));
// DEFAULT-NEXT:         let %14: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%19)));
// DEFAULT-NEXT:         write<ptr<vla<i32, %14>>>(%9, null<ptr<vla<i32, %14>>>);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%2), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(mul<u64, overflow=wrap>(read<u64>(%14), const<u64>(4)), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), const<u64>(4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %15: u64 [synthetic] = widen<u64, reason=assign>(read<u32>(%4));
// DEFAULT-NEXT:         let %16: u64 [synthetic] = widen<u64, reason=assign>(read<u32>(%4));
// DEFAULT-NEXT:         let %10 vla: vla<vla<i32, %16>, %15> [storage=automatic];
// DEFAULT-NEXT:         let %17: u64 [synthetic] = widen<u64, reason=assign>(read<u32>(%4));
// DEFAULT-NEXT:         let %11 vp: ptr<vla<i32, %17>> [storage=automatic] = pointer_cast<ptr<vla<i32, %17>>, reason=assign>(addr_of<ptr<vla<i32, %16>>>(deref(ptr_offset<ptr<vla<i32, %16>>, subtract=false, element=vla<i32, %16>, overflow=ub>(array_decay<ptr<vla<i32, %16>>, length=None>(%10), const<i32>(0)))));
// DEFAULT-NEXT:         let %12 vpp: ptr<vla<i32, %17>> [storage=automatic];
// DEFAULT-NEXT:         let %20: ptr<vla<i32, %17>> [synthetic] = read<ptr<vla<i32, %17>>>(%11);
// DEFAULT-NEXT:         let %21: ptr<vla<i32, %17>> [synthetic] = ptr_offset<ptr<vla<i32, %17>>, subtract=false, element=vla<i32, %17>, overflow=ub>(read<ptr<vla<i32, %17>>>(%20), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<vla<i32, %17>>>(%11, read<ptr<vla<i32, %17>>>(%21));
// DEFAULT-NEXT:         write<ptr<vla<i32, %17>>>(%12, read<ptr<vla<i32, %17>>>(%21));
// DEFAULT-NEXT:         if ne<ptr<vla<i32, %17>>>(read<ptr<vla<i32, %17>>>(%11), pointer_cast<ptr<vla<i32, %17>>, reason=usual_arith>(addr_of<ptr<vla<i32, %16>>>(deref(ptr_offset<ptr<vla<i32, %16>>, subtract=false, element=vla<i32, %16>, overflow=ub>(array_decay<ptr<vla<i32, %16>>, length=None>(%10), const<i32>(1))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
