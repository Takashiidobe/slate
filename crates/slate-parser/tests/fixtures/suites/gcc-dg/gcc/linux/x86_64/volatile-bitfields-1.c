/* { dg-options "-fstrict-volatile-bitfields" } */
/* { dg-do run } */

extern int puts(const char *);
extern void abort(void) __attribute__((noreturn));

typedef struct {
  volatile unsigned short a:8, b:8;
} BitStruct;

BitStruct bits = {1, 2};

void check(int i, int j)
{
  if (i != 1 || j != 2) puts("FAIL"), abort();
}

int main ()
{
  check(bits.a, bits.b);

  return 0;
}

// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     type @type0 = struct {
// DEFAULT-NEXT:         field0 a: volatile u16 : 8;
// DEFAULT-NEXT:         field1 b: volatile u16 : 8;
// DEFAULT-NEXT:     } [size=2, align=2, offsets=[0, 1], bit_offsets=[Some(0), Some(8)], bit_units=[(0, 2)], field_units=[Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type1 BitStruct = @type0;
// DEFAULT-NEXT:     global %4 bits: @type0 [storage=static] = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(1))), field1 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(2)))) [linkage=external];
// DEFAULT-NEXT:     global %10 .str10: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([70, 65, 73, 76, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @puts(%9 <unnamed>: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %5 @check(%6 i: i32, %7 j: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(%6), const<i32>(1)), ne<i32>(read<i32>(%7), const<i32>(2)))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%10)));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%5, reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile>(bitfield0<unit=0, bytes=0..2, bits=0..8>(%4)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile>(bitfield1<unit=0, bytes=0..2, bits=8..16>(%4)))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
