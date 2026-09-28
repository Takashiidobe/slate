typedef unsigned long long uint64_t;
void f(uint64_t *a, uint64_t aa) __attribute__((noinline));
void f(uint64_t *a, uint64_t aa) {
  uint64_t new_value  = aa;
  uint64_t old_value  = *a;
  int      bit_size   = 32;
  uint64_t mask       = (uint64_t)(unsigned)(-1);
  uint64_t tmp        = old_value & mask;
  new_value          &= mask;
  /* On overflow we need to add 1 in the upper bits */
  if (tmp > new_value)
    new_value += 1ull << bit_size;
  /* Add in the upper bits from the old value */
  new_value += old_value & ~mask;
  *a         = new_value;
}
int main(void) {
  uint64_t value, new_value, old_value;
  value     = 0x100000001;
  old_value = value;
  new_value = (value + 1) & (uint64_t)(unsigned)(-1);
  f(&value, new_value);
  if (value != old_value + 1)
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
// DEFAULT-NEXT:     type @type0 uint64_t = u64;
// DEFAULT-NEXT:     fn %1 @f(%2 a: ptr<u64>, %3 aa: u64) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %4 new_value: u64 [storage=automatic] = read<u64>(%3);
// DEFAULT-NEXT:         let %5 old_value: u64 [storage=automatic] = read<u64>(deref(read<ptr<u64>>(%2)));
// DEFAULT-NEXT:         let %6 bit_size: i32 [storage=automatic] = const<i32>(32);
// DEFAULT-NEXT:         let %7 mask: u64 [storage=automatic] = widen<u64, reason=explicit>(reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         let %8 tmp: u64 [storage=automatic] = and<u64>(read<u64>(%5), read<u64>(%7));
// DEFAULT-NEXT:         let %16: u64 [synthetic] = read<u64>(%4);
// DEFAULT-NEXT:         let %17: u64 [synthetic] = and<u64>(read<u64>(%16), read<u64>(%7));
// DEFAULT-NEXT:         write<u64>(%4, read<u64>(%17));
// DEFAULT-NEXT:         if gt<u64>(read<u64>(%8), read<u64>(%4))
// DEFAULT-NEXT:             let %18: u64 [synthetic] = read<u64>(%4);
// DEFAULT-NEXT:             let %19: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%18), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), read<i32>(%6)));
// DEFAULT-NEXT:             write<u64>(%4, read<u64>(%19));
// DEFAULT-NEXT:         let %20: u64 [synthetic] = read<u64>(%4);
// DEFAULT-NEXT:         let %21: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%20), and<u64>(read<u64>(%5), not<u64>(read<u64>(%7))));
// DEFAULT-NEXT:         write<u64>(%4, read<u64>(%21));
// DEFAULT-NEXT:         write<u64>(deref(read<ptr<u64>>(%2)), read<u64>(%4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %10 value: u64 [storage=automatic];
// DEFAULT-NEXT:         let %11 new_value: u64 [storage=automatic];
// DEFAULT-NEXT:         let %12 old_value: u64 [storage=automatic];
// DEFAULT-NEXT:         write<u64>(%10, reinterpret<u64, reason=assign, fits=always>(const<i64>(4294967297)));
// DEFAULT-NEXT:         write<u64>(%12, read<u64>(%10));
// DEFAULT-NEXT:         write<u64>(%11, and<u64>(add<u64, overflow=wrap>(read<u64>(%10), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), widen<u64, reason=explicit>(reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<u64>, u64) -> void>(%1, addr_of<ptr<u64>>(%10), read<u64>(%11));
// DEFAULT-NEXT:         if ne<u64>(read<u64>(%10), add<u64, overflow=wrap>(read<u64>(%12), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%15);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
