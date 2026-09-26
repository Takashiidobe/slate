void abort(void);
void f(int i) {
  if (i > 4 + 3 * 16)
    abort();
}

int main() {
  unsigned int buflen, i;
  buflen = 4 + 3 * 16;
  for (i = 4; i < buflen; i += 3)
    f(i);
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @f(%2 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if gt<i32>(read<i32>(%2), add<i32, overflow=ub>(const<i32>(4), mul<i32, overflow=ub>(const<i32>(3), const<i32>(16))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %4 buflen: u32 [storage=automatic];
// DEFAULT-NEXT:         let %5 i: u32 [storage=automatic];
// DEFAULT-NEXT:         write<u32>(%4, reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(const<i32>(4), mul<i32, overflow=ub>(const<i32>(3), const<i32>(16)))));
// DEFAULT-NEXT:         for %6
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u32>(%5, reinterpret<u32, reason=assign, fits=always>(const<i32>(4)));
// DEFAULT-NEXT:             condition: lt<u32>(read<u32>(%5), read<u32>(%4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %7: u32 [synthetic] = read<u32>(%5);
// DEFAULT-NEXT:                 let %8: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%7), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:                 write<u32>(%5, read<u32>(%8));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32) -> void>(%1, reinterpret<i32, reason=arg, fits=unknown>(read<u32>(%5)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
