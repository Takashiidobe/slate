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
// DEFAULT-NEXT:     type @type[[TYPE_v16di:[0-9]+]] v16di = vector<i64, 16>;
// DEFAULT-NEXT:     type @type[[TYPE_v16si:[0-9]+]] v16si = vector<i32, 16>;
// DEFAULT-NEXT:     global %[[VALUE_g2:[0-9]+]] g2: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g12:[0-9]+]] g12: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g18:[0-9]+]] g18: vector<i64, 16> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3:[0-9]+]] g3: vector<i32, 16> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g27:[0-9]+]] g27: ptr<void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_dirty_stack:[0-9]+]] @dirty_stack() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_buf:[0-9]+]] buf: volatile array<i8, 1024> [storage=automatic] [align=16];
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i:[0-9]+]] i: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(widen<u64, reason=usual_arith>(read<u32>(%[[VALUE_i]])), const<u64>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE1]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_i]], read<u32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8, volatile>(deref(ptr_offset<ptr<volatile i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<volatile i8>, length=Some(1024)>(%[[VALUE_buf]]), read<u32>(%[[VALUE_i]]))), truncate<i8, reason=assign, fits=unknown>(const<i32>(165)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_f31:[0-9]+]] @f31() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         label %[[VALUE_lbl_br1:[0-9]+]] lbl_br1:
// DEFAULT-NEXT:             write<vector<i64, 16>>(%[[VALUE_g18]], not<vector<i64, 16>, elementwise=true>(read<vector<i64, 16>>(%[[VALUE_g18]])));
// DEFAULT-NEXT:         write<vector<i32, 16>>(%[[VALUE_g3]], not<vector<i32, 16>, elementwise=true>(read<vector<i32, 16>>(%[[VALUE_g3]])));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%[[VALUE_g2]]), const<i64>(0))
// DEFAULT-NEXT:             goto %[[VALUE_lbl_br1]];
// DEFAULT-NEXT:         label %[[VALUE_lbl_b5:[0-9]+]] lbl_b5:
// DEFAULT-NEXT:             switch %[[VALUE3:[0-9]+]] read<i64>(%[[VALUE_g12]])
// DEFAULT-NEXT:                 case %[[VALUE3]] const<i64>(4):
// DEFAULT-NEXT:                     case %[[VALUE3]] const<i64>(0):
// DEFAULT-NEXT:                         goto %[[VALUE_lbl_sw8:[0-9]+]];
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         label %[[VALUE_lbl_sw8]] lbl_sw8:
// DEFAULT-NEXT:             if ne<ptr<void>>(read<ptr<void>>(%[[VALUE_g27]]), null<ptr<void>>)
// DEFAULT-NEXT:                 goto %[[VALUE_lbl_b5]];
// DEFAULT-NEXT:         write<vector<i64, 16>>(%[[VALUE_g18]], not<vector<i64, 16>, elementwise=true>(read<vector<i64, 16>>(%[[VALUE_g18]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_dirty_stack]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_f31]]);
// DEFAULT-NEXT:         for %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE5]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE6]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i64>(read<i64>(lane(%[[VALUE_g18]], read<i32>(%[[VALUE_i_2]]))), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
