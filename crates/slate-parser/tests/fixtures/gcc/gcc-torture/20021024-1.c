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
// DEFAULT-NEXT:     global %2 cp: ptr<u64> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 m: u64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %13 r: array<u64, 64> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %0 @exit(%15 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @bar(%7 rop: u32, %8 r: ptr<u64>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %9 rs1: u32 [storage=automatic];
// DEFAULT-NEXT:         let %10 rs2: u32 [storage=automatic];
// DEFAULT-NEXT:         let %11 rd: u32 [storage=automatic];
// DEFAULT-NEXT:         label %6 top:
// DEFAULT-NEXT:             write<u32>(%10, and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%7), const<i32>(23)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(511))));
// DEFAULT-NEXT:         write<u32>(%9, and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%7), const<i32>(9)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(511))));
// DEFAULT-NEXT:         write<u32>(%11, and<u32>(read<u32>(%7), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(511))));
// DEFAULT-NEXT:         write<u64>(deref(read<ptr<u64>>(%2)), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(1))));
// DEFAULT-NEXT:         write<u64>(%3, add<u64, overflow=wrap>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%8), read<u32>(%9)))), read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%8), read<u32>(%10))))));
// DEFAULT-NEXT:         write<u64>(deref(read<ptr<u64>>(%2)), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(2))));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if not<bool>(ne<u32>(read<u32>(%11), const<u32>(0)))
// DEFAULT-NEXT:             goto %6;
// DEFAULT-NEXT:         write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%8), read<u32>(%11))), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %14 cr: u64 [storage=automatic];
// DEFAULT-NEXT:         write<ptr<u64>>(%2, addr_of<ptr<u64>>(%14));
// DEFAULT-NEXT:         write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(64)>(%13), const<i32>(4))), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(47))));
// DEFAULT-NEXT:         write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(64)>(%13), const<i32>(8))), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(11))));
// DEFAULT-NEXT:         call<void, signature=fn(u32, ptr<u64>) -> void>(%5, reinterpret<u32, reason=arg, fits=unknown>(or<i32>(or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(8), const<i32>(23)), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(4), const<i32>(9))), const<i32>(15))), array_decay<ptr<u64>, length=Some(64)>(%13));
// DEFAULT-NEXT:         if ne<u64>(read<u64>(%3), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(add<i32, overflow=ub>(const<i32>(47), const<i32>(11)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%0, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
