void abort(void);
void exit(int);

unsigned short int showbug(unsigned short int *a, unsigned short int *b) {
  *a += *b - 8;
  return (*a >= 8);
}

int main() {
  unsigned short int x = 0;
  unsigned short int y = 10;

  if (showbug(&x, &y) != 0)
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
// DEFAULT-NEXT:     fn %1 @exit(%8 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @showbug(%3 a: ptr<u16>, %4 b: ptr<u16>) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %9: ptr<u16> [synthetic] = read<ptr<u16>>(%3);
// DEFAULT-NEXT:         let %10: u16 [synthetic] = read<u16>(deref(read<ptr<u16>>(%9)));
// DEFAULT-NEXT:         let %11: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%10))), sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(read<ptr<u16>>(%4))))), const<i32>(8)))));
// DEFAULT-NEXT:         write<u16>(deref(read<ptr<u16>>(%9)), read<u16>(%11));
// DEFAULT-NEXT:         return from_bool<u16, reason=return>(ge<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(read<ptr<u16>>(%3))))), const<i32>(8)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 x: u16 [storage=automatic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %7 y: u16 [storage=automatic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(10)));
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(ptr<u16>, ptr<u16>) -> u16>(%2, addr_of<ptr<u16>>(%6), addr_of<ptr<u16>>(%7)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
