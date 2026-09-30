void abort(void);

void fn_4parms(unsigned char a, long *b, long *c, unsigned int *d) {
  if (*b != 1 || *c != 2 || *d != 3)
    abort();
}

int main() {
  unsigned char a = 0;
  unsigned long b = 1, c = 2;
  unsigned int  d = 3;

  fn_4parms(a, &b, &c, &d);
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
// DEFAULT-NEXT:     fn %[[VALUE_fn_4parms:[0-9]+]] @fn_4parms(%[[VALUE_a:[0-9]+]] a: u8, %[[VALUE_b:[0-9]+]] b: ptr<i64>, %[[VALUE_c:[0-9]+]] c: ptr<i64>, %[[VALUE_d:[0-9]+]] d: ptr<u32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i64>(read<i64>(deref(read<ptr<i64>>(%[[VALUE_b]]))), widen<i64, reason=usual_arith>(const<i32>(1))), ne<i64>(read<i64>(deref(read<ptr<i64>>(%[[VALUE_c]]))), widen<i64, reason=usual_arith>(const<i32>(2)))), ne<u32>(read<u32>(deref(read<ptr<u32>>(%[[VALUE_d]]))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(3))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a_2:[0-9]+]] a: u8 [storage=automatic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE_b_2:[0-9]+]] b: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE_c_2:[0-9]+]] c: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(2)));
// DEFAULT-NEXT:         let %[[VALUE_d_2:[0-9]+]] d: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(3));
// DEFAULT-NEXT:         call<void, signature=fn(u8, ptr<i64>, ptr<i64>, ptr<u32>) -> void>(%[[VALUE_fn_4parms]], read<u8>(%[[VALUE_a_2]]), pointer_cast<ptr<i64>, reason=arg>(addr_of<ptr<u64>>(%[[VALUE_b_2]])), pointer_cast<ptr<i64>, reason=arg>(addr_of<ptr<u64>>(%[[VALUE_c_2]])), addr_of<ptr<u32>>(%[[VALUE_d_2]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
