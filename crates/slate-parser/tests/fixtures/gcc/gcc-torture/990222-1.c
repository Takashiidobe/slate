void abort(void);

char line[4] = {'1', '9', '9', '\0'};

int main() {
  char *ptr = line + 3;

  while ((*--ptr += 1) > '9')
    *ptr = '0';
  if (line[0] != '2' || line[1] != '0' || line[2] != '0')
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
// DEFAULT-NEXT:     global %1 line: array<i8, 4> [storage=static] = aggregate<array<i8, 4>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(49)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(57)), index2 = truncate<i8, reason=assign, fits=always>(const<i32>(57)), index3 = truncate<i8, reason=assign, fits=always>(const<i32>(0))) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %3 ptr: ptr<i8> [storage=automatic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(%1), const<i32>(3));
// DEFAULT-NEXT:         while %4 {
// DEFAULT-NEXT:             let %5: ptr<i8> [synthetic] = read<ptr<i8>>(%3);
// DEFAULT-NEXT:             let %6: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(read<ptr<i8>>(%5), const<i32>(1));
// DEFAULT-NEXT:             write<ptr<i8>>(%3, read<ptr<i8>>(%6));
// DEFAULT-NEXT:             let %7: ptr<i8> [synthetic] = read<ptr<i8>>(%6);
// DEFAULT-NEXT:             let %8: i8 [synthetic] = read<i8>(deref(read<ptr<i8>>(%7)));
// DEFAULT-NEXT:             let %9: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%8)), const<i32>(1)));
// DEFAULT-NEXT:             write<i8>(deref(read<ptr<i8>>(%7)), read<i8>(%9));
// DEFAULT-NEXT:             yield gt<i32>(widen<i32, reason=promotion>(read<i8>(%9)), const<i32>(57));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             write<i8>(deref(read<ptr<i8>>(%3)), truncate<i8, reason=assign, fits=always>(const<i32>(48)));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(%1), const<i32>(0))))), const<i32>(50)), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(%1), const<i32>(1))))), const<i32>(48))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(%1), const<i32>(2))))), const<i32>(48)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
