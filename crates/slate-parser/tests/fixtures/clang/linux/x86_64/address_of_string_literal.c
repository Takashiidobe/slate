struct id { unsigned long driver_data; };

static const struct id ids[] = {
  { .driver_data = 0 ? 0 : (unsigned long)&"PARPORT_SERIAL" },
};

const char (*narrow)[4] = &"abc";
const int (*wide)[3] = &L"ab";

int first(void) {
  const char (*p)[3] = &"xy";
  return (*p)[1] + (int)ids[0].driver_data;
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
// DEFAULT-NEXT:     type @type[[TYPE_id:[0-9]+]] id = struct {
// DEFAULT-NEXT:         field0 driver_data: u64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 15> [storage=static] = code_units<array<i8, 15>>([80, 65, 82, 80, 79, 82, 84, 95, 83, 69, 82, 73, 65, 76, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_ids:[0-9]+]] ids: array<@type[[TYPE_id]], 1> [storage=static] [const] = aggregate<array<@type[[TYPE_id]], 1>, zero_fill=false>(index0 = aggregate<@type[[TYPE_id]], zero_fill=false>(field0 = conditional<u64>(ne<i32>(const<i32>(0), const<i32>(0)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), ptr_to_int<u64, reason=explicit>(addr_of<ptr<array<i8, 15>>>(%[[VALUE_str]]))))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([97, 98, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_narrow:[0-9]+]] narrow: ptr<const array<i8, 4>> [storage=static] = pointer_cast<ptr<const array<i8, 4>>, reason=assign>(addr_of<ptr<array<i8, 4>>>(%[[VALUE_str_2]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i32, 3> [storage=static] = code_units<array<i32, 3>>([97, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_wide:[0-9]+]] wide: ptr<const array<i32, 3>> [storage=static] = pointer_cast<ptr<const array<i32, 3>>, reason=assign>(addr_of<ptr<array<i32, 3>>>(%[[VALUE_str_3]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([120, 121, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_first:[0-9]+]] @first() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<const array<i8, 3>> [storage=automatic] = pointer_cast<ptr<const array<i8, 3>>, reason=assign>(addr_of<ptr<array<i8, 3>>>(%[[VALUE_str_4]]));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(deref(read<ptr<const array<i8, 3>>>(%[[VALUE_p]]))), const<i32>(1))))), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(read<u64>(field0(deref(ptr_offset<ptr<const @type[[TYPE_id]]>, subtract=false, element=@type[[TYPE_id]], overflow=ub>(array_decay<ptr<const @type[[TYPE_id]]>, length=Some(1)>(%[[VALUE_ids]]), const<i32>(0))))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
