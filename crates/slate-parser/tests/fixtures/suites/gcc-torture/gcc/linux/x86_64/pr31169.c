extern void abort();

#define HOST_WIDE_INT          long
#define HOST_BITS_PER_WIDE_INT (sizeof(long) * 8)

struct tree_type {
  unsigned int precision : 9;
};

int sign_bit_p(struct tree_type *t, HOST_WIDE_INT val_hi,
               unsigned HOST_WIDE_INT val_lo) {
  unsigned HOST_WIDE_INT mask_lo, lo;
  HOST_WIDE_INT          mask_hi, hi;
  int                    width = t->precision;

  if (width > HOST_BITS_PER_WIDE_INT) {
    hi = (unsigned HOST_WIDE_INT)1 << (width - HOST_BITS_PER_WIDE_INT - 1);
    lo = 0;

    mask_hi =
        ((unsigned HOST_WIDE_INT) - 1 >> (2 * HOST_BITS_PER_WIDE_INT - width));
    mask_lo = -1;
  } else {
    hi = 0;
    lo = (unsigned HOST_WIDE_INT)1 << (width - 1);

    mask_hi = 0;
    mask_lo =
        ((unsigned HOST_WIDE_INT) - 1 >> (HOST_BITS_PER_WIDE_INT - width));
  }

  if ((val_hi & mask_hi) == hi && (val_lo & mask_lo) == lo)
    return 1;

  return 0;
}

int main() {
  struct tree_type t;
  t.precision = 1;
  if (!sign_bit_p(&t, 0, -1))
    abort();
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
// DEFAULT-NEXT:     type @type0 tree_type = struct {
// DEFAULT-NEXT:         field0 precision: u32 : 9;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 2)], field_units=[Some(0)]];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @sign_bit_p(%3 t: ptr<@type0>, %4 val_hi: i64, %5 val_lo: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %6 mask_lo: u64 [storage=automatic];
// DEFAULT-NEXT:         let %7 lo: u64 [storage=automatic];
// DEFAULT-NEXT:         let %8 mask_hi: i64 [storage=automatic];
// DEFAULT-NEXT:         let %9 hi: i64 [storage=automatic];
// DEFAULT-NEXT:         let %10 width: i32 [storage=automatic] = reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..2, bits=0..9>(deref(read<ptr<@type0>>(%3)))));
// DEFAULT-NEXT:         if gt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%10))), mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i64>(%9, reinterpret<i64, reason=assign, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), sub<u64, overflow=wrap>(sub<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%10))), mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:                 write<u64>(%7, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:                 write<i64>(%8, reinterpret<i64, reason=assign, fits=unknown>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%10)))))));
// DEFAULT-NEXT:                 write<u64>(%6, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i64>(%9, widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 write<u64>(%7, shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), sub<i32, overflow=ub>(read<i32>(%10), const<i32>(1))));
// DEFAULT-NEXT:                 write<i64>(%8, widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 write<u64>(%6, shr<u64, amount_out_of_range=ub, fill=zero_extend>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%10))))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if logical_and<bool>(eq<i64>(and<i64>(read<i64>(%4), read<i64>(%8)), read<i64>(%9)), eq<u64>(and<u64>(read<u64>(%5), read<u64>(%6)), read<u64>(%7)))
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %12 t: @type0 [storage=automatic];
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..2, bits=0..9>(%12), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(ptr<@type0>, i64, u64) -> i32>(%2, addr_of<ptr<@type0>>(%12), widen<i64, reason=arg>(const<i32>(0)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
