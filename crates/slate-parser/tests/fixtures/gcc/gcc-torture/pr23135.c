/* Based on execute/simd-1.c, modified by joern.rennecke@st.com to
   trigger a reload bug.  Verified for gcc mainline from 20050722 13:00 UTC
   for sh-elf -m4 -O2.  */
/* { dg-options "-Wno-psabi -fwrapv" } */
/* { dg-add-options stack_size } */

#ifndef STACK_SIZE
#define STACK_SIZE (256 * 1024)
#endif

extern void abort(void);
extern void exit(int);

typedef struct {
  char c[STACK_SIZE / 2];
} big_t;

typedef int __attribute__((mode(SI))) __attribute__((vector_size(8))) vecint;
typedef int __attribute__((mode(SI)))                                 siint;

vecint i = {150, 100};
vecint j = {10, 13};
vecint k;

union {
  vecint v;
  siint  i[2];
} res;

void verify(siint a1, siint a2, siint b1, siint b2, big_t big) {
  if (a1 != b1 || a2 != b2)
    abort();
}

int main() {
  big_t  big;
  vecint k0, k1, k2, k3, k4, k5, k6, k7;

  k0    = i + j;
  res.v = k0;

  verify(res.i[0], res.i[1], 160, 113, big);

  k1    = i * j;
  res.v = k1;

  verify(res.i[0], res.i[1], 1500, 1300, big);

  k2 = i / j;
  /* This is the observed failure - reload 0 has the wrong type and thus the
     conflict with reload 1 is missed:

  (insn:HI 94 92 96 1 pr23135.c:46 (parallel [
              (set (subreg:SI (reg:DI 253) 0)
                  (div:SI (reg:SI 4 r4)
                      (reg:SI 5 r5)))
              (clobber (reg:SI 146 pr))
              (clobber (reg:DF 64 fr0))
              (clobber (reg:DF 66 fr2))
              (use (reg:PSI 151 ))
              (use (reg/f:SI 256))
          ]) 60 {divsi3_i4} (insn_list:REG_DEP_TRUE 90 (insn_list:REG_DEP_TRUE
  89 (insn_list:REG_DEP_TRUE 42 (insn_list:REG_DEP_TRUE 83
  (insn_list:REG_DEP_TRUE 92 (insn_list:REG_DEP_TRUE 91 (nil)))))))
      (expr_list:REG_DEAD (reg:SI 4 r4)
          (expr_list:REG_DEAD (reg:SI 5 r5)
              (expr_list:REG_UNUSED (reg:DF 66 fr2)
                  (expr_list:REG_UNUSED (reg:DF 64 fr0)
                      (expr_list:REG_UNUSED (reg:SI 146 pr)
                          (insn_list:REG_RETVAL 91 (nil))))))))

  Reloads for insn # 94
  Reload 0: reload_in (SI) = (plus:SI (reg/f:SI 14 r14)
                                                      (const_int 64 [0x40]))
          GENERAL_REGS, RELOAD_FOR_OUTADDR_ADDRESS (opnum = 0)
          reload_in_reg: (plus:SI (reg/f:SI 14 r14)
                                                      (const_int 64 [0x40]))
          reload_reg_rtx: (reg:SI 3 r3)
  Reload 1: GENERAL_REGS, RELOAD_FOR_OUTPUT_ADDRESS (opnum = 0), can't combine,
  se condary_reload_p reload_reg_rtx: (reg:SI 3 r3) Reload 2: reload_out (SI) =
  (mem:SI (plus:SI (plus:SI (reg/f:SI 14 r14) (const_int 64 [0x40])) (const_int
  28 [0x1c])) [ 16 S8 A32]) FPUL_REGS, RELOAD_FOR_OUTPUT (opnum = 0)
          reload_out_reg: (subreg:SI (reg:DI 253) 0)
          reload_reg_rtx: (reg:SI 150 fpul)
          secondary_out_reload = 1

  Reload 3: reload_in (SI) = (symbol_ref:SI ("__sdivsi3_i4") [flags 0x1])
          GENERAL_REGS, RELOAD_FOR_INPUT (opnum = 1), can't combine
          reload_in_reg: (reg/f:SI 256)
          reload_reg_rtx: (reg:SI 3 r3)
    */

  res.v = k2;

  verify(res.i[0], res.i[1], 15, 7, big);

  k3    = i & j;
  res.v = k3;

  verify(res.i[0], res.i[1], 2, 4, big);

  k4    = i | j;
  res.v = k4;

  verify(res.i[0], res.i[1], 158, 109, big);

  k5    = i ^ j;
  res.v = k5;

  verify(res.i[0], res.i[1], 156, 105, big);

  k6    = -i;
  res.v = k6;
  verify(res.i[0], res.i[1], -150, -100, big);

  k7    = ~i;
  res.v = k7;
  verify(res.i[0], res.i[1], -151, -101, big);

  k     = k0 + k1 + k3 + k4 + k5 + k6 + k7;
  res.v = k;
  verify(res.i[0], res.i[1], 1675, 1430, big);

  k     = k0 * k1 * k3 * k4 * k5 * k6 * k7;
  res.v = k;
  verify(res.i[0], res.i[1], 1456467968, -1579586240, big);

  k     = k0 / k1 / k2 / k3 / k4 / k5 / k6 / k7;
  res.v = k;
  verify(res.i[0], res.i[1], 0, 0, big);

  exit(0);
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
// DEFAULT-NEXT:     type @type0 = struct {
// DEFAULT-NEXT:         field0 c: array<i8, 131072>;
// DEFAULT-NEXT:     } [size=131072, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type1 big_t = @type0;
// DEFAULT-NEXT:     type @type2 vecint = vector<i32, 2>;
// DEFAULT-NEXT:     type @type3 siint = i32;
// DEFAULT-NEXT:     type @type4 = union {
// DEFAULT-NEXT:         field0 v: vector<i32, 2>;
// DEFAULT-NEXT:         field1 i: array<i32, 2>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     global %6 i: vector<i32, 2> [storage=static] = aggregate<vector<i32, 2>, zero_fill=false>(index0 = const<i32>(150), index1 = const<i32>(100)) [linkage=external];
// DEFAULT-NEXT:     global %7 j: vector<i32, 2> [storage=static] = aggregate<vector<i32, 2>, zero_fill=false>(index0 = const<i32>(10), index1 = const<i32>(13)) [linkage=external];
// DEFAULT-NEXT:     global %8 k: vector<i32, 2> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %10 res: @type4 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%27 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %11 @verify(%12 a1: i32, %13 a2: i32, %14 b1: i32, %15 b2: i32, %16 big: @type0) -> void [linkage=external] [abi=sysv64(scalar, scalar, scalar, scalar, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(%12), read<i32>(%14)), ne<i32>(read<i32>(%13), read<i32>(%15)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %18 big: @type0 [storage=automatic];
// DEFAULT-NEXT:         let %19 k0: vector<i32, 2> [storage=automatic];
// DEFAULT-NEXT:         let %20 k1: vector<i32, 2> [storage=automatic];
// DEFAULT-NEXT:         let %21 k2: vector<i32, 2> [storage=automatic];
// DEFAULT-NEXT:         let %22 k3: vector<i32, 2> [storage=automatic];
// DEFAULT-NEXT:         let %23 k4: vector<i32, 2> [storage=automatic];
// DEFAULT-NEXT:         let %24 k5: vector<i32, 2> [storage=automatic];
// DEFAULT-NEXT:         let %25 k6: vector<i32, 2> [storage=automatic];
// DEFAULT-NEXT:         let %26 k7: vector<i32, 2> [storage=automatic];
// DEFAULT-NEXT:         write<vector<i32, 2>>(%19, add<vector<i32, 2>, elementwise=true, overflow=wrap>(read<vector<i32, 2>>(%6), read<vector<i32, 2>>(%7)));
// DEFAULT-NEXT:         write<vector<i32, 2>>(field0(%10), read<vector<i32, 2>>(%19));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, @type0) -> void, abi=sysv64(scalar, scalar, scalar, scalar, native_c) -> void>(%11, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field1(%10)), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field1(%10)), const<i32>(1)))), const<i32>(160), const<i32>(113), copy<@type0, reason=arg>(read<@type0>(%18)));
// DEFAULT-NEXT:         write<vector<i32, 2>>(%20, mul<vector<i32, 2>, elementwise=true, overflow=wrap>(read<vector<i32, 2>>(%6), read<vector<i32, 2>>(%7)));
// DEFAULT-NEXT:         write<vector<i32, 2>>(field0(%10), read<vector<i32, 2>>(%20));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, @type0) -> void, abi=sysv64(scalar, scalar, scalar, scalar, native_c) -> void>(%11, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field1(%10)), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field1(%10)), const<i32>(1)))), const<i32>(1500), const<i32>(1300), copy<@type0, reason=arg>(read<@type0>(%18)));
// DEFAULT-NEXT:         write<vector<i32, 2>>(%21, div<vector<i32, 2>, elementwise=true, by_zero=ub, min_by_neg_one=ub>(read<vector<i32, 2>>(%6), read<vector<i32, 2>>(%7)));
// DEFAULT-NEXT:         write<vector<i32, 2>>(field0(%10), read<vector<i32, 2>>(%21));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, @type0) -> void, abi=sysv64(scalar, scalar, scalar, scalar, native_c) -> void>(%11, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field1(%10)), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field1(%10)), const<i32>(1)))), const<i32>(15), const<i32>(7), copy<@type0, reason=arg>(read<@type0>(%18)));
// DEFAULT-NEXT:         write<vector<i32, 2>>(%22, and<vector<i32, 2>, elementwise=true>(read<vector<i32, 2>>(%6), read<vector<i32, 2>>(%7)));
// DEFAULT-NEXT:         write<vector<i32, 2>>(field0(%10), read<vector<i32, 2>>(%22));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, @type0) -> void, abi=sysv64(scalar, scalar, scalar, scalar, native_c) -> void>(%11, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field1(%10)), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field1(%10)), const<i32>(1)))), const<i32>(2), const<i32>(4), copy<@type0, reason=arg>(read<@type0>(%18)));
// DEFAULT-NEXT:         write<vector<i32, 2>>(%23, or<vector<i32, 2>, elementwise=true>(read<vector<i32, 2>>(%6), read<vector<i32, 2>>(%7)));
// DEFAULT-NEXT:         write<vector<i32, 2>>(field0(%10), read<vector<i32, 2>>(%23));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, @type0) -> void, abi=sysv64(scalar, scalar, scalar, scalar, native_c) -> void>(%11, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field1(%10)), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field1(%10)), const<i32>(1)))), const<i32>(158), const<i32>(109), copy<@type0, reason=arg>(read<@type0>(%18)));
// DEFAULT-NEXT:         write<vector<i32, 2>>(%24, xor<vector<i32, 2>, elementwise=true>(read<vector<i32, 2>>(%6), read<vector<i32, 2>>(%7)));
// DEFAULT-NEXT:         write<vector<i32, 2>>(field0(%10), read<vector<i32, 2>>(%24));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, @type0) -> void, abi=sysv64(scalar, scalar, scalar, scalar, native_c) -> void>(%11, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field1(%10)), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field1(%10)), const<i32>(1)))), const<i32>(156), const<i32>(105), copy<@type0, reason=arg>(read<@type0>(%18)));
// DEFAULT-NEXT:         write<vector<i32, 2>>(%25, neg<vector<i32, 2>, elementwise=true, overflow=wrap>(read<vector<i32, 2>>(%6)));
// DEFAULT-NEXT:         write<vector<i32, 2>>(field0(%10), read<vector<i32, 2>>(%25));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, @type0) -> void, abi=sysv64(scalar, scalar, scalar, scalar, native_c) -> void>(%11, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field1(%10)), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field1(%10)), const<i32>(1)))), neg<i32, overflow=ub>(const<i32>(150)), neg<i32, overflow=ub>(const<i32>(100)), copy<@type0, reason=arg>(read<@type0>(%18)));
// DEFAULT-NEXT:         write<vector<i32, 2>>(%26, not<vector<i32, 2>, elementwise=true>(read<vector<i32, 2>>(%6)));
// DEFAULT-NEXT:         write<vector<i32, 2>>(field0(%10), read<vector<i32, 2>>(%26));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, @type0) -> void, abi=sysv64(scalar, scalar, scalar, scalar, native_c) -> void>(%11, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field1(%10)), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field1(%10)), const<i32>(1)))), neg<i32, overflow=ub>(const<i32>(151)), neg<i32, overflow=ub>(const<i32>(101)), copy<@type0, reason=arg>(read<@type0>(%18)));
// DEFAULT-NEXT:         write<vector<i32, 2>>(%8, add<vector<i32, 2>, elementwise=true, overflow=wrap>(add<vector<i32, 2>, elementwise=true, overflow=wrap>(add<vector<i32, 2>, elementwise=true, overflow=wrap>(add<vector<i32, 2>, elementwise=true, overflow=wrap>(add<vector<i32, 2>, elementwise=true, overflow=wrap>(add<vector<i32, 2>, elementwise=true, overflow=wrap>(read<vector<i32, 2>>(%19), read<vector<i32, 2>>(%20)), read<vector<i32, 2>>(%22)), read<vector<i32, 2>>(%23)), read<vector<i32, 2>>(%24)), read<vector<i32, 2>>(%25)), read<vector<i32, 2>>(%26)));
// DEFAULT-NEXT:         write<vector<i32, 2>>(field0(%10), read<vector<i32, 2>>(%8));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, @type0) -> void, abi=sysv64(scalar, scalar, scalar, scalar, native_c) -> void>(%11, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field1(%10)), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field1(%10)), const<i32>(1)))), const<i32>(1675), const<i32>(1430), copy<@type0, reason=arg>(read<@type0>(%18)));
// DEFAULT-NEXT:         write<vector<i32, 2>>(%8, mul<vector<i32, 2>, elementwise=true, overflow=wrap>(mul<vector<i32, 2>, elementwise=true, overflow=wrap>(mul<vector<i32, 2>, elementwise=true, overflow=wrap>(mul<vector<i32, 2>, elementwise=true, overflow=wrap>(mul<vector<i32, 2>, elementwise=true, overflow=wrap>(mul<vector<i32, 2>, elementwise=true, overflow=wrap>(read<vector<i32, 2>>(%19), read<vector<i32, 2>>(%20)), read<vector<i32, 2>>(%22)), read<vector<i32, 2>>(%23)), read<vector<i32, 2>>(%24)), read<vector<i32, 2>>(%25)), read<vector<i32, 2>>(%26)));
// DEFAULT-NEXT:         write<vector<i32, 2>>(field0(%10), read<vector<i32, 2>>(%8));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, @type0) -> void, abi=sysv64(scalar, scalar, scalar, scalar, native_c) -> void>(%11, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field1(%10)), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field1(%10)), const<i32>(1)))), const<i32>(1456467968), neg<i32, overflow=ub>(const<i32>(1579586240)), copy<@type0, reason=arg>(read<@type0>(%18)));
// DEFAULT-NEXT:         write<vector<i32, 2>>(%8, div<vector<i32, 2>, elementwise=true, by_zero=ub, min_by_neg_one=ub>(div<vector<i32, 2>, elementwise=true, by_zero=ub, min_by_neg_one=ub>(div<vector<i32, 2>, elementwise=true, by_zero=ub, min_by_neg_one=ub>(div<vector<i32, 2>, elementwise=true, by_zero=ub, min_by_neg_one=ub>(div<vector<i32, 2>, elementwise=true, by_zero=ub, min_by_neg_one=ub>(div<vector<i32, 2>, elementwise=true, by_zero=ub, min_by_neg_one=ub>(div<vector<i32, 2>, elementwise=true, by_zero=ub, min_by_neg_one=ub>(read<vector<i32, 2>>(%19), read<vector<i32, 2>>(%20)), read<vector<i32, 2>>(%21)), read<vector<i32, 2>>(%22)), read<vector<i32, 2>>(%23)), read<vector<i32, 2>>(%24)), read<vector<i32, 2>>(%25)), read<vector<i32, 2>>(%26)));
// DEFAULT-NEXT:         write<vector<i32, 2>>(field0(%10), read<vector<i32, 2>>(%8));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, @type0) -> void, abi=sysv64(scalar, scalar, scalar, scalar, native_c) -> void>(%11, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field1(%10)), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field1(%10)), const<i32>(1)))), const<i32>(0), const<i32>(0), copy<@type0, reason=arg>(read<@type0>(%18)));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
