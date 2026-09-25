#include <stdio.h>

void abort(void);
void exit(int);

#ifdef DEBUG
#define abort() printf("error, line %d\n", __LINE__)
#endif

int count;

void a1() { ++count; }

void b(unsigned short data) {
  if (data & 0x8000)
    a1();
  data <<= 1;

  if (data & 0x8000)
    a1();
  data <<= 1;

  if (data & 0x8000)
    a1();
}

int main(void) {
  count = 0;
  b(0);
  if (count != 0)
    abort();

  count = 0;
  b(0x8000);
  if (count != 1)
    abort();

  count = 0;
  b(0x4000);
  if (count != 1)
    abort();

  count = 0;
  b(0x2000);
  if (count != 1)
    abort();

  count = 0;
  b(0xc000);
  if (count != 2)
    abort();

  count = 0;
  b(0xa000);
  if (count != 2)
    abort();

  count = 0;
  b(0x6000);
  if (count != 2)
    abort();

  count = 0;
  b(0xe000);
  if (count != 3)
    abort();

#ifdef DEBUG
  printf("Done.\n");
#endif
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
// DEFAULT-NEXT:     global %2 count: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%7 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @a1() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %8: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %9: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%8), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%9));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @b(%5 data: u16) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%5))), const<i32>(32768)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         let %10: u16 [synthetic] = read<u16>(%5);
// DEFAULT-NEXT:         let %11: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%10))), const<i32>(1))));
// DEFAULT-NEXT:         write<u16>(%5, read<u16>(%11));
// DEFAULT-NEXT:         if ne<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%5))), const<i32>(32768)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         let %12: u16 [synthetic] = read<u16>(%5);
// DEFAULT-NEXT:         let %13: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%12))), const<i32>(1))));
// DEFAULT-NEXT:         write<u16>(%5, read<u16>(%13));
// DEFAULT-NEXT:         if ne<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%5))), const<i32>(32768)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i32>(%2, const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(u16) -> void>(%4, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%2), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i32>(%2, const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(u16) -> void>(%4, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(32768))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%2), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i32>(%2, const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(u16) -> void>(%4, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(16384))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%2), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i32>(%2, const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(u16) -> void>(%4, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(8192))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%2), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i32>(%2, const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(u16) -> void>(%4, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(49152))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%2), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i32>(%2, const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(u16) -> void>(%4, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(40960))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%2), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i32>(%2, const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(u16) -> void>(%4, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(24576))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%2), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i32>(%2, const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(u16) -> void>(%4, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(57344))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%2), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
