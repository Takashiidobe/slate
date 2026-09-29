#include <stdio.h>

void abort(void);
void exit(int);

#ifdef DEBUG
#define abort() printf("error, line %d\n", __LINE__)
#endif

int count;

void a1() { ++count; }

void b(unsigned char data) {
  if (data & 0x80)
    a1();
  data <<= 1;

  if (data & 0x80)
    a1();
  data <<= 1;

  if (data & 0x80)
    a1();
}

int main(void) {
  count = 0;
  b(0);
  if (count != 0)
    abort();

  count = 0;
  b(0x80);
  if (count != 1)
    abort();

  count = 0;
  b(0x40);
  if (count != 1)
    abort();

  count = 0;
  b(0x20);
  if (count != 1)
    abort();

  count = 0;
  b(0xc0);
  if (count != 2)
    abort();

  count = 0;
  b(0xa0);
  if (count != 2)
    abort();

  count = 0;
  b(0x60);
  if (count != 2)
    abort();

  count = 0;
  b(0xe0);
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
// DEFAULT-NEXT:     global %[[VALUE_count:[0-9]+]] count: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_a1:[0-9]+]] @a1() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_count]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_b:[0-9]+]] @b(%[[VALUE_data:[0-9]+]] data: u8) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_data]]))), const<i32>(128)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_a1]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: u8 [synthetic] = read<u8>(%[[VALUE_data]]);
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE3]]))), const<i32>(1))));
// DEFAULT-NEXT:         write<u8>(%[[VALUE_data]], read<u8>(%[[VALUE4]]));
// DEFAULT-NEXT:         if ne<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_data]]))), const<i32>(128)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_a1]]);
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: u8 [synthetic] = read<u8>(%[[VALUE_data]]);
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE5]]))), const<i32>(1))));
// DEFAULT-NEXT:         write<u8>(%[[VALUE_data]], read<u8>(%[[VALUE6]]));
// DEFAULT-NEXT:         if ne<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_data]]))), const<i32>(128)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_a1]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i32>(%[[VALUE_count]], const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(u8) -> void>(%[[VALUE_b]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_count]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_count]], const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(u8) -> void>(%[[VALUE_b]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(const<i32>(128))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_count]]), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_count]], const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(u8) -> void>(%[[VALUE_b]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(64))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_count]]), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_count]], const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(u8) -> void>(%[[VALUE_b]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(32))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_count]]), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_count]], const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(u8) -> void>(%[[VALUE_b]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(const<i32>(192))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_count]]), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_count]], const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(u8) -> void>(%[[VALUE_b]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(const<i32>(160))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_count]]), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_count]], const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(u8) -> void>(%[[VALUE_b]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(96))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_count]]), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_count]], const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(u8) -> void>(%[[VALUE_b]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(const<i32>(224))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_count]]), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
