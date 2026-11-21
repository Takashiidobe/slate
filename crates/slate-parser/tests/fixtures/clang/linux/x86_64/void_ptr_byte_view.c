static int all_bytes_identical(const void *src, unsigned long size) {
  unsigned char b = ((const unsigned char *)src)[0];
  for (unsigned long p = 1; p < size; p++) {
    if (((const unsigned char *)src)[p] != b) {
      return 0;
    }
  }
  return 1;
}

int main(void) {
  unsigned char buf[4] = {7, 7, 7, 7};
  return all_bytes_identical(buf, 4);
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
// DEFAULT-NEXT:     fn %[[VALUE_all_bytes_identical:[0-9]+]] @all_bytes_identical(%[[VALUE_src:[0-9]+]] src: ptr<const void>, %[[VALUE_size:[0-9]+]] size: u64) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: u8 [storage=automatic] = read<u8>(deref(ptr_offset<ptr<const u8>, subtract=false, element=u8, overflow=ub>(pointer_cast<ptr<const u8>, reason=explicit>(read<ptr<const void>>(%[[VALUE_src]])), const<i32>(0))));
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_p:[0-9]+]] p: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:             condition: lt<u64>(read<u64>(%[[VALUE_p]]), read<u64>(%[[VALUE_size]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_p]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE1]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_p]], read<u64>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<const u8>, subtract=false, element=u8, overflow=ub>(pointer_cast<ptr<const u8>, reason=explicit>(read<ptr<const void>>(%[[VALUE_src]])), read<u64>(%[[VALUE_p]])))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_b]]))))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             return const<i32>(0);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_buf:[0-9]+]] buf: array<u8, 4> [storage=automatic] = aggregate<array<u8, 4>, zero_fill=false>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(7))), index1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(7))), index2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(7))), index3 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(7))));
// DEFAULT-NEXT:         return call<i32, signature=fn(ptr<const void>, u64) -> i32>(%[[VALUE_all_bytes_identical]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<u8>, length=Some(4)>(%[[VALUE_buf]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
