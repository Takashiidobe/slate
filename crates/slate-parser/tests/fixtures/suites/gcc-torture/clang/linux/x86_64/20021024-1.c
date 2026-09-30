/* Origin: PR target/6981 from Mattias Engdegaard <mattias@virtutech.se>.  */
/* { dg-require-effective-target int32plus } */

void exit(int);
void abort(void);

unsigned long long *cp, m;

void foo(void) {}

void bar(unsigned rop, unsigned long long *r) {
  unsigned rs1, rs2, rd;

top:
  rs2 = (rop >> 23) & 0x1ff;
  rs1 = (rop >> 9) & 0x1ff;
  rd  = rop & 0x1ff;

  *cp = 1;
  m   = r[rs1] + r[rs2];
  *cp = 2;
  foo();
  if (!rd)
    goto top;
  r[rd] = 1;
}

int main(void) {
  static unsigned long long r[64];
  unsigned long long        cr;
  cp = &cr;

  r[4] = 47;
  r[8] = 11;
  bar((8 << 23) | (4 << 9) | 15, r);

  if (m != 47 + 11)
    abort();
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
// DEFAULT-NEXT:     global %[[VALUE_cp:[0-9]+]] cp: ptr<u64> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_m:[0-9]+]] m: u64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_r:[0-9]+]] r: array<u64, 64> [storage=static] [align=16] [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_rop:[0-9]+]] rop: u32, %[[VALUE_r_2:[0-9]+]] r: ptr<u64>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_rs1:[0-9]+]] rs1: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_rs2:[0-9]+]] rs2: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_rd:[0-9]+]] rd: u32 [storage=automatic];
// DEFAULT-NEXT:         label %[[VALUE_top:[0-9]+]] top:
// DEFAULT-NEXT:             write<u32>(%[[VALUE_rs2]], and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_rop]]), const<i32>(23)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(511))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_rs1]], and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_rop]]), const<i32>(9)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(511))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_rd]], and<u32>(read<u32>(%[[VALUE_rop]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(511))));
// DEFAULT-NEXT:         write<u64>(deref(read<ptr<u64>>(%[[VALUE_cp]])), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(1))));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_m]], add<u64, overflow=wrap>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%[[VALUE_r_2]]), read<u32>(%[[VALUE_rs1]])))), read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%[[VALUE_r_2]]), read<u32>(%[[VALUE_rs2]]))))));
// DEFAULT-NEXT:         write<u64>(deref(read<ptr<u64>>(%[[VALUE_cp]])), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(2))));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_foo]]);
// DEFAULT-NEXT:         if not<bool>(ne<u32>(read<u32>(%[[VALUE_rd]]), const<u32>(0)))
// DEFAULT-NEXT:             goto %[[VALUE_top]];
// DEFAULT-NEXT:         write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%[[VALUE_r_2]]), read<u32>(%[[VALUE_rd]]))), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_cr:[0-9]+]] cr: u64 [storage=automatic];
// DEFAULT-NEXT:         write<ptr<u64>>(%[[VALUE_cp]], addr_of<ptr<u64>>(%[[VALUE_cr]]));
// DEFAULT-NEXT:         write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(64)>(%[[VALUE_r]]), const<i32>(4))), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(47))));
// DEFAULT-NEXT:         write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(64)>(%[[VALUE_r]]), const<i32>(8))), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(11))));
// DEFAULT-NEXT:         call<void, signature=fn(u32, ptr<u64>) -> void>(%[[VALUE_bar]], reinterpret<u32, reason=arg, fits=unknown>(or<i32>(or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(8), const<i32>(23)), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(4), const<i32>(9))), const<i32>(15))), array_decay<ptr<u64>, length=Some(64)>(%[[VALUE_r]]));
// DEFAULT-NEXT:         if ne<u64>(read<u64>(%[[VALUE_m]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(add<i32, overflow=ub>(const<i32>(47), const<i32>(11)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
