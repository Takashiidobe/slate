void abort(void);
void exit(int);

unsigned sat_add(unsigned i) {
  unsigned ret = i + 1;
  if (ret < i)
    ret = i;
  return ret;
}

unsigned sat_add2(unsigned i) {
  unsigned ret = i + 1;
  if (ret > i)
    return ret;
  return i;
}

unsigned sat_add3(unsigned i) {
  unsigned ret = i - 1;
  if (ret > i)
    ret = i;
  return ret;
}

unsigned sat_add4(unsigned i) {
  unsigned ret = i - 1;
  if (ret < i)
    return ret;
  return i;
}

int main(void) {
  if (sat_add(~0U) != ~0U)
    abort();
  if (sat_add2(~0U) != ~0U)
    abort();
  if (sat_add3(0U) != 0U)
    abort();
  if (sat_add4(0U) != 0U)
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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_sat_add:[0-9]+]] @sat_add(%[[VALUE_i:[0-9]+]] i: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_ret:[0-9]+]] ret: u32 [storage=automatic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE_i]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if lt<u32>(read<u32>(%[[VALUE_ret]]), read<u32>(%[[VALUE_i]]))
// DEFAULT-NEXT:             write<u32>(%[[VALUE_ret]], read<u32>(%[[VALUE_i]]));
// DEFAULT-NEXT:         return read<u32>(%[[VALUE_ret]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_sat_add2:[0-9]+]] @sat_add2(%[[VALUE_i_2:[0-9]+]] i: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_ret_2:[0-9]+]] ret: u32 [storage=automatic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE_i_2]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if gt<u32>(read<u32>(%[[VALUE_ret_2]]), read<u32>(%[[VALUE_i_2]]))
// DEFAULT-NEXT:             return read<u32>(%[[VALUE_ret_2]]);
// DEFAULT-NEXT:         return read<u32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_sat_add3:[0-9]+]] @sat_add3(%[[VALUE_i_3:[0-9]+]] i: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_ret_3:[0-9]+]] ret: u32 [storage=automatic] = sub<u32, overflow=wrap>(read<u32>(%[[VALUE_i_3]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if gt<u32>(read<u32>(%[[VALUE_ret_3]]), read<u32>(%[[VALUE_i_3]]))
// DEFAULT-NEXT:             write<u32>(%[[VALUE_ret_3]], read<u32>(%[[VALUE_i_3]]));
// DEFAULT-NEXT:         return read<u32>(%[[VALUE_ret_3]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_sat_add4:[0-9]+]] @sat_add4(%[[VALUE_i_4:[0-9]+]] i: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_ret_4:[0-9]+]] ret: u32 [storage=automatic] = sub<u32, overflow=wrap>(read<u32>(%[[VALUE_i_4]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if lt<u32>(read<u32>(%[[VALUE_ret_4]]), read<u32>(%[[VALUE_i_4]]))
// DEFAULT-NEXT:             return read<u32>(%[[VALUE_ret_4]]);
// DEFAULT-NEXT:         return read<u32>(%[[VALUE_i_4]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_sat_add]], not<u32>(const<u32>(0))), not<u32>(const<u32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_sat_add2]], not<u32>(const<u32>(0))), not<u32>(const<u32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_sat_add3]], const<u32>(0)), const<u32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_sat_add4]], const<u32>(0)), const<u32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
