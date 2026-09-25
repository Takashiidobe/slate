void abort(void);
void exit(int);

struct {
  int a : 4;
  int   : 4;
  int b : 4;
  int c : 4;
} x = {2, 3, 4};

int main(void) {
  if (x.a != 2)
    abort();
  if (x.b != 3)
    abort();
  if (x.c != 4)
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
// DEFAULT-NEXT:     type @type0 = struct {
// DEFAULT-NEXT:         field0 a: i32 : 4;
// DEFAULT-NEXT:         field1 <anonymous>: i32 : 4;
// DEFAULT-NEXT:         field2 b: i32 : 4;
// DEFAULT-NEXT:         field3 c: i32 : 4;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0, 1, 1], bit_offsets=[Some(0), Some(4), Some(8), Some(12)], bit_units=[(0, 2)], field_units=[Some(0), Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     global %3 x: @type0 [storage=static] = aggregate<@type0, zero_fill=false>(field0 = const<i32>(2), field2 = const<i32>(3), field3 = const<i32>(4)) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%5 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(bitfield0<unit=0, bytes=0..2, bits=0..4>(%3)), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(bitfield2<unit=0, bytes=0..2, bits=8..12>(%3)), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(bitfield3<unit=0, bytes=0..2, bits=12..16>(%3)), const<i32>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
