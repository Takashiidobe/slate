extern void abort(void);

union _D_rep {
  unsigned short rep[4];
  double         val;
};

int add(double *key, double *table) {
  unsigned i            = 0;
  double  *deletedEntry = 0;
  while (1) {
    double *entry = table + i;

    if (*entry == *key)
      break;

    union _D_rep _D_inf = {{0, 0, 0, 0x7ff0}};
    if (*entry != _D_inf.val)
      abort();

    union _D_rep _D_inf2 = {{0, 0, 0, 0x7ff0}};
    if (!_D_inf2.val)
      deletedEntry = entry;

    i++;
  }
  if (deletedEntry)
    *deletedEntry = 0.0;
  return 0;
}

int main() {
  union _D_rep infinit  = {{0, 0, 0, 0x7ff0}};
  double       table[2] = {infinit.val, 23};
  double       key      = 23;
  int          ret      = add(&key, table);
  return ret;
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
// DEFAULT-NEXT:     type @type0 _D_rep = union {
// DEFAULT-NEXT:         field0 rep: array<u16, 4>;
// DEFAULT-NEXT:         field1 val: f64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @add(%3 key: ptr<f64>, %4 table: ptr<f64>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 i: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:         let %6 deletedEntry: ptr<f64> [storage=automatic] = null<ptr<f64>>;
// DEFAULT-NEXT:         while %15 ne<i32>(const<i32>(1), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %7 entry: ptr<f64> [storage=automatic] = ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%4), read<u32>(%5));
// DEFAULT-NEXT:                 if eq<f64, exceptions=ignore>(read<f64>(deref(read<ptr<f64>>(%7))), read<f64>(deref(read<ptr<f64>>(%3))))
// DEFAULT-NEXT:                     break %15;
// DEFAULT-NEXT:                 let %8 _D_inf: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = aggregate<array<u16, 4>, zero_fill=false>(index0 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0))), index1 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0))), index2 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0))), index3 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(32752)))));
// DEFAULT-NEXT:                 if ne<f64, exceptions=ignore>(read<f64>(deref(read<ptr<f64>>(%7))), read<f64>(field1(%8)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                 let %9 _D_inf2: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = aggregate<array<u16, 4>, zero_fill=false>(index0 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0))), index1 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0))), index2 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0))), index3 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(32752)))));
// DEFAULT-NEXT:                 if not<bool>(ne<f64, exceptions=ignore>(read<f64>(field1(%9)), const<f64>(0.0)))
// DEFAULT-NEXT:                     write<ptr<f64>>(%6, read<ptr<f64>>(%7));
// DEFAULT-NEXT:                 let %16: u32 [synthetic] = read<u32>(%5);
// DEFAULT-NEXT:                 let %17: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%16), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%5, read<u32>(%17));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if ne<ptr<f64>>(read<ptr<f64>>(%6), null<ptr<f64>>)
// DEFAULT-NEXT:             write<f64>(deref(read<ptr<f64>>(%6)), const<f64>(0.0));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %11 infinit: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = aggregate<array<u16, 4>, zero_fill=false>(index0 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0))), index1 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0))), index2 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0))), index3 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(32752)))));
// DEFAULT-NEXT:         let %12 table: array<f64, 2> [storage=automatic] [align=16] = aggregate<array<f64, 2>, zero_fill=false>(index0 = read<f64>(field1(%11)), index1 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(23)));
// DEFAULT-NEXT:         let %13 key: f64 [storage=automatic] = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(23));
// DEFAULT-NEXT:         let %14 ret: i32 [storage=automatic] = call<i32, signature=fn(ptr<f64>, ptr<f64>) -> i32>(%2, addr_of<ptr<f64>>(%13), array_decay<ptr<f64>, length=Some(2)>(%12));
// DEFAULT-NEXT:         return read<i32>(%14);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
