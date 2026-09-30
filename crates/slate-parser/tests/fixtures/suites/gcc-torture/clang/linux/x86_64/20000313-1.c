void abort(void);

unsigned int buggy(unsigned int *param) {
  unsigned int accu, zero = 0, borrow;
  accu    = -*param;
  borrow  = -(accu > zero);
  *param += accu;
  return borrow;
}

int main(void) {
  unsigned int param  = 1;
  unsigned int borrow = buggy(&param);

  if (param != 0)
    abort();
  if (borrow + 1 != 0)
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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_buggy:[0-9]+]] @buggy(%[[VALUE_param:[0-9]+]] param: ptr<u32>) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_accu:[0-9]+]] accu: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_zero:[0-9]+]] zero: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_borrow:[0-9]+]] borrow: u32 [storage=automatic];
// DEFAULT-NEXT:         write<u32>(%[[VALUE_accu]], neg<u32, overflow=wrap>(read<u32>(deref(read<ptr<u32>>(%[[VALUE_param]])))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_borrow]], reinterpret<u32, reason=assign, fits=unknown>(neg<i32, overflow=ub>(from_bool<i32, reason=promotion>(gt<u32>(read<u32>(%[[VALUE_accu]]), read<u32>(%[[VALUE_zero]]))))));
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: ptr<u32> [synthetic] = read<ptr<u32>>(%[[VALUE_param]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: u32 [synthetic] = read<u32>(deref(read<ptr<u32>>(%[[VALUE0]])));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE1]]), read<u32>(%[[VALUE_accu]]));
// DEFAULT-NEXT:         write<u32>(deref(read<ptr<u32>>(%[[VALUE0]])), read<u32>(%[[VALUE2]]));
// DEFAULT-NEXT:         return read<u32>(%[[VALUE_borrow]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_param_2:[0-9]+]] param: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_borrow_2:[0-9]+]] borrow: u32 [storage=automatic] = call<u32, signature=fn(ptr<u32>) -> u32>(%[[VALUE_buggy]], addr_of<ptr<u32>>(%[[VALUE_param_2]]));
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%[[VALUE_param_2]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_borrow_2]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
