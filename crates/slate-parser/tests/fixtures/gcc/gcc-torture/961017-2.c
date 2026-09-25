void abort(void);
void exit(int);

int main(void) {
  int i = 0;

  if (sizeof(unsigned long int) == 4) {
    unsigned long int z = 0;

    do {
      z -= 0x00004000;
      i++;
      if (i > 0x00040000)
        abort();
    } while (z > 0);
    exit(0);
  } else if (sizeof(unsigned int) == 4) {
    unsigned int z = 0;

    do {
      z -= 0x00004000;
      i++;
      if (i > 0x00040000)
        abort();
    } while (z > 0);
    exit(0);
  } else
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
// DEFAULT-NEXT:     fn %1 @exit(%6 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %3 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         if eq<u64>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %4 z: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                 do %7
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %9: u64 [synthetic] = read<u64>(%4);
// DEFAULT-NEXT:                         let %10: u64 [synthetic] = sub<u64, overflow=wrap>(read<u64>(%9), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(16384))));
// DEFAULT-NEXT:                         write<u64>(%4, read<u64>(%10));
// DEFAULT-NEXT:                         let %11: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                         let %12: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%11), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%3, read<i32>(%12));
// DEFAULT-NEXT:                         if gt<i32>(read<i32>(%3), const<i32>(262144))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while gt<u64>(read<u64>(%4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))));
// DEFAULT-NEXT:                 call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %5 z: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:                     do %8
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             let %13: u32 [synthetic] = read<u32>(%5);
// DEFAULT-NEXT:                             let %14: u32 [synthetic] = sub<u32, overflow=wrap>(read<u32>(%13), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(16384)));
// DEFAULT-NEXT:                             write<u32>(%5, read<u32>(%14));
// DEFAULT-NEXT:                             let %15: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                             let %16: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%15), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%3, read<i32>(%16));
// DEFAULT-NEXT:                             if gt<i32>(read<i32>(%3), const<i32>(262144))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     while gt<u32>(read<u32>(%5), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                     call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
