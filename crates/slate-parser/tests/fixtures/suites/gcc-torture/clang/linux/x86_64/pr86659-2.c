#define ENDIANBIG __attribute((scalar_storage_order("little-endian")))

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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 field0: u64 : 29;
// DEFAULT-NEXT:         field1 field1: u64 : 4;
// DEFAULT-NEXT:         field2 field2: u64 : 31;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 3, 4], bit_offsets=[Some(0), Some(29), Some(33)], bit_units=[(0, 8)], field_units=[Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_struct1:[0-9]+]] struct1 = @type[[TYPE0]];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_value1:[0-9]+]] value1: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_value2:[0-9]+]] value2: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_value3:[0-9]+]] value3: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_flag:[0-9]+]] flag: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_var1:[0-9]+]] var1: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:         write<u64>(bitfield0<unit=0, bytes=0..8, bits=0..29>(%[[VALUE_var1]]), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(23))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_flag]], reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..29>(%[[VALUE_var1]]))))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_value1]], conditional<i32>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..29>(%[[VALUE_var1]])))), const<i32>(0)), const<i32>(10), const<i32>(20)));
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..29>(%[[VALUE_var1]])))), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_value2]], const<i32>(10));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_value2]], const<i32>(20));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<i32>(%[[VALUE_value3]], conditional<i32>(ne<u32>(read<u32>(%[[VALUE_flag]]), const<u32>(0)), const<i32>(10), const<i32>(20)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_value1]]), const<i32>(10))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_value2]]), const<i32>(10))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_value3]]), const<i32>(10))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
