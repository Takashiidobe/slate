void abort(void);
void exit(int);

struct {
  unsigned int f1 : 1, f2 : 1, f3 : 3, f4 : 3, f5 : 2, f6 : 1, f7 : 1;
} result = {1, 1, 7, 7, 3, 1, 1};

int main(void) {
  if ((result.f3 & ~7) != 0 || (result.f4 & ~7) != 0)
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
// DEFAULT-NEXT:         field0 f1: u32 : 1;
// DEFAULT-NEXT:         field1 f2: u32 : 1;
// DEFAULT-NEXT:         field2 f3: u32 : 3;
// DEFAULT-NEXT:         field3 f4: u32 : 3;
// DEFAULT-NEXT:         field4 f5: u32 : 2;
// DEFAULT-NEXT:         field5 f6: u32 : 1;
// DEFAULT-NEXT:         field6 f7: u32 : 1;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0, 0, 0, 1, 1, 1], bit_offsets=[Some(0), Some(1), Some(2), Some(5), Some(8), Some(10), Some(11)], bit_units=[(0, 2)], field_units=[Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     global %3 result: @type0 [storage=static] = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(1)), field1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(1)), field2 = reinterpret<u32, reason=assign, fits=always>(const<i32>(7)), field3 = reinterpret<u32, reason=assign, fits=always>(const<i32>(7)), field4 = reinterpret<u32, reason=assign, fits=always>(const<i32>(3)), field5 = reinterpret<u32, reason=assign, fits=always>(const<i32>(1)), field6 = reinterpret<u32, reason=assign, fits=always>(const<i32>(1))) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%5 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..2, bits=2..5>(%3))), not<i32>(const<i32>(7))), const<i32>(0)), ne<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=0..2, bits=5..8>(%3))), not<i32>(const<i32>(7))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
