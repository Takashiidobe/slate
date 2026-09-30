typedef enum { POSITION_ASIS, POSITION_UNSPECIFIED } unit_position;

typedef enum { STATUS_UNKNOWN, STATUS_UNSPECIFIED } unit_status;

typedef struct {
  unit_position position;
  unit_status   status;
} unit_flags;

extern void abort(void);

void new_unit(unit_flags *flags) {
  if (flags->status == STATUS_UNSPECIFIED)
    flags->status = STATUS_UNKNOWN;

  if (flags->position == POSITION_UNSPECIFIED)
    flags->position = POSITION_ASIS;

  switch (flags->status) {
  case STATUS_UNKNOWN:
    break;

  default:
    abort();
  }
}

int main() {
  unit_flags f;
  f.status = STATUS_UNSPECIFIED;
  new_unit(&f);
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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_POSITION_ASIS:[0-9]+]] POSITION_ASIS = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_POSITION_UNSPECIFIED:[0-9]+]] POSITION_UNSPECIFIED = const<i32>(1);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_unit_position:[0-9]+]] unit_position = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_POSITION_ASIS]] STATUS_UNKNOWN = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_POSITION_UNSPECIFIED]] STATUS_UNSPECIFIED = const<i32>(1);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_unit_status:[0-9]+]] unit_status = @type[[TYPE1]];
// DEFAULT-NEXT:     type @type[[TYPE2:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 position: @type[[TYPE0]];
// DEFAULT-NEXT:         field1 status: @type[[TYPE1]];
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_unit_flags:[0-9]+]] unit_flags = @type[[TYPE2]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_new_unit:[0-9]+]] @new_unit(%[[VALUE_flags:[0-9]+]] flags: ptr<@type[[TYPE2]]>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<u32>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE1]]>(field1(deref(read<ptr<@type[[TYPE2]]>>(%[[VALUE_flags]]))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))
// DEFAULT-NEXT:             write<@type[[TYPE1]]>(field1(deref(read<ptr<@type[[TYPE2]]>>(%[[VALUE_flags]]))), int_to_enum<@type[[TYPE1]], reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if eq<u32>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE0]]>(field0(deref(read<ptr<@type[[TYPE2]]>>(%[[VALUE_flags]]))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))
// DEFAULT-NEXT:             write<@type[[TYPE0]]>(field0(deref(read<ptr<@type[[TYPE2]]>>(%[[VALUE_flags]]))), int_to_enum<@type[[TYPE0]], reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         switch %[[VALUE0:[0-9]+]] enum_to_int<u32, reason=promotion>(read<@type[[TYPE1]]>(field1(deref(read<ptr<@type[[TYPE2]]>>(%[[VALUE_flags]])))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE0]] const<u32>(0):
// DEFAULT-NEXT:                     break %[[VALUE0]];
// DEFAULT-NEXT:                 default %[[VALUE0]]:
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_f:[0-9]+]] f: @type[[TYPE2]] [storage=automatic];
// DEFAULT-NEXT:         write<@type[[TYPE1]]>(field1(%[[VALUE_f]]), int_to_enum<@type[[TYPE1]], reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE2]]>) -> void>(%[[VALUE_new_unit]], addr_of<ptr<@type[[TYPE2]]>>(%[[VALUE_f]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
