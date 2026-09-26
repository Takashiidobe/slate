void abort(void);
void exit(int);

unsigned calc_mp(unsigned mod) {
  unsigned a, b, c;
  c = -1;
  a = c / mod;
  b = 0 - a * mod;
  if (b > mod) {
    a += 1;
    b -= mod;
  }
  return b;
}

int main(int argc, char *argv[]) {
  unsigned x = 1234;
  unsigned y = calc_mp(x);

  if ((sizeof(y) == 4 && y != 680) || (sizeof(y) == 2 && y != 134))
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
// DEFAULT-NEXT:     fn %1 @exit(%12 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @calc_mp(%3 mod: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %5 b: u32 [storage=automatic];
// DEFAULT-NEXT:         let %6 c: u32 [storage=automatic];
// DEFAULT-NEXT:         write<u32>(%6, reinterpret<u32, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         write<u32>(%4, div<u32, by_zero=ub>(read<u32>(%6), read<u32>(%3)));
// DEFAULT-NEXT:         write<u32>(%5, sub<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)), mul<u32, overflow=wrap>(read<u32>(%4), read<u32>(%3))));
// DEFAULT-NEXT:         if gt<u32>(read<u32>(%5), read<u32>(%3))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %13: u32 [synthetic] = read<u32>(%4);
// DEFAULT-NEXT:                 let %14: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%13), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%4, read<u32>(%14));
// DEFAULT-NEXT:                 let %15: u32 [synthetic] = read<u32>(%5);
// DEFAULT-NEXT:                 let %16: u32 [synthetic] = sub<u32, overflow=wrap>(read<u32>(%15), read<u32>(%3));
// DEFAULT-NEXT:                 write<u32>(%5, read<u32>(%16));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<u32>(%5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main(%8 argc: i32, %9 argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %10 x: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(1234));
// DEFAULT-NEXT:         let %11 y: u32 [storage=automatic] = call<u32, signature=fn(u32) -> u32>(%2, read<u32>(%10));
// DEFAULT-NEXT:         if logical_or<bool>(logical_and<bool>(eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))), ne<u32>(read<u32>(%11), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(680)))), logical_and<bool>(eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), ne<u32>(read<u32>(%11), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(134)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
