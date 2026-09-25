/* Source: PR 321 modified for test suite by Neil Booth 14 Jan 2001.  */

void abort(void);
void exit(int);

typedef unsigned long long uint64;
unsigned long              pars;

uint64  b[32];
uint64 *r = b;

void alpha_ep_extbl_i_eq_0() {
  unsigned int rb, ra, rc;

  rb = (((unsigned long)(pars) >> 27)) & 0x1fUL;
  ra = (((unsigned int)(pars) >> 5)) & 0x1fUL;
  rc = (((unsigned int)(pars) >> 0)) & 0x1fUL;
  {
    uint64 temp = ((r[ra] >> ((r[rb] & 0x7) << 3)) & 0x00000000000000FFLL);
    if (rc != 31)
      r[rc] = temp;
  }
}

int main(void) {
  if (sizeof(uint64) == 8) {
    b[17] = 0x0000000000303882ULL; /* rb */
    b[2]  = 0x534f4f4c494d000aULL; /* ra & rc */

    pars = 0x88000042; /* 17, 2, 2 coded */
    alpha_ep_extbl_i_eq_0();

    if (b[2] != 0x4d)
      abort();
  }

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
// DEFAULT-NEXT:     type @type0 uint64 = u64;
// DEFAULT-NEXT:     global %3 pars: u64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 b: array<u64, 32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 r: ptr<u64> [storage=static] = array_decay<ptr<u64>, length=Some(32)>(%4) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%12 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %6 @alpha_ep_extbl_i_eq_0() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %7 rb: u32 [storage=automatic];
// DEFAULT-NEXT:         let %8 ra: u32 [storage=automatic];
// DEFAULT-NEXT:         let %9 rc: u32 [storage=automatic];
// DEFAULT-NEXT:         write<u32>(%7, truncate<u32, reason=assign, fits=unknown>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%3), const<i32>(27)), const<u64>(31))));
// DEFAULT-NEXT:         write<u32>(%8, truncate<u32, reason=assign, fits=unknown>(and<u64>(widen<u64, reason=usual_arith>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(truncate<u32, reason=explicit, fits=unknown>(read<u64>(%3)), const<i32>(5))), const<u64>(31))));
// DEFAULT-NEXT:         write<u32>(%9, truncate<u32, reason=assign, fits=unknown>(and<u64>(widen<u64, reason=usual_arith>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(truncate<u32, reason=explicit, fits=unknown>(read<u64>(%3)), const<i32>(0))), const<u64>(31))));
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %10 temp: u64 [storage=automatic] = and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%5), read<u32>(%8)))), shl<u64, overflow=wrap, amount_out_of_range=ub>(and<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%5), read<u32>(%7)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(7)))), const<i32>(3))), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(255)));
// DEFAULT-NEXT:             if ne<u32>(read<u32>(%9), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(31)))
// DEFAULT-NEXT:                 write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%5), read<u32>(%9))), read<u64>(%10));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if eq<u64>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(32)>(%4), const<i32>(17))), const<u64>(3160194));
// DEFAULT-NEXT:                 write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(32)>(%4), const<i32>(2))), const<u64>(6003104017374052362));
// DEFAULT-NEXT:                 write<u64>(%3, widen<u64, reason=assign>(const<u32>(2281701442)));
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:                 if ne<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(32)>(%4), const<i32>(2)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(77))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
