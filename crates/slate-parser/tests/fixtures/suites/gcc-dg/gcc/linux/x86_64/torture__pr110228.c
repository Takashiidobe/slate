/* { dg-do run { target x86_64-*-* i?86-*-* } } */
/* { dg-require-effective-target lp64 } */

unsigned a[4] = {1,1,1,1};
unsigned tt1 = 0;

__attribute__((noipa))
static void bug(unsigned * p, unsigned *t, int n, int t2)
{
  for(int i = 0; i < n; i++)
    {
      _Bool LookupFlags ;
      unsigned v = t[i];
      unsigned tt = tt1;
      if (v == 0)
	LookupFlags = 0;
      else if (v == 1)
	LookupFlags = 1;
      if (LookupFlags) {
	  tt|=3u;
	  LookupFlags = 0;
      }
      asm("movq $-1, %q1":"+a"(LookupFlags));
      *p = tt;
    }
}

int main()
{
  unsigned r = 42;
  bug(&r,a, sizeof(a)/sizeof(a[0]), 1);
  __builtin_printf("%u\n", r);
  if (r != 3) __builtin_abort();
}

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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: array<u32, 4> [storage=static] [align=16] = aggregate<array<u32, 4>, zero_fill=false>(index0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(1)), index1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(1)), index2 = reinterpret<u32, reason=assign, fits=always>(const<i32>(1)), index3 = reinterpret<u32, reason=assign, fits=always>(const<i32>(1))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_tt1:[0-9]+]] tt1: u32 [storage=static] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_bug:[0-9]+]] @bug(%[[VALUE_p:[0-9]+]] p: ptr<u32>, %[[VALUE_t:[0-9]+]] t: ptr<u32>, %[[VALUE_n:[0-9]+]] n: i32, %[[VALUE_t2:[0-9]+]] t2: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_n]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_LookupFlags:[0-9]+]] LookupFlags: bool [storage=automatic];
// DEFAULT-NEXT:                     let %[[VALUE_v:[0-9]+]] v: u32 [storage=automatic] = read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(%[[VALUE_t]]), read<i32>(%[[VALUE_i]]))));
// DEFAULT-NEXT:                     let %[[VALUE_tt:[0-9]+]] tt: u32 [storage=automatic] = read<u32>(%[[VALUE_tt1]]);
// DEFAULT-NEXT:                     if eq<u32>(read<u32>(%[[VALUE_v]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:                         write<bool>(%[[VALUE_LookupFlags]], ne<i32, reason=assign>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         if eq<u32>(read<u32>(%[[VALUE_v]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))
// DEFAULT-NEXT:                             write<bool>(%[[VALUE_LookupFlags]], ne<i32, reason=assign>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:                     if read<bool>(%[[VALUE_LookupFlags]])
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             let %[[VALUE3:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_tt]]);
// DEFAULT-NEXT:                             let %[[VALUE4:[0-9]+]]: u32 [synthetic] = or<u32>(read<u32>(%[[VALUE3]]), const<u32>(3));
// DEFAULT-NEXT:                             write<u32>(%[[VALUE_tt]], read<u32>(%[[VALUE4]]));
// DEFAULT-NEXT:                             write<bool>(%[[VALUE_LookupFlags]], ne<i32, reason=assign>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     asm "movq $-1, %q1" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:                         template: "movq $-1, " %q0(64);
// DEFAULT-NEXT:                         inlateout 0 "a" [{ax}] width 8 place<bool>(%[[VALUE_LookupFlags]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     write<u32>(deref(read<ptr<u32>>(%[[VALUE_p]])), read<u32>(%[[VALUE_tt]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_printf:[0-9]+]] @__builtin_printf(%[[VALUE5:[0-9]+]] <unnamed>: ptr<const i8>, ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(42));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<u32>, ptr<u32>, i32, i32) -> void>(%[[VALUE_bug]], addr_of<ptr<u32>>(%[[VALUE_r]]), array_decay<ptr<u32>, length=Some(4)>(%[[VALUE_a]]), reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=unknown>(div<u64, by_zero=ub>(const<u64>(16), const<u64>(4)))), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str]])), read<u32>(%[[VALUE_r]]));
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%[[VALUE_r]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
