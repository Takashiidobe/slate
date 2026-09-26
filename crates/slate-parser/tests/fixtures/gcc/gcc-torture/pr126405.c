/* AArch64 wrong code at -O2.  Store motion creates several SSA versions of an
   oversized vector temporary (V16DI, 128 bytes, no register mode).  The
   partition holding the loop-carried versions has an anonymous representative,
   so out-of-SSA left it and the partition of the copy taken for the use after
   the loop sharing one MEM_EXPR.  The load/store pair-fusion pass then treated
   two stores 144 bytes apart as adjacent, fused them, and left the tail of one
   slot uninitialised.  Self-checking: aborts if the result is wrong.  */

typedef long __attribute__((vector_size(16 * sizeof(long)))) v16di;
typedef int __attribute__((vector_size(16 * sizeof(int))))   v16si;

long  g2, g12;
v16di g18;
v16si g3;
void *g27;

/* The wrong value is read from an uninitialised stack slot, so make sure the
   stack the callee reuses does not happen to be zero.  */
__attribute__((noipa)) static void dirty_stack(void) {
  volatile char buf[1024];
  for (unsigned i = 0; i < sizeof(buf); i++)
    buf[i] = 0xa5;
}

void f31(void) {
lbl_br1:
  g18 = ~g18;
  g3  = ~g3;
  if (g2)
    goto lbl_br1;
lbl_b5:
  switch (g12)
  case 4:
  case 0:
    goto lbl_sw8;
  __builtin_abort();
lbl_sw8:
  if (g27)
    goto lbl_b5;
  g18 = ~g18;
}

int main(void) {
  dirty_stack();
  f31();
  for (int i = 0; i < 16; i++)
    if (g18[i] != 0)
      __builtin_abort();
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
// DEFAULT-NEXT:     type @type0 v16di = vector<i64, 16>;
// DEFAULT-NEXT:     type @type1 v16si = vector<i32, 16>;
// DEFAULT-NEXT:     global %2 g2: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 g12: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 g18: vector<i64, 16> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 g3: vector<i32, 16> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 g27: ptr<void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %7 @dirty_stack() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %8 buf: volatile array<i8, 1024> [storage=automatic] [align=16];
// DEFAULT-NEXT:         for %16
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %9 i: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(widen<u64, reason=usual_arith>(read<u32>(%9)), const<u64>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %19: u32 [synthetic] = read<u32>(%9);
// DEFAULT-NEXT:                 let %20: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%19), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%9, read<u32>(%20));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8, volatile>(deref(ptr_offset<ptr<volatile i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<volatile i8>, length=Some(1024)>(%8), read<u32>(%9))), truncate<i8, reason=assign, fits=unknown>(const<i32>(165)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @f31() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         label %11 lbl_br1:
// DEFAULT-NEXT:             write<vector<i64, 16>>(%4, not<vector<i64, 16>, elementwise=true>(read<vector<i64, 16>>(%4)));
// DEFAULT-NEXT:         write<vector<i32, 16>>(%5, not<vector<i32, 16>, elementwise=true>(read<vector<i32, 16>>(%5)));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%2), const<i64>(0))
// DEFAULT-NEXT:             goto %11;
// DEFAULT-NEXT:         label %12 lbl_b5:
// DEFAULT-NEXT:             switch %17 read<i64>(%3)
// DEFAULT-NEXT:                 case %17 const<i64>(4):
// DEFAULT-NEXT:                     case %17 const<i64>(0):
// DEFAULT-NEXT:                         goto %13;
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         label %13 lbl_sw8:
// DEFAULT-NEXT:             if ne<ptr<void>>(read<ptr<void>>(%6), null<ptr<void>>)
// DEFAULT-NEXT:                 goto %12;
// DEFAULT-NEXT:         write<vector<i64, 16>>(%4, not<vector<i64, 16>, elementwise=true>(read<vector<i64, 16>>(%4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%7);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:         for %18
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %15 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%15), const<i32>(16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %21: i32 [synthetic] = read<i32>(%15);
// DEFAULT-NEXT:                 let %22: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%21), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%15, read<i32>(%22));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i64>(read<i64>(lane(%4, read<i32>(%15))), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
