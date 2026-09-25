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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%15 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @sat_add(%3 i: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 ret: u32 [storage=automatic] = add<u32, overflow=wrap>(read<u32>(%3), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if lt<u32>(read<u32>(%4), read<u32>(%3))
// DEFAULT-NEXT:             write<u32>(%4, read<u32>(%3));
// DEFAULT-NEXT:         return read<u32>(%4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @sat_add2(%6 i: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7 ret: u32 [storage=automatic] = add<u32, overflow=wrap>(read<u32>(%6), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if gt<u32>(read<u32>(%7), read<u32>(%6))
// DEFAULT-NEXT:             return read<u32>(%7);
// DEFAULT-NEXT:         return read<u32>(%6);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @sat_add3(%9 i: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %10 ret: u32 [storage=automatic] = sub<u32, overflow=wrap>(read<u32>(%9), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if gt<u32>(read<u32>(%10), read<u32>(%9))
// DEFAULT-NEXT:             write<u32>(%10, read<u32>(%9));
// DEFAULT-NEXT:         return read<u32>(%10);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @sat_add4(%12 i: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %13 ret: u32 [storage=automatic] = sub<u32, overflow=wrap>(read<u32>(%12), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if lt<u32>(read<u32>(%13), read<u32>(%12))
// DEFAULT-NEXT:             return read<u32>(%13);
// DEFAULT-NEXT:         return read<u32>(%12);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%2, not<u32>(const<u32>(0))), not<u32>(const<u32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%5, not<u32>(const<u32>(0))), not<u32>(const<u32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%8, const<u32>(0)), const<u32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%11, const<u32>(0)), const<u32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
