#define ENDIANBIG __attribute((scalar_storage_order("big-endian")))

typedef struct ENDIANBIG {
  unsigned long long field0 : 29;
  unsigned long long field1 : 4;
  unsigned long long field2 : 31;
} struct1;

int main(void) {
  int          value1 = 0;
  int          value2 = 0;
  int          value3 = 0;
  unsigned int flag;
  struct1      var1;
  var1.field0 = 23;

  flag   = var1.field0;
  value1 = ((var1.field0) ? 10 : 20);
  if (var1.field0) {
    value2 = 10;
  } else {
    value2 = 20;
  }

  value3 = ((flag) ? 10 : 20);

  if (value1 != 10)
    __builtin_abort();

  if (value2 != 10)
    __builtin_abort();

  if (value3 != 10)
    __builtin_abort();

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
// DEFAULT-NEXT:     type @type0 = struct {
// DEFAULT-NEXT:         field0 field0: u64 : 29;
// DEFAULT-NEXT:         field1 field1: u64 : 4;
// DEFAULT-NEXT:         field2 field2: u64 : 31;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 3, 4], bit_offsets=[Some(0), Some(29), Some(33)], bit_units=[(0, 8)], field_units=[Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type1 struct1 = @type0;
// DEFAULT-NEXT:     fn %8 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %3 value1: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %4 value2: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %5 value3: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %6 flag: u32 [storage=automatic];
// DEFAULT-NEXT:         let %7 var1: @type0 [storage=automatic];
// DEFAULT-NEXT:         write<u64>(bitfield0<unit=0, bytes=0..8, bits=0..29>(%7), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(23))));
// DEFAULT-NEXT:         write<u32>(%6, reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..29>(%7))))));
// DEFAULT-NEXT:         write<i32>(%3, conditional<i32>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..29>(%7)))), const<i32>(0)), const<i32>(10), const<i32>(20)));
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..29>(%7)))), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i32>(%4, const<i32>(10));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i32>(%4, const<i32>(20));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<i32>(%5, conditional<i32>(ne<u32>(read<u32>(%6), const<u32>(0)), const<i32>(10), const<i32>(20)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%3), const<i32>(10))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%4), const<i32>(10))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%5), const<i32>(10))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
