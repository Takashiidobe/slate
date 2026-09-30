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
// DEFAULT-NEXT:     type @type[[TYPE__D_rep:[0-9]+]] _D_rep = union {
// DEFAULT-NEXT:         field0 rep: array<u16, 4>;
// DEFAULT-NEXT:         field1 val: f64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_add:[0-9]+]] @add(%[[VALUE_key:[0-9]+]] key: ptr<f64>, %[[VALUE_table:[0-9]+]] table: ptr<f64>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_deletedEntry:[0-9]+]] deletedEntry: ptr<f64> [storage=automatic] = null<ptr<f64>>;
// DEFAULT-NEXT:         while %[[VALUE0:[0-9]+]] ne<i32>(const<i32>(1), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_entry:[0-9]+]] entry: ptr<f64> [storage=automatic] = ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_table]]), read<u32>(%[[VALUE_i]]));
// DEFAULT-NEXT:                 if eq<f64, exceptions=ignore>(read<f64>(deref(read<ptr<f64>>(%[[VALUE_entry]]))), read<f64>(deref(read<ptr<f64>>(%[[VALUE_key]]))))
// DEFAULT-NEXT:                     break %[[VALUE0]];
// DEFAULT-NEXT:                 let %[[VALUE__D_inf:[0-9]+]] _D_inf: @type[[TYPE__D_rep]] [storage=automatic] = aggregate<@type[[TYPE__D_rep]], zero_fill=false>(field0 = aggregate<array<u16, 4>, zero_fill=false>(index0 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0))), index1 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0))), index2 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0))), index3 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(32752)))));
// DEFAULT-NEXT:                 if ne<f64, exceptions=ignore>(read<f64>(deref(read<ptr<f64>>(%[[VALUE_entry]]))), read<f64>(field1(%[[VALUE__D_inf]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE__D_inf2:[0-9]+]] _D_inf2: @type[[TYPE__D_rep]] [storage=automatic] = aggregate<@type[[TYPE__D_rep]], zero_fill=false>(field0 = aggregate<array<u16, 4>, zero_fill=false>(index0 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0))), index1 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0))), index2 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0))), index3 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(32752)))));
// DEFAULT-NEXT:                 if not<bool>(ne<f64, exceptions=ignore>(read<f64>(field1(%[[VALUE__D_inf2]])), const<f64>(0.0)))
// DEFAULT-NEXT:                     write<ptr<f64>>(%[[VALUE_deletedEntry]], read<ptr<f64>>(%[[VALUE_entry]]));
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE1]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_i]], read<u32>(%[[VALUE2]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if ne<ptr<f64>>(read<ptr<f64>>(%[[VALUE_deletedEntry]]), null<ptr<f64>>)
// DEFAULT-NEXT:             write<f64>(deref(read<ptr<f64>>(%[[VALUE_deletedEntry]])), const<f64>(0.0));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_infinit:[0-9]+]] infinit: @type[[TYPE__D_rep]] [storage=automatic] = aggregate<@type[[TYPE__D_rep]], zero_fill=false>(field0 = aggregate<array<u16, 4>, zero_fill=false>(index0 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0))), index1 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0))), index2 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0))), index3 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(32752)))));
// DEFAULT-NEXT:         let %[[VALUE_table_2:[0-9]+]] table: array<f64, 2> [storage=automatic] [align=16] = aggregate<array<f64, 2>, zero_fill=false>(index0 = read<f64>(field1(%[[VALUE_infinit]])), index1 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(23)));
// DEFAULT-NEXT:         let %[[VALUE_key_2:[0-9]+]] key: f64 [storage=automatic] = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(23));
// DEFAULT-NEXT:         let %[[VALUE_ret:[0-9]+]] ret: i32 [storage=automatic] = call<i32, signature=fn(ptr<f64>, ptr<f64>) -> i32>(%[[VALUE_add]], addr_of<ptr<f64>>(%[[VALUE_key_2]]), array_decay<ptr<f64>, length=Some(2)>(%[[VALUE_table_2]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_ret]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
